/* Original heap initialization, pool construction and canonical allocation/task bodies.
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
SA_STAGE_EXPORT int sa_player_work_allocation_8c098a10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c098a10;
L8c098a10: *stop=0x8c098a10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c098a68u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c098a12; /* mov.l 0x8c098a68,r4 */
L8c098a12: *stop=0x8c098a12u;r[1]=0u;goto L8c098a14; /* mov #0x0,r1 */
L8c098a14: *stop=0x8c098a14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c098a70u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c098a16; /* mov.l 0x8c098a70,r0 */
L8c098a16: *stop=0x8c098a16u;r[5]=4294967295u;goto L8c098a18; /* mov #-0x1,r5 */
L8c098a18: *stop=0x8c098a18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c098a6cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c098a1a; /* mov.l 0x8c098a6c,r7 */
L8c098a1a: *stop=0x8c098a1au;r[5]=r[5]&255u;goto L8c098a1c; /* extu.b r5,r5 */
L8c098a1c: *stop=0x8c098a1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c098a1e; /* mov.l r14,@-r15 */
L8c098a1e: *stop=0x8c098a1eu;r[14]=r[4];goto L8c098a20; /* mov r4,r14 */
L8c098a20: *stop=0x8c098a20u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[4],4);if(io_status)return io_status;goto L8c098a22; /* mov.l r4,@r7 */
L8c098a22: *stop=0x8c098a22u;r[14]+=r[0];goto L8c098a24; /* add r0,r14 */
L8c098a24: *stop=0x8c098a24u;r[6]=r[1];goto L8c098a26; /* mov r1,r6 */
L8c098a26: *stop=0x8c098a26u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[5],1);if(io_status)return io_status;goto L8c098a28; /* mov.b r5,@r4 */
L8c098a28: *stop=0x8c098a28u;r[4]+=1u;goto L8c098a2a; /* add #0x1,r4 */
L8c098a2a: *stop=0x8c098a2au;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[5],1);if(io_status)return io_status;goto L8c098a2c; /* mov.b r5,@r4 */
L8c098a2c: *stop=0x8c098a2cu;r[6]+=3u;goto L8c098a2e; /* add #0x3,r6 */
L8c098a2e: *stop=0x8c098a2eu;r[4]+=1u;goto L8c098a30; /* add #0x1,r4 */
L8c098a30: *stop=0x8c098a30u;*sr=(*sr&~1u)|(r[6]>=r[0]);goto L8c098a32; /* cmp/hs r0,r6 */
L8c098a32: *stop=0x8c098a32u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[5],1);if(io_status)return io_status;goto L8c098a34; /* mov.b r5,@r4 */
L8c098a34: take=!(*sr&1u);*stop=0x8c098a36u;r[4]+=1u;if(take)goto L8c098a26;goto L8c098a38; /* bf/s 0x8c098a26 */
L8c098a38: *stop=0x8c098a38u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c098a3a; /* mov.l @r7,r2 */
L8c098a3a: *stop=0x8c098a3au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c098a3c; /* mov.l r14,@r2 */
L8c098a3c: *stop=0x8c098a3cu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098a3e; /* mov.l @r7,r3 */
L8c098a3e: *stop=0x8c098a3eu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c098a40; /* mov.l r1,@(0x4,r3) */
L8c098a40: *stop=0x8c098a40u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c098a42; /* mov.l @r7,r2 */
L8c098a42: *stop=0x8c098a42u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[14],4);if(io_status)return io_status;goto L8c098a44; /* mov.l r14,@(0x8,r2) */
L8c098a44: *stop=0x8c098a44u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098a46; /* mov.l @r7,r3 */
L8c098a46: *stop=0x8c098a46u;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[1],4);if(io_status)return io_status;goto L8c098a48; /* mov.l r1,@(0xc,r3) */
L8c098a48: *stop=0x8c098a48u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c098a4a; /* mov.l r1,@r14 */
L8c098a4a: *stop=0x8c098a4au;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c098a4c; /* mov.l @r7,r2 */
L8c098a4c: *stop=0x8c098a4cu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c098a4e; /* mov.l r2,@(0x4,r14) */
L8c098a4e: *stop=0x8c098a4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c098a74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098a50; /* mov.l 0x8c098a74,r2 */
L8c098a50: *stop=0x8c098a50u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[1],4);if(io_status)return io_status;goto L8c098a52; /* mov.l r1,@(0x8,r14) */
L8c098a52: *stop=0x8c098a52u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098a54; /* mov.l @r7,r3 */
L8c098a54: *stop=0x8c098a54u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c098a56; /* mov.l r3,@(0xc,r14) */
L8c098a56: *stop=0x8c098a56u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c098a58; /* mov.l r1,@r2 */
L8c098a58: *stop=0x8c098a58u;if(*pr!=entry_pr)return -3;*stop=0x8c098a5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

static int sa_player_work_allocation_run_8c09844a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c098456u)goto L8c098456;goto L8c09844a;
L8c09844a: *stop=0x8c09844au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c09844c; /* mov.l r12,@-r15 */
L8c09844c: *stop=0x8c09844cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c09844e; /* mov.l r11,@-r15 */
L8c09844e: *stop=0x8c09844eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c098450; /* sts.l PR,@-r15 */
L8c098450: *stop=0x8c098450u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0985a4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c098452; /* mov.l 0x8c0985a4,r4 */
L8c098452: target=0x8c098a80u;*pr=0x8c098456u;*stop=0x8c098454u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098456; /* bsr 0x8c098a80 */
L8c098456: *stop=0x8c098456u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0985a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098458; /* mov.l 0x8c0985a8,r2 */
L8c098458: *stop=0x8c098458u;r[3]=r[0];goto L8c09845a; /* mov r0,r3 */
L8c09845a: *stop=0x8c09845au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0985acu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c09845c; /* mov.l 0x8c0985ac,r6 */
L8c09845c: *stop=0x8c09845cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09845e; /* tst r3,r3 */
L8c09845e: *stop=0x8c09845eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c098460; /* mov.l r0,@r2 */
L8c098460: take=(*sr&1u);*stop=0x8c098462u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[0],4);if(io_status)return io_status;if(take)goto L8c098526;goto L8c098464; /* bt/s 0x8c098526 */
L8c098464: *stop=0x8c098464u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0985b0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c098466; /* mov.l 0x8c0985b0,r4 */
L8c098466: *stop=0x8c098466u;r[7]=r[4];goto L8c098468; /* mov r4,r7 */
L8c098468: *stop=0x8c098468u;r[7]+=32u;goto L8c09846a; /* add #0x20,r7 */
L8c09846a: *stop=0x8c09846au;r[5]=r[4];goto L8c09846c; /* mov r4,r5 */
L8c09846c: *stop=0x8c09846cu;*sr=(*sr&~1u)|(r[5]>=r[7]);goto L8c09846e; /* cmp/hs r7,r5 */
L8c09846e: take=(*sr&1u);*stop=0x8c098470u;r[4]=0u;if(take)goto L8c09847e;goto L8c098472; /* bt/s 0x8c09847e */
L8c098472: *stop=0x8c098472u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c098474; /* mov.l r4,@r5 */
L8c098474: *stop=0x8c098474u;r[5]+=4u;goto L8c098476; /* add #0x4,r5 */
L8c098476: *stop=0x8c098476u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c098478; /* mov.l r4,@r5 */
L8c098478: *stop=0x8c098478u;r[5]+=4u;goto L8c09847a; /* add #0x4,r5 */
L8c09847a: *stop=0x8c09847au;*sr=(*sr&~1u)|(r[5]>=r[7]);goto L8c09847c; /* cmp/hs r7,r5 */
L8c09847c: if(!(*sr&1u))goto L8c098472;goto L8c09847e; /* bf 0x8c098472 */
L8c09847e: *stop=0x8c09847eu;r[12]=16u;goto L8c098480; /* mov #0x10,r12 */
L8c098480: *stop=0x8c098480u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c098482; /* mov.l @r6,r6 */
L8c098482: *stop=0x8c098482u;r[11]=16u;goto L8c098484; /* mov #0x10,r11 */
L8c098484: *stop=0x8c098484u;r[1]=r[4];goto L8c098486; /* mov r4,r1 */
L8c098486: *stop=0x8c098486u;r[12]<<=8;goto L8c098488; /* shll8 r12 */
L8c098488: *stop=0x8c098488u;r[5]=r[6];goto L8c09848a; /* mov r6,r5 */
L8c09848a: *stop=0x8c09848au;r[12]+=4294967295u;goto L8c09848c; /* add #-0x1,r12 */
L8c09848c: *stop=0x8c09848cu;r[7]=r[4];goto L8c09848e; /* mov r4,r7 */
L8c09848e: *stop=0x8c09848eu;r[11]<<=8;goto L8c098490; /* shll8 r11 */
L8c098490: *stop=0x8c098490u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]);goto L8c098492; /* cmp/ge r12,r7 */
L8c098492: take=!(*sr&1u);*stop=0x8c098494u;r[5]+=48u;if(take)goto L8c098498;goto L8c098496; /* bf/s 0x8c098498 */
L8c098496: *stop=0x8c098496u;r[5]=r[4];goto L8c098498; /* mov r4,r5 */
L8c098498: *stop=0x8c098498u;r[7]+=1u;goto L8c09849a; /* add #0x1,r7 */
L8c09849a: *stop=0x8c09849au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[5],4);if(io_status)return io_status;goto L8c09849c; /* mov.l r5,@r6 */
L8c09849c: *stop=0x8c09849cu;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[1],4);if(io_status)return io_status;goto L8c09849e; /* mov.l r1,@(0x4,r6) */
L8c09849e: *stop=0x8c09849eu;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]);goto L8c0984a0; /* cmp/ge r12,r7 */
L8c0984a0: *stop=0x8c0984a0u;r[1]=r[5];goto L8c0984a2; /* mov r5,r1 */
L8c0984a2: *stop=0x8c0984a2u;if(!valid_access(r[6]+28u,4u))return -3;wr(m,bus,&io_status,r[6]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c0984a4; /* mov.l r4,@(0x1c,r6) */
L8c0984a4: *stop=0x8c0984a4u;if(!valid_access(r[6]+32u,4u))return -3;wr(m,bus,&io_status,r[6]+0x20u,r[4],4);if(io_status)return io_status;goto L8c0984a6; /* mov.l r4,@(0x20,r6) */
L8c0984a6: *stop=0x8c0984a6u;if(!valid_access(r[6]+36u,4u))return -3;wr(m,bus,&io_status,r[6]+0x24u,r[4],4);if(io_status)return io_status;goto L8c0984a8; /* mov.l r4,@(0x24,r6) */
L8c0984a8: *stop=0x8c0984a8u;if(!valid_access(r[6]+40u,4u))return -3;wr(m,bus,&io_status,r[6]+0x28u,r[4],4);if(io_status)return io_status;goto L8c0984aa; /* mov.l r4,@(0x28,r6) */
L8c0984aa: *stop=0x8c0984aau;if(!valid_access(r[6]+44u,4u))return -3;wr(m,bus,&io_status,r[6]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c0984ac; /* mov.l r4,@(0x2c,r6) */
L8c0984ac: *stop=0x8c0984acu;if(!valid_access(r[6]+12u,4u))return -3;wr(m,bus,&io_status,r[6]+0xcu,r[4],4);if(io_status)return io_status;goto L8c0984ae; /* mov.l r4,@(0xc,r6) */
L8c0984ae: *stop=0x8c0984aeu;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[4],4);if(io_status)return io_status;goto L8c0984b0; /* mov.l r4,@(0x8,r6) */
L8c0984b0: *stop=0x8c0984b0u;if(!valid_access(r[6]+16u,4u))return -3;wr(m,bus,&io_status,r[6]+0x10u,r[4],4);if(io_status)return io_status;goto L8c0984b2; /* mov.l r4,@(0x10,r6) */
L8c0984b2: *stop=0x8c0984b2u;if(!valid_access(r[6]+20u,4u))return -3;wr(m,bus,&io_status,r[6]+0x14u,r[4],4);if(io_status)return io_status;goto L8c0984b4; /* mov.l r4,@(0x14,r6) */
L8c0984b4: *stop=0x8c0984b4u;if(!valid_access(r[6]+24u,4u))return -3;wr(m,bus,&io_status,r[6]+0x18u,r[4],4);if(io_status)return io_status;goto L8c0984b6; /* mov.l r4,@(0x18,r6) */
L8c0984b6: take=!(*sr&1u);*stop=0x8c0984b8u;r[5]+=48u;if(take)goto L8c0984bc;goto L8c0984ba; /* bf/s 0x8c0984bc */
L8c0984ba: *stop=0x8c0984bau;r[5]=r[4];goto L8c0984bc; /* mov r4,r5 */
L8c0984bc: *stop=0x8c0984bcu;r[7]+=1u;goto L8c0984be; /* add #0x1,r7 */
L8c0984be: *stop=0x8c0984beu;r[0]=r[5];goto L8c0984c0; /* mov r5,r0 */
L8c0984c0: *stop=0x8c0984c0u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]);goto L8c0984c2; /* cmp/ge r12,r7 */
L8c0984c2: *stop=0x8c0984c2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c0984c4; /* mov.l r5,@r1 */
L8c0984c4: *stop=0x8c0984c4u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[6],4);if(io_status)return io_status;goto L8c0984c6; /* mov.l r6,@(0x4,r1) */
L8c0984c6: *stop=0x8c0984c6u;if(!valid_access(r[1]+28u,4u))return -3;wr(m,bus,&io_status,r[1]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c0984c8; /* mov.l r4,@(0x1c,r1) */
L8c0984c8: *stop=0x8c0984c8u;if(!valid_access(r[1]+32u,4u))return -3;wr(m,bus,&io_status,r[1]+0x20u,r[4],4);if(io_status)return io_status;goto L8c0984ca; /* mov.l r4,@(0x20,r1) */
L8c0984ca: *stop=0x8c0984cau;if(!valid_access(r[1]+36u,4u))return -3;wr(m,bus,&io_status,r[1]+0x24u,r[4],4);if(io_status)return io_status;goto L8c0984cc; /* mov.l r4,@(0x24,r1) */
L8c0984cc: *stop=0x8c0984ccu;if(!valid_access(r[1]+40u,4u))return -3;wr(m,bus,&io_status,r[1]+0x28u,r[4],4);if(io_status)return io_status;goto L8c0984ce; /* mov.l r4,@(0x28,r1) */
L8c0984ce: *stop=0x8c0984ceu;if(!valid_access(r[1]+44u,4u))return -3;wr(m,bus,&io_status,r[1]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c0984d0; /* mov.l r4,@(0x2c,r1) */
L8c0984d0: *stop=0x8c0984d0u;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[4],4);if(io_status)return io_status;goto L8c0984d2; /* mov.l r4,@(0xc,r1) */
L8c0984d2: *stop=0x8c0984d2u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[4],4);if(io_status)return io_status;goto L8c0984d4; /* mov.l r4,@(0x8,r1) */
L8c0984d4: *stop=0x8c0984d4u;if(!valid_access(r[1]+16u,4u))return -3;wr(m,bus,&io_status,r[1]+0x10u,r[4],4);if(io_status)return io_status;goto L8c0984d6; /* mov.l r4,@(0x10,r1) */
L8c0984d6: *stop=0x8c0984d6u;if(!valid_access(r[1]+20u,4u))return -3;wr(m,bus,&io_status,r[1]+0x14u,r[4],4);if(io_status)return io_status;goto L8c0984d8; /* mov.l r4,@(0x14,r1) */
L8c0984d8: *stop=0x8c0984d8u;if(!valid_access(r[1]+24u,4u))return -3;wr(m,bus,&io_status,r[1]+0x18u,r[4],4);if(io_status)return io_status;goto L8c0984da; /* mov.l r4,@(0x18,r1) */
L8c0984da: take=!(*sr&1u);*stop=0x8c0984dcu;r[5]+=48u;if(take)goto L8c0984e0;goto L8c0984de; /* bf/s 0x8c0984e0 */
L8c0984de: *stop=0x8c0984deu;r[5]=r[4];goto L8c0984e0; /* mov r4,r5 */
L8c0984e0: *stop=0x8c0984e0u;r[7]+=1u;goto L8c0984e2; /* add #0x1,r7 */
L8c0984e2: *stop=0x8c0984e2u;r[6]=r[5];goto L8c0984e4; /* mov r5,r6 */
L8c0984e4: *stop=0x8c0984e4u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]);goto L8c0984e6; /* cmp/ge r12,r7 */
L8c0984e6: *stop=0x8c0984e6u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c0984e8; /* mov.l r5,@r0 */
L8c0984e8: *stop=0x8c0984e8u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0984ea; /* mov.l r1,@(0x4,r0) */
L8c0984ea: *stop=0x8c0984eau;if(!valid_access(r[0]+28u,4u))return -3;wr(m,bus,&io_status,r[0]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c0984ec; /* mov.l r4,@(0x1c,r0) */
L8c0984ec: *stop=0x8c0984ecu;if(!valid_access(r[0]+32u,4u))return -3;wr(m,bus,&io_status,r[0]+0x20u,r[4],4);if(io_status)return io_status;goto L8c0984ee; /* mov.l r4,@(0x20,r0) */
L8c0984ee: *stop=0x8c0984eeu;if(!valid_access(r[0]+36u,4u))return -3;wr(m,bus,&io_status,r[0]+0x24u,r[4],4);if(io_status)return io_status;goto L8c0984f0; /* mov.l r4,@(0x24,r0) */
L8c0984f0: *stop=0x8c0984f0u;if(!valid_access(r[0]+40u,4u))return -3;wr(m,bus,&io_status,r[0]+0x28u,r[4],4);if(io_status)return io_status;goto L8c0984f2; /* mov.l r4,@(0x28,r0) */
L8c0984f2: *stop=0x8c0984f2u;if(!valid_access(r[0]+44u,4u))return -3;wr(m,bus,&io_status,r[0]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c0984f4; /* mov.l r4,@(0x2c,r0) */
L8c0984f4: *stop=0x8c0984f4u;if(!valid_access(r[0]+12u,4u))return -3;wr(m,bus,&io_status,r[0]+0xcu,r[4],4);if(io_status)return io_status;goto L8c0984f6; /* mov.l r4,@(0xc,r0) */
L8c0984f6: *stop=0x8c0984f6u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[4],4);if(io_status)return io_status;goto L8c0984f8; /* mov.l r4,@(0x8,r0) */
L8c0984f8: *stop=0x8c0984f8u;if(!valid_access(r[0]+16u,4u))return -3;wr(m,bus,&io_status,r[0]+0x10u,r[4],4);if(io_status)return io_status;goto L8c0984fa; /* mov.l r4,@(0x10,r0) */
L8c0984fa: *stop=0x8c0984fau;if(!valid_access(r[0]+20u,4u))return -3;wr(m,bus,&io_status,r[0]+0x14u,r[4],4);if(io_status)return io_status;goto L8c0984fc; /* mov.l r4,@(0x14,r0) */
L8c0984fc: *stop=0x8c0984fcu;if(!valid_access(r[0]+24u,4u))return -3;wr(m,bus,&io_status,r[0]+0x18u,r[4],4);if(io_status)return io_status;goto L8c0984fe; /* mov.l r4,@(0x18,r0) */
L8c0984fe: take=!(*sr&1u);*stop=0x8c098500u;r[5]+=48u;if(take)goto L8c098504;goto L8c098502; /* bf/s 0x8c098504 */
L8c098502: *stop=0x8c098502u;r[5]=r[4];goto L8c098504; /* mov r4,r5 */
L8c098504: *stop=0x8c098504u;r[7]+=1u;goto L8c098506; /* add #0x1,r7 */
L8c098506: *stop=0x8c098506u;r[1]=r[6];goto L8c098508; /* mov r6,r1 */
L8c098508: *stop=0x8c098508u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[11]);goto L8c09850a; /* cmp/ge r11,r7 */
L8c09850a: *stop=0x8c09850au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[5],4);if(io_status)return io_status;goto L8c09850c; /* mov.l r5,@r6 */
L8c09850c: *stop=0x8c09850cu;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[0],4);if(io_status)return io_status;goto L8c09850e; /* mov.l r0,@(0x4,r6) */
L8c09850e: *stop=0x8c09850eu;if(!valid_access(r[6]+28u,4u))return -3;wr(m,bus,&io_status,r[6]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c098510; /* mov.l r4,@(0x1c,r6) */
L8c098510: *stop=0x8c098510u;if(!valid_access(r[6]+32u,4u))return -3;wr(m,bus,&io_status,r[6]+0x20u,r[4],4);if(io_status)return io_status;goto L8c098512; /* mov.l r4,@(0x20,r6) */
L8c098512: *stop=0x8c098512u;if(!valid_access(r[6]+36u,4u))return -3;wr(m,bus,&io_status,r[6]+0x24u,r[4],4);if(io_status)return io_status;goto L8c098514; /* mov.l r4,@(0x24,r6) */
L8c098514: *stop=0x8c098514u;if(!valid_access(r[6]+40u,4u))return -3;wr(m,bus,&io_status,r[6]+0x28u,r[4],4);if(io_status)return io_status;goto L8c098516; /* mov.l r4,@(0x28,r6) */
L8c098516: *stop=0x8c098516u;if(!valid_access(r[6]+44u,4u))return -3;wr(m,bus,&io_status,r[6]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c098518; /* mov.l r4,@(0x2c,r6) */
L8c098518: *stop=0x8c098518u;if(!valid_access(r[6]+12u,4u))return -3;wr(m,bus,&io_status,r[6]+0xcu,r[4],4);if(io_status)return io_status;goto L8c09851a; /* mov.l r4,@(0xc,r6) */
L8c09851a: *stop=0x8c09851au;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[4],4);if(io_status)return io_status;goto L8c09851c; /* mov.l r4,@(0x8,r6) */
L8c09851c: *stop=0x8c09851cu;if(!valid_access(r[6]+16u,4u))return -3;wr(m,bus,&io_status,r[6]+0x10u,r[4],4);if(io_status)return io_status;goto L8c09851e; /* mov.l r4,@(0x10,r6) */
L8c09851e: *stop=0x8c09851eu;if(!valid_access(r[6]+20u,4u))return -3;wr(m,bus,&io_status,r[6]+0x14u,r[4],4);if(io_status)return io_status;goto L8c098520; /* mov.l r4,@(0x14,r6) */
L8c098520: *stop=0x8c098520u;if(!valid_access(r[6]+24u,4u))return -3;wr(m,bus,&io_status,r[6]+0x18u,r[4],4);if(io_status)return io_status;goto L8c098522; /* mov.l r4,@(0x18,r6) */
L8c098522: take=!(*sr&1u);*stop=0x8c098524u;r[6]=r[5];if(take)goto L8c098490;goto L8c098526; /* bf/s 0x8c098490 */
L8c098526: *stop=0x8c098526u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c098528; /* lds.l @r15+,PR */
L8c098528: *stop=0x8c098528u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c09852a; /* mov.l @r15+,r11 */
L8c09852a: *stop=0x8c09852au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c09852cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_work_allocation_8c09844a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09844a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098456(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09844a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098456u);}

SA_STAGE_EXPORT int sa_player_work_allocation_8c098a80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c098a80;
L8c098a80: *stop=0x8c098a80u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c098a82; /* tst r4,r4 */
L8c098a82: if((*sr&1u))goto L8c098b06;goto L8c098a84; /* bt 0x8c098b06 */
L8c098a84: *stop=0x8c098a84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c098b60u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c098a86; /* mov.l 0x8c098b60,r7 */
L8c098a86: *stop=0x8c098a86u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[6]=loaded;}goto L8c098a88; /* mov.l @r7,r6 */
L8c098a88: *stop=0x8c098a88u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c098a8a; /* tst r6,r6 */
L8c098a8a: if((*sr&1u))goto L8c098b06;goto L8c098a8c; /* bt 0x8c098b06 */
L8c098a8c: *stop=0x8c098a8cu;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098a8e; /* mov.l @(0x8,r6),r3 */
L8c098a8e: *stop=0x8c098a8eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c098a90; /* tst r3,r3 */
L8c098a90: if((*sr&1u))goto L8c098b06;goto L8c098a92; /* bt 0x8c098b06 */
L8c098a92: *stop=0x8c098a92u;r[5]=r[4];goto L8c098a94; /* mov r4,r5 */
L8c098a94: *stop=0x8c098a94u;r[4]=r[6];goto L8c098a96; /* mov r6,r4 */
L8c098a96: *stop=0x8c098a96u;r[3]=4294967280u;goto L8c098a98; /* mov #-0x10,r3 */
L8c098a98: *stop=0x8c098a98u;r[5]+=31u;goto L8c098a9a; /* add #0x1f,r5 */
L8c098a9a: *stop=0x8c098a9au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c098a9c; /* tst r4,r4 */
L8c098a9c: *stop=0x8c098a9cu;r[5]&=r[3];goto L8c098a9e; /* and r3,r5 */
L8c098a9e: take=(*sr&1u);*stop=0x8c098aa0u;r[6]=0u;if(take)goto L8c098b06;goto L8c098aa2; /* bt/s 0x8c098b06 */
L8c098aa2: *stop=0x8c098aa2u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c098aa4; /* mov.l @(0x8,r4),r1 */
L8c098aa4: *stop=0x8c098aa4u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c098aa6; /* tst r1,r1 */
L8c098aa6: if((*sr&1u))goto L8c098b06;goto L8c098aa8; /* bt 0x8c098b06 */
L8c098aa8: *stop=0x8c098aa8u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c098aaa; /* mov.l @(0x8,r4),r1 */
L8c098aaa: *stop=0x8c098aaau;r[1]-=r[4];goto L8c098aac; /* sub r4,r1 */
L8c098aac: *stop=0x8c098aacu;*sr=(*sr&~1u)|(r[5]==r[1]);goto L8c098aae; /* cmp/eq r5,r1 */
L8c098aae: if(!(*sr&1u))goto L8c098ac6;goto L8c098ab0; /* bf 0x8c098ac6 */
L8c098ab0: *stop=0x8c098ab0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[5]=loaded;}goto L8c098ab2; /* mov.l @r4,r5 */
L8c098ab2: *stop=0x8c098ab2u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c098ab4; /* tst r5,r5 */
L8c098ab4: take=(*sr&1u);*stop=0x8c098ab6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}if(take)goto L8c098aba;goto L8c098ab8; /* bt/s 0x8c098aba */
L8c098ab8: *stop=0x8c098ab8u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[1],4);if(io_status)return io_status;goto L8c098aba; /* mov.l r1,@(0x4,r5) */
L8c098aba: *stop=0x8c098abau;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c098abc; /* tst r1,r1 */
L8c098abc: if((*sr&1u))goto L8c098ac2;goto L8c098abe; /* bt 0x8c098ac2 */
L8c098abe: *stop=0x8c098ac0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c098af0; /* bra 0x8c098af0 */
L8c098ac2: *stop=0x8c098ac4u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[5],4);if(io_status)return io_status;goto L8c098af0; /* bra 0x8c098af0 */
L8c098ac6: *stop=0x8c098ac6u;*sr=(*sr&~1u)|(r[1]>r[5]);goto L8c098ac8; /* cmp/hi r5,r1 */
L8c098ac8: if(!(*sr&1u))goto L8c098afa;goto L8c098aca; /* bf 0x8c098afa */
L8c098aca: *stop=0x8c098acau;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c098acc; /* mov.l @(0x4,r4),r1 */
L8c098acc: *stop=0x8c098accu;r[5]+=r[4];goto L8c098ace; /* add r4,r5 */
L8c098ace: *stop=0x8c098aceu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c098ad0; /* mov.l @r4,r0 */
L8c098ad0: *stop=0x8c098ad0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c098ad2; /* tst r0,r0 */
L8c098ad2: *stop=0x8c098ad2u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c098ad4; /* mov.l r0,@r5 */
L8c098ad4: *stop=0x8c098ad4u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[1],4);if(io_status)return io_status;goto L8c098ad6; /* mov.l r1,@(0x4,r5) */
L8c098ad6: *stop=0x8c098ad6u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098ad8; /* mov.l @(0x8,r4),r3 */
L8c098ad8: *stop=0x8c098ad8u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[3],4);if(io_status)return io_status;goto L8c098ada; /* mov.l r3,@(0x8,r5) */
L8c098ada: *stop=0x8c098adau;if(!valid_access(r[5]+12u,4u))return -3;wr(m,bus,&io_status,r[5]+0xcu,r[4],4);if(io_status)return io_status;goto L8c098adc; /* mov.l r4,@(0xc,r5) */
L8c098adc: *stop=0x8c098adcu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098ade; /* mov.l @(0x8,r4),r2 */
L8c098ade: *stop=0x8c098adeu;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[5],4);if(io_status)return io_status;goto L8c098ae0; /* mov.l r5,@(0xc,r2) */
L8c098ae0: take=(*sr&1u);*stop=0x8c098ae2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;if(take)goto L8c098ae6;goto L8c098ae4; /* bt/s 0x8c098ae6 */
L8c098ae4: *stop=0x8c098ae4u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[5],4);if(io_status)return io_status;goto L8c098ae6; /* mov.l r5,@(0x4,r0) */
L8c098ae6: *stop=0x8c098ae6u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c098ae8; /* tst r1,r1 */
L8c098ae8: if((*sr&1u))goto L8c098aee;goto L8c098aea; /* bt 0x8c098aee */
L8c098aea: *stop=0x8c098aecu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c098af0; /* bra 0x8c098af0 */
L8c098aee: *stop=0x8c098aeeu;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[5],4);if(io_status)return io_status;goto L8c098af0; /* mov.l r5,@r7 */
L8c098af0: *stop=0x8c098af0u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[6],4);if(io_status)return io_status;goto L8c098af2; /* mov.l r6,@r4 */
L8c098af2: *stop=0x8c098af2u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[6],4);if(io_status)return io_status;goto L8c098af4; /* mov.l r6,@(0x4,r4) */
L8c098af4: *stop=0x8c098af4u;r[4]+=16u;goto L8c098af6; /* add #0x10,r4 */
L8c098af6: *stop=0x8c098af6u;if(*pr!=entry_pr)return -3;*stop=0x8c098af8u;r[0]=r[4];return 0; /* rts */
L8c098afa: *stop=0x8c098afau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c098afc; /* mov.l @r4,r4 */
L8c098afc: *stop=0x8c098afcu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c098afe; /* tst r4,r4 */
L8c098afe: if((*sr&1u))goto L8c098b06;goto L8c098b00; /* bt 0x8c098b06 */
L8c098b00: *stop=0x8c098b00u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098b02; /* mov.l @(0x8,r4),r3 */
L8c098b02: *stop=0x8c098b02u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c098b04; /* tst r3,r3 */
L8c098b04: if(!(*sr&1u))goto L8c098aa8;goto L8c098b06; /* bf 0x8c098aa8 */
L8c098b06: *stop=0x8c098b06u;r[0]=0u;goto L8c098b08; /* mov #0x0,r0 */
L8c098b08: *stop=0x8c098b08u;if(*pr!=entry_pr)return -3;*stop=0x8c098b0au;return 0; /* rts */
}

static int sa_player_work_allocation_run_8c098b0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c098b20u)goto L8c098b20;goto L8c098b0c;
L8c098b0c: *stop=0x8c098b0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c098b0e; /* mov.l r14,@-r15 */
L8c098b0e: *stop=0x8c098b0eu;r[14]=r[5];goto L8c098b10; /* mov r5,r14 */
L8c098b10: *stop=0x8c098b10u;r[3]=4294967292u;goto L8c098b12; /* mov #-0x4,r3 */
L8c098b12: *stop=0x8c098b12u;r[14]+=3u;goto L8c098b14; /* add #0x3,r14 */
L8c098b14: *stop=0x8c098b14u;r[14]&=r[3];goto L8c098b16; /* and r3,r14 */
L8c098b16: *stop=0x8c098b16u;*macl=r[4]*r[14];goto L8c098b18; /* mul.l r4,r14 */
L8c098b18: *stop=0x8c098b18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c098b1a; /* sts.l PR,@-r15 */
L8c098b1a: *stop=0x8c098b1au;r[14]=*macl;goto L8c098b1c; /* sts MACL,r14 */
L8c098b1c: target=0x8c098a80u;*pr=0x8c098b20u;*stop=0x8c098b1eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098b20; /* bsr 0x8c098a80 */
L8c098b20: *stop=0x8c098b20u;r[6]=r[0];goto L8c098b22; /* mov r0,r6 */
L8c098b22: *stop=0x8c098b22u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c098b24; /* tst r6,r6 */
L8c098b24: if((*sr&1u))goto L8c098b3a;goto L8c098b26; /* bt 0x8c098b3a */
L8c098b26: *stop=0x8c098b26u;r[4]=r[14];goto L8c098b28; /* mov r14,r4 */
L8c098b28: *stop=0x8c098b28u;r[4]>>=2;goto L8c098b2a; /* shlr2 r4 */
L8c098b2a: *stop=0x8c098b2au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c098b2c; /* tst r4,r4 */
L8c098b2c: *stop=0x8c098b2cu;r[7]=r[6];goto L8c098b2e; /* mov r6,r7 */
L8c098b2e: take=(*sr&1u);*stop=0x8c098b30u;r[5]=0u;if(take)goto L8c098b3a;goto L8c098b32; /* bt/s 0x8c098b3a */
L8c098b32: *stop=0x8c098b32u;r[4]--;*sr=(*sr&~1u)|(!r[4]);goto L8c098b34; /* dt r4 */
L8c098b34: *stop=0x8c098b34u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[5],4);if(io_status)return io_status;goto L8c098b36; /* mov.l r5,@r7 */
L8c098b36: take=!(*sr&1u);*stop=0x8c098b38u;r[7]+=4u;if(take)goto L8c098b32;goto L8c098b3a; /* bf/s 0x8c098b32 */
L8c098b3a: *stop=0x8c098b3au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c098b3c; /* lds.l @r15+,PR */
L8c098b3c: *stop=0x8c098b3cu;r[0]=r[6];goto L8c098b3e; /* mov r6,r0 */
L8c098b3e: *stop=0x8c098b3eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c098b40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_work_allocation_8c098b0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c098b0c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098b20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c098b0c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098b20u);}

static int sa_player_work_allocation_run_8c09852e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c098544u)goto L8c098544;if(resume_pc==0x8c09855au)goto L8c09855a;if(resume_pc==0x8c098564u)goto L8c098564;if(resume_pc==0x8c098576u)goto L8c098576;if(resume_pc==0x8c098580u)goto L8c098580;if(resume_pc==0x8c098592u)goto L8c098592;if(resume_pc==0x8c09859cu)goto L8c09859c;if(resume_pc==0x8c0985ceu)goto L8c0985ce;if(resume_pc==0x8c0985d8u)goto L8c0985d8;goto L8c09852e;
L8c09852e: *stop=0x8c09852eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c098530; /* mov.l r14,@-r15 */
L8c098530: *stop=0x8c098530u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c098532; /* mov.l r13,@-r15 */
L8c098532: *stop=0x8c098532u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c098534; /* mov.l r12,@-r15 */
L8c098534: *stop=0x8c098534u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c098536; /* sts.l PR,@-r15 */
L8c098536: *stop=0x8c098536u;r[15]+=4294967284u;goto L8c098538; /* add #-0xc,r15 */
L8c098538: *stop=0x8c098538u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[4],2);if(io_status)return io_status;goto L8c09853a; /* mov.w r4,@r15 */
L8c09853a: *stop=0x8c09853au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c09853c; /* mov.l r5,@(0x4,r15) */
L8c09853c: *stop=0x8c09853cu;r[5]=r[6];goto L8c09853e; /* mov r6,r5 */
L8c09853e: *stop=0x8c09853eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c098540; /* mov.l r6,@(0x8,r15) */
L8c098540: target=0x8c0985ecu;*pr=0x8c098544u;*stop=0x8c098542u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098544; /* bsr 0x8c0985ec */
L8c098544: *stop=0x8c098544u;r[14]=r[0];goto L8c098546; /* mov r0,r14 */
L8c098546: *stop=0x8c098546u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c098548; /* tst r14,r14 */
L8c098548: if((*sr&1u))goto L8c0985d8;goto L8c09854a; /* bt 0x8c0985d8 */
L8c09854a: *stop=0x8c09854au;if(!valid_access(r[15],2u))return -3;r[13]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c09854c; /* mov.w @r15,r13 */
L8c09854c: *stop=0x8c09854cu;r[3]=2u;goto L8c09854e; /* mov #0x2,r3 */
L8c09854e: *stop=0x8c09854eu;r[13]=r[13]&65535u;goto L8c098550; /* extu.w r13,r13 */
L8c098550: *stop=0x8c098550u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c098552; /* tst r13,r3 */
L8c098552: if((*sr&1u))goto L8c09856a;goto L8c098554; /* bt 0x8c09856a */
L8c098554: *stop=0x8c098554u;r[5]=64u;goto L8c098556; /* mov #0x40,r5 */
L8c098556: target=0x8c098b0cu;*pr=0x8c09855au;*stop=0x8c098558u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09855a; /* bsr 0x8c098b0c */
L8c09855a: *stop=0x8c09855au;r[12]=r[0];goto L8c09855c; /* mov r0,r12 */
L8c09855c: *stop=0x8c09855cu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c09855e; /* tst r12,r12 */
L8c09855e: if(!(*sr&1u))goto L8c098568;goto L8c098560; /* bf 0x8c098568 */
L8c098560: target=0x8c09865cu;*pr=0x8c098564u;*stop=0x8c098562u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098564; /* bsr 0x8c09865c */
L8c098564: *stop=0x8c098566u;goto L8c0985d8; /* bra 0x8c0985d8 */
L8c098568: *stop=0x8c098568u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[12],4);if(io_status)return io_status;goto L8c09856a; /* mov.l r12,@(0x20,r14) */
L8c09856a: *stop=0x8c09856au;r[3]=4u;goto L8c09856c; /* mov #0x4,r3 */
L8c09856c: *stop=0x8c09856cu;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c09856e; /* tst r13,r3 */
L8c09856e: if((*sr&1u))goto L8c098586;goto L8c098570; /* bt 0x8c098586 */
L8c098570: *stop=0x8c098570u;r[5]=28u;goto L8c098572; /* mov #0x1c,r5 */
L8c098572: target=0x8c098b0cu;*pr=0x8c098576u;*stop=0x8c098574u;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098576; /* bsr 0x8c098b0c */
L8c098576: *stop=0x8c098576u;r[12]=r[0];goto L8c098578; /* mov r0,r12 */
L8c098578: *stop=0x8c098578u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c09857a; /* tst r12,r12 */
L8c09857a: if(!(*sr&1u))goto L8c098584;goto L8c09857c; /* bf 0x8c098584 */
L8c09857c: target=0x8c09865cu;*pr=0x8c098580u;*stop=0x8c09857eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098580; /* bsr 0x8c09865c */
L8c098580: *stop=0x8c098582u;goto L8c0985d8; /* bra 0x8c0985d8 */
L8c098584: *stop=0x8c098584u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[12],4);if(io_status)return io_status;goto L8c098586; /* mov.l r12,@(0x28,r14) */
L8c098586: *stop=0x8c098586u;r[3]=1u;goto L8c098588; /* mov #0x1,r3 */
L8c098588: *stop=0x8c098588u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c09858a; /* tst r13,r3 */
L8c09858a: if((*sr&1u))goto L8c0985c2;goto L8c09858c; /* bt 0x8c0985c2 */
L8c09858c: *stop=0x8c09858cu;r[5]=64u;goto L8c09858e; /* mov #0x40,r5 */
L8c09858e: target=0x8c098b0cu;*pr=0x8c098592u;*stop=0x8c098590u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098592; /* bsr 0x8c098b0c */
L8c098592: *stop=0x8c098592u;r[12]=r[0];goto L8c098594; /* mov r0,r12 */
L8c098594: *stop=0x8c098594u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c098596; /* tst r12,r12 */
L8c098596: if(!(*sr&1u))goto L8c0985c0;goto L8c098598; /* bf 0x8c0985c0 */
L8c098598: target=0x8c09865cu;*pr=0x8c09859cu;*stop=0x8c09859au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09859c; /* bsr 0x8c09865c */
L8c09859c: *stop=0x8c09859eu;goto L8c0985d8; /* bra 0x8c0985d8 */
L8c0985c0: *stop=0x8c0985c0u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[12],4);if(io_status)return io_status;goto L8c0985c2; /* mov.l r12,@(0x24,r14) */
L8c0985c2: *stop=0x8c0985c2u;r[3]=8u;goto L8c0985c4; /* mov #0x8,r3 */
L8c0985c4: *stop=0x8c0985c4u;*sr=(*sr&~1u)|!(r[3]&r[13]);goto L8c0985c6; /* tst r3,r13 */
L8c0985c6: if((*sr&1u))goto L8c0985de;goto L8c0985c8; /* bt 0x8c0985de */
L8c0985c8: *stop=0x8c0985c8u;r[5]=16u;goto L8c0985ca; /* mov #0x10,r5 */
L8c0985ca: target=0x8c098b0cu;*pr=0x8c0985ceu;*stop=0x8c0985ccu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0985ce; /* bsr 0x8c098b0c */
L8c0985ce: *stop=0x8c0985ceu;r[13]=r[0];goto L8c0985d0; /* mov r0,r13 */
L8c0985d0: *stop=0x8c0985d0u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c0985d2; /* tst r13,r13 */
L8c0985d2: if(!(*sr&1u))goto L8c0985dc;goto L8c0985d4; /* bf 0x8c0985dc */
L8c0985d4: target=0x8c09865cu;*pr=0x8c0985d8u;*stop=0x8c0985d6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0985d8; /* bsr 0x8c09865c */
L8c0985d8: *stop=0x8c0985dau;r[0]=0u;goto L8c0985e0; /* bra 0x8c0985e0 */
L8c0985dc: *stop=0x8c0985dcu;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[13],4);if(io_status)return io_status;goto L8c0985de; /* mov.l r13,@(0x2c,r14) */
L8c0985de: *stop=0x8c0985deu;r[0]=r[14];goto L8c0985e0; /* mov r14,r0 */
L8c0985e0: *stop=0x8c0985e0u;r[15]+=12u;goto L8c0985e2; /* add #0xc,r15 */
L8c0985e2: *stop=0x8c0985e2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0985e4; /* lds.l @r15+,PR */
L8c0985e4: *stop=0x8c0985e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0985e6; /* mov.l @r15+,r12 */
L8c0985e6: *stop=0x8c0985e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0985e8; /* mov.l @r15+,r13 */
L8c0985e8: *stop=0x8c0985e8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0985eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_work_allocation_8c09852e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098544(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098544u);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c09855a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09855au);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098564(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098564u);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098576(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098576u);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098580(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098580u);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c098592(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098592u);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c09859c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09859cu);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c0985ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0985ceu);}
SA_STAGE_EXPORT int sa_player_work_allocation_resume_8c0985d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_allocation_run_8c09852e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0985d8u);}

SA_STAGE_EXPORT int sa_player_work_allocation_8c0985ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0985ec;
L8c0985ec: *stop=0x8c0985ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0986c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0985ee; /* mov.l 0x8c0986c0,r3 */
L8c0985ee: *stop=0x8c0985eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0985f0; /* mov.l r14,@-r15 */
L8c0985f0: *stop=0x8c0985f0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c0985f2; /* mov.l @r3,r14 */
L8c0985f2: *stop=0x8c0985f2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0985f4; /* tst r14,r14 */
L8c0985f4: if(!(*sr&1u))goto L8c0985fc;goto L8c0985f6; /* bf 0x8c0985fc */
L8c0985f6: *stop=0x8c0985f6u;r[0]=0u;goto L8c0985f8; /* mov #0x0,r0 */
L8c0985f8: *stop=0x8c0985f8u;if(*pr!=entry_pr)return -3;*stop=0x8c0985fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c0985fc: *stop=0x8c0985fcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[7]=loaded;}goto L8c0985fe; /* mov.l @r14,r7 */
L8c0985fe: *stop=0x8c0985feu;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c098600; /* tst r7,r7 */
L8c098600: take=(*sr&1u);*stop=0x8c098602u;r[6]=0u;if(take)goto L8c098606;goto L8c098604; /* bt/s 0x8c098606 */
L8c098604: *stop=0x8c098604u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[6],4);if(io_status)return io_status;goto L8c098606; /* mov.l r6,@(0x4,r7) */
L8c098606: *stop=0x8c098606u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0986c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098608; /* mov.l 0x8c0986c0,r2 */
L8c098608: *stop=0x8c098608u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c09860a; /* tst r5,r5 */
L8c09860a: take=(*sr&1u);*stop=0x8c09860cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[7],4);if(io_status)return io_status;if(take)goto L8c098612;goto L8c09860e; /* bt/s 0x8c098612 */
L8c09860e: *stop=0x8c098610u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[5],4);if(io_status)return io_status;goto L8c098616; /* bra 0x8c098616 */
L8c098612: *stop=0x8c098612u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0986c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098614; /* mov.l 0x8c0986c4,r3 */
L8c098614: *stop=0x8c098614u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c098616; /* mov.l r3,@(0x10,r14) */
L8c098616: *stop=0x8c098616u;r[0]=r[4];goto L8c098618; /* mov r4,r0 */
L8c098618: *stop=0x8c098618u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c09861a; /* cmp/eq #0x7,r0 */
L8c09861a: if((*sr&1u))goto L8c098644;goto L8c09861c; /* bt 0x8c098644 */
L8c09861c: *stop=0x8c09861cu;r[0]=r[4];goto L8c09861e; /* mov r4,r0 */
L8c09861e: *stop=0x8c09861eu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c098620; /* cmp/eq #0x8,r0 */
L8c098620: if(!(*sr&1u))goto L8c098624;goto L8c098622; /* bf 0x8c098624 */
L8c098622: *stop=0x8c098622u;r[4]=7u;goto L8c098624; /* mov #0x7,r4 */
L8c098624: *stop=0x8c098624u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0986c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098626; /* mov.l 0x8c0986c8,r3 */
L8c098626: *stop=0x8c098626u;r[0]=r[4];goto L8c098628; /* mov r4,r0 */
L8c098628: *stop=0x8c098628u;r[0]<<=2;goto L8c09862a; /* shll2 r0 */
L8c09862a: *stop=0x8c09862au;r[0]+=r[3];goto L8c09862c; /* add r3,r0 */
L8c09862c: *stop=0x8c09862cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c09862e; /* mov.l @r0,r2 */
L8c09862e: *stop=0x8c09862eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c098630; /* tst r2,r2 */
L8c098630: if(!(*sr&1u))goto L8c09863a;goto L8c098632; /* bf 0x8c09863a */
L8c098632: *stop=0x8c098632u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[14],4);if(io_status)return io_status;goto L8c098634; /* mov.l r14,@r0 */
L8c098634: *stop=0x8c098634u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[6],4);if(io_status)return io_status;goto L8c098636; /* mov.l r6,@r14 */
L8c098636: *stop=0x8c098638u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c098644; /* bra 0x8c098644 */
L8c09863a: *stop=0x8c09863au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c09863c; /* mov.l @r0,r4 */
L8c09863c: *stop=0x8c09863cu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c09863e; /* mov.l r6,@(0x4,r14) */
L8c09863e: *stop=0x8c09863eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c098640; /* mov.l r4,@r14 */
L8c098640: *stop=0x8c098640u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c098642; /* mov.l r14,@(0x4,r4) */
L8c098642: *stop=0x8c098642u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[14],4);if(io_status)return io_status;goto L8c098644; /* mov.l r14,@r0 */
L8c098644: *stop=0x8c098644u;r[0]=r[14];goto L8c098646; /* mov r14,r0 */
L8c098646: *stop=0x8c098646u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[6],4);if(io_status)return io_status;goto L8c098648; /* mov.l r6,@(0x1c,r14) */
L8c098648: *stop=0x8c098648u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[6],4);if(io_status)return io_status;goto L8c09864a; /* mov.l r6,@(0x20,r14) */
L8c09864a: *stop=0x8c09864au;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[6],4);if(io_status)return io_status;goto L8c09864c; /* mov.l r6,@(0x24,r14) */
L8c09864c: *stop=0x8c09864cu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[6],4);if(io_status)return io_status;goto L8c09864e; /* mov.l r6,@(0x28,r14) */
L8c09864e: *stop=0x8c09864eu;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[6],4);if(io_status)return io_status;goto L8c098650; /* mov.l r6,@(0x2c,r14) */
L8c098650: *stop=0x8c098650u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[6],4);if(io_status)return io_status;goto L8c098652; /* mov.l r6,@(0x14,r14) */
L8c098652: *stop=0x8c098652u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[6],4);if(io_status)return io_status;goto L8c098654; /* mov.l r6,@(0x18,r14) */
L8c098654: *stop=0x8c098654u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[6],4);if(io_status)return io_status;goto L8c098656; /* mov.l r6,@(0xc,r14) */
L8c098656: *stop=0x8c098656u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c098658; /* mov.l r6,@(0x8,r14) */
L8c098658: *stop=0x8c098658u;if(*pr!=entry_pr)return -3;*stop=0x8c09865au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
