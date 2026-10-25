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
SA_STAGE_EXPORT int sa_movie_heap_8c6021d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6021d2;
L8c6021d2: *stop=0x8c6021d4u;if(!call)return -2;return call(ctx,0x8c6021d6u,m,f,sr,r,pr); /* bra 0x8c6021d6 */
}

SA_STAGE_EXPORT int sa_movie_heap_8c6021d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6021d6;
L8c6021d6: *stop=0x8c6021d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6021d8; /* mov.l r14,@-r15 */
L8c6021d8: *stop=0x8c6021d8u;r[15]+=4294967292u;goto L8c6021da; /* add #-0x4,r15 */
L8c6021da: *stop=0x8c6021dau;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6021dc; /* stc SR,r0 */
L8c6021dc: *stop=0x8c6021dcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c602270u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6021de; /* mov.w 0x8c602270,r3 */
L8c6021de: *stop=0x8c6021deu;r[0]>>=2;goto L8c6021e0; /* shlr2 r0 */
L8c6021e0: *stop=0x8c6021e0u;r[0]>>=2;goto L8c6021e2; /* shlr2 r0 */
L8c6021e2: *stop=0x8c6021e2u;r[0]&=0xfu;goto L8c6021e4; /* and #0xf,r0 */
L8c6021e4: *stop=0x8c6021e4u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c6021e6; /* mov.w r0,@r15 */
L8c6021e6: *stop=0x8c6021e6u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6021e8; /* stc SR,r0 */
L8c6021e8: *stop=0x8c6021e8u;r[0]&=r[3];goto L8c6021ea; /* and r3,r0 */
L8c6021ea: *stop=0x8c6021eau;r[0]|=0xf0u;goto L8c6021ec; /* or #0xf0,r0 */
L8c6021ec: *stop=0x8c6021ecu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6021ee; /* ldc r0,SR */
L8c6021ee: *stop=0x8c6021eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602284u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6021f0; /* mov.l 0x8c602284,r14 */
L8c6021f0: *stop=0x8c6021f0u;r[5]=r[4];goto L8c6021f2; /* mov r4,r5 */
L8c6021f2: *stop=0x8c6021f2u;r[5]+=31u;goto L8c6021f4; /* add #0x1f,r5 */
L8c6021f4: *stop=0x8c6021f4u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[7]=loaded;}goto L8c6021f6; /* mov.w @r15,r7 */
L8c6021f6: *stop=0x8c6021f6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6021f8; /* mov.l @r14,r1 */
L8c6021f8: *stop=0x8c6021f8u;r[5]>>=2;goto L8c6021fa; /* shlr2 r5 */
L8c6021fa: *stop=0x8c6021fau;r[5]>>=2;goto L8c6021fc; /* shlr2 r5 */
L8c6021fc: *stop=0x8c6021fcu;r[6]=r[1];goto L8c6021fe; /* mov r1,r6 */
L8c6021fe: *stop=0x8c6021feu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[4]=loaded;}goto L8c602200; /* mov.l @r6,r4 */
L8c602200: *stop=0x8c602200u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c602202; /* shlr r5 */
L8c602202: *stop=0x8c602202u;r[7]=r[7]&65535u;goto L8c602204; /* extu.w r7,r7 */
L8c602204: *stop=0x8c602204u;r[5]+=1u;goto L8c602206; /* add #0x1,r5 */
L8c602206: *stop=0x8c602206u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602208; /* mov.l @(0x4,r4),r2 */
L8c602208: *stop=0x8c602208u;*sr=(*sr&~1u)|(r[2]>=r[5]);goto L8c60220a; /* cmp/hs r5,r2 */
L8c60220a: if(!(*sr&1u))goto L8c602246;goto L8c60220c; /* bf 0x8c602246 */
L8c60220c: *stop=0x8c60220cu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60220e; /* mov.l @(0x4,r4),r1 */
L8c60220e: *stop=0x8c60220eu;*sr=(*sr&~1u)|(r[5]==r[1]);goto L8c602210; /* cmp/eq r5,r1 */
L8c602210: if(!(*sr&1u))goto L8c602218;goto L8c602212; /* bf 0x8c602218 */
L8c602212: *stop=0x8c602212u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c602214; /* mov.l @r4,r2 */
L8c602214: *stop=0x8c602216u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c602228; /* bra 0x8c602228 */
L8c602218: *stop=0x8c602218u;r[1]-=r[5];goto L8c60221a; /* sub r5,r1 */
L8c60221a: *stop=0x8c60221au;r[3]=r[1];goto L8c60221c; /* mov r1,r3 */
L8c60221c: *stop=0x8c60221cu;r[3]<<=2;goto L8c60221e; /* shll2 r3 */
L8c60221e: *stop=0x8c60221eu;r[3]<<=2;goto L8c602220; /* shll2 r3 */
L8c602220: *stop=0x8c602220u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[1],4);if(io_status)return io_status;goto L8c602222; /* mov.l r1,@(0x4,r4) */
L8c602222: *stop=0x8c602222u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c602224; /* shll r3 */
L8c602224: *stop=0x8c602224u;r[4]+=r[3];goto L8c602226; /* add r3,r4 */
L8c602226: *stop=0x8c602226u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c602228; /* mov.l r5,@(0x4,r4) */
L8c602228: *stop=0x8c602228u;r[0]=r[7];goto L8c60222a; /* mov r7,r0 */
L8c60222a: *stop=0x8c60222au;{uint32_t loaded=sx(m,bus,&io_status,0x8c602270u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60222c; /* mov.w 0x8c602270,r3 */
L8c60222c: *stop=0x8c60222cu;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c60222e; /* stc SR,r2 */
L8c60222e: *stop=0x8c60222eu;r[0]&=0xfu;goto L8c602230; /* and #0xf,r0 */
L8c602230: *stop=0x8c602230u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[6],4);if(io_status)return io_status;goto L8c602232; /* mov.l r6,@r14 */
L8c602232: *stop=0x8c602232u;r[0]<<=2;goto L8c602234; /* shll2 r0 */
L8c602234: *stop=0x8c602234u;r[2]&=r[3];goto L8c602236; /* and r3,r2 */
L8c602236: *stop=0x8c602236u;r[0]<<=2;goto L8c602238; /* shll2 r0 */
L8c602238: *stop=0x8c602238u;r[0]|=r[2];goto L8c60223a; /* or r2,r0 */
L8c60223a: *stop=0x8c60223au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60223c; /* ldc r0,SR */
L8c60223c: *stop=0x8c60223cu;r[0]=r[4];goto L8c60223e; /* mov r4,r0 */
L8c60223e: *stop=0x8c60223eu;r[0]+=32u;goto L8c602240; /* add #0x20,r0 */
L8c602240: *stop=0x8c602240u;r[15]+=4u;goto L8c602242; /* add #0x4,r15 */
L8c602242: *stop=0x8c602242u;if(*pr!=entry_pr)return -3;*stop=0x8c602244u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c602246: *stop=0x8c602246u;*sr=(*sr&~1u)|(r[1]==r[4]);goto L8c602248; /* cmp/eq r1,r4 */
L8c602248: if(!(*sr&1u))goto L8c602264;goto L8c60224a; /* bf 0x8c602264 */
L8c60224a: *stop=0x8c60224au;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c60224c; /* stc SR,r3 */
L8c60224c: *stop=0x8c60224cu;r[0]=r[7];goto L8c60224e; /* mov r7,r0 */
L8c60224e: *stop=0x8c60224eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c602270u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c602250; /* mov.w 0x8c602270,r2 */
L8c602250: *stop=0x8c602250u;r[0]&=0xfu;goto L8c602252; /* and #0xf,r0 */
L8c602252: *stop=0x8c602252u;r[0]<<=2;goto L8c602254; /* shll2 r0 */
L8c602254: *stop=0x8c602254u;r[3]&=r[2];goto L8c602256; /* and r2,r3 */
L8c602256: *stop=0x8c602256u;r[0]<<=2;goto L8c602258; /* shll2 r0 */
L8c602258: *stop=0x8c602258u;r[0]|=r[3];goto L8c60225a; /* or r3,r0 */
L8c60225a: *stop=0x8c60225au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60225c; /* ldc r0,SR */
L8c60225c: *stop=0x8c60225cu;r[0]=0u;goto L8c60225e; /* mov #0x0,r0 */
L8c60225e: *stop=0x8c60225eu;r[15]+=4u;goto L8c602260; /* add #0x4,r15 */
L8c602260: *stop=0x8c602260u;if(*pr!=entry_pr)return -3;*stop=0x8c602262u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c602264: *stop=0x8c602264u;r[6]=r[4];goto L8c602266; /* mov r4,r6 */
L8c602266: *stop=0x8c602268u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c602206; /* bra 0x8c602206 */
}

SA_STAGE_EXPORT int sa_movie_heap_8c602128(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602128;
L8c602128: *stop=0x8c602128u;r[4]+=31u;goto L8c60212a; /* add #0x1f,r4 */
L8c60212a: *stop=0x8c60212au;{uint32_t loaded=rd(m,bus,&io_status,0x8c602278u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60212c; /* mov.l 0x8c602278,r3 */
L8c60212c: *stop=0x8c60212cu;r[6]=4294967264u;goto L8c60212e; /* mov #-0x20,r6 */
L8c60212e: *stop=0x8c60212eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602274u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c602130; /* mov.l 0x8c602274,r7 */
L8c602130: *stop=0x8c602130u;r[4]&=r[6];goto L8c602132; /* and r6,r4 */
L8c602132: *stop=0x8c602132u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60227cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602134; /* mov.l 0x8c60227c,r2 */
L8c602134: *stop=0x8c602134u;r[5]&=r[6];goto L8c602136; /* and r6,r5 */
L8c602136: *stop=0x8c602136u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c602138; /* mov.l r14,@-r15 */
L8c602138: *stop=0x8c602138u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[4],4);if(io_status)return io_status;goto L8c60213a; /* mov.l r4,@r7 */
L8c60213a: *stop=0x8c60213au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c60213c; /* mov.l r4,@r3 */
L8c60213c: *stop=0x8c60213cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602280u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c60213e; /* mov.l 0x8c602280,r4 */
L8c60213e: *stop=0x8c60213eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602284u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602140; /* mov.l 0x8c602284,r3 */
L8c602140: *stop=0x8c602140u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c602142; /* mov.l r5,@r2 */
L8c602142: *stop=0x8c602142u;r[2]=0u;goto L8c602144; /* mov #0x0,r2 */
L8c602144: *stop=0x8c602144u;r[5]>>=2;goto L8c602146; /* shlr2 r5 */
L8c602146: *stop=0x8c602146u;r[1]=r[4];goto L8c602148; /* mov r4,r1 */
L8c602148: *stop=0x8c602148u;r[5]>>=2;goto L8c60214a; /* shlr2 r5 */
L8c60214a: *stop=0x8c60214au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c60214c; /* mov.l r4,@r3 */
L8c60214c: *stop=0x8c60214cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c60214e; /* mov.l r1,@r4 */
L8c60214e: *stop=0x8c60214eu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c602150; /* shlr r5 */
L8c602150: *stop=0x8c602150u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[2],4);if(io_status)return io_status;goto L8c602152; /* mov.l r2,@(0x4,r4) */
L8c602152: *stop=0x8c602152u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[14]=loaded;}goto L8c602154; /* mov.l @r7,r14 */
L8c602154: *stop=0x8c602154u;r[4]=r[14];goto L8c602156; /* mov r14,r4 */
L8c602156: *stop=0x8c602156u;r[4]+=32u;goto L8c602158; /* add #0x20,r4 */
L8c602158: *stop=0x8c602158u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c60215a; /* mov.l r5,@(0x4,r14) */
L8c60215a: *stop=0x8c60215cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,0x8c602288u,m,f,sr,r,pr); /* bra 0x8c602288 */
}

SA_STAGE_EXPORT int sa_movie_heap_8c602288(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602288;
L8c602288: *stop=0x8c602288u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c60228a; /* stc SR,r0 */
L8c60228a: *stop=0x8c60228au;{uint32_t loaded=sx(m,bus,&io_status,0x8c60238au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60228c; /* mov.w 0x8c60238a,r3 */
L8c60228c: *stop=0x8c60228cu;r[0]>>=2;goto L8c60228e; /* shlr2 r0 */
L8c60228e: *stop=0x8c60228eu;r[0]>>=2;goto L8c602290; /* shlr2 r0 */
L8c602290: *stop=0x8c602290u;r[0]&=0xfu;goto L8c602292; /* and #0xf,r0 */
L8c602292: *stop=0x8c602292u;r[7]=r[0];goto L8c602294; /* mov r0,r7 */
L8c602294: *stop=0x8c602294u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c602296; /* stc SR,r0 */
L8c602296: *stop=0x8c602296u;r[0]&=r[3];goto L8c602298; /* and r3,r0 */
L8c602298: *stop=0x8c602298u;r[0]|=0xf0u;goto L8c60229a; /* or #0xf0,r0 */
L8c60229a: *stop=0x8c60229au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60229c; /* ldc r0,SR */
L8c60229c: *stop=0x8c60229cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60229e; /* tst r4,r4 */
L8c60229e: if((*sr&1u))goto L8c60230c;goto L8c6022a0; /* bt 0x8c60230c */
L8c6022a0: *stop=0x8c6022a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60238cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6022a2; /* mov.l 0x8c60238c,r3 */
L8c6022a2: *stop=0x8c6022a2u;r[5]=r[4];goto L8c6022a4; /* mov r4,r5 */
L8c6022a4: *stop=0x8c6022a4u;r[5]+=4294967264u;goto L8c6022a6; /* add #-0x20,r5 */
L8c6022a6: *stop=0x8c6022a8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6022ba; /* bra 0x8c6022ba */
L8c6022aa: *stop=0x8c6022aau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6022ac; /* mov.l @r4,r6 */
L8c6022ac: *stop=0x8c6022acu;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c6022ae; /* cmp/hs r6,r4 */
L8c6022ae: if(!(*sr&1u))goto L8c6022b8;goto L8c6022b0; /* bf 0x8c6022b8 */
L8c6022b0: *stop=0x8c6022b0u;*sr=(*sr&~1u)|(r[5]>r[4]);goto L8c6022b2; /* cmp/hi r4,r5 */
L8c6022b2: if((*sr&1u))goto L8c6022c4;goto L8c6022b4; /* bt 0x8c6022c4 */
L8c6022b4: *stop=0x8c6022b4u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c6022b6; /* cmp/hs r6,r5 */
L8c6022b6: if(!(*sr&1u))goto L8c6022c4;goto L8c6022b8; /* bf 0x8c6022c4 */
L8c6022b8: *stop=0x8c6022b8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6022ba; /* mov.l @r4,r4 */
L8c6022ba: *stop=0x8c6022bau;*sr=(*sr&~1u)|(r[5]>r[4]);goto L8c6022bc; /* cmp/hi r4,r5 */
L8c6022bc: if(!(*sr&1u))goto L8c6022aa;goto L8c6022be; /* bf 0x8c6022aa */
L8c6022be: *stop=0x8c6022beu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6022c0; /* mov.l @r4,r2 */
L8c6022c0: *stop=0x8c6022c0u;*sr=(*sr&~1u)|(r[5]>=r[2]);goto L8c6022c2; /* cmp/hs r2,r5 */
L8c6022c2: if((*sr&1u))goto L8c6022aa;goto L8c6022c4; /* bt 0x8c6022aa */
L8c6022c4: *stop=0x8c6022c4u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6022c6; /* mov.l @(0x4,r5),r1 */
L8c6022c6: *stop=0x8c6022c6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6022c8; /* mov.l @r4,r3 */
L8c6022c8: *stop=0x8c6022c8u;r[1]<<=2;goto L8c6022ca; /* shll2 r1 */
L8c6022ca: *stop=0x8c6022cau;r[1]<<=2;goto L8c6022cc; /* shll2 r1 */
L8c6022cc: *stop=0x8c6022ccu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c6022ce; /* shll r1 */
L8c6022ce: *stop=0x8c6022ceu;r[1]+=r[5];goto L8c6022d0; /* add r5,r1 */
L8c6022d0: *stop=0x8c6022d0u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c6022d2; /* cmp/eq r3,r1 */
L8c6022d2: if(!(*sr&1u))goto L8c6022e6;goto L8c6022d4; /* bf 0x8c6022e6 */
L8c6022d4: *stop=0x8c6022d4u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6022d6; /* mov.l @r4,r1 */
L8c6022d6: *stop=0x8c6022d6u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6022d8; /* mov.l @(0x4,r5),r2 */
L8c6022d8: *stop=0x8c6022d8u;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6022da; /* mov.l @(0x4,r1),r3 */
L8c6022da: *stop=0x8c6022dau;r[2]+=r[3];goto L8c6022dc; /* add r3,r2 */
L8c6022dc: *stop=0x8c6022dcu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6022de; /* mov.l r2,@(0x4,r5) */
L8c6022de: *stop=0x8c6022deu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6022e0; /* mov.l @r4,r1 */
L8c6022e0: *stop=0x8c6022e0u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6022e2; /* mov.l @r1,r3 */
L8c6022e2: *stop=0x8c6022e4u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6022ea; /* bra 0x8c6022ea */
L8c6022e6: *stop=0x8c6022e6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6022e8; /* mov.l @r4,r1 */
L8c6022e8: *stop=0x8c6022e8u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c6022ea; /* mov.l r1,@r5 */
L8c6022ea: *stop=0x8c6022eau;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6022ec; /* mov.l @(0x4,r4),r3 */
L8c6022ec: *stop=0x8c6022ecu;r[3]<<=2;goto L8c6022ee; /* shll2 r3 */
L8c6022ee: *stop=0x8c6022eeu;r[3]<<=2;goto L8c6022f0; /* shll2 r3 */
L8c6022f0: *stop=0x8c6022f0u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6022f2; /* shll r3 */
L8c6022f2: *stop=0x8c6022f2u;r[3]+=r[4];goto L8c6022f4; /* add r4,r3 */
L8c6022f4: *stop=0x8c6022f4u;*sr=(*sr&~1u)|(r[5]==r[3]);goto L8c6022f6; /* cmp/eq r5,r3 */
L8c6022f6: if(!(*sr&1u))goto L8c602306;goto L8c6022f8; /* bf 0x8c602306 */
L8c6022f8: *stop=0x8c6022f8u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6022fa; /* mov.l @(0x4,r4),r2 */
L8c6022fa: *stop=0x8c6022fau;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6022fc; /* mov.l @(0x4,r5),r3 */
L8c6022fc: *stop=0x8c6022fcu;r[2]+=r[3];goto L8c6022fe; /* add r3,r2 */
L8c6022fe: *stop=0x8c6022feu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[2],4);if(io_status)return io_status;goto L8c602300; /* mov.l r2,@(0x4,r4) */
L8c602300: *stop=0x8c602300u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c602302; /* mov.l @r5,r1 */
L8c602302: *stop=0x8c602304u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c602308; /* bra 0x8c602308 */
L8c602306: *stop=0x8c602306u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c602308; /* mov.l r5,@r4 */
L8c602308: *stop=0x8c602308u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60238cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60230a; /* mov.l 0x8c60238c,r2 */
L8c60230a: *stop=0x8c60230au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c60230c; /* mov.l r4,@r2 */
L8c60230c: *stop=0x8c60230cu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c60230e; /* stc SR,r3 */
L8c60230e: *stop=0x8c60230eu;r[0]=r[7]&65535u;goto L8c602310; /* extu.w r7,r0 */
L8c602310: *stop=0x8c602310u;{uint32_t loaded=sx(m,bus,&io_status,0x8c60238au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c602312; /* mov.w 0x8c60238a,r2 */
L8c602312: *stop=0x8c602312u;r[0]&=0xfu;goto L8c602314; /* and #0xf,r0 */
L8c602314: *stop=0x8c602314u;r[0]<<=2;goto L8c602316; /* shll2 r0 */
L8c602316: *stop=0x8c602316u;r[3]&=r[2];goto L8c602318; /* and r2,r3 */
L8c602318: *stop=0x8c602318u;r[0]<<=2;goto L8c60231a; /* shll2 r0 */
L8c60231a: *stop=0x8c60231au;r[0]|=r[3];goto L8c60231c; /* or r3,r0 */
L8c60231c: *stop=0x8c60231cu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60231e; /* ldc r0,SR */
L8c60231e: *stop=0x8c60231eu;if(*pr!=entry_pr)return -3;*stop=0x8c602320u;return 0; /* rts */
}
