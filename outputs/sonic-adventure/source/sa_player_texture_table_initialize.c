/* Original complete texture table initialization.
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
SA_STAGE_EXPORT int sa_player_texture_table_initialize_8c64e5e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e5e0;
L8c64e5e0: *stop=0x8c64e5e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e5e2; /* mov.l r14,@-r15 */
L8c64e5e2: *stop=0x8c64e5e2u;r[7]=4294967295u;goto L8c64e5e4; /* mov #-0x1,r7 */
L8c64e5e4: *stop=0x8c64e5e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e814u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64e5e6; /* mov.l 0x8c64e814,r14 */
L8c64e5e6: *stop=0x8c64e5e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e810u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e5e8; /* mov.l 0x8c64e810,r6 */
L8c64e5e8: *stop=0x8c64e5e8u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c64e5ea; /* mov.l r4,@r6 */
L8c64e5ea: *stop=0x8c64e5eau;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c64e5ec; /* mov.l r5,@r14 */
L8c64e5ec: *stop=0x8c64e5ecu;r[5]=0u;goto L8c64e5ee; /* mov #0x0,r5 */
L8c64e5ee: *stop=0x8c64e5eeu;r[1]=r[5];goto L8c64e5f0; /* mov r5,r1 */
L8c64e5f0: *stop=0x8c64e5f2u;r[4]=r[5];goto L8c64e60c; /* bra 0x8c64e60c */
L8c64e5f4: *stop=0x8c64e5f4u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e5f6; /* mov.l @r6,r2 */
L8c64e5f6: *stop=0x8c64e5f6u;r[0]=64u;goto L8c64e5f8; /* mov #0x40,r0 */
L8c64e5f8: *stop=0x8c64e5f8u;r[1]+=1u;goto L8c64e5fa; /* add #0x1,r1 */
L8c64e5fa: *stop=0x8c64e5fau;r[2]+=r[4];goto L8c64e5fc; /* add r4,r2 */
L8c64e5fc: *stop=0x8c64e5fcu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[7],4);if(io_status)return io_status;goto L8c64e5fe; /* mov.l r7,@r2 */
L8c64e5fe: *stop=0x8c64e5feu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e600; /* mov.l @r6,r3 */
L8c64e600: *stop=0x8c64e600u;r[3]+=r[4];goto L8c64e602; /* add r4,r3 */
L8c64e602: *stop=0x8c64e602u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[7],4);if(io_status)return io_status;goto L8c64e604; /* mov.l r7,@(0x4,r3) */
L8c64e604: *stop=0x8c64e604u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e606; /* mov.l @r6,r2 */
L8c64e606: *stop=0x8c64e606u;r[2]+=r[4];goto L8c64e608; /* add r4,r2 */
L8c64e608: *stop=0x8c64e608u;r[4]+=68u;goto L8c64e60a; /* add #0x44,r4 */
L8c64e60a: *stop=0x8c64e60au;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[5],2);if(io_status)return io_status;goto L8c64e60c; /* mov.w r5,@(r0,r2) */
L8c64e60c: *stop=0x8c64e60cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e60e; /* mov.l @r14,r3 */
L8c64e60e: *stop=0x8c64e60eu;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c64e610; /* cmp/hs r3,r1 */
L8c64e610: if(!(*sr&1u))goto L8c64e5f4;goto L8c64e612; /* bf 0x8c64e5f4 */
L8c64e612: *stop=0x8c64e612u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e81cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e614; /* mov.l 0x8c64e81c,r3 */
L8c64e614: *stop=0x8c64e614u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e818u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e616; /* mov.l 0x8c64e818,r1 */
L8c64e616: *stop=0x8c64e616u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c64e618; /* mov.l r5,@r1 */
L8c64e618: *stop=0x8c64e618u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c64e61a; /* mov.l r5,@r3 */
L8c64e61a: *stop=0x8c64e61au;if(*pr!=entry_pr)return -3;*stop=0x8c64e61cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
