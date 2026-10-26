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
SA_STAGE_EXPORT int sa_movie_midi_commands_8c643b9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c643b9c;
L8c643b9c: *stop=0x8c643b9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643bf4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643b9e; /* mov.l 0x8c643bf4,r2 */
L8c643b9e: *stop=0x8c643b9eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643ba0; /* mov.l r14,@-r15 */
L8c643ba0: *stop=0x8c643ba0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643ba2; /* sts.l PR,@-r15 */
L8c643ba2: *stop=0x8c643ba2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643ba4; /* mov.l @r2,r3 */
L8c643ba4: *stop=0x8c643ba4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c643ba6; /* tst r3,r3 */
L8c643ba6: if(!(*sr&1u))goto L8c643bae;goto L8c643ba8; /* bf 0x8c643bae */
L8c643ba8: *stop=0x8c643ba8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643bf8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643baa; /* mov.l 0x8c643bf8,r14 */
L8c643baa: *stop=0x8c643bacu;goto L8c643bde; /* bra 0x8c643bde */
L8c643bae: *stop=0x8c643baeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c643bb0; /* tst r4,r4 */
L8c643bb0: if(!(*sr&1u))goto L8c643bb8;goto L8c643bb2; /* bf 0x8c643bb8 */
L8c643bb2: *stop=0x8c643bb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643bfcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643bb4; /* mov.l 0x8c643bfc,r14 */
L8c643bb4: *stop=0x8c643bb6u;goto L8c643bde; /* bra 0x8c643bde */
L8c643bb8: *stop=0x8c643bb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643bba; /* mov.l 0x8c643c00,r2 */
L8c643bba: *stop=0x8c643bbau;{uint32_t loaded=sx(m,bus,&io_status,0x8c643bf2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c643bbc; /* mov.w 0x8c643bf2,r3 */
L8c643bbc: *stop=0x8c643bbcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c04u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643bbe; /* mov.l 0x8c643c04,r1 */
L8c643bbe: *stop=0x8c643bbeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c643bc0; /* mov.l r3,@r2 */
L8c643bc0: *stop=0x8c643bc0u;r[2]+=9u;goto L8c643bc2; /* add #0x9,r2 */
L8c643bc2: *stop=0x8c643bc2u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}goto L8c643bc4; /* mov.b @r5,r3 */
L8c643bc4: *stop=0x8c643bc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643bc6; /* mov.l 0x8c643c00,r4 */
L8c643bc6: *stop=0x8c643bc6u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[3],1);if(io_status)return io_status;goto L8c643bc8; /* mov.b r3,@r1 */
L8c643bc8: *stop=0x8c643bc8u;if(!valid_access(r[5]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643bca; /* mov.b @(0x1,r5),r0 */
L8c643bca: *stop=0x8c643bcau;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643bcc; /* mov.l 0x8c643c08,r3 */
L8c643bcc: *stop=0x8c643bccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c0cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643bce; /* mov.l 0x8c643c0c,r1 */
L8c643bce: *stop=0x8c643bceu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[0],1);if(io_status)return io_status;goto L8c643bd0; /* mov.b r0,@r3 */
L8c643bd0: *stop=0x8c643bd0u;r[3]+=1u;goto L8c643bd2; /* add #0x1,r3 */
L8c643bd2: *stop=0x8c643bd2u;if(!valid_access(r[5]+2u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5]+0x2u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643bd4; /* mov.b @(0x2,r5),r0 */
L8c643bd4: *stop=0x8c643bd4u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[0],1);if(io_status)return io_status;goto L8c643bd6; /* mov.b r0,@r3 */
L8c643bd6: *stop=0x8c643bd6u;if(!valid_access(r[5]+3u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5]+0x3u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643bd8; /* mov.b @(0x3,r5),r0 */
L8c643bd8: target=r[1];*pr=0x8c643bdcu;*stop=0x8c643bdau;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[0],1);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643bdc; /* jsr @r1 */
L8c643bdc: *stop=0x8c643bdcu;r[14]=r[0];goto L8c643bde; /* mov r0,r14 */
L8c643bde: *stop=0x8c643bdeu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643be0; /* tst r14,r14 */
L8c643be0: if((*sr&1u))goto L8c643bea;goto L8c643be2; /* bt 0x8c643bea */
L8c643be2: *stop=0x8c643be2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643be4; /* mov.l 0x8c643c14,r2 */
L8c643be4: *stop=0x8c643be4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643c10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643be6; /* mov.l 0x8c643c10,r4 */
L8c643be6: target=r[2];*pr=0x8c643beau;*stop=0x8c643be8u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643bea; /* jsr @r2 */
L8c643bea: *stop=0x8c643beau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643bec; /* lds.l @r15+,PR */
L8c643bec: *stop=0x8c643becu;r[0]=r[14];goto L8c643bee; /* mov r14,r0 */
L8c643bee: *stop=0x8c643beeu;if(*pr!=entry_pr)return -3;*stop=0x8c643bf0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_midi_commands_8c608c14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608c14;
L8c608c14: *stop=0x8c608c14u;r[5]>>=8;goto L8c608c16; /* shlr8 r5 */
L8c608c16: *stop=0x8c608c16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608ce4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608c18; /* mov.l 0x8c608ce4,r3 */
L8c608c18: *stop=0x8c608c18u;r[5]>>=2;goto L8c608c1a; /* shlr2 r5 */
L8c608c1a: *stop=0x8c608c1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c608ce8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608c1c; /* mov.l 0x8c608ce8,r1 */
L8c608c1c: *stop=0x8c608c1cu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c608c1e; /* shlr r5 */
L8c608c1e: *stop=0x8c608c1eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c608c20; /* mov.l r4,@r3 */
L8c608c20: *stop=0x8c608c20u;r[5]<<=8;goto L8c608c22; /* shll8 r5 */
L8c608c22: *stop=0x8c608c22u;r[5]<<=2;goto L8c608c24; /* shll2 r5 */
L8c608c24: *stop=0x8c608c24u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c608c26; /* shll r5 */
L8c608c26: *stop=0x8c608c26u;if(*pr!=entry_pr)return -3;*stop=0x8c608c28u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;return 0; /* rts */
}
