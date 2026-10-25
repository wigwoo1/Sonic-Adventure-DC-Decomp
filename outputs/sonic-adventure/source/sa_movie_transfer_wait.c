/* Original driver-control word update; explicit machine/platform state.
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
SA_STAGE_EXPORT int sa_movie_transfer_wait_8c65a5dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 unsigned completion_wait_visits=0;(void)completion_wait_visits;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a5dc;
L8c65a5dc: *stop=0x8c65a5dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a5de; /* mov.l r14,@-r15 */
L8c65a5de: *stop=0x8c65a5deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65a5e0; /* mov.l r13,@-r15 */
L8c65a5e0: *stop=0x8c65a5e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65a5e2; /* mov.l r12,@-r15 */
L8c65a5e2: *stop=0x8c65a5e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65a5e4; /* mov.l r11,@-r15 */
L8c65a5e4: *stop=0x8c65a5e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65a5e6; /* mov.l r10,@-r15 */
L8c65a5e6: *stop=0x8c65a5e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65a5e8; /* mov.l r9,@-r15 */
L8c65a5e8: *stop=0x8c65a5e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a5ea; /* sts.l PR,@-r15 */
L8c65a5ea: *stop=0x8c65a5eau;r[15]+=4294967280u;goto L8c65a5ec; /* add #-0x10,r15 */
L8c65a5ec: *stop=0x8c65a5ecu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a5ee; /* stc SR,r0 */
L8c65a5ee: *stop=0x8c65a5eeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a5f0; /* mov.w 0x8c65a6b2,r3 */
L8c65a5f0: *stop=0x8c65a5f0u;r[9]=0u;goto L8c65a5f2; /* mov #0x0,r9 */
L8c65a5f2: *stop=0x8c65a5f2u;r[11]=r[4];goto L8c65a5f4; /* mov r4,r11 */
L8c65a5f4: *stop=0x8c65a5f4u;r[13]=r[11];goto L8c65a5f6; /* mov r11,r13 */
L8c65a5f6: *stop=0x8c65a5f6u;r[0]>>=2;goto L8c65a5f8; /* shlr2 r0 */
L8c65a5f8: *stop=0x8c65a5f8u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c65a5fa; /* mov.l r5,@(0xc,r15) */
L8c65a5fa: *stop=0x8c65a5fau;r[0]>>=2;goto L8c65a5fc; /* shlr2 r0 */
L8c65a5fc: *stop=0x8c65a5fcu;r[14]=r[9];goto L8c65a5fe; /* mov r9,r14 */
L8c65a5fe: *stop=0x8c65a5feu;r[0]&=0xfu;goto L8c65a600; /* and #0xf,r0 */
L8c65a600: *stop=0x8c65a600u;r[12]=r[0];goto L8c65a602; /* mov r0,r12 */
L8c65a602: *stop=0x8c65a602u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a604; /* stc SR,r0 */
L8c65a604: *stop=0x8c65a604u;r[0]&=r[3];goto L8c65a606; /* and r3,r0 */
L8c65a606: *stop=0x8c65a606u;r[0]|=0xf0u;goto L8c65a608; /* or #0xf0,r0 */
L8c65a608: *stop=0x8c65a608u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a60a; /* ldc r0,SR */
L8c65a60a: *stop=0x8c65a60au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6b8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c65a60c; /* mov.l 0x8c65a6b8,r10 */
L8c65a60c: *stop=0x8c65a60cu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a60e; /* mov.l @r10,r2 */
L8c65a60e: *stop=0x8c65a60eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65a610; /* tst r2,r2 */
L8c65a610: if(!(*sr&1u))goto L8c65a618;goto L8c65a612; /* bf 0x8c65a618 */
L8c65a612: *stop=0x8c65a612u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6bcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a614; /* mov.l 0x8c65a6bc,r14 */
L8c65a614: *stop=0x8c65a616u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a618: *stop=0x8c65a618u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c65a61a; /* tst r11,r11 */
L8c65a61a: if(!(*sr&1u))goto L8c65a622;goto L8c65a61c; /* bf 0x8c65a622 */
L8c65a61c: *stop=0x8c65a61cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6c0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a61e; /* mov.l 0x8c65a6c0,r14 */
L8c65a61e: *stop=0x8c65a620u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a622: *stop=0x8c65a622u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a624; /* mov.l 0x8c65a6c4,r3 */
L8c65a624: *stop=0x8c65a624u;r[5]=r[15];goto L8c65a626; /* mov r15,r5 */
L8c65a626: *stop=0x8c65a626u;r[5]+=8u;goto L8c65a628; /* add #0x8,r5 */
L8c65a628: target=r[3];*pr=0x8c65a62cu;*stop=0x8c65a62au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a62c; /* jsr @r3 */
L8c65a62c: *stop=0x8c65a62cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a62e; /* mov.l @(0x8,r15),r0 */
L8c65a62e: *stop=0x8c65a62eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c65a630; /* cmp/eq #0x2,r0 */
L8c65a630: if(!(*sr&1u))goto L8c65a638;goto L8c65a632; /* bf 0x8c65a638 */
L8c65a632: *stop=0x8c65a632u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6c8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a634; /* mov.l 0x8c65a6c8,r14 */
L8c65a634: *stop=0x8c65a636u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a638: *stop=0x8c65a638u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65a63a; /* mov.w 0x8c65a6b4,r1 */
L8c65a63a: *stop=0x8c65a63au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a63c; /* mov.l 0x8c65a6cc,r2 */
L8c65a63c: *stop=0x8c65a63cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[9],4);if(io_status)return io_status;goto L8c65a63e; /* mov.l r9,@r2 */
L8c65a63e: *stop=0x8c65a63eu;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[9],4);if(io_status)return io_status;goto L8c65a640; /* mov.l r9,@(0x18,r13) */
L8c65a640: *stop=0x8c65a640u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a642; /* mov.l @r10,r0 */
L8c65a642: *stop=0x8c65a642u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a644; /* mov.l @(r0,r1),r0 */
L8c65a644: *stop=0x8c65a644u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65a646; /* cmp/eq #0x0,r0 */
L8c65a646: if((*sr&1u))goto L8c65a650;goto L8c65a648; /* bt 0x8c65a650 */
L8c65a648: *stop=0x8c65a648u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a64a; /* cmp/eq #0x1,r0 */
L8c65a64a: if((*sr&1u))goto L8c65a662;goto L8c65a64c; /* bt 0x8c65a662 */
L8c65a64c: *stop=0x8c65a64eu;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a650: *stop=0x8c65a650u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a652; /* mov.l 0x8c65a6d0,r3 */
L8c65a652: *stop=0x8c65a652u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a654; /* mov.l @(0xc,r15),r5 */
L8c65a654: target=r[3];*pr=0x8c65a658u;*stop=0x8c65a656u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a658; /* jsr @r3 */
L8c65a658: *stop=0x8c65a658u;r[14]=r[0];goto L8c65a65a; /* mov r0,r14 */
L8c65a65a: *stop=0x8c65a65au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a65c; /* tst r14,r14 */
L8c65a65c: if((*sr&1u))goto L8c65a6fa;goto L8c65a65e; /* bt 0x8c65a6fa */
L8c65a65e: *stop=0x8c65a660u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a662: *stop=0x8c65a662u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a664; /* mov.l 0x8c65a6d4,r3 */
L8c65a664: *stop=0x8c65a664u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a666; /* mov.l @(0xc,r15),r5 */
L8c65a666: target=r[3];*pr=0x8c65a66au;*stop=0x8c65a668u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a66a; /* jsr @r3 */
L8c65a66a: *stop=0x8c65a66au;r[14]=r[0];goto L8c65a66c; /* mov r0,r14 */
L8c65a66c: *stop=0x8c65a66cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a66e; /* tst r14,r14 */
L8c65a66e: if(!(*sr&1u))goto L8c65a6ee;goto L8c65a670; /* bf 0x8c65a6ee */
L8c65a670: *stop=0x8c65a670u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b6u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65a672; /* mov.w 0x8c65a6b6,r0 */
L8c65a672: *stop=0x8c65a672u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a674; /* mov.l @r10,r2 */
L8c65a674: *stop=0x8c65a674u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a676; /* mov.l @(r0,r2),r3 */
L8c65a676: *stop=0x8c65a676u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65a678; /* tst r3,r3 */
L8c65a678: if(!(*sr&1u))goto L8c65a6ee;goto L8c65a67a; /* bf 0x8c65a6ee */
L8c65a67a: *stop=0x8c65a67au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a67c; /* mov.l 0x8c65a6d8,r2 */
L8c65a67c: *stop=0x8c65a67cu;r[5]=r[15];goto L8c65a67e; /* mov r15,r5 */
L8c65a67e: *stop=0x8c65a67eu;r[5]+=4u;goto L8c65a680; /* add #0x4,r5 */
L8c65a680: target=r[2];*pr=0x8c65a684u;*stop=0x8c65a682u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a684; /* jsr @r2 */
L8c65a684: *stop=0x8c65a684u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a686; /* mov.l 0x8c65a6dc,r3 */
L8c65a686: *stop=0x8c65a686u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a688; /* mov.l @(0x4,r15),r5 */
L8c65a688: target=r[3];*pr=0x8c65a68cu;*stop=0x8c65a68au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a68c; /* jsr @r3 */
L8c65a68c: *stop=0x8c65a68cu;r[14]=r[0];goto L8c65a68e; /* mov r0,r14 */
L8c65a68e: *stop=0x8c65a68eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a690; /* tst r14,r14 */
L8c65a690: if(!(*sr&1u))goto L8c65a6ee;goto L8c65a692; /* bf 0x8c65a6ee */
L8c65a692: *stop=0x8c65a692u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c65a694; /* stc SR,r2 */
L8c65a694: *stop=0x8c65a694u;r[0]=r[12];goto L8c65a696; /* mov r12,r0 */
L8c65a696: *stop=0x8c65a696u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a698; /* mov.w 0x8c65a6b2,r3 */
L8c65a698: *stop=0x8c65a698u;r[0]&=0xfu;goto L8c65a69a; /* and #0xf,r0 */
L8c65a69a: *stop=0x8c65a69au;r[0]<<=2;goto L8c65a69c; /* shll2 r0 */
L8c65a69c: *stop=0x8c65a69cu;r[2]&=r[3];goto L8c65a69e; /* and r3,r2 */
L8c65a69e: *stop=0x8c65a69eu;r[0]<<=2;goto L8c65a6a0; /* shll2 r0 */
L8c65a6a0: *stop=0x8c65a6a0u;r[0]|=r[2];goto L8c65a6a2; /* or r2,r0 */
L8c65a6a2: *stop=0x8c65a6a2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a6a4; /* ldc r0,SR */
L8c65a6a4: *stop=0x8c65a6a4u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a6a6; /* mov.l @(0x10,r13),r0 */
L8c65a6a6: *stop=0x8c65a6a6u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c65a6a8; /* cmp/eq #-0x1,r0 */
L8c65a6a8: take=!(*sr&1u);*stop=0x8c65a6aau;r[12]=4294967295u;if(take)goto L8c65a6ee;goto L8c65a6ac; /* bf/s 0x8c65a6ee */
L8c65a6ac: *stop=0x8c65a6acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6e0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c65a6ae; /* mov.l 0x8c65a6e0,r11 */
L8c65a6ae: *stop=0x8c65a6b0u;goto L8c65a6e8; /* bra 0x8c65a6e8 */
L8c65a6e4: target=r[11];*pr=0x8c65a6e8u;*stop=0x8c65a6e6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a6e8; /* jsr @r11 */
L8c65a6e8: *stop=0x8c65a6e8u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a6ea; /* mov.l @(0x18,r13),r0 */
L8c65a6ea: *stop=0x8c65a6eau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a6ec; /* cmp/eq #0x1,r0 */
L8c65a6ec:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c65a6ecu;if(!(*sr&1u) && ++completion_wait_visits>=2u)return -4;
#endif
 if(!(*sr&1u))goto L8c65a6e4;goto L8c65a6ee; /* bf 0x8c65a6e4 */
L8c65a6ee: *stop=0x8c65a6eeu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a6f0; /* tst r14,r14 */
L8c65a6f0: if((*sr&1u))goto L8c65a6fa;goto L8c65a6f2; /* bt 0x8c65a6fa */
L8c65a6f2: *stop=0x8c65a6f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a72cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a6f4; /* mov.l 0x8c65a72c,r3 */
L8c65a6f4: *stop=0x8c65a6f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a728u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a6f6; /* mov.l 0x8c65a728,r4 */
L8c65a6f6: target=r[3];*pr=0x8c65a6fau;*stop=0x8c65a6f8u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a6fa; /* jsr @r3 */
L8c65a6fa: *stop=0x8c65a6fau;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c65a6fc; /* cmp/pz r12 */
L8c65a6fc: if(!(*sr&1u))goto L8c65a710;goto L8c65a6fe; /* bf 0x8c65a710 */
L8c65a6fe: *stop=0x8c65a6feu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c65a700; /* stc SR,r3 */
L8c65a700: *stop=0x8c65a700u;r[0]=r[12];goto L8c65a702; /* mov r12,r0 */
L8c65a702: *stop=0x8c65a702u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a724u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65a704; /* mov.w 0x8c65a724,r2 */
L8c65a704: *stop=0x8c65a704u;r[0]&=0xfu;goto L8c65a706; /* and #0xf,r0 */
L8c65a706: *stop=0x8c65a706u;r[0]<<=2;goto L8c65a708; /* shll2 r0 */
L8c65a708: *stop=0x8c65a708u;r[3]&=r[2];goto L8c65a70a; /* and r2,r3 */
L8c65a70a: *stop=0x8c65a70au;r[0]<<=2;goto L8c65a70c; /* shll2 r0 */
L8c65a70c: *stop=0x8c65a70cu;r[0]|=r[3];goto L8c65a70e; /* or r3,r0 */
L8c65a70e: *stop=0x8c65a70eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a710; /* ldc r0,SR */
L8c65a710: *stop=0x8c65a710u;r[0]=r[14];goto L8c65a712; /* mov r14,r0 */
L8c65a712: *stop=0x8c65a712u;r[15]+=16u;goto L8c65a714; /* add #0x10,r15 */
L8c65a714: *stop=0x8c65a714u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a716; /* lds.l @r15+,PR */
L8c65a716: *stop=0x8c65a716u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65a718; /* mov.l @r15+,r9 */
L8c65a718: *stop=0x8c65a718u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65a71a; /* mov.l @r15+,r10 */
L8c65a71a: *stop=0x8c65a71au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65a71c; /* mov.l @r15+,r11 */
L8c65a71c: *stop=0x8c65a71cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65a71e; /* mov.l @r15+,r12 */
L8c65a71e: *stop=0x8c65a71eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65a720; /* mov.l @r15+,r13 */
L8c65a720: *stop=0x8c65a720u;if(*pr!=entry_pr)return -3;*stop=0x8c65a722u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
