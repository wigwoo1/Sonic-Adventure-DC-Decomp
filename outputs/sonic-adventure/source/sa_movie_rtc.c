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
SA_STAGE_EXPORT int sa_movie_rtc_8c10be78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10be78;
L8c10be78: *stop=0x8c10be78u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10be7a; /* mov.l r14,@-r15 */
L8c10be7a: *stop=0x8c10be7au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10be7c; /* sts.l PR,@-r15 */
L8c10be7c: *stop=0x8c10be7cu;r[15]+=4294967276u;goto L8c10be7e; /* add #-0x14,r15 */
L8c10be7e: *stop=0x8c10be7eu;r[3]=1u;goto L8c10be80; /* mov #0x1,r3 */
L8c10be80: *stop=0x8c10be80u;r[14]=r[15];goto L8c10be82; /* mov r15,r14 */
L8c10be82: *stop=0x8c10be82u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10be84; /* mov.l r4,@r15 */
L8c10be84: *stop=0x8c10be84u;r[14]+=12u;goto L8c10be86; /* add #0xc,r14 */
L8c10be86: *stop=0x8c10be86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bfccu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10be88; /* mov.l 0x8c10bfcc,r4 */
L8c10be88: *stop=0x8c10be88u;r[6]=4u;goto L8c10be8a; /* mov #0x4,r6 */
L8c10be8a: *stop=0x8c10be8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bfd0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10be8c; /* mov.l 0x8c10bfd0,r2 */
L8c10be8c: *stop=0x8c10be8cu;r[7]=2u;goto L8c10be8e; /* mov #0x2,r7 */
L8c10be8e: *stop=0x8c10be8eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10be90; /* mov.l r3,@-r15 */
L8c10be90: *stop=0x8c10be90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10be92; /* mov.l r3,@-r15 */
L8c10be92: target=r[2];*pr=0x8c10be96u;*stop=0x8c10be94u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10be96; /* jsr @r2 */
L8c10be96: *stop=0x8c10be96u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10be98; /* tst r0,r0 */
L8c10be98: take=(*sr&1u);*stop=0x8c10be9au;r[15]+=8u;if(take)goto L8c10bea6;goto L8c10be9c; /* bt/s 0x8c10bea6 */
L8c10be9c: *stop=0x8c10be9cu;r[0]=4294967295u;goto L8c10be9e; /* mov #-0x1,r0 */
L8c10be9e: *stop=0x8c10be9eu;r[15]+=20u;goto L8c10bea0; /* add #0x14,r15 */
L8c10bea0: *stop=0x8c10bea0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10bea2; /* lds.l @r15+,PR */
L8c10bea2: *stop=0x8c10bea2u;if(*pr!=entry_pr)return -3;*stop=0x8c10bea4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c10bea6: *stop=0x8c10bea6u;r[3]=r[14];goto L8c10bea8; /* mov r14,r3 */
L8c10bea8: *stop=0x8c10bea8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bfd4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10beaa; /* mov.l 0x8c10bfd4,r0 */
L8c10beaa: *stop=0x8c10beaau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10beac; /* mov.l @r15,r2 */
L8c10beac: *stop=0x8c10beacu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10beae; /* mov.l r3,@(0x8,r15) */
L8c10beae: *stop=0x8c10beaeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}r[3]+=4;goto L8c10beb0; /* mov.l @r3+,r1 */
L8c10beb0: *stop=0x8c10beb0u;r[1]<<=16;goto L8c10beb2; /* shll16 r1 */
L8c10beb2: *stop=0x8c10beb2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10beb4; /* mov.l r3,@(0x4,r15) */
L8c10beb4: *stop=0x8c10beb4u;r[1]&=r[0];goto L8c10beb6; /* and r0,r1 */
L8c10beb6: *stop=0x8c10beb6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10beb8; /* mov.l @r3,r0 */
L8c10beb8: *stop=0x8c10beb8u;r[0]=r[0]&65535u;goto L8c10beba; /* extu.w r0,r0 */
L8c10beba: *stop=0x8c10bebau;r[1]|=r[0];goto L8c10bebc; /* or r0,r1 */
L8c10bebc: *stop=0x8c10bebcu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c10bebe; /* mov.l r1,@r2 */
L8c10bebe: *stop=0x8c10bebeu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10bec0; /* mov.l @(0x8,r15),r4 */
L8c10bec0: target=0x8c10c018u;*pr=0x8c10bec4u;*stop=0x8c10bec2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10bec4; /* bsr 0x8c10c018 */
L8c10bec4: *stop=0x8c10bec4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10bec6; /* mov.l @(0x4,r15),r4 */
L8c10bec6: target=0x8c10c018u;*pr=0x8c10becau;*stop=0x8c10bec8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10beca; /* bsr 0x8c10c018 */
L8c10beca: *stop=0x8c10becau;r[0]=0u;goto L8c10becc; /* mov #0x0,r0 */
L8c10becc: *stop=0x8c10beccu;r[15]+=20u;goto L8c10bece; /* add #0x14,r15 */
L8c10bece: *stop=0x8c10beceu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10bed0; /* lds.l @r15+,PR */
L8c10bed0: *stop=0x8c10bed0u;if(*pr!=entry_pr)return -3;*stop=0x8c10bed2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_rtc_8c10c018(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10c018;
L8c10c018: *stop=0x8c10c018u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c038u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10c01a; /* mov.l 0x8c10c038,r2 */
L8c10c01a: *stop=0x8c10c01au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10c01c; /* mov.l @r2,r0 */
L8c10c01c: *stop=0x8c10c01cu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c10c01e; /* tst #0x8,r0 */
L8c10c01e: if((*sr&1u))goto L8c10c034;goto L8c10c020; /* bt 0x8c10c034 */
L8c10c020: *stop=0x8c10c020u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c040u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10c022; /* mov.l 0x8c10c040,r3 */
L8c10c022: *stop=0x8c10c022u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c03cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c10c024; /* mov.l 0x8c10c03c,r6 */
L8c10c024: *stop=0x8c10c024u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10c026; /* mov.l @r3,r1 */
L8c10c026: *stop=0x8c10c026u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10c028; /* mov.l @r6,r5 */
L8c10c028: *stop=0x8c10c028u;*sr=(*sr&~1u)|(r[5]>=r[1]);goto L8c10c02a; /* cmp/hs r1,r5 */
L8c10c02a: if((*sr&1u))goto L8c10c034;goto L8c10c02c; /* bt 0x8c10c034 */
L8c10c02c: *stop=0x8c10c02cu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c10c02e; /* mov.l r4,@r5 */
L8c10c02e: *stop=0x8c10c02eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10c030; /* mov.l @r6,r1 */
L8c10c030: *stop=0x8c10c030u;r[1]+=4u;goto L8c10c032; /* add #0x4,r1 */
L8c10c032: *stop=0x8c10c032u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[1],4);if(io_status)return io_status;goto L8c10c034; /* mov.l r1,@r6 */
L8c10c034: *stop=0x8c10c034u;if(*pr!=entry_pr)return -3;*stop=0x8c10c036u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_rtc_8c10d460(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d460;
L8c10d460: *stop=0x8c10d460u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10d462; /* mov.l r14,@-r15 */
L8c10d462: *stop=0x8c10d462u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10d464; /* mov.l r13,@-r15 */
L8c10d464: *stop=0x8c10d464u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10d466; /* mov.l r12,@-r15 */
L8c10d466: *stop=0x8c10d466u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10d468; /* mov.l r11,@-r15 */
L8c10d468: *stop=0x8c10d468u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10d46a; /* mov.l r10,@-r15 */
L8c10d46a: *stop=0x8c10d46au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10d46c; /* mov.l r9,@-r15 */
L8c10d46c: *stop=0x8c10d46cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d46e; /* sts.l PR,@-r15 */
L8c10d46e: *stop=0x8c10d46eu;r[15]+=4294967288u;goto L8c10d470; /* add #-0x8,r15 */
L8c10d470: *stop=0x8c10d470u;r[9]=r[5];goto L8c10d472; /* mov r5,r9 */
L8c10d472: *stop=0x8c10d472u;r[10]=r[4];goto L8c10d474; /* mov r4,r10 */
L8c10d474: *stop=0x8c10d474u;r[4]=4294967295u;goto L8c10d476; /* mov #-0x1,r4 */
L8c10d476: *stop=0x8c10d476u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c10d478; /* mov.l r6,@r15 */
L8c10d478: *stop=0x8c10d478u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d47a; /* mov.l @(0x24,r15),r0 */
L8c10d47a: *stop=0x8c10d47au;r[14]=r[7];goto L8c10d47c; /* mov r7,r14 */
L8c10d47c: *stop=0x8c10d47cu;r[5]=1u;goto L8c10d47e; /* mov #0x1,r5 */
L8c10d47e: *stop=0x8c10d47eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d480; /* cmp/eq #0x0,r0 */
L8c10d480: take=(*sr&1u);*stop=0x8c10d482u;r[11]=0u;if(take)goto L8c10d490;goto L8c10d484; /* bt/s 0x8c10d490 */
L8c10d484: *stop=0x8c10d484u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d486; /* cmp/eq #0x1,r0 */
L8c10d486: if((*sr&1u))goto L8c10d494;goto L8c10d488; /* bt 0x8c10d494 */
L8c10d488: *stop=0x8c10d488u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d48a; /* cmp/eq #0x2,r0 */
L8c10d48a: if((*sr&1u))goto L8c10d498;goto L8c10d48c; /* bt 0x8c10d498 */
L8c10d48c: *stop=0x8c10d48eu;goto L8c10d49a; /* bra 0x8c10d49a */
L8c10d490: *stop=0x8c10d492u;r[12]=r[11];goto L8c10d49a; /* bra 0x8c10d49a */
L8c10d494: *stop=0x8c10d496u;r[12]=r[5];goto L8c10d49a; /* bra 0x8c10d49a */
L8c10d498: *stop=0x8c10d498u;r[12]=r[4];goto L8c10d49a; /* mov r4,r12 */
L8c10d49a: *stop=0x8c10d49au;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d49c; /* mov.l @(0x28,r15),r0 */
L8c10d49c: *stop=0x8c10d49cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d49e; /* cmp/eq #0x0,r0 */
L8c10d49e: if((*sr&1u))goto L8c10d4ac;goto L8c10d4a0; /* bt 0x8c10d4ac */
L8c10d4a0: *stop=0x8c10d4a0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d4a2; /* cmp/eq #0x1,r0 */
L8c10d4a2: if((*sr&1u))goto L8c10d4b0;goto L8c10d4a4; /* bt 0x8c10d4b0 */
L8c10d4a4: *stop=0x8c10d4a4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d4a6; /* cmp/eq #0x2,r0 */
L8c10d4a6: if((*sr&1u))goto L8c10d4b4;goto L8c10d4a8; /* bt 0x8c10d4b4 */
L8c10d4a8: *stop=0x8c10d4aau;goto L8c10d4b6; /* bra 0x8c10d4b6 */
L8c10d4ac: *stop=0x8c10d4aeu;r[13]=r[11];goto L8c10d4b6; /* bra 0x8c10d4b6 */
L8c10d4b0: *stop=0x8c10d4b2u;r[13]=r[5];goto L8c10d4b6; /* bra 0x8c10d4b6 */
L8c10d4b4: *stop=0x8c10d4b4u;r[13]=r[4];goto L8c10d4b6; /* mov r4,r13 */
L8c10d4b6: *stop=0x8c10d4b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d538u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d4b8; /* mov.l 0x8c10d538,r3 */
L8c10d4b8: *stop=0x8c10d4b8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d532u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d4ba; /* mov.w 0x8c10d532,r1 */
L8c10d4ba: *stop=0x8c10d4bau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d4bc; /* mov.l @r3,r0 */
L8c10d4bc: *stop=0x8c10d4bcu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d4be; /* mov.l @(r0,r1),r0 */
L8c10d4be: *stop=0x8c10d4beu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d4c0; /* cmp/eq #0x1,r0 */
L8c10d4c0: if(!(*sr&1u))goto L8c10d4d6;goto L8c10d4c2; /* bf 0x8c10d4d6 */
L8c10d4c2: *stop=0x8c10d4c2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d4c4; /* stc SR,r0 */
L8c10d4c4: *stop=0x8c10d4c4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d534u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d4c6; /* mov.w 0x8c10d534,r3 */
L8c10d4c6: *stop=0x8c10d4c6u;r[0]>>=2;goto L8c10d4c8; /* shlr2 r0 */
L8c10d4c8: *stop=0x8c10d4c8u;r[0]>>=2;goto L8c10d4ca; /* shlr2 r0 */
L8c10d4ca: *stop=0x8c10d4cau;r[0]&=0xfu;goto L8c10d4cc; /* and #0xf,r0 */
L8c10d4cc: *stop=0x8c10d4ccu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10d4ce; /* mov.l r0,@(0x4,r15) */
L8c10d4ce: *stop=0x8c10d4ceu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d4d0; /* stc SR,r0 */
L8c10d4d0: *stop=0x8c10d4d0u;r[0]&=r[3];goto L8c10d4d2; /* and r3,r0 */
L8c10d4d2: *stop=0x8c10d4d2u;r[0]|=0xf0u;goto L8c10d4d4; /* or #0xf0,r0 */
L8c10d4d4: *stop=0x8c10d4d4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d4d6; /* ldc r0,SR */
L8c10d4d6: *stop=0x8c10d4d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d53cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d4d8; /* mov.l 0x8c10d53c,r2 */
L8c10d4d8: target=r[2];*pr=0x8c10d4dcu;*stop=0x8c10d4dau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d4dc; /* jsr @r2 */
L8c10d4dc: *stop=0x8c10d4dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d540u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d4de; /* mov.l 0x8c10d540,r1 */
L8c10d4de: target=r[1];*pr=0x8c10d4e2u;*stop=0x8c10d4e0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d4e2; /* jsr @r1 */
L8c10d4e2: *stop=0x8c10d4e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d4e4; /* mov.l @r15,r0 */
L8c10d4e4: *stop=0x8c10d4e4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d4e6; /* cmp/eq #0x1,r0 */
L8c10d4e6: if((*sr&1u))goto L8c10d4f4;goto L8c10d4e8; /* bt 0x8c10d4f4 */
L8c10d4e8: *stop=0x8c10d4e8u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d4ea; /* cmp/eq #0x2,r0 */
L8c10d4ea: if((*sr&1u))goto L8c10d510;goto L8c10d4ec; /* bt 0x8c10d510 */
L8c10d4ec: *stop=0x8c10d4ecu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c10d4ee; /* cmp/eq #0x4,r0 */
L8c10d4ee: if((*sr&1u))goto L8c10d544;goto L8c10d4f0; /* bt 0x8c10d544 */
L8c10d4f0: *stop=0x8c10d4f2u;goto L8c10d562; /* bra 0x8c10d562 */
L8c10d4f4: *stop=0x8c10d4f4u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c10d4f6; /* cmp/pl r14 */
L8c10d4f6: *stop=0x8c10d4f6u;r[5]=r[10];goto L8c10d4f8; /* mov r10,r5 */
L8c10d4f8: *stop=0x8c10d4f8u;r[6]=r[9];goto L8c10d4fa; /* mov r9,r6 */
L8c10d4fa: take=!(*sr&1u);*stop=0x8c10d4fcu;r[4]=r[11];if(take)goto L8c10d562;goto L8c10d4fe; /* bf/s 0x8c10d562 */
L8c10d4fe: *stop=0x8c10d4feu;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10d500; /* mov.b @r5,r2 */
L8c10d500: *stop=0x8c10d500u;r[4]+=1u;goto L8c10d502; /* add #0x1,r4 */
L8c10d502: *stop=0x8c10d502u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[14]);goto L8c10d504; /* cmp/ge r14,r4 */
L8c10d504: *stop=0x8c10d504u;r[5]+=r[12];goto L8c10d506; /* add r12,r5 */
L8c10d506: *stop=0x8c10d506u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[2],1);if(io_status)return io_status;goto L8c10d508; /* mov.b r2,@r6 */
L8c10d508: take=!(*sr&1u);*stop=0x8c10d50au;r[6]+=r[13];if(take)goto L8c10d4fe;goto L8c10d50c; /* bf/s 0x8c10d4fe */
L8c10d50c: *stop=0x8c10d50eu;goto L8c10d562; /* bra 0x8c10d562 */
L8c10d510: *stop=0x8c10d510u;r[7]=r[12];goto L8c10d512; /* mov r12,r7 */
L8c10d512: *stop=0x8c10d512u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c10d514; /* shll r7 */
L8c10d514: *stop=0x8c10d514u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c10d516; /* shll r13 */
L8c10d516: *stop=0x8c10d516u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c10d518; /* cmp/pl r14 */
L8c10d518: *stop=0x8c10d518u;r[6]=r[10];goto L8c10d51a; /* mov r10,r6 */
L8c10d51a: *stop=0x8c10d51au;r[4]=r[9];goto L8c10d51c; /* mov r9,r4 */
L8c10d51c: take=!(*sr&1u);*stop=0x8c10d51eu;r[5]=r[11];if(take)goto L8c10d562;goto L8c10d520; /* bf/s 0x8c10d562 */
L8c10d520: *stop=0x8c10d520u;if(!valid_access(r[6],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[2]=loaded;}goto L8c10d522; /* mov.w @r6,r2 */
L8c10d522: *stop=0x8c10d522u;r[5]+=1u;goto L8c10d524; /* add #0x1,r5 */
L8c10d524: *stop=0x8c10d524u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[14]);goto L8c10d526; /* cmp/ge r14,r5 */
L8c10d526: *stop=0x8c10d526u;r[6]+=r[7];goto L8c10d528; /* add r7,r6 */
L8c10d528: *stop=0x8c10d528u;if(!valid_access(r[4],2u))return -3;wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c10d52a; /* mov.w r2,@r4 */
L8c10d52a: take=!(*sr&1u);*stop=0x8c10d52cu;r[4]+=r[13];if(take)goto L8c10d520;goto L8c10d52e; /* bf/s 0x8c10d520 */
L8c10d52e: *stop=0x8c10d530u;goto L8c10d562; /* bra 0x8c10d562 */
L8c10d544: *stop=0x8c10d544u;r[7]=r[12];goto L8c10d546; /* mov r12,r7 */
L8c10d546: *stop=0x8c10d546u;r[7]<<=2;goto L8c10d548; /* shll2 r7 */
L8c10d548: *stop=0x8c10d548u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c10d54a; /* cmp/pl r14 */
L8c10d54a: *stop=0x8c10d54au;r[4]=r[9];goto L8c10d54c; /* mov r9,r4 */
L8c10d54c: *stop=0x8c10d54cu;r[5]=r[10];goto L8c10d54e; /* mov r10,r5 */
L8c10d54e: *stop=0x8c10d54eu;r[6]=r[11];goto L8c10d550; /* mov r11,r6 */
L8c10d550: take=!(*sr&1u);*stop=0x8c10d552u;r[13]<<=2;if(take)goto L8c10d562;goto L8c10d554; /* bf/s 0x8c10d562 */
L8c10d554: *stop=0x8c10d554u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d556; /* mov.l @r5,r2 */
L8c10d556: *stop=0x8c10d556u;r[6]+=1u;goto L8c10d558; /* add #0x1,r6 */
L8c10d558: *stop=0x8c10d558u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[14]);goto L8c10d55a; /* cmp/ge r14,r6 */
L8c10d55a: *stop=0x8c10d55au;r[5]+=r[7];goto L8c10d55c; /* add r7,r5 */
L8c10d55c: *stop=0x8c10d55cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c10d55e; /* mov.l r2,@r4 */
L8c10d55e: take=!(*sr&1u);*stop=0x8c10d560u;r[4]+=r[13];if(take)goto L8c10d554;goto L8c10d562; /* bf/s 0x8c10d554 */
L8c10d562: *stop=0x8c10d562u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d5a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d564; /* mov.l 0x8c10d5a0,r3 */
L8c10d564: target=r[3];*pr=0x8c10d568u;*stop=0x8c10d566u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d568; /* jsr @r3 */
L8c10d568: *stop=0x8c10d568u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d5a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d56a; /* mov.l 0x8c10d5a4,r3 */
L8c10d56a: *stop=0x8c10d56au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d59au,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d56c; /* mov.w 0x8c10d59a,r1 */
L8c10d56c: *stop=0x8c10d56cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d56e; /* mov.l @r3,r0 */
L8c10d56e: *stop=0x8c10d56eu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d570; /* mov.l @(r0,r1),r0 */
L8c10d570: *stop=0x8c10d570u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d572; /* cmp/eq #0x1,r0 */
L8c10d572: if(!(*sr&1u))goto L8c10d586;goto L8c10d574; /* bf 0x8c10d586 */
L8c10d574: *stop=0x8c10d574u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d576; /* mov.l @(0x4,r15),r0 */
L8c10d576: *stop=0x8c10d576u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c10d578; /* stc SR,r2 */
L8c10d578: *stop=0x8c10d578u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d59cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d57a; /* mov.w 0x8c10d59c,r3 */
L8c10d57a: *stop=0x8c10d57au;r[0]&=0xfu;goto L8c10d57c; /* and #0xf,r0 */
L8c10d57c: *stop=0x8c10d57cu;r[0]<<=2;goto L8c10d57e; /* shll2 r0 */
L8c10d57e: *stop=0x8c10d57eu;r[2]&=r[3];goto L8c10d580; /* and r3,r2 */
L8c10d580: *stop=0x8c10d580u;r[0]<<=2;goto L8c10d582; /* shll2 r0 */
L8c10d582: *stop=0x8c10d582u;r[0]|=r[2];goto L8c10d584; /* or r2,r0 */
L8c10d584: *stop=0x8c10d584u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d586; /* ldc r0,SR */
L8c10d586: *stop=0x8c10d586u;r[0]=0u;goto L8c10d588; /* mov #0x0,r0 */
L8c10d588: *stop=0x8c10d588u;r[15]+=8u;goto L8c10d58a; /* add #0x8,r15 */
L8c10d58a: *stop=0x8c10d58au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d58c; /* lds.l @r15+,PR */
L8c10d58c: *stop=0x8c10d58cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10d58e; /* mov.l @r15+,r9 */
L8c10d58e: *stop=0x8c10d58eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10d590; /* mov.l @r15+,r10 */
L8c10d590: *stop=0x8c10d590u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10d592; /* mov.l @r15+,r11 */
L8c10d592: *stop=0x8c10d592u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10d594; /* mov.l @r15+,r12 */
L8c10d594: *stop=0x8c10d594u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10d596; /* mov.l @r15+,r13 */
L8c10d596: *stop=0x8c10d596u;if(*pr!=entry_pr)return -3;*stop=0x8c10d598u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_rtc_8c10bd96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10bd96;
L8c10bd96: *stop=0x8c10bd96u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10bd98; /* mov.l r14,@-r15 */
L8c10bd98: *stop=0x8c10bd98u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10bd9a; /* mov.l r13,@-r15 */
L8c10bd9a: *stop=0x8c10bd9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10bd9c; /* mov.l r12,@-r15 */
L8c10bd9c: *stop=0x8c10bd9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10bd9e; /* mov.l r11,@-r15 */
L8c10bd9e: *stop=0x8c10bd9eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10bda0; /* mov.l r10,@-r15 */
L8c10bda0: *stop=0x8c10bda0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10bda2; /* mov.l r9,@-r15 */
L8c10bda2: *stop=0x8c10bda2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10bda4; /* sts.l PR,@-r15 */
L8c10bda4: *stop=0x8c10bda4u;r[15]+=4294967284u;goto L8c10bda6; /* add #-0xc,r15 */
L8c10bda6: *stop=0x8c10bda6u;r[11]=r[15];goto L8c10bda8; /* mov r15,r11 */
L8c10bda8: *stop=0x8c10bda8u;r[11]+=4u;goto L8c10bdaa; /* add #0x4,r11 */
L8c10bdaa: *stop=0x8c10bdaau;r[13]=r[11];goto L8c10bdac; /* mov r11,r13 */
L8c10bdac: *stop=0x8c10bdacu;r[14]=r[13];goto L8c10bdae; /* mov r13,r14 */
L8c10bdae: *stop=0x8c10bdaeu;r[10]=r[13];goto L8c10bdb0; /* mov r13,r10 */
L8c10bdb0: *stop=0x8c10bdb0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10bdb2; /* mov.l r13,@r15 */
L8c10bdb2: *stop=0x8c10bdb2u;r[12]=r[4];goto L8c10bdb4; /* mov r4,r12 */
L8c10bdb4: *stop=0x8c10bdb4u;r[9]=r[4];goto L8c10bdb6; /* mov r4,r9 */
L8c10bdb6: *stop=0x8c10bdb8u;r[10]+=8u;goto L8c10bdc8; /* bra 0x8c10bdc8 */
L8c10bdba: target=0x8c10be78u;*pr=0x8c10bdbeu;*stop=0x8c10bdbcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10bdbe; /* bsr 0x8c10be78 */
L8c10bdbe: *stop=0x8c10bdbeu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10bdc0; /* tst r0,r0 */
L8c10bdc0: if((*sr&1u))goto L8c10bdc4;goto L8c10bdc2; /* bt 0x8c10bdc4 */
L8c10bdc2: *stop=0x8c10bdc2u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[12],4);if(io_status)return io_status;goto L8c10bdc4; /* mov.l r12,@r13 */
L8c10bdc4: *stop=0x8c10bdc4u;r[14]+=4u;goto L8c10bdc6; /* add #0x4,r14 */
L8c10bdc6: *stop=0x8c10bdc6u;r[13]+=4u;goto L8c10bdc8; /* add #0x4,r13 */
L8c10bdc8: *stop=0x8c10bdc8u;*sr=(*sr&~1u)|(r[14]>=r[10]);goto L8c10bdca; /* cmp/hs r10,r14 */
L8c10bdca: if(!(*sr&1u))goto L8c10bdba;goto L8c10bdcc; /* bf 0x8c10bdba */
L8c10bdcc: *stop=0x8c10bdccu;r[14]=r[11];goto L8c10bdce; /* mov r11,r14 */
L8c10bdce: *stop=0x8c10bdceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10bdd0; /* mov.l @r15,r4 */
L8c10bdd0: *stop=0x8c10bdd0u;r[14]+=4u;goto L8c10bdd2; /* add #0x4,r14 */
L8c10bdd2: *stop=0x8c10bdd2u;*sr=(*sr&~1u)|(r[4]>=r[14]);goto L8c10bdd4; /* cmp/hs r14,r4 */
L8c10bdd4: *stop=0x8c10bdd4u;r[6]=0u;goto L8c10bdd6; /* mov #0x0,r6 */
L8c10bdd6: take=(*sr&1u);*stop=0x8c10bdd8u;r[5]=r[14];if(take)goto L8c10bdf0;goto L8c10bdda; /* bt/s 0x8c10bdf0 */
L8c10bdda: *stop=0x8c10bddau;r[7]=1u;goto L8c10bddc; /* mov #0x1,r7 */
L8c10bddc: *stop=0x8c10bddcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10bdde; /* mov.l @r4,r3 */
L8c10bdde: *stop=0x8c10bddeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10bde0; /* mov.l @r5,r2 */
L8c10bde0: *stop=0x8c10bde0u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c10bde2; /* cmp/eq r2,r3 */
L8c10bde2: if((*sr&1u))goto L8c10bde8;goto L8c10bde4; /* bt 0x8c10bde8 */
L8c10bde4: *stop=0x8c10bde6u;r[6]=r[7];goto L8c10bdf0; /* bra 0x8c10bdf0 */
L8c10bde8: *stop=0x8c10bde8u;r[4]+=4u;goto L8c10bdea; /* add #0x4,r4 */
L8c10bdea: *stop=0x8c10bdeau;*sr=(*sr&~1u)|(r[4]>=r[14]);goto L8c10bdec; /* cmp/hs r14,r4 */
L8c10bdec: take=!(*sr&1u);*stop=0x8c10bdeeu;r[5]+=4u;if(take)goto L8c10bddc;goto L8c10bdf0; /* bf/s 0x8c10bddc */
L8c10bdf0: *stop=0x8c10bdf0u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c10bdf2; /* tst r6,r6 */
L8c10bdf2: if(!(*sr&1u))goto L8c10bdf8;goto L8c10bdf4; /* bf 0x8c10bdf8 */
L8c10bdf4: *stop=0x8c10bdf4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}goto L8c10bdf6; /* mov.l @r15,r9 */
L8c10bdf6: *stop=0x8c10bdf6u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[9]=loaded;}goto L8c10bdf8; /* mov.l @r9,r9 */
L8c10bdf8: *stop=0x8c10bdf8u;r[0]=r[9];goto L8c10bdfa; /* mov r9,r0 */
L8c10bdfa: *stop=0x8c10bdfau;r[15]+=12u;goto L8c10bdfc; /* add #0xc,r15 */
L8c10bdfc: *stop=0x8c10bdfcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10bdfe; /* lds.l @r15+,PR */
L8c10bdfe: *stop=0x8c10bdfeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10be00; /* mov.l @r15+,r9 */
L8c10be00: *stop=0x8c10be00u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10be02; /* mov.l @r15+,r10 */
L8c10be02: *stop=0x8c10be02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10be04; /* mov.l @r15+,r11 */
L8c10be04: *stop=0x8c10be04u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10be06; /* mov.l @r15+,r12 */
L8c10be06: *stop=0x8c10be06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10be08; /* mov.l @r15+,r13 */
L8c10be08: *stop=0x8c10be08u;if(*pr!=entry_pr)return -3;*stop=0x8c10be0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
