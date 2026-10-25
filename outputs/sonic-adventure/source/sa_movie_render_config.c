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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f7800u && (a&0x1fffffffu)<0x005f78c0u)return 1;
 if(n==4u && (a&0x1fffffffu)==0x005f688cu)return 1;
 if((n==1u||n==2u||n==4u) && !(a&(n-1u)) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_render_config_8c6526bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6526bc;
L8c6526bc: *stop=0x8c6526bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6526be; /* mov.l r14,@-r15 */
L8c6526be: *stop=0x8c6526beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6526c0; /* mov.l r13,@-r15 */
L8c6526c0: *stop=0x8c6526c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6526c2; /* mov.l r12,@-r15 */
L8c6526c2: *stop=0x8c6526c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6526c4; /* mov.l r11,@-r15 */
L8c6526c4: *stop=0x8c6526c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6526c6; /* mov.l r10,@-r15 */
L8c6526c6: *stop=0x8c6526c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6526c8; /* mov.l r9,@-r15 */
L8c6526c8: *stop=0x8c6526c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6526ca; /* mov.l r8,@-r15 */
L8c6526ca: *stop=0x8c6526cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6526cc; /* sts.l PR,@-r15 */
L8c6526cc: *stop=0x8c6526ccu;r[15]+=4294967284u;goto L8c6526ce; /* add #-0xc,r15 */
L8c6526ce: *stop=0x8c6526ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652754u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6526d0; /* mov.l 0x8c652754,r3 */
L8c6526d0: *stop=0x8c6526d0u;r[13]=0u;goto L8c6526d2; /* mov #0x0,r13 */
L8c6526d2: *stop=0x8c6526d2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6526d4; /* mov.l r4,@r15 */
L8c6526d4: *stop=0x8c6526d4u;r[10]=r[5];goto L8c6526d6; /* mov r5,r10 */
L8c6526d6: *stop=0x8c6526d6u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6526d8; /* mov.l r6,@(0x8,r15) */
L8c6526d8: *stop=0x8c6526d8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[7],4);if(io_status)return io_status;goto L8c6526da; /* mov.l r7,@(0x4,r15) */
L8c6526da: target=r[3];*pr=0x8c6526deu;*stop=0x8c6526dcu;r[14]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6526de; /* jsr @r3 */
L8c6526de: *stop=0x8c6526deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6526e0; /* mov.l @r15,r6 */
L8c6526e0: *stop=0x8c6526e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652758u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6526e2; /* mov.l 0x8c652758,r5 */
L8c6526e2: *stop=0x8c6526e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652744u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6526e4; /* mov.l 0x8c652744,r9 */
L8c6526e4: *stop=0x8c6526e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652748u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6526e6; /* mov.l 0x8c652748,r11 */
L8c6526e6: target=r[11];*pr=0x8c6526eau;*stop=0x8c6526e8u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6526ea; /* jsr @r11 */
L8c6526ea: *stop=0x8c6526eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65275cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6526ec; /* mov.l 0x8c65275c,r5 */
L8c6526ec: *stop=0x8c6526ecu;r[12]=r[0];goto L8c6526ee; /* mov r0,r12 */
L8c6526ee: *stop=0x8c6526eeu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6526f0; /* mov.l @(0x4,r15),r6 */
L8c6526f0: target=r[11];*pr=0x8c6526f4u;*stop=0x8c6526f2u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6526f4; /* jsr @r11 */
L8c6526f4: *stop=0x8c6526f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652760u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6526f6; /* mov.l 0x8c652760,r5 */
L8c6526f6: *stop=0x8c6526f6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6526f8; /* mov.l r0,@r15 */
L8c6526f8: *stop=0x8c6526f8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6526fa; /* mov.l @(0x8,r15),r6 */
L8c6526fa: target=r[11];*pr=0x8c6526feu;*stop=0x8c6526fcu;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6526fe; /* jsr @r11 */
L8c6526fe: *stop=0x8c6526feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65274cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652700; /* mov.l 0x8c65274c,r2 */
L8c652700: *stop=0x8c652700u;r[3]=3u;goto L8c652702; /* mov #0x3,r3 */
L8c652702: *stop=0x8c652702u;r[6]=r[0];goto L8c652704; /* mov r0,r6 */
L8c652704: *stop=0x8c652704u;r[3]&=r[12];goto L8c652706; /* and r12,r3 */
L8c652706: *stop=0x8c652706u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c652708; /* mov.l r3,@r2 */
L8c652708: *stop=0x8c652708u;r[0]=r[10];goto L8c65270a; /* mov r10,r0 */
L8c65270a: *stop=0x8c65270au;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c65270c; /* cmp/eq #0x5,r0 */
L8c65270c: take=(*sr&1u);*stop=0x8c65270eu;r[4]=1u;if(take)goto L8c652722;goto L8c652710; /* bt/s 0x8c652722 */
L8c652710: *stop=0x8c652710u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c652712; /* cmp/eq #0x4,r0 */
L8c652712: if((*sr&1u))goto L8c652722;goto L8c652714; /* bt 0x8c652722 */
L8c652714: *stop=0x8c652714u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c652716; /* cmp/eq #0x3,r0 */
L8c652716: if((*sr&1u))goto L8c652722;goto L8c652718; /* bt 0x8c652722 */
L8c652718: *stop=0x8c652718u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65271a; /* cmp/eq #0x1,r0 */
L8c65271a: if((*sr&1u))goto L8c652722;goto L8c65271c; /* bt 0x8c652722 */
L8c65271c: *stop=0x8c65271cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65271e; /* cmp/eq #0x0,r0 */
L8c65271e: if((*sr&1u))goto L8c652722;goto L8c652720; /* bt 0x8c652722 */
L8c652720: *stop=0x8c652720u;r[14]=r[4];goto L8c652722; /* mov r4,r14 */
L8c652722: *stop=0x8c652722u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c652724; /* mov.l @r15,r3 */
L8c652724: *stop=0x8c652724u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c652726; /* tst r3,r3 */
L8c652726: if((*sr&1u))goto L8c65272c;goto L8c652728; /* bt 0x8c65272c */
L8c652728: *stop=0x8c65272au;r[8]=r[4];goto L8c65272e; /* bra 0x8c65272e */
L8c65272c: *stop=0x8c65272cu;r[8]=r[13];goto L8c65272e; /* mov r13,r8 */
L8c65272e: *stop=0x8c65272eu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c652730; /* tst r6,r6 */
L8c652730: if((*sr&1u))goto L8c652780;goto L8c652732; /* bt 0x8c652780 */
L8c652732: *stop=0x8c652734u;r[9]=r[4];goto L8c652782; /* bra 0x8c652782 */
L8c652780: *stop=0x8c652780u;r[9]=r[13];goto L8c652782; /* mov r13,r9 */
L8c652782: *stop=0x8c652782u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c652784; /* tst r14,r14 */
L8c652784: if((*sr&1u))goto L8c65278a;goto L8c652786; /* bt 0x8c65278a */
L8c652786: *stop=0x8c652788u;goto L8c652880; /* bra 0x8c652880 */
L8c65278a: *stop=0x8c65278au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65278c; /* mov.l 0x8c6527e4,r3 */
L8c65278c: *stop=0x8c65278cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c65278e; /* mov.l r12,@r3 */
L8c65278e: *stop=0x8c65278eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652790; /* mov.l 0x8c6527e8,r2 */
L8c652790: *stop=0x8c652790u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[10],4);if(io_status)return io_status;goto L8c652792; /* mov.l r10,@r2 */
L8c652792: *stop=0x8c652792u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527ecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c652794; /* mov.l 0x8c6527ec,r1 */
L8c652794: *stop=0x8c652794u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[9],4);if(io_status)return io_status;goto L8c652796; /* mov.l r9,@r1 */
L8c652796: *stop=0x8c652796u;r[3]+=12u;goto L8c652798; /* add #0xc,r3 */
L8c652798: *stop=0x8c652798u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[8],4);if(io_status)return io_status;goto L8c65279a; /* mov.l r8,@r3 */
L8c65279a: *stop=0x8c65279au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6527e2u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65279c; /* mov.w 0x8c6527e2,r5 */
L8c65279c: *stop=0x8c65279cu;r[2]+=4294967292u;goto L8c65279e; /* add #-0x4,r2 */
L8c65279e: *stop=0x8c65279eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6527e0u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c6527a0; /* mov.w 0x8c6527e0,r6 */
L8c6527a0: *stop=0x8c6527a0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6527a2; /* mov.l @r2,r0 */
L8c6527a2: *stop=0x8c6527a2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6527a4; /* cmp/eq #0x1,r0 */
L8c6527a4: if(!(*sr&1u))goto L8c6527b2;goto L8c6527a6; /* bf 0x8c6527b2 */
L8c6527a6: *stop=0x8c6527a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6527a8; /* mov.l 0x8c6527f0,r1 */
L8c6527a8: *stop=0x8c6527a8u;r[11]=r[5];goto L8c6527aa; /* mov r5,r11 */
L8c6527aa: *stop=0x8c6527aau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6527ac; /* mov.l r13,@r15 */
L8c6527ac: *stop=0x8c6527acu;r[12]=r[6];goto L8c6527ae; /* mov r6,r12 */
L8c6527ae: *stop=0x8c6527b0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[12],4);if(io_status)return io_status;goto L8c652816; /* bra 0x8c652816 */
L8c6527b2: *stop=0x8c6527b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6527b4; /* mov.l 0x8c6527e4,r1 */
L8c6527b4: *stop=0x8c6527b4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6527b6; /* mov.l @r1,r0 */
L8c6527b6: *stop=0x8c6527b6u;r[0]&=r[4];goto L8c6527b8; /* and r4,r0 */
L8c6527b8: *stop=0x8c6527b8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6527ba; /* cmp/eq #0x1,r0 */
L8c6527ba: if((*sr&1u))goto L8c6527cc;goto L8c6527bc; /* bt 0x8c6527cc */
L8c6527bc: *stop=0x8c6527bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6527be; /* mov.l 0x8c6527e4,r3 */
L8c6527be: *stop=0x8c6527beu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6527c0; /* mov.l @r3,r0 */
L8c6527c0: *stop=0x8c6527c0u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c6527c2; /* tst #0x2c,r0 */
L8c6527c2: if((*sr&1u))goto L8c6527cc;goto L8c6527c4; /* bt 0x8c6527cc */
L8c6527c4: *stop=0x8c6527c4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6527c6; /* tst r4,r4 */
L8c6527c6: if((*sr&1u))goto L8c6527cc;goto L8c6527c8; /* bt 0x8c6527cc */
L8c6527c8: *stop=0x8c6527cau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6527ce; /* bra 0x8c6527ce */
L8c6527cc: *stop=0x8c6527ccu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6527ce; /* mov.l r13,@r15 */
L8c6527ce: *stop=0x8c6527ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6527e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6527d0; /* mov.l 0x8c6527e4,r3 */
L8c6527d0: *stop=0x8c6527d0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6527d2; /* mov.l @r3,r0 */
L8c6527d2: *stop=0x8c6527d2u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c6527d4; /* tst #0x10,r0 */
L8c6527d4: if((*sr&1u))goto L8c652800;goto L8c6527d6; /* bt 0x8c652800 */
L8c6527d6: *stop=0x8c6527d8u;r[11]=r[5];goto L8c652802; /* bra 0x8c652802 */
L8c652800: *stop=0x8c652800u;{uint32_t loaded=sx(m,bus,&io_status,0x8c652840u,2);if(io_status)return io_status;r[11]=loaded;}goto L8c652802; /* mov.w 0x8c652840,r11 */
L8c652802: *stop=0x8c652802u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652844u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652804; /* mov.l 0x8c652844,r3 */
L8c652804: *stop=0x8c652804u;{uint32_t loaded=sx(m,bus,&io_status,0x8c652842u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c652806; /* mov.w 0x8c652842,r5 */
L8c652806: *stop=0x8c652806u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c652808; /* mov.l @r3,r0 */
L8c652808: *stop=0x8c652808u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c65280a; /* tst #0x20,r0 */
L8c65280a: if((*sr&1u))goto L8c652810;goto L8c65280c; /* bt 0x8c652810 */
L8c65280c: *stop=0x8c65280eu;r[12]=r[6];goto L8c652812; /* bra 0x8c652812 */
L8c652810: *stop=0x8c652810u;r[12]=r[5];goto L8c652812; /* mov r5,r12 */
L8c652812: *stop=0x8c652812u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652848u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652814; /* mov.l 0x8c652848,r3 */
L8c652814: *stop=0x8c652814u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c652816; /* mov.l r5,@r3 */
L8c652816: *stop=0x8c652816u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65284cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652818; /* mov.l 0x8c65284c,r2 */
L8c652818: *stop=0x8c652818u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65281a; /* mov.l @r2,r0 */
L8c65281a: *stop=0x8c65281au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65281c; /* cmp/eq #0x1,r0 */
L8c65281c: if((*sr&1u))goto L8c65282a;goto L8c65281e; /* bt 0x8c65282a */
L8c65281e: *stop=0x8c65281eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c652820; /* cmp/eq #0x0,r0 */
L8c652820: if((*sr&1u))goto L8c652834;goto L8c652822; /* bt 0x8c652834 */
L8c652822: *stop=0x8c652822u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c652824; /* cmp/eq #0x2,r0 */
L8c652824: if((*sr&1u))goto L8c652860;goto L8c652826; /* bt 0x8c652860 */
L8c652826: *stop=0x8c652828u;goto L8c65286a; /* bra 0x8c65286a */
L8c65282a: *stop=0x8c65282au;{uint32_t loaded=rd(m,bus,&io_status,0x8c652850u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65282c; /* mov.l 0x8c652850,r3 */
L8c65282c: target=r[3];*pr=0x8c652830u;*stop=0x8c65282eu;r[14]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652830; /* jsr @r3 */
L8c652830: *stop=0x8c652832u;goto L8c65286c; /* bra 0x8c65286c */
L8c652834: *stop=0x8c652834u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652854u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652836; /* mov.l 0x8c652854,r3 */
L8c652836: target=r[3];*pr=0x8c65283au;*stop=0x8c652838u;r[14]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65283a; /* jsr @r3 */
L8c65283a: *stop=0x8c65283cu;goto L8c65286c; /* bra 0x8c65286c */
L8c652860: *stop=0x8c652860u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6528e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652862; /* mov.l 0x8c6528e4,r3 */
L8c652862: target=r[3];*pr=0x8c652866u;*stop=0x8c652864u;r[14]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652866; /* jsr @r3 */
L8c652866: *stop=0x8c652868u;goto L8c65286c; /* bra 0x8c65286c */
L8c65286a: *stop=0x8c65286au;r[14]=r[4];goto L8c65286c; /* mov r4,r14 */
L8c65286c: *stop=0x8c65286cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65286e; /* mov.l r8,@-r15 */
L8c65286e: *stop=0x8c65286eu;r[5]=r[12];goto L8c652870; /* mov r12,r5 */
L8c652870: *stop=0x8c652870u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c652872; /* mov.l r9,@-r15 */
L8c652872: *stop=0x8c652872u;r[6]=r[10];goto L8c652874; /* mov r10,r6 */
L8c652874: *stop=0x8c652874u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6528e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652876; /* mov.l 0x8c6528e8,r3 */
L8c652876: *stop=0x8c652876u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c652878; /* mov.l @(0x8,r15),r7 */
L8c652878: target=r[3];*pr=0x8c65287cu;*stop=0x8c65287au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65287c; /* jsr @r3 */
L8c65287c: target=0x8c6525c0u;*pr=0x8c652880u;*stop=0x8c65287eu;r[15]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652880; /* bsr 0x8c6525c0 */
L8c652880: *stop=0x8c652880u;r[0]=r[14];goto L8c652882; /* mov r14,r0 */
L8c652882: *stop=0x8c652882u;r[15]+=12u;goto L8c652884; /* add #0xc,r15 */
L8c652884: *stop=0x8c652884u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652886; /* lds.l @r15+,PR */
L8c652886: *stop=0x8c652886u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c652888; /* mov.l @r15+,r8 */
L8c652888: *stop=0x8c652888u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65288a; /* mov.l @r15+,r9 */
L8c65288a: *stop=0x8c65288au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65288c; /* mov.l @r15+,r10 */
L8c65288c: *stop=0x8c65288cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65288e; /* mov.l @r15+,r11 */
L8c65288e: *stop=0x8c65288eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c652890; /* mov.l @r15+,r12 */
L8c652890: *stop=0x8c652890u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c652892; /* mov.l @r15+,r13 */
L8c652892: *stop=0x8c652892u;if(*pr!=entry_pr)return -3;*stop=0x8c652894u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c6525c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6525c0;
L8c6525c0: *stop=0x8c6525c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6525c2; /* mov.l r14,@-r15 */
L8c6525c2: *stop=0x8c6525c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6525c4; /* sts.l PR,@-r15 */
L8c6525c4: *stop=0x8c6525c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65266cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6525c6; /* mov.l 0x8c65266c,r3 */
L8c6525c6: *stop=0x8c6525c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652668u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6525c8; /* mov.l 0x8c652668,r14 */
L8c6525c8: *stop=0x8c6525c8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6525ca; /* mov.l @r3,r5 */
L8c6525ca: target=r[14];*pr=0x8c6525ceu;*stop=0x8c6525ccu;r[4]=104u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525ce; /* jsr @r14 */
L8c6525ce: *stop=0x8c6525ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652670u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6525d0; /* mov.l 0x8c652670,r2 */
L8c6525d0: *stop=0x8c6525d0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6525d2; /* mov.l @r2,r5 */
L8c6525d2: target=r[14];*pr=0x8c6525d6u;*stop=0x8c6525d4u;r[4]=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525d6; /* jsr @r14 */
L8c6525d6: *stop=0x8c6525d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652674u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6525d8; /* mov.l 0x8c652674,r3 */
L8c6525d8: *stop=0x8c6525d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6525da; /* mov.l @r3,r5 */
L8c6525da: target=r[14];*pr=0x8c6525deu;*stop=0x8c6525dcu;r[4]=92u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525de; /* jsr @r14 */
L8c6525de: *stop=0x8c6525deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652678u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6525e0; /* mov.l 0x8c652678,r2 */
L8c6525e0: *stop=0x8c6525e0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6525e2; /* mov.l @r2,r5 */
L8c6525e2: target=r[14];*pr=0x8c6525e6u;*stop=0x8c6525e4u;r[4]=76u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525e6; /* jsr @r14 */
L8c6525e6: *stop=0x8c6525e6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c652660u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6525e8; /* mov.w 0x8c652660,r4 */
L8c6525e8: *stop=0x8c6525e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65267cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6525ea; /* mov.l 0x8c65267c,r3 */
L8c6525ea: target=r[14];*pr=0x8c6525eeu;*stop=0x8c6525ecu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525ee; /* jsr @r14 */
L8c6525ee: *stop=0x8c6525eeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c652662u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6525f0; /* mov.w 0x8c652662,r4 */
L8c6525f0: *stop=0x8c6525f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652680u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6525f2; /* mov.l 0x8c652680,r2 */
L8c6525f2: target=r[14];*pr=0x8c6525f6u;*stop=0x8c6525f4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525f6; /* jsr @r14 */
L8c6525f6: *stop=0x8c6525f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652684u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6525f8; /* mov.l 0x8c652684,r3 */
L8c6525f8: *stop=0x8c6525f8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6525fa; /* mov.l @r3,r5 */
L8c6525fa: target=r[14];*pr=0x8c6525feu;*stop=0x8c6525fcu;r[4]=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6525fe; /* jsr @r14 */
L8c6525fe: *stop=0x8c6525feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652688u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652600; /* mov.l 0x8c652688,r2 */
L8c652600: *stop=0x8c652600u;r[3]=4294967294u;goto L8c652602; /* mov #-0x2,r3 */
L8c652602: *stop=0x8c652602u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c652604; /* mov.l @r2,r5 */
L8c652604: *stop=0x8c652604u;r[5]&=r[3];goto L8c652606; /* and r3,r5 */
L8c652606: target=r[14];*pr=0x8c65260au;*stop=0x8c652608u;r[4]=68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65260a; /* jsr @r14 */
L8c65260a: *stop=0x8c65260au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65268cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65260c; /* mov.l 0x8c65268c,r3 */
L8c65260c: *stop=0x8c65260cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65260e; /* mov.l @r3,r0 */
L8c65260e: *stop=0x8c65260eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c652664u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c652610; /* mov.w 0x8c652664,r4 */
L8c652610: *stop=0x8c652610u;r[0]|=0x8u;goto L8c652612; /* or #0x8,r0 */
L8c652612: target=r[14];*pr=0x8c652616u;*stop=0x8c652614u;r[5]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652616; /* jsr @r14 */
L8c652616: *stop=0x8c652616u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652618; /* lds.l @r15+,PR */
L8c652618: *stop=0x8c652618u;if(*pr!=entry_pr)return -3;*stop=0x8c65261au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c65796e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65796e;
L8c65796e: *stop=0x8c65796eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c657970; /* mov.l r14,@-r15 */
L8c657970: *stop=0x8c657970u;r[14]=0u;goto L8c657972; /* mov #0x0,r14 */
L8c657972: *stop=0x8c657972u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c657974; /* mov.l r13,@-r15 */
L8c657974: *stop=0x8c657974u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c657976; /* sts.l PR,@-r15 */
L8c657976: *stop=0x8c657976u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657978; /* mov.l 0x8c657a04,r3 */
L8c657978: *stop=0x8c657978u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c65797a; /* mov.l r14,@r3 */
L8c65797a: *stop=0x8c65797au;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a08u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65797c; /* mov.l 0x8c657a08,r2 */
L8c65797c: *stop=0x8c65797cu;r[4]=4294967295u;goto L8c65797e; /* mov #-0x1,r4 */
L8c65797e: *stop=0x8c65797eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c657980; /* mov.l r4,@r2 */
L8c657980: *stop=0x8c657980u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a0cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657982; /* mov.l 0x8c657a0c,r1 */
L8c657982: *stop=0x8c657982u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c657984; /* mov.l r4,@r1 */
L8c657984: *stop=0x8c657984u;r[3]+=12u;goto L8c657986; /* add #0xc,r3 */
L8c657986: *stop=0x8c657986u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c657988; /* mov.l r4,@r3 */
L8c657988: *stop=0x8c657988u;r[2]+=12u;goto L8c65798a; /* add #0xc,r2 */
L8c65798a: *stop=0x8c65798au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c65798c; /* mov.l r14,@r2 */
L8c65798c: *stop=0x8c65798cu;r[1]+=12u;goto L8c65798e; /* add #0xc,r1 */
L8c65798e: *stop=0x8c65798eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c657990; /* mov.l r14,@r1 */
L8c657990: *stop=0x8c657990u;r[3]+=12u;goto L8c657992; /* add #0xc,r3 */
L8c657992: *stop=0x8c657992u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c657994; /* mov.l r14,@r3 */
L8c657994: *stop=0x8c657994u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657996; /* mov.l 0x8c657a10,r0 */
L8c657996: *stop=0x8c657996u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[14],4);if(io_status)return io_status;goto L8c657998; /* mov.l r14,@r0 */
L8c657998: *stop=0x8c657998u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65799a; /* mov.l 0x8c657a14,r1 */
L8c65799a: *stop=0x8c65799au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65799c; /* mov.l @r1,r2 */
L8c65799c: *stop=0x8c65799cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65799e; /* tst r2,r2 */
L8c65799e: take=(*sr&1u);*stop=0x8c6579a0u;r[13]=10u;if(take)goto L8c6579ba;goto L8c6579a2; /* bt/s 0x8c6579ba */
L8c6579a2: *stop=0x8c6579a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579a4; /* mov.l 0x8c657a18,r3 */
L8c6579a4: *stop=0x8c6579a4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6579a6; /* mov.l @r3,r2 */
L8c6579a6: target=r[2];*pr=0x8c6579aau;*stop=0x8c6579a8u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6579aa; /* jsr @r2 */
L8c6579aa: *stop=0x8c6579aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6579ac; /* mov.l 0x8c657a10,r2 */
L8c6579ac: *stop=0x8c6579acu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6579ae; /* mov.l @r2,r1 */
L8c6579ae: *stop=0x8c6579aeu;*sr=(*sr&~1u)|(r[1]>r[13]);goto L8c6579b0; /* cmp/hi r13,r1 */
L8c6579b0: if((*sr&1u))goto L8c6579ba;goto L8c6579b2; /* bt 0x8c6579ba */
L8c6579b2: *stop=0x8c6579b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6579b4; /* mov.l 0x8c657a14,r0 */
L8c6579b4: *stop=0x8c6579b4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579b6; /* mov.l @r0,r3 */
L8c6579b6: *stop=0x8c6579b6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6579b8; /* tst r3,r3 */
L8c6579b8: if(!(*sr&1u))goto L8c6579a2;goto L8c6579ba; /* bf 0x8c6579a2 */
L8c6579ba: *stop=0x8c6579bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6579bc; /* mov.l 0x8c657a1c,r0 */
L8c6579bc: *stop=0x8c6579bcu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579be; /* mov.l @r0,r3 */
L8c6579be: *stop=0x8c6579beu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6579c0; /* tst r3,r3 */
L8c6579c0: if(!(*sr&1u))goto L8c6579e6;goto L8c6579c2; /* bf 0x8c6579e6 */
L8c6579c2: *stop=0x8c6579c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579c4; /* mov.l 0x8c657a10,r3 */
L8c6579c4: *stop=0x8c6579c4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6579c6; /* mov.l r14,@r3 */
L8c6579c6: *stop=0x8c6579c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6579c8; /* mov.l 0x8c657a20,r1 */
L8c6579c8: *stop=0x8c6579c8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6579ca; /* mov.l @r1,r2 */
L8c6579ca: *stop=0x8c6579cau;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6579cc; /* tst r2,r2 */
L8c6579cc: if((*sr&1u))goto L8c6579e6;goto L8c6579ce; /* bt 0x8c6579e6 */
L8c6579ce: *stop=0x8c6579ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6579d0; /* mov.l 0x8c657a18,r2 */
L8c6579d0: *stop=0x8c6579d0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579d2; /* mov.l @r2,r3 */
L8c6579d2: target=r[3];*pr=0x8c6579d6u;*stop=0x8c6579d4u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6579d6; /* jsr @r3 */
L8c6579d6: *stop=0x8c6579d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579d8; /* mov.l 0x8c657a10,r3 */
L8c6579d8: *stop=0x8c6579d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6579da; /* mov.l @r3,r1 */
L8c6579da: *stop=0x8c6579dau;*sr=(*sr&~1u)|(r[1]>r[13]);goto L8c6579dc; /* cmp/hi r13,r1 */
L8c6579dc: if((*sr&1u))goto L8c6579e6;goto L8c6579de; /* bt 0x8c6579e6 */
L8c6579de: *stop=0x8c6579deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6579e0; /* mov.l 0x8c657a20,r0 */
L8c6579e0: *stop=0x8c6579e0u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6579e2; /* mov.l @r0,r2 */
L8c6579e2: *stop=0x8c6579e2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6579e4; /* tst r2,r2 */
L8c6579e4: if(!(*sr&1u))goto L8c6579ce;goto L8c6579e6; /* bf 0x8c6579ce */
L8c6579e6: *stop=0x8c6579e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6579e8; /* mov.l 0x8c657a20,r2 */
L8c6579e8: *stop=0x8c6579e8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6579ea; /* mov.l r14,@r2 */
L8c6579ea: *stop=0x8c6579eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6579ec; /* mov.l 0x8c657a14,r3 */
L8c6579ec: *stop=0x8c6579ecu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6579ee; /* mov.l r14,@r3 */
L8c6579ee: *stop=0x8c6579eeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6579f0; /* lds.l @r15+,PR */
L8c6579f0: *stop=0x8c6579f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6579f2; /* mov.l @r15+,r13 */
L8c6579f2: *stop=0x8c6579f2u;if(*pr!=entry_pr)return -3;*stop=0x8c6579f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c657e00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c657e00;
L8c657e00: *stop=0x8c657e00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c657e02; /* mov.l r14,@-r15 */
L8c657e02: *stop=0x8c657e02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c657e04; /* mov.l r13,@-r15 */
L8c657e04: *stop=0x8c657e04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c657e06; /* mov.l r12,@-r15 */
L8c657e06: *stop=0x8c657e06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c657e08; /* mov.l r11,@-r15 */
L8c657e08: *stop=0x8c657e08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c657e0a; /* mov.l r10,@-r15 */
L8c657e0a: *stop=0x8c657e0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c657e0c; /* mov.l r9,@-r15 */
L8c657e0c: *stop=0x8c657e0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c657e0e; /* mov.l r8,@-r15 */
L8c657e0e: *stop=0x8c657e0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c657e10; /* sts.l PR,@-r15 */
L8c657e10: *stop=0x8c657e10u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657ea0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c657e12; /* mov.w 0x8c657ea0,r0 */
L8c657e12: *stop=0x8c657e12u;r[15]+=r[0];goto L8c657e14; /* add r0,r15 */
L8c657e14: *stop=0x8c657e14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657ea8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657e16; /* mov.l 0x8c657ea8,r2 */
L8c657e16: *stop=0x8c657e16u;r[0]=r[15];goto L8c657e18; /* mov r15,r0 */
L8c657e18: *stop=0x8c657e18u;r[0]+=8u;goto L8c657e1a; /* add #0x8,r0 */
L8c657e1a: *stop=0x8c657e1au;r[11]=r[6];goto L8c657e1c; /* mov r6,r11 */
L8c657e1c: *stop=0x8c657e1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657e1e; /* mov.l @r2,r3 */
L8c657e1e: *stop=0x8c657e1eu;r[11]<<=2;goto L8c657e20; /* shll2 r11 */
L8c657e20: *stop=0x8c657e20u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657e22; /* mov.l @(0x4,r2),r1 */
L8c657e22: *stop=0x8c657e22u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c657e24; /* mov.l r3,@r0 */
L8c657e24: *stop=0x8c657e24u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[1],4);if(io_status)return io_status;goto L8c657e26; /* mov.l r1,@(0x4,r0) */
L8c657e26: *stop=0x8c657e26u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657e28; /* mov.l @(0xc,r2),r1 */
L8c657e28: *stop=0x8c657e28u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657e2a; /* mov.l @(0x8,r2),r3 */
L8c657e2a: *stop=0x8c657e2au;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[3],4);if(io_status)return io_status;goto L8c657e2c; /* mov.l r3,@(0x8,r0) */
L8c657e2c: *stop=0x8c657e2cu;if(!valid_access(r[0]+12u,4u))return -3;wr(m,bus,&io_status,r[0]+0xcu,r[1],4);if(io_status)return io_status;goto L8c657e2e; /* mov.l r1,@(0xc,r0) */
L8c657e2e: *stop=0x8c657e2eu;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657e30; /* mov.l @(0x14,r2),r1 */
L8c657e30: *stop=0x8c657e30u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657e32; /* mov.l @(0x10,r2),r3 */
L8c657e32: *stop=0x8c657e32u;if(!valid_access(r[0]+16u,4u))return -3;wr(m,bus,&io_status,r[0]+0x10u,r[3],4);if(io_status)return io_status;goto L8c657e34; /* mov.l r3,@(0x10,r0) */
L8c657e34: *stop=0x8c657e34u;r[3]=r[15];goto L8c657e36; /* mov r15,r3 */
L8c657e36: *stop=0x8c657e36u;if(!valid_access(r[0]+20u,4u))return -3;wr(m,bus,&io_status,r[0]+0x14u,r[1],4);if(io_status)return io_status;goto L8c657e38; /* mov.l r1,@(0x14,r0) */
L8c657e38: *stop=0x8c657e38u;r[3]+=8u;goto L8c657e3a; /* add #0x8,r3 */
L8c657e3a: *stop=0x8c657e3au;{uint32_t loaded=sx(m,bus,&io_status,0x8c657ea2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c657e3c; /* mov.w 0x8c657ea2,r0 */
L8c657e3c: *stop=0x8c657e3cu;r[11]+=r[3];goto L8c657e3e; /* add r3,r11 */
L8c657e3e: *stop=0x8c657e3eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657e40; /* mov.l @(r0,r15),r1 */
L8c657e40: *stop=0x8c657e40u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c657e42; /* tst r1,r1 */
L8c657e42: take=(*sr&1u);*stop=0x8c657e44u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[11]=loaded;}if(take)goto L8c657e4e;goto L8c657e46; /* bt/s 0x8c657e4e */
L8c657e46: *stop=0x8c657e46u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657ea4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c657e48; /* mov.w 0x8c657ea4,r1 */
L8c657e48: *stop=0x8c657e48u;*sr=(*sr&~1u)|(r[1]==r[5]);goto L8c657e4a; /* cmp/eq r1,r5 */
L8c657e4a: if(!(*sr&1u))goto L8c657e4e;goto L8c657e4c; /* bf 0x8c657e4e */
L8c657e4c: *stop=0x8c657e4cu;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c657e4e; /* shll r5 */
L8c657e4e: *stop=0x8c657e4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657eacu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657e50; /* mov.l 0x8c657eac,r3 */
L8c657e50: *stop=0x8c657e50u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c657e52; /* mov.l r4,@r3 */
L8c657e52: *stop=0x8c657e52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657eb0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657e54; /* mov.l 0x8c657eb0,r2 */
L8c657e54: *stop=0x8c657e54u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c657e56; /* mov.l r5,@r2 */
L8c657e56: *stop=0x8c657e56u;r[14]=r[15];goto L8c657e58; /* mov r15,r14 */
L8c657e58: *stop=0x8c657e58u;r[13]=0u;goto L8c657e5a; /* mov #0x0,r13 */
L8c657e5a: *stop=0x8c657e5au;r[0]=64u;goto L8c657e5c; /* mov #0x40,r0 */
L8c657e5c: *stop=0x8c657e5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657eb4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657e5e; /* mov.l 0x8c657eb4,r1 */
L8c657e5e: *stop=0x8c657e5eu;r[14]+=32u;goto L8c657e60; /* add #0x20,r14 */
L8c657e60: *stop=0x8c657e60u;r[12]=1u;goto L8c657e62; /* mov #0x1,r12 */
L8c657e62: *stop=0x8c657e62u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[12],4);if(io_status)return io_status;goto L8c657e64; /* mov.l r12,@(0x20,r14) */
L8c657e64: *stop=0x8c657e64u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[13],4);if(io_status)return io_status;goto L8c657e66; /* mov.l r13,@(0x38,r14) */
L8c657e66: *stop=0x8c657e66u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[13],4);if(io_status)return io_status;goto L8c657e68; /* mov.l r13,@(0x34,r14) */
L8c657e68: *stop=0x8c657e68u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c657e6a; /* mov.l r13,@(r0,r14) */
L8c657e6a: *stop=0x8c657e6au;r[0]=112u;goto L8c657e6c; /* mov #0x70,r0 */
L8c657e6c: *stop=0x8c657e6cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c657e6e; /* mov.l r13,@(r0,r14) */
L8c657e6e: *stop=0x8c657e6eu;r[0]=104u;goto L8c657e70; /* mov #0x68,r0 */
L8c657e70: *stop=0x8c657e70u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c657e72; /* mov.l r13,@(r0,r14) */
L8c657e72: *stop=0x8c657e72u;r[0]=100u;goto L8c657e74; /* mov #0x64,r0 */
L8c657e74: *stop=0x8c657e74u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c657e76; /* mov.l r13,@(r0,r14) */
L8c657e76: *stop=0x8c657e76u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657e78; /* mov.l @r1,r3 */
L8c657e78: *stop=0x8c657e78u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c657e7a; /* tst r12,r3 */
L8c657e7a: if((*sr&1u))goto L8c657e84;goto L8c657e7c; /* bt 0x8c657e84 */
L8c657e7c: *stop=0x8c657e7cu;r[0]=108u;goto L8c657e7e; /* mov #0x6c,r0 */
L8c657e7e: *stop=0x8c657e7eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[12],4);if(io_status)return io_status;goto L8c657e80; /* mov.l r12,@(r0,r14) */
L8c657e80: *stop=0x8c657e82u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[12],4);if(io_status)return io_status;goto L8c657e92; /* bra 0x8c657e92 */
L8c657e84: *stop=0x8c657e84u;r[0]=108u;goto L8c657e86; /* mov #0x6c,r0 */
L8c657e86: *stop=0x8c657e86u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c657e88; /* mov.l r13,@(r0,r14) */
L8c657e88: *stop=0x8c657e88u;r[0]=r[7];goto L8c657e8a; /* mov r7,r0 */
L8c657e8a: *stop=0x8c657e8au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657e8c; /* cmp/eq #0x1,r0 */
L8c657e8c: if(!(*sr&1u))goto L8c657ec0;goto L8c657e8e; /* bf 0x8c657ec0 */
L8c657e8e: *stop=0x8c657e8eu;r[2]=2u;goto L8c657e90; /* mov #0x2,r2 */
L8c657e90: *stop=0x8c657e90u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c657e92; /* mov.l r2,@(0x4,r15) */
L8c657e92: *stop=0x8c657e94u;r[8]=r[13];goto L8c657ec4; /* bra 0x8c657ec4 */
L8c657ec0: *stop=0x8c657ec0u;r[8]=r[12];goto L8c657ec2; /* mov r12,r8 */
L8c657ec2: *stop=0x8c657ec2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[12],4);if(io_status)return io_status;goto L8c657ec4; /* mov.l r12,@(0x4,r15) */
L8c657ec4: *stop=0x8c657ec4u;r[0]=r[11];goto L8c657ec6; /* mov r11,r0 */
L8c657ec6: *stop=0x8c657ec6u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c657ec8; /* cmp/eq #0x10,r0 */
L8c657ec8: if((*sr&1u))goto L8c657ed6;goto L8c657eca; /* bt 0x8c657ed6 */
L8c657eca: *stop=0x8c657ecau;*sr=(*sr&~1u)|(r[0]==0x00000018u);goto L8c657ecc; /* cmp/eq #0x18,r0 */
L8c657ecc: if((*sr&1u))goto L8c657f04;goto L8c657ece; /* bt 0x8c657f04 */
L8c657ece: *stop=0x8c657eceu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c657ed0; /* cmp/eq #0x20,r0 */
L8c657ed0: if((*sr&1u))goto L8c657f0e;goto L8c657ed2; /* bt 0x8c657f0e */
L8c657ed2: *stop=0x8c657ed4u;goto L8c657f18; /* bra 0x8c657f18 */
L8c657ed6: *stop=0x8c657ed6u;r[0]=r[6];goto L8c657ed8; /* mov r6,r0 */
L8c657ed8: *stop=0x8c657ed8u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c657eda; /* cmp/eq #0x3,r0 */
L8c657eda: if(!(*sr&1u))goto L8c657ee2;goto L8c657edc; /* bf 0x8c657ee2 */
L8c657edc: *stop=0x8c657edcu;r[2]=3u;goto L8c657ede; /* mov #0x3,r2 */
L8c657ede: *stop=0x8c657ee0u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[2],4);if(io_status)return io_status;goto L8c657ef4; /* bra 0x8c657ef4 */
L8c657ee2: *stop=0x8c657ee2u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c657ee4; /* tst r6,r6 */
L8c657ee4: if(!(*sr&1u))goto L8c657eec;goto L8c657ee6; /* bf 0x8c657eec */
L8c657ee6: *stop=0x8c657ee6u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[12],4);if(io_status)return io_status;goto L8c657ee8; /* mov.l r12,@(0x30,r14) */
L8c657ee8: *stop=0x8c657eeau;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c657efc; /* bra 0x8c657efc */
L8c657eec: *stop=0x8c657eecu;r[0]=r[6];goto L8c657eee; /* mov r6,r0 */
L8c657eee: *stop=0x8c657eeeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657ef0; /* cmp/eq #0x1,r0 */
L8c657ef0: if(!(*sr&1u))goto L8c657ef8;goto L8c657ef2; /* bf 0x8c657ef8 */
L8c657ef2: *stop=0x8c657ef2u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[13],4);if(io_status)return io_status;goto L8c657ef4; /* mov.l r13,@(0x30,r14) */
L8c657ef4: *stop=0x8c657ef6u;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[13],4);if(io_status)return io_status;goto L8c657efc; /* bra 0x8c657efc */
L8c657ef8: *stop=0x8c657ef8u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[12],4);if(io_status)return io_status;goto L8c657efa; /* mov.l r12,@(0x30,r14) */
L8c657efa: *stop=0x8c657efau;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c657efc; /* mov.l r12,@(0x3c,r14) */
L8c657efc: *stop=0x8c657efcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c657fa0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c657efe; /* mov.w 0x8c657fa0,r0 */
L8c657efe: *stop=0x8c657efeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657f00; /* mov.l @(r0,r15),r2 */
L8c657f00: *stop=0x8c657f02u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c657f18; /* bra 0x8c657f18 */
L8c657f04: *stop=0x8c657f04u;r[1]=4u;goto L8c657f06; /* mov #0x4,r1 */
L8c657f06: *stop=0x8c657f06u;r[3]=2u;goto L8c657f08; /* mov #0x2,r3 */
L8c657f08: *stop=0x8c657f08u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[1],4);if(io_status)return io_status;goto L8c657f0a; /* mov.l r1,@(0x30,r14) */
L8c657f0a: *stop=0x8c657f0cu;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[3],4);if(io_status)return io_status;goto L8c657f16; /* bra 0x8c657f16 */
L8c657f0e: *stop=0x8c657f0eu;r[2]=3u;goto L8c657f10; /* mov #0x3,r2 */
L8c657f10: *stop=0x8c657f10u;r[3]=6u;goto L8c657f12; /* mov #0x6,r3 */
L8c657f12: *stop=0x8c657f12u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[3],4);if(io_status)return io_status;goto L8c657f14; /* mov.l r3,@(0x30,r14) */
L8c657f14: *stop=0x8c657f14u;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[2],4);if(io_status)return io_status;goto L8c657f16; /* mov.l r2,@(0x3c,r14) */
L8c657f16: *stop=0x8c657f16u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[13],4);if(io_status)return io_status;goto L8c657f18; /* mov.l r13,@(0x2c,r14) */
L8c657f18: *stop=0x8c657f18u;*macl=r[11]*r[4];goto L8c657f1a; /* mul.l r11,r4 */
L8c657f1a: *stop=0x8c657f1au;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[13],4);if(io_status)return io_status;goto L8c657f1c; /* mov.l r13,@(0x28,r14) */
L8c657f1c: *stop=0x8c657f1cu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[7],4);if(io_status)return io_status;goto L8c657f1e; /* mov.l r7,@(0x24,r14) */
L8c657f1e: *stop=0x8c657f1eu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c657f20; /* mov.l r13,@(0x1c,r14) */
L8c657f20: *stop=0x8c657f20u;r[2]=*macl;goto L8c657f22; /* sts MACL,r2 */
L8c657f22: *stop=0x8c657f22u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c657f24; /* cmp/pz r2 */
L8c657f24: take=(*sr&1u);*stop=0x8c657f26u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c657f2a;goto L8c657f28; /* bt/s 0x8c657f2a */
L8c657f28: *stop=0x8c657f28u;r[2]+=63u;goto L8c657f2a; /* add #0x3f,r2 */
L8c657f2a: *stop=0x8c657f2au;r[3]=4294967290u;goto L8c657f2c; /* mov #-0x6,r3 */
L8c657f2c: *stop=0x8c657f2cu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c657f2e; /* shad r3,r2 */
L8c657f2e: *stop=0x8c657f2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657fa8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657f30; /* mov.l 0x8c657fa8,r3 */
L8c657f30: *stop=0x8c657f30u;r[0]=68u;goto L8c657f32; /* mov #0x44,r0 */
L8c657f32: *stop=0x8c657f32u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c657f34; /* mov.l r2,@(r0,r14) */
L8c657f34: *stop=0x8c657f34u;r[6]=32u;goto L8c657f36; /* mov #0x20,r6 */
L8c657f36: *stop=0x8c657f36u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657f38; /* mov.l @r15,r1 */
L8c657f38: target=r[3];*pr=0x8c657f3cu;*stop=0x8c657f3au;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657f3c; /* jsr @r3 */
L8c657f3c: *stop=0x8c657f3cu;r[10]=r[11];goto L8c657f3e; /* mov r11,r10 */
L8c657f3e: *stop=0x8c657f3eu;*sr=(*sr&~1u)|((int32_t)r[10]>=0);goto L8c657f40; /* cmp/pz r10 */
L8c657f40: *stop=0x8c657f40u;r[0]+=4294967295u;goto L8c657f42; /* add #-0x1,r0 */
L8c657f42: take=(*sr&1u);*stop=0x8c657f44u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;if(take)goto L8c657f48;goto L8c657f46; /* bt/s 0x8c657f48 */
L8c657f46: *stop=0x8c657f46u;r[10]+=7u;goto L8c657f48; /* add #0x7,r10 */
L8c657f48: *stop=0x8c657f48u;*sr=(*sr&~1u)|(r[10]&1u);r[10]=(r[10]>>1)|(r[10]&0x80000000u);goto L8c657f4a; /* shar r10 */
L8c657f4a: *stop=0x8c657f4au;r[0]=r[7];goto L8c657f4c; /* mov r7,r0 */
L8c657f4c: *stop=0x8c657f4cu;*sr=(*sr&~1u)|(r[10]&1u);r[10]=(r[10]>>1)|(r[10]&0x80000000u);goto L8c657f4e; /* shar r10 */
L8c657f4e: *stop=0x8c657f4eu;r[9]=r[5];goto L8c657f50; /* mov r5,r9 */
L8c657f50: *stop=0x8c657f50u;*sr=(*sr&~1u)|(r[10]&1u);r[10]=(r[10]>>1)|(r[10]&0x80000000u);goto L8c657f52; /* shar r10 */
L8c657f52: *stop=0x8c657f52u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657f54; /* cmp/eq #0x1,r0 */
L8c657f54: *stop=0x8c657f54u;*macl=r[4]*r[10];goto L8c657f56; /* mul.l r4,r10 */
L8c657f56: *stop=0x8c657f56u;r[10]=*macl;goto L8c657f58; /* sts MACL,r10 */
L8c657f58: take=!(*sr&1u);*stop=0x8c657f5au;r[9]+=4294967295u;if(take)goto L8c657fce;goto L8c657f5c; /* bf/s 0x8c657fce */
L8c657f5c: *stop=0x8c657f5cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c657fa2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c657f5e; /* mov.w 0x8c657fa2,r3 */
L8c657f5e: *stop=0x8c657f5eu;*sr=(*sr&~1u)|(r[3]==r[5]);goto L8c657f60; /* cmp/eq r3,r5 */
L8c657f60: if(!(*sr&1u))goto L8c657fc0;goto L8c657f62; /* bf 0x8c657fc0 */
L8c657f62: *stop=0x8c657f62u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657fa4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c657f64; /* mov.w 0x8c657fa4,r1 */
L8c657f64: *stop=0x8c657f64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657fa8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657f66; /* mov.l 0x8c657fa8,r2 */
L8c657f66: *stop=0x8c657f66u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[1],4);if(io_status)return io_status;goto L8c657f68; /* mov.l r1,@(0x4,r14) */
L8c657f68: *stop=0x8c657f68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657f6a; /* mov.l @r15,r1 */
L8c657f6a: target=r[2];*pr=0x8c657f6eu;*stop=0x8c657f6cu;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657f6e; /* jsr @r2 */
L8c657f6e: *stop=0x8c657f6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657facu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657f70; /* mov.l 0x8c657fac,r2 */
L8c657f70: *stop=0x8c657f70u;r[0]+=1u;goto L8c657f72; /* add #0x1,r0 */
L8c657f72: *stop=0x8c657f72u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],4);if(io_status)return io_status;goto L8c657f74; /* mov.l r0,@(0x8,r14) */
L8c657f74: *stop=0x8c657f74u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657f76; /* mov.l @r2,r0 */
L8c657f76: *stop=0x8c657f76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657fb0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657f78; /* mov.l 0x8c657fb0,r1 */
L8c657f78: *stop=0x8c657f78u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c657f7a; /* tst #0x2,r0 */
L8c657f7a: *stop=0x8c657f7au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c657f7c; /* mov.l r13,@r1 */
L8c657f7c: *stop=0x8c657f7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657fb4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657f7e; /* mov.l 0x8c657fb4,r3 */
L8c657f7e: *stop=0x8c657f80u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c65804c; /* bra 0x8c65804c */
L8c657fc0: *stop=0x8c657fc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658024u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657fc2; /* mov.l 0x8c658024,r1 */
L8c657fc2: *stop=0x8c657fc2u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[9],4);if(io_status)return io_status;goto L8c657fc4; /* mov.l r9,@(0x4,r14) */
L8c657fc4: *stop=0x8c657fc4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c657fc6; /* mov.l r13,@r1 */
L8c657fc6: *stop=0x8c657fc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658028u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657fc8; /* mov.l 0x8c658028,r2 */
L8c657fc8: *stop=0x8c657fc8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[10],4);if(io_status)return io_status;goto L8c657fca; /* mov.l r10,@r2 */
L8c657fca: *stop=0x8c657fccu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[12],4);if(io_status)return io_status;goto L8c65804c; /* bra 0x8c65804c */
L8c657fce: *stop=0x8c657fceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65802cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657fd0; /* mov.l 0x8c65802c,r3 */
L8c657fd0: *stop=0x8c657fd0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657fd2; /* mov.l @r3,r2 */
L8c657fd2: *stop=0x8c657fd2u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c657fd4; /* tst r12,r2 */
L8c657fd4: if((*sr&1u))goto L8c657fe4;goto L8c657fd6; /* bt 0x8c657fe4 */
L8c657fd6: *stop=0x8c657fd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658024u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657fd8; /* mov.l 0x8c658024,r2 */
L8c657fd8: *stop=0x8c657fd8u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[9],4);if(io_status)return io_status;goto L8c657fda; /* mov.l r9,@(0x4,r14) */
L8c657fda: *stop=0x8c657fdau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c657fdc; /* mov.l r13,@r2 */
L8c657fdc: *stop=0x8c657fdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658028u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657fde; /* mov.l 0x8c658028,r1 */
L8c657fde: *stop=0x8c657fdeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[10],4);if(io_status)return io_status;goto L8c657fe0; /* mov.l r10,@r1 */
L8c657fe0: *stop=0x8c657fe2u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[12],4);if(io_status)return io_status;goto L8c65804c; /* bra 0x8c65804c */
L8c657fe4: *stop=0x8c657fe4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658020u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c657fe6; /* mov.w 0x8c658020,r2 */
L8c657fe6: *stop=0x8c657fe6u;*sr=(*sr&~1u)|(r[2]==r[5]);goto L8c657fe8; /* cmp/eq r2,r5 */
L8c657fe8: if(!(*sr&1u))goto L8c658040;goto L8c657fea; /* bf 0x8c658040 */
L8c657fea: *stop=0x8c657feau;{uint32_t loaded=sx(m,bus,&io_status,0x8c658022u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c657fec; /* mov.w 0x8c658022,r0 */
L8c657fec: *stop=0x8c657fecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658024u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657fee; /* mov.l 0x8c658024,r1 */
L8c657fee: *stop=0x8c657feeu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c657ff0; /* mov.l r0,@(0x4,r14) */
L8c657ff0: *stop=0x8c657ff0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c657ff2; /* mov.l r13,@r1 */
L8c657ff2: *stop=0x8c657ff2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658028u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657ff4; /* mov.l 0x8c658028,r3 */
L8c657ff4: *stop=0x8c657ff4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c657ff6; /* mov.l r10,@r3 */
L8c657ff6: *stop=0x8c657ff6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658030u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657ff8; /* mov.l 0x8c658030,r2 */
L8c657ff8: *stop=0x8c657ff8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657ffa; /* mov.l @r15,r1 */
L8c657ffa: target=r[2];*pr=0x8c657ffeu;*stop=0x8c657ffcu;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657ffe; /* jsr @r2 */
L8c657ffe: *stop=0x8c657ffeu;r[0]+=1u;goto L8c658000; /* add #0x1,r0 */
L8c658000: *stop=0x8c658002u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],4);if(io_status)return io_status;goto L8c65804c; /* bra 0x8c65804c */
L8c658040: *stop=0x8c658040u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6580c4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658042; /* mov.l 0x8c6580c4,r1 */
L8c658042: *stop=0x8c658042u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[9],4);if(io_status)return io_status;goto L8c658044; /* mov.l r9,@(0x4,r14) */
L8c658044: *stop=0x8c658044u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c658046; /* mov.l r13,@r1 */
L8c658046: *stop=0x8c658046u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6580c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658048; /* mov.l 0x8c6580c8,r3 */
L8c658048: *stop=0x8c658048u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c65804a; /* mov.l r10,@r3 */
L8c65804a: *stop=0x8c65804au;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[12],4);if(io_status)return io_status;goto L8c65804c; /* mov.l r12,@(0x8,r14) */
L8c65804c: *stop=0x8c65804cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6580c2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65804e; /* mov.w 0x8c6580c2,r0 */
L8c65804e: *stop=0x8c65804eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6580c0u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c658050; /* mov.w 0x8c6580c0,r7 */
L8c658050: *stop=0x8c658050u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658052; /* mov.l @(r0,r15),r2 */
L8c658052: *stop=0x8c658052u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c658054; /* tst r2,r2 */
L8c658054: if((*sr&1u))goto L8c6580f6;goto L8c658056; /* bt 0x8c6580f6 */
L8c658056: *stop=0x8c658056u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6580ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658058; /* mov.l 0x8c6580cc,r3 */
L8c658058: *stop=0x8c658058u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65805a; /* mov.l @r3,r2 */
L8c65805a: *stop=0x8c65805au;r[2]|=r[12];goto L8c65805c; /* or r12,r2 */
L8c65805c: *stop=0x8c65805cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c65805e; /* mov.l r2,@r3 */
L8c65805e: *stop=0x8c65805eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6580d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658060; /* mov.l 0x8c6580d0,r2 */
L8c658060: *stop=0x8c658060u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[12],4);if(io_status)return io_status;goto L8c658062; /* mov.l r12,@(0xc,r14) */
L8c658062: *stop=0x8c658062u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c658064; /* mov.l @r2,r3 */
L8c658064: *stop=0x8c658064u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c658066; /* tst r12,r3 */
L8c658066: take=(*sr&1u);*stop=0x8c658068u;r[10]=2u;if(take)goto L8c658074;goto L8c65806a; /* bt/s 0x8c658074 */
L8c65806a: *stop=0x8c65806au;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[7],4);if(io_status)return io_status;goto L8c65806c; /* mov.l r7,@(0x10,r14) */
L8c65806c: *stop=0x8c65806cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65806e; /* mov.l @(0x4,r14),r3 */
L8c65806e: *stop=0x8c65806eu;r[3]+=4294967294u;goto L8c658070; /* add #-0x2,r3 */
L8c658070: *stop=0x8c658072u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c65818c; /* bra 0x8c65818c */
L8c658074: *stop=0x8c658074u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6580d0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658076; /* mov.l 0x8c6580d0,r1 */
L8c658076: *stop=0x8c658076u;r[3]=4294967276u;goto L8c658078; /* mov #-0x14,r3 */
L8c658078: *stop=0x8c658078u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65807a; /* mov.l @r1,r0 */
L8c65807a: *stop=0x8c65807au;r[0]&=r[3];goto L8c65807c; /* and r3,r0 */
L8c65807c: *stop=0x8c65807cu;*sr=(*sr&~1u)|(r[0]==0x00000024u);goto L8c65807e; /* cmp/eq #0x24,r0 */
L8c65807e: if((*sr&1u))goto L8c658094;goto L8c658080; /* bt 0x8c658094 */
L8c658080: *stop=0x8c658080u;*sr=(*sr&~1u)|(r[0]==0x00000028u);goto L8c658082; /* cmp/eq #0x28,r0 */
L8c658082: if((*sr&1u))goto L8c658094;goto L8c658084; /* bt 0x8c658094 */
L8c658084: *stop=0x8c658084u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c658086; /* cmp/eq #0x20,r0 */
L8c658086: if((*sr&1u))goto L8c6580e0;goto L8c658088; /* bt 0x8c6580e0 */
L8c658088: *stop=0x8c658088u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65808a; /* cmp/eq #0x0,r0 */
L8c65808a: if((*sr&1u))goto L8c6580e0;goto L8c65808c; /* bt 0x8c6580e0 */
L8c65808c: *stop=0x8c65808cu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c65808e; /* cmp/eq #0x4,r0 */
L8c65808e: if((*sr&1u))goto L8c6580e0;goto L8c658090; /* bt 0x8c6580e0 */
L8c658090: *stop=0x8c658092u;goto L8c65818c; /* bra 0x8c65818c */
L8c658094: *stop=0x8c658094u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6580d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658096; /* mov.l 0x8c6580d4,r2 */
L8c658096: *stop=0x8c658096u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[7],4);if(io_status)return io_status;goto L8c658098; /* mov.l r7,@(0x10,r14) */
L8c658098: *stop=0x8c658098u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65809a; /* mov.l @r15,r1 */
L8c65809a: target=r[2];*pr=0x8c65809eu;*stop=0x8c65809cu;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65809e; /* jsr @r2 */
L8c65809e: *stop=0x8c65809eu;r[0]+=1u;goto L8c6580a0; /* add #0x1,r0 */
L8c6580a0: *stop=0x8c6580a0u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],4);if(io_status)return io_status;goto L8c6580a2; /* mov.l r0,@(0x8,r14) */
L8c6580a2: *stop=0x8c6580a2u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6580a4; /* mov.l @(0x4,r14),r2 */
L8c6580a4: *stop=0x8c6580a4u;r[2]+=4294967294u;goto L8c6580a6; /* add #-0x2,r2 */
L8c6580a6: *stop=0x8c6580a8u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c65818c; /* bra 0x8c65818c */
L8c6580e0: *stop=0x8c6580e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658144u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6580e2; /* mov.l 0x8c658144,r0 */
L8c6580e2: *stop=0x8c6580e2u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[13],4);if(io_status)return io_status;goto L8c6580e4; /* mov.l r13,@r0 */
L8c6580e4: *stop=0x8c6580e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658148u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6580e6; /* mov.l 0x8c658148,r2 */
L8c6580e6: *stop=0x8c6580e6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c6580e8; /* mov.l r13,@r2 */
L8c6580e8: *stop=0x8c6580e8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658140u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6580ea; /* mov.w 0x8c658140,r3 */
L8c6580ea: *stop=0x8c6580eau;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6580ec; /* mov.l r3,@(0x10,r14) */
L8c6580ec: *stop=0x8c6580ecu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[12],4);if(io_status)return io_status;goto L8c6580ee; /* mov.l r12,@(0x8,r14) */
L8c6580ee: *stop=0x8c6580eeu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6580f0; /* mov.l @(0x4,r14),r1 */
L8c6580f0: *stop=0x8c6580f0u;r[1]+=4294967294u;goto L8c6580f2; /* add #-0x2,r1 */
L8c6580f2: *stop=0x8c6580f4u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[1],4);if(io_status)return io_status;goto L8c65818c; /* bra 0x8c65818c */
L8c6580f6: *stop=0x8c6580f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65814cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6580f8; /* mov.l 0x8c65814c,r3 */
L8c6580f8: *stop=0x8c6580f8u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[13],4);if(io_status)return io_status;goto L8c6580fa; /* mov.l r13,@(0xc,r14) */
L8c6580fa: *stop=0x8c6580fau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6580fc; /* mov.l @r3,r0 */
L8c6580fc: *stop=0x8c6580fcu;r[0]&=r[12];goto L8c6580fe; /* and r12,r0 */
L8c6580fe: *stop=0x8c6580feu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c658100; /* cmp/eq #0x1,r0 */
L8c658100: if(!(*sr&1u))goto L8c65810a;goto L8c658102; /* bf 0x8c65810a */
L8c658102: *stop=0x8c658102u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c658104; /* tst r13,r13 */
L8c658104: if((*sr&1u))goto L8c658160;goto L8c658106; /* bt 0x8c658160 */
L8c658106: *stop=0x8c658108u;goto L8c658120; /* bra 0x8c658120 */
L8c65810a: *stop=0x8c65810au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65814cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65810c; /* mov.l 0x8c65814c,r2 */
L8c65810c: *stop=0x8c65810cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65810e; /* mov.l @r2,r0 */
L8c65810e: *stop=0x8c65810eu;r[0]&=0x2cu;goto L8c658110; /* and #0x2c,r0 */
L8c658110: *stop=0x8c658110u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c658112; /* cmp/eq #0x20,r0 */
L8c658112: if(!(*sr&1u))goto L8c65811c;goto L8c658114; /* bf 0x8c65811c */
L8c658114: *stop=0x8c658114u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c658116; /* tst r12,r12 */
L8c658116: if((*sr&1u))goto L8c658160;goto L8c658118; /* bt 0x8c658160 */
L8c658118: *stop=0x8c65811au;goto L8c658120; /* bra 0x8c658120 */
L8c65811c: *stop=0x8c65811cu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c65811e; /* tst r13,r13 */
L8c65811e: if((*sr&1u))goto L8c658160;goto L8c658120; /* bt 0x8c658160 */
L8c658120: *stop=0x8c658120u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658140u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c658122; /* mov.w 0x8c658140,r3 */
L8c658122: *stop=0x8c658122u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658142u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658124; /* mov.w 0x8c658142,r2 */
L8c658124: *stop=0x8c658124u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c658126; /* mov.l r3,@(0x10,r14) */
L8c658126: *stop=0x8c658126u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658144u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658128; /* mov.l 0x8c658144,r3 */
L8c658128: *stop=0x8c658128u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c65812a; /* mov.l r2,@(0x4,r14) */
L8c65812a: *stop=0x8c65812au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c65812c; /* mov.l r13,@r3 */
L8c65812c: *stop=0x8c65812cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658148u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65812e; /* mov.l 0x8c658148,r2 */
L8c65812e: *stop=0x8c65812eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c658130; /* mov.l r13,@r2 */
L8c658130: *stop=0x8c658132u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[12],4);if(io_status)return io_status;goto L8c65818a; /* bra 0x8c65818a */
L8c658160: *stop=0x8c658160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658162; /* mov.l 0x8c6581e4,r2 */
L8c658162: *stop=0x8c658162u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c658164; /* mov.l @r2,r3 */
L8c658164: *stop=0x8c658164u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c658166; /* tst r12,r3 */
L8c658166: if((*sr&1u))goto L8c65816e;goto L8c658168; /* bt 0x8c65816e */
L8c658168: *stop=0x8c658168u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6581e0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65816a; /* mov.w 0x8c6581e0,r0 */
L8c65816a: *stop=0x8c65816cu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c65818a; /* bra 0x8c65818a */
L8c65816e: *stop=0x8c65816eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658170; /* mov.l 0x8c6581e4,r3 */
L8c658170: *stop=0x8c658170u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c658172; /* mov.l @r3,r1 */
L8c658172: *stop=0x8c658172u;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c658174; /* tst r6,r1 */
L8c658174: if((*sr&1u))goto L8c658180;goto L8c658176; /* bt 0x8c658180 */
L8c658176: *stop=0x8c658176u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[7],4);if(io_status)return io_status;goto L8c658178; /* mov.l r7,@(0x10,r14) */
L8c658178: *stop=0x8c658178u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65817a; /* mov.l @(0x4,r14),r1 */
L8c65817a: *stop=0x8c65817au;r[1]+=4294967294u;goto L8c65817c; /* add #-0x2,r1 */
L8c65817c: *stop=0x8c65817eu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[1],4);if(io_status)return io_status;goto L8c65818a; /* bra 0x8c65818a */
L8c658180: *stop=0x8c658180u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6581e0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c658182; /* mov.w 0x8c6581e0,r1 */
L8c658182: *stop=0x8c658182u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[1],4);if(io_status)return io_status;goto L8c658184; /* mov.l r1,@(0x10,r14) */
L8c658184: *stop=0x8c658184u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c658186; /* mov.l @(0x4,r14),r0 */
L8c658186: *stop=0x8c658186u;r[0]+=4294967295u;goto L8c658188; /* add #-0x1,r0 */
L8c658188: *stop=0x8c658188u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c65818a; /* mov.l r0,@(0x4,r14) */
L8c65818a: *stop=0x8c65818au;r[10]=r[12];goto L8c65818c; /* mov r12,r10 */
L8c65818c: *stop=0x8c65818cu;r[3]=64u;goto L8c65818e; /* mov #0x40,r3 */
L8c65818e: *stop=0x8c65818eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6581e2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658190; /* mov.w 0x8c6581e2,r2 */
L8c658190: *stop=0x8c658190u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;goto L8c658192; /* mov.l r3,@(0x14,r14) */
L8c658192: *stop=0x8c658192u;r[1]=r[4];goto L8c658194; /* mov r4,r1 */
L8c658194: *stop=0x8c658194u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658196; /* mov.l 0x8c6581e8,r3 */
L8c658196: *stop=0x8c658196u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c658198; /* mov.l r2,@(0x18,r14) */
L8c658198: target=r[3];*pr=0x8c65819cu;*stop=0x8c65819au;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65819c; /* jsr @r3 */
L8c65819c: *stop=0x8c65819cu;*macl=r[10]*r[0];goto L8c65819e; /* mul.l r10,r0 */
L8c65819e: *stop=0x8c65819eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6581a0; /* mov.l 0x8c6581ec,r2 */
L8c6581a0: *stop=0x8c6581a0u;r[0]=*macl;goto L8c6581a2; /* sts MACL,r0 */
L8c6581a2: *stop=0x8c6581a2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c6581a4; /* mov.l r0,@r2 */
L8c6581a4: *stop=0x8c6581a4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6581a6; /* mov.l @(0x4,r15),r1 */
L8c6581a6: *stop=0x8c6581a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6581a8; /* mov.l 0x8c6581e8,r3 */
L8c6581a8: *stop=0x8c6581a8u;*macl=r[1]*r[5];goto L8c6581aa; /* mul.l r1,r5 */
L8c6581aa: *stop=0x8c6581aau;r[1]=*macl;goto L8c6581ac; /* sts MACL,r1 */
L8c6581ac: target=r[3];*pr=0x8c6581b0u;*stop=0x8c6581aeu;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6581b0; /* jsr @r3 */
L8c6581b0: *stop=0x8c6581b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6581b2; /* mov.l 0x8c6581f0,r1 */
L8c6581b2: *stop=0x8c6581b2u;r[0]+=r[8];goto L8c6581b4; /* add r8,r0 */
L8c6581b4: *stop=0x8c6581b4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c6581b6; /* mov.l r0,@r1 */
L8c6581b6: *stop=0x8c6581b6u;r[0]=r[1];goto L8c6581b8; /* mov r1,r0 */
L8c6581b8: *stop=0x8c6581b8u;r[6]=15u;goto L8c6581ba; /* mov #0xf,r6 */
L8c6581ba: *stop=0x8c6581bau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6581bc; /* mov.l @r0,r3 */
L8c6581bc: *stop=0x8c6581bcu;*sr=(*sr&~1u)|(r[3]>r[6]);goto L8c6581be; /* cmp/hi r6,r3 */
L8c6581be: if(!(*sr&1u))goto L8c6581c4;goto L8c6581c0; /* bf 0x8c6581c4 */
L8c6581c0: *stop=0x8c6581c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6581f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6581c2; /* mov.l 0x8c6581f0,r3 */
L8c6581c2: *stop=0x8c6581c2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6581c4; /* mov.l r6,@r3 */
L8c6581c4: *stop=0x8c6581c4u;r[0]=76u;goto L8c6581c6; /* mov #0x4c,r0 */
L8c6581c6: *stop=0x8c6581c6u;r[2]=9u;goto L8c6581c8; /* mov #0x9,r2 */
L8c6581c8: *stop=0x8c6581c8u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c6581ca; /* mov.l r2,@(r0,r14) */
L8c6581ca: *stop=0x8c6581cau;r[0]=72u;goto L8c6581cc; /* mov #0x48,r0 */
L8c6581cc: *stop=0x8c6581ccu;r[3]=6u;goto L8c6581ce; /* mov #0x6,r3 */
L8c6581ce: *stop=0x8c6581ceu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c6581d0; /* mov.l r3,@(r0,r14) */
L8c6581d0: *stop=0x8c6581d0u;r[0]=r[11];goto L8c6581d2; /* mov r11,r0 */
L8c6581d2: *stop=0x8c6581d2u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c6581d4; /* cmp/eq #0x10,r0 */
L8c6581d4: if(!(*sr&1u))goto L8c658200;goto L8c6581d6; /* bf 0x8c658200 */
L8c6581d6: *stop=0x8c6581d6u;r[0]=80u;goto L8c6581d8; /* mov #0x50,r0 */
L8c6581d8: *stop=0x8c6581d8u;r[2]=8u;goto L8c6581da; /* mov #0x8,r2 */
L8c6581da: *stop=0x8c6581dcu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c658206; /* bra 0x8c658206 */
L8c658200: *stop=0x8c658200u;r[0]=80u;goto L8c658202; /* mov #0x50,r0 */
L8c658202: *stop=0x8c658202u;r[1]=11u;goto L8c658204; /* mov #0xb,r1 */
L8c658204: *stop=0x8c658204u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c658206; /* mov.l r1,@(r0,r14) */
L8c658206: *stop=0x8c658206u;r[0]=88u;goto L8c658208; /* mov #0x58,r0 */
L8c658208: *stop=0x8c658208u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658260u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65820a; /* mov.l 0x8c658260,r3 */
L8c65820a: *stop=0x8c65820au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c65820c; /* mov.l r13,@(r0,r14) */
L8c65820c: *stop=0x8c65820cu;r[0]=84u;goto L8c65820e; /* mov #0x54,r0 */
L8c65820e: *stop=0x8c65820eu;r[4]+=4294967295u;goto L8c658210; /* add #-0x1,r4 */
L8c658210: *stop=0x8c658210u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658212; /* mov.l r4,@(r0,r14) */
L8c658212: *stop=0x8c658212u;r[0]=96u;goto L8c658214; /* mov #0x60,r0 */
L8c658214: *stop=0x8c658214u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c658216; /* mov.l r13,@(r0,r14) */
L8c658216: *stop=0x8c658216u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658218; /* mov.l @r3,r0 */
L8c658218: *stop=0x8c658218u;r[0]&=r[12];goto L8c65821a; /* and r12,r0 */
L8c65821a: *stop=0x8c65821au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65821c; /* cmp/eq #0x1,r0 */
L8c65821c: if(!(*sr&1u))goto L8c658226;goto L8c65821e; /* bf 0x8c658226 */
L8c65821e: *stop=0x8c65821eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c658220; /* tst r13,r13 */
L8c658220: if((*sr&1u))goto L8c658280;goto L8c658222; /* bt 0x8c658280 */
L8c658222: *stop=0x8c658224u;goto L8c65823c; /* bra 0x8c65823c */
L8c658226: *stop=0x8c658226u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658260u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658228; /* mov.l 0x8c658260,r2 */
L8c658228: *stop=0x8c658228u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65822a; /* mov.l @r2,r0 */
L8c65822a: *stop=0x8c65822au;r[0]&=0x2cu;goto L8c65822c; /* and #0x2c,r0 */
L8c65822c: *stop=0x8c65822cu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c65822e; /* cmp/eq #0x20,r0 */
L8c65822e: if(!(*sr&1u))goto L8c658238;goto L8c658230; /* bf 0x8c658238 */
L8c658230: *stop=0x8c658230u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c658232; /* tst r12,r12 */
L8c658232: if((*sr&1u))goto L8c658280;goto L8c658234; /* bt 0x8c658280 */
L8c658234: *stop=0x8c658236u;goto L8c65823c; /* bra 0x8c65823c */
L8c658238: *stop=0x8c658238u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c65823a; /* tst r13,r13 */
L8c65823a: if((*sr&1u))goto L8c658280;goto L8c65823c; /* bt 0x8c658280 */
L8c65823c: *stop=0x8c65823cu;r[3]=0u;goto L8c65823e; /* mov #0x0,r3 */
L8c65823e: *stop=0x8c65823eu;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[5]);goto L8c658240; /* cmp/gt r5,r3 */
L8c658240: *stop=0x8c658240u;r[0]=92u;goto L8c658242; /* mov #0x5c,r0 */
L8c658242: *stop=0x8c658242u;{uint64_t sum=(uint64_t)r[5]+r[3]+(*sr&1u);r[5]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}goto L8c658244; /* addc r3,r5 */
L8c658244: *stop=0x8c658244u;*sr=(*sr&~1u)|(r[5]&1u);r[5]=(r[5]>>1)|(r[5]&0x80000000u);goto L8c658246; /* shar r5 */
L8c658246: *stop=0x8c658246u;r[5]+=4294967295u;goto L8c658248; /* add #-0x1,r5 */
L8c658248: *stop=0x8c65824au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c658284; /* bra 0x8c658284 */
L8c658280: *stop=0x8c658280u;r[0]=92u;goto L8c658282; /* mov #0x5c,r0 */
L8c658282: *stop=0x8c658282u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[9],4);if(io_status)return io_status;goto L8c658284; /* mov.l r9,@(r0,r14) */
L8c658284: *stop=0x8c658284u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6582c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658286; /* mov.l 0x8c6582c4,r3 */
L8c658286: *stop=0x8c658286u;r[0]=116u;goto L8c658288; /* mov #0x74,r0 */
L8c658288: *stop=0x8c658288u;r[2]=r[14];goto L8c65828a; /* mov r14,r2 */
L8c65828a: *stop=0x8c65828au;r[15]-=r[0];goto L8c65828c; /* sub r0,r15 */
L8c65828c: target=r[3];*pr=0x8c658290u;*stop=0x8c65828eu;r[1]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658290; /* jsr @r3 */
L8c658290: target=0x8c657c60u;*pr=0x8c658294u;*stop=0x8c658292u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658294; /* bsr 0x8c657c60 */
L8c658294: *stop=0x8c658294u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6582c8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658296; /* mov.l 0x8c6582c8,r1 */
L8c658296: *stop=0x8c658296u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c658298; /* mov.l r13,@r1 */
L8c658298: *stop=0x8c658298u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6582ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65829a; /* mov.l 0x8c6582cc,r3 */
L8c65829a: *stop=0x8c65829au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c65829c; /* mov.l r13,@r3 */
L8c65829c: *stop=0x8c65829cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6582c0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65829e; /* mov.w 0x8c6582c0,r1 */
L8c65829e: *stop=0x8c65829eu;r[0]=r[13];goto L8c6582a0; /* mov r13,r0 */
L8c6582a0: *stop=0x8c6582a0u;r[15]+=r[1];goto L8c6582a2; /* add r1,r15 */
L8c6582a2: *stop=0x8c6582a2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6582a4; /* lds.l @r15+,PR */
L8c6582a4: *stop=0x8c6582a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6582a6; /* mov.l @r15+,r8 */
L8c6582a6: *stop=0x8c6582a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6582a8; /* mov.l @r15+,r9 */
L8c6582a8: *stop=0x8c6582a8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6582aa; /* mov.l @r15+,r10 */
L8c6582aa: *stop=0x8c6582aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6582ac; /* mov.l @r15+,r11 */
L8c6582ac: *stop=0x8c6582acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6582ae; /* mov.l @r15+,r12 */
L8c6582ae: *stop=0x8c6582aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6582b0; /* mov.l @r15+,r13 */
L8c6582b0: *stop=0x8c6582b0u;if(*pr!=entry_pr)return -3;*stop=0x8c6582b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c6585c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6585c0;
L8c6585c0: *stop=0x8c6585c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6585c2; /* mov.l r14,@-r15 */
L8c6585c2: *stop=0x8c6585c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6585c4; /* sts.l PR,@-r15 */
L8c6585c4: *stop=0x8c6585c4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658660u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6585c6; /* mov.w 0x8c658660,r0 */
L8c6585c6: *stop=0x8c6585c6u;r[15]+=r[0];goto L8c6585c8; /* add r0,r15 */
L8c6585c8: *stop=0x8c6585c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658662u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6585ca; /* mov.w 0x8c658662,r3 */
L8c6585ca: *stop=0x8c6585cau;r[14]=r[15];goto L8c6585cc; /* mov r15,r14 */
L8c6585cc: *stop=0x8c6585ccu;r[5]=1u;goto L8c6585ce; /* mov #0x1,r5 */
L8c6585ce: *stop=0x8c6585ceu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6585d0; /* mov.l r3,@(0x8,r14) */
L8c6585d0: *stop=0x8c6585d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658674u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6585d2; /* mov.l 0x8c658674,r3 */
L8c6585d2: *stop=0x8c6585d2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6585d4; /* mov.l @r3,r0 */
L8c6585d4: *stop=0x8c6585d4u;r[0]&=r[5];goto L8c6585d6; /* and r5,r0 */
L8c6585d6: *stop=0x8c6585d6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6585d8; /* cmp/eq #0x1,r0 */
L8c6585d8: if((*sr&1u))goto L8c6585ec;goto L8c6585da; /* bt 0x8c6585ec */
L8c6585da: *stop=0x8c6585dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c658674u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6585dc; /* mov.l 0x8c658674,r2 */
L8c6585dc: *stop=0x8c6585dcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6585de; /* mov.l @r2,r0 */
L8c6585de: *stop=0x8c6585deu;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c6585e0; /* tst #0x2c,r0 */
L8c6585e0: if((*sr&1u))goto L8c6585ec;goto L8c6585e2; /* bt 0x8c6585ec */
L8c6585e2: *stop=0x8c6585e2u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6585e4; /* tst r5,r5 */
L8c6585e4: if((*sr&1u))goto L8c6585ec;goto L8c6585e6; /* bt 0x8c6585ec */
L8c6585e6: *stop=0x8c6585e6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658664u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6585e8; /* mov.w 0x8c658664,r3 */
L8c6585e8: *stop=0x8c6585eau;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;goto L8c6585f0; /* bra 0x8c6585f0 */
L8c6585ec: *stop=0x8c6585ecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c658666u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6585ee; /* mov.w 0x8c658666,r1 */
L8c6585ee: *stop=0x8c6585eeu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c6585f0; /* mov.l r1,@(0x14,r14) */
L8c6585f0: *stop=0x8c6585f0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658668u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6585f2; /* mov.w 0x8c658668,r3 */
L8c6585f2: *stop=0x8c6585f2u;r[0]=116u;goto L8c6585f4; /* mov #0x74,r0 */
L8c6585f4: *stop=0x8c6585f4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65866au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6585f6; /* mov.w 0x8c65866a,r2 */
L8c6585f6: *stop=0x8c6585f6u;r[4]=44u;goto L8c6585f8; /* mov #0x2c,r4 */
L8c6585f8: *stop=0x8c6585f8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c6585fa; /* mov.l r3,@r14 */
L8c6585fa: *stop=0x8c6585fau;r[6]=63u;goto L8c6585fc; /* mov #0x3f,r6 */
L8c6585fc: *stop=0x8c6585fcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65866cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6585fe; /* mov.w 0x8c65866c,r3 */
L8c6585fe: *stop=0x8c6585feu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c658600; /* mov.l r2,@(0x4,r14) */
L8c658600: *stop=0x8c658600u;r[2]=5u;goto L8c658602; /* mov #0x5,r2 */
L8c658602: *stop=0x8c658602u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[4],4);if(io_status)return io_status;goto L8c658604; /* mov.l r4,@(0xc,r14) */
L8c658604: *stop=0x8c658604u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c658606; /* mov.l r3,@(0x10,r14) */
L8c658606: *stop=0x8c658606u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65866eu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c658608; /* mov.w 0x8c65866e,r3 */
L8c658608: *stop=0x8c658608u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[6],4);if(io_status)return io_status;goto L8c65860a; /* mov.l r6,@(0x18,r14) */
L8c65860a: *stop=0x8c65860au;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c65860c; /* mov.l r2,@(0x1c,r14) */
L8c65860c: *stop=0x8c65860cu;r[2]=21u;goto L8c65860e; /* mov #0x15,r2 */
L8c65860e: *stop=0x8c65860eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658610; /* mov.l r3,@(r0,r14) */
L8c658610: *stop=0x8c658610u;r[0]=120u;goto L8c658612; /* mov #0x78,r0 */
L8c658612: *stop=0x8c658612u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658674u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658614; /* mov.l 0x8c658674,r3 */
L8c658614: *stop=0x8c658614u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c658616; /* mov.l r2,@(r0,r14) */
L8c658616: *stop=0x8c658616u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658618; /* mov.l @r3,r0 */
L8c658618: *stop=0x8c658618u;r[0]&=r[5];goto L8c65861a; /* and r5,r0 */
L8c65861a: *stop=0x8c65861au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65861c; /* cmp/eq #0x1,r0 */
L8c65861c: if((*sr&1u))goto L8c658630;goto L8c65861e; /* bt 0x8c658630 */
L8c65861e: *stop=0x8c65861eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658674u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658620; /* mov.l 0x8c658674,r1 */
L8c658620: *stop=0x8c658620u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658622; /* mov.l @r1,r2 */
L8c658622: *stop=0x8c658622u;*sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c658624; /* tst r2,r4 */
L8c658624: if((*sr&1u))goto L8c658630;goto L8c658626; /* bt 0x8c658630 */
L8c658626: *stop=0x8c658626u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c658628; /* tst r5,r5 */
L8c658628: if((*sr&1u))goto L8c658630;goto L8c65862a; /* bt 0x8c658630 */
L8c65862a: *stop=0x8c65862au;{uint32_t loaded=sx(m,bus,&io_status,0x8c658670u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65862c; /* mov.w 0x8c658670,r3 */
L8c65862c: *stop=0x8c65862eu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c658634; /* bra 0x8c658634 */
L8c658630: *stop=0x8c658630u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658672u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c658632; /* mov.w 0x8c658672,r1 */
L8c658632: *stop=0x8c658632u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[1],4);if(io_status)return io_status;goto L8c658634; /* mov.l r1,@(0x20,r14) */
L8c658634: *stop=0x8c658634u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658674u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658636; /* mov.l 0x8c658674,r3 */
L8c658636: *stop=0x8c658636u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658638; /* mov.l @r3,r0 */
L8c658638: *stop=0x8c658638u;r[0]&=r[5];goto L8c65863a; /* and r5,r0 */
L8c65863a: *stop=0x8c65863au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65863c; /* cmp/eq #0x1,r0 */
L8c65863c: take=(*sr&1u);*stop=0x8c65863eu;r[4]=0u;if(take)goto L8c658680;goto L8c658640; /* bt/s 0x8c658680 */
L8c658640: *stop=0x8c658640u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658674u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658642; /* mov.l 0x8c658674,r2 */
L8c658642: *stop=0x8c658642u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658644; /* mov.l @r2,r0 */
L8c658644: *stop=0x8c658644u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658646; /* tst #0x2c,r0 */
L8c658646: if((*sr&1u))goto L8c658680;goto L8c658648; /* bt 0x8c658680 */
L8c658648: *stop=0x8c658648u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c65864a; /* tst r5,r5 */
L8c65864a: if((*sr&1u))goto L8c658680;goto L8c65864c; /* bt 0x8c658680 */
L8c65864c: *stop=0x8c65864eu;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[5],4);if(io_status)return io_status;goto L8c658682; /* bra 0x8c658682 */
L8c658680: *stop=0x8c658680u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[4],4);if(io_status)return io_status;goto L8c658682; /* mov.l r4,@(0x30,r14) */
L8c658682: *stop=0x8c658682u;r[0]=64u;goto L8c658684; /* mov #0x40,r0 */
L8c658684: *stop=0x8c658684u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658700u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658686; /* mov.l 0x8c658700,r3 */
L8c658686: *stop=0x8c658686u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[4],4);if(io_status)return io_status;goto L8c658688; /* mov.l r4,@(0x34,r14) */
L8c658688: *stop=0x8c658688u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[4],4);if(io_status)return io_status;goto L8c65868a; /* mov.l r4,@(0x38,r14) */
L8c65868a: *stop=0x8c65868au;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[4],4);if(io_status)return io_status;goto L8c65868c; /* mov.l r4,@(0x3c,r14) */
L8c65868c: *stop=0x8c65868cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c65868e; /* mov.l r4,@(r0,r14) */
L8c65868e: *stop=0x8c65868eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658690; /* mov.l @r3,r0 */
L8c658690: *stop=0x8c658690u;r[0]&=r[5];goto L8c658692; /* and r5,r0 */
L8c658692: *stop=0x8c658692u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c658694; /* cmp/eq #0x1,r0 */
L8c658694: if(!(*sr&1u))goto L8c65869e;goto L8c658696; /* bf 0x8c65869e */
L8c658696: *stop=0x8c658696u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c658698; /* tst r4,r4 */
L8c658698: if((*sr&1u))goto L8c6586b8;goto L8c65869a; /* bt 0x8c6586b8 */
L8c65869a: *stop=0x8c65869cu;goto L8c6586b2; /* bra 0x8c6586b2 */
L8c65869e: *stop=0x8c65869eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658700u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6586a0; /* mov.l 0x8c658700,r2 */
L8c6586a0: *stop=0x8c6586a0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6586a2; /* mov.l @r2,r0 */
L8c6586a2: *stop=0x8c6586a2u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c6586a4; /* tst #0x2c,r0 */
L8c6586a4: if((*sr&1u))goto L8c6586ae;goto L8c6586a6; /* bt 0x8c6586ae */
L8c6586a6: *stop=0x8c6586a6u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6586a8; /* tst r5,r5 */
L8c6586a8: if((*sr&1u))goto L8c6586b8;goto L8c6586aa; /* bt 0x8c6586b8 */
L8c6586aa: *stop=0x8c6586acu;goto L8c6586b2; /* bra 0x8c6586b2 */
L8c6586ae: *stop=0x8c6586aeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6586b0; /* tst r4,r4 */
L8c6586b0: if((*sr&1u))goto L8c6586b8;goto L8c6586b2; /* bt 0x8c6586b8 */
L8c6586b2: *stop=0x8c6586b2u;r[0]=68u;goto L8c6586b4; /* mov #0x44,r0 */
L8c6586b4: *stop=0x8c6586b6u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c6586be; /* bra 0x8c6586be */
L8c6586b8: *stop=0x8c6586b8u;r[0]=68u;goto L8c6586ba; /* mov #0x44,r0 */
L8c6586ba: *stop=0x8c6586bau;r[2]=2u;goto L8c6586bc; /* mov #0x2,r2 */
L8c6586bc: *stop=0x8c6586bcu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c6586be; /* mov.l r2,@(r0,r14) */
L8c6586be: *stop=0x8c6586beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658704u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6586c0; /* mov.l 0x8c658704,r3 */
L8c6586c0: *stop=0x8c6586c0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6586c2; /* mov.l @r3,r0 */
L8c6586c2: *stop=0x8c6586c2u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c6586c4; /* cmp/eq #0x6,r0 */
L8c6586c4: if(!(*sr&1u))goto L8c6586ce;goto L8c6586c6; /* bf 0x8c6586ce */
L8c6586c6: *stop=0x8c6586c6u;r[0]=112u;goto L8c6586c8; /* mov #0x70,r0 */
L8c6586c8: *stop=0x8c6586c8u;r[1]=22u;goto L8c6586ca; /* mov #0x16,r1 */
L8c6586ca: *stop=0x8c6586ccu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c6586d2; /* bra 0x8c6586d2 */
L8c6586ce: *stop=0x8c6586ceu;r[0]=112u;goto L8c6586d0; /* mov #0x70,r0 */
L8c6586d0: *stop=0x8c6586d0u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c6586d2; /* mov.l r6,@(r0,r14) */
L8c6586d2: *stop=0x8c6586d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658700u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6586d4; /* mov.l 0x8c658700,r3 */
L8c6586d4: *stop=0x8c6586d4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6586d6; /* mov.l @r3,r0 */
L8c6586d6: *stop=0x8c6586d6u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c6586d8; /* tst #0x10,r0 */
L8c6586d8: take=(*sr&1u);*stop=0x8c6586dau;r[3]=31u;if(take)goto L8c658720;goto L8c6586dc; /* bt/s 0x8c658720 */
L8c6586dc: *stop=0x8c6586dcu;r[0]=108u;goto L8c6586de; /* mov #0x6c,r0 */
L8c6586de: *stop=0x8c6586e0u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658724; /* bra 0x8c658724 */
L8c658720: *stop=0x8c658720u;r[0]=108u;goto L8c658722; /* mov #0x6c,r0 */
L8c658722: *stop=0x8c658722u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c658724; /* mov.l r5,@(r0,r14) */
L8c658724: *stop=0x8c658724u;r[0]=72u;goto L8c658726; /* mov #0x48,r0 */
L8c658726: *stop=0x8c658726u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6587a0u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c658728; /* mov.w 0x8c6587a0,r7 */
L8c658728: *stop=0x8c658728u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[3],4);if(io_status)return io_status;goto L8c65872a; /* mov.l r3,@(0x24,r14) */
L8c65872a: *stop=0x8c65872au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6587a2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65872c; /* mov.w 0x8c6587a2,r3 */
L8c65872c: *stop=0x8c65872cu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[4],4);if(io_status)return io_status;goto L8c65872e; /* mov.l r4,@(0x28,r14) */
L8c65872e: *stop=0x8c65872eu;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[5],4);if(io_status)return io_status;goto L8c658730; /* mov.l r5,@(0x2c,r14) */
L8c658730: *stop=0x8c658730u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c658732; /* mov.l r5,@(r0,r14) */
L8c658732: *stop=0x8c658732u;r[0]=76u;goto L8c658734; /* mov #0x4c,r0 */
L8c658734: *stop=0x8c658734u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658736; /* mov.l r4,@(r0,r14) */
L8c658736: *stop=0x8c658736u;r[0]=80u;goto L8c658738; /* mov #0x50,r0 */
L8c658738: *stop=0x8c658738u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c65873a; /* mov.l r4,@(r0,r14) */
L8c65873a: *stop=0x8c65873au;r[0]=84u;goto L8c65873c; /* mov #0x54,r0 */
L8c65873c: *stop=0x8c65873cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c65873e; /* mov.l r4,@(r0,r14) */
L8c65873e: *stop=0x8c65873eu;r[0]=88u;goto L8c658740; /* mov #0x58,r0 */
L8c658740: *stop=0x8c658740u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658742; /* mov.l r4,@(r0,r14) */
L8c658742: *stop=0x8c658742u;r[0]=92u;goto L8c658744; /* mov #0x5c,r0 */
L8c658744: *stop=0x8c658744u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658746; /* mov.l r4,@(r0,r14) */
L8c658746: *stop=0x8c658746u;r[0]=104u;goto L8c658748; /* mov #0x68,r0 */
L8c658748: *stop=0x8c658748u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c65874a; /* mov.l r4,@(r0,r14) */
L8c65874a: *stop=0x8c65874au;r[0]=96u;goto L8c65874c; /* mov #0x60,r0 */
L8c65874c: *stop=0x8c65874cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c65874e; /* mov.l r4,@(r0,r14) */
L8c65874e: *stop=0x8c65874eu;r[0]=100u;goto L8c658750; /* mov #0x64,r0 */
L8c658750: *stop=0x8c658750u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[7],4);if(io_status)return io_status;goto L8c658752; /* mov.l r7,@(r0,r14) */
L8c658752: *stop=0x8c658752u;r[0]=r[3];goto L8c658754; /* mov r3,r0 */
L8c658754: *stop=0x8c658754u;r[0]+=74u;goto L8c658756; /* add #0x4a,r0 */
L8c658756: *stop=0x8c658756u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658758; /* mov.l r3,@(r0,r14) */
L8c658758: *stop=0x8c658758u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6587a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65875a; /* mov.l 0x8c6587a4,r3 */
L8c65875a: *stop=0x8c65875au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65875c; /* mov.l @r3,r0 */
L8c65875c: *stop=0x8c65875cu;r[0]&=r[5];goto L8c65875e; /* and r5,r0 */
L8c65875e: *stop=0x8c65875eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c658760; /* cmp/eq #0x1,r0 */
L8c658760: take=!(*sr&1u);*stop=0x8c658762u;r[6]=46u;if(take)goto L8c65876c;goto L8c658764; /* bf/s 0x8c65876c */
L8c658764: *stop=0x8c658764u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c658766; /* tst r4,r4 */
L8c658766: if((*sr&1u))goto L8c6587c0;goto L8c658768; /* bt 0x8c6587c0 */
L8c658768: *stop=0x8c65876au;goto L8c658780; /* bra 0x8c658780 */
L8c65876c: *stop=0x8c65876cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6587a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65876e; /* mov.l 0x8c6587a4,r2 */
L8c65876e: *stop=0x8c65876eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658770; /* mov.l @r2,r0 */
L8c658770: *stop=0x8c658770u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658772; /* tst #0x2c,r0 */
L8c658772: if((*sr&1u))goto L8c65877c;goto L8c658774; /* bt 0x8c65877c */
L8c658774: *stop=0x8c658774u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c658776; /* tst r5,r5 */
L8c658776: if((*sr&1u))goto L8c6587c0;goto L8c658778; /* bt 0x8c6587c0 */
L8c658778: *stop=0x8c65877au;goto L8c658780; /* bra 0x8c658780 */
L8c65877c: *stop=0x8c65877cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65877e; /* tst r4,r4 */
L8c65877e: if((*sr&1u))goto L8c6587c0;goto L8c658780; /* bt 0x8c6587c0 */
L8c658780: *stop=0x8c658780u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6587a0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c658782; /* mov.w 0x8c6587a0,r0 */
L8c658782: *stop=0x8c658782u;r[3]=45u;goto L8c658784; /* mov #0x2d,r3 */
L8c658784: *stop=0x8c658784u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658786; /* mov.l r3,@(r0,r14) */
L8c658786: *stop=0x8c658788u;goto L8c6587c4; /* bra 0x8c6587c4 */
L8c6587c0: *stop=0x8c6587c0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658860u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6587c2; /* mov.w 0x8c658860,r0 */
L8c6587c2: *stop=0x8c6587c2u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c6587c4; /* mov.l r6,@(r0,r14) */
L8c6587c4: *stop=0x8c6587c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658874u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6587c6; /* mov.l 0x8c658874,r3 */
L8c6587c6: *stop=0x8c6587c6u;r[0]+=4u;goto L8c6587c8; /* add #0x4,r0 */
L8c6587c8: *stop=0x8c6587c8u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c6587ca; /* mov.l r6,@(r0,r14) */
L8c6587ca: *stop=0x8c6587cau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[7],4);if(io_status)return io_status;goto L8c6587cc; /* mov.l r7,@r3 */
L8c6587cc: *stop=0x8c6587ccu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6587ce; /* mov.l @(0xc,r14),r2 */
L8c6587ce: *stop=0x8c6587ceu;r[1]=0u;goto L8c6587d0; /* mov #0x0,r1 */
L8c6587d0: *stop=0x8c6587d0u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[2]);goto L8c6587d2; /* cmp/gt r2,r1 */
L8c6587d2: *stop=0x8c6587d2u;{uint64_t sum=(uint64_t)r[2]+r[1]+(*sr&1u);r[2]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}goto L8c6587d4; /* addc r1,r2 */
L8c6587d4: *stop=0x8c6587d4u;r[1]=r[3];goto L8c6587d6; /* mov r3,r1 */
L8c6587d6: *stop=0x8c6587d6u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c6587d8; /* shar r2 */
L8c6587d8: *stop=0x8c6587d8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6587da; /* mov.l @r1,r0 */
L8c6587da: *stop=0x8c6587dau;r[2]+=r[0];goto L8c6587dc; /* add r0,r2 */
L8c6587dc: *stop=0x8c6587dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658878u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6587de; /* mov.l 0x8c658878,r0 */
L8c6587de: *stop=0x8c6587deu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6587e0; /* mov.l r2,@r0 */
L8c6587e0: *stop=0x8c6587e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65887cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6587e2; /* mov.l 0x8c65887c,r2 */
L8c6587e2: *stop=0x8c6587e2u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6587e4; /* mov.l @(0xc,r14),r3 */
L8c6587e4: *stop=0x8c6587e4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6587e6; /* mov.l r3,@r2 */
L8c6587e6: *stop=0x8c6587e6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658862u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6587e8; /* mov.w 0x8c658862,r0 */
L8c6587e8: *stop=0x8c6587e8u;r[2]=r[14];goto L8c6587ea; /* mov r14,r2 */
L8c6587ea: *stop=0x8c6587eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c658880u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6587ec; /* mov.l 0x8c658880,r3 */
L8c6587ec: *stop=0x8c6587ecu;r[15]-=r[0];goto L8c6587ee; /* sub r0,r15 */
L8c6587ee: target=r[3];*pr=0x8c6587f2u;*stop=0x8c6587f0u;r[1]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6587f2; /* jsr @r3 */
L8c6587f2: target=0x8c6582e0u;*pr=0x8c6587f6u;*stop=0x8c6587f4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6587f6; /* bsr 0x8c6582e0 */
L8c6587f6: *stop=0x8c6587f6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658862u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6587f8; /* mov.w 0x8c658862,r2 */
L8c6587f8: *stop=0x8c6587f8u;r[15]+=r[2];goto L8c6587fa; /* add r2,r15 */
L8c6587fa: *stop=0x8c6587fau;{uint32_t loaded=sx(m,bus,&io_status,0x8c658862u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6587fc; /* mov.w 0x8c658862,r1 */
L8c6587fc: *stop=0x8c6587fcu;r[15]+=r[1];goto L8c6587fe; /* add r1,r15 */
L8c6587fe: *stop=0x8c6587feu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c658800; /* lds.l @r15+,PR */
L8c658800: *stop=0x8c658800u;if(*pr!=entry_pr)return -3;*stop=0x8c658802u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c658804(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c658804;
L8c658804: *stop=0x8c658804u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c658806; /* mov.l r14,@-r15 */
L8c658806: *stop=0x8c658806u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c658808; /* sts.l PR,@-r15 */
L8c658808: *stop=0x8c658808u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658864u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65880a; /* mov.w 0x8c658864,r0 */
L8c65880a: *stop=0x8c65880au;r[15]+=r[0];goto L8c65880c; /* add r0,r15 */
L8c65880c: *stop=0x8c65880cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c658866u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65880e; /* mov.w 0x8c658866,r3 */
L8c65880e: *stop=0x8c65880eu;r[14]=r[15];goto L8c658810; /* mov r15,r14 */
L8c658810: *stop=0x8c658810u;r[4]=1u;goto L8c658812; /* mov #0x1,r4 */
L8c658812: *stop=0x8c658812u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c658814; /* mov.l r3,@(0x8,r14) */
L8c658814: *stop=0x8c658814u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658884u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658816; /* mov.l 0x8c658884,r3 */
L8c658816: *stop=0x8c658816u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658818; /* mov.l @r3,r0 */
L8c658818: *stop=0x8c658818u;r[0]&=r[4];goto L8c65881a; /* and r4,r0 */
L8c65881a: *stop=0x8c65881au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65881c; /* cmp/eq #0x1,r0 */
L8c65881c: if((*sr&1u))goto L8c658830;goto L8c65881e; /* bt 0x8c658830 */
L8c65881e: *stop=0x8c65881eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658884u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658820; /* mov.l 0x8c658884,r2 */
L8c658820: *stop=0x8c658820u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658822; /* mov.l @r2,r0 */
L8c658822: *stop=0x8c658822u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658824; /* tst #0x2c,r0 */
L8c658824: if((*sr&1u))goto L8c658830;goto L8c658826; /* bt 0x8c658830 */
L8c658826: *stop=0x8c658826u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c658828; /* tst r4,r4 */
L8c658828: if((*sr&1u))goto L8c658830;goto L8c65882a; /* bt 0x8c658830 */
L8c65882a: *stop=0x8c65882au;{uint32_t loaded=sx(m,bus,&io_status,0x8c658868u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65882c; /* mov.w 0x8c658868,r3 */
L8c65882c: *stop=0x8c65882eu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;goto L8c658834; /* bra 0x8c658834 */
L8c658830: *stop=0x8c658830u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65886au,2);if(io_status)return io_status;r[1]=loaded;}goto L8c658832; /* mov.w 0x8c65886a,r1 */
L8c658832: *stop=0x8c658832u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c658834; /* mov.l r1,@(0x14,r14) */
L8c658834: *stop=0x8c658834u;r[3]=126u;goto L8c658836; /* mov #0x7e,r3 */
L8c658836: *stop=0x8c658836u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65886cu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658838; /* mov.w 0x8c65886c,r2 */
L8c658838: *stop=0x8c658838u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c65883a; /* mov.l r3,@r14 */
L8c65883a: *stop=0x8c65883au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65886eu,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65883c; /* mov.w 0x8c65886e,r5 */
L8c65883c: *stop=0x8c65883cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658884u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65883e; /* mov.l 0x8c658884,r3 */
L8c65883e: *stop=0x8c65883eu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c658840; /* mov.l r2,@(0x4,r14) */
L8c658840: *stop=0x8c658840u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658842; /* mov.l @r3,r0 */
L8c658842: *stop=0x8c658842u;r[0]&=r[4];goto L8c658844; /* and r4,r0 */
L8c658844: *stop=0x8c658844u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c658846; /* cmp/eq #0x1,r0 */
L8c658846: take=(*sr&1u);*stop=0x8c658848u;r[6]=18u;if(take)goto L8c6588a0;goto L8c65884a; /* bt/s 0x8c6588a0 */
L8c65884a: *stop=0x8c65884au;{uint32_t loaded=rd(m,bus,&io_status,0x8c658884u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65884c; /* mov.l 0x8c658884,r2 */
L8c65884c: *stop=0x8c65884cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65884e; /* mov.l @r2,r0 */
L8c65884e: *stop=0x8c65884eu;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658850; /* tst #0x2c,r0 */
L8c658850: if((*sr&1u))goto L8c6588a0;goto L8c658852; /* bt 0x8c6588a0 */
L8c658852: *stop=0x8c658852u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c658854; /* tst r4,r4 */
L8c658854: if((*sr&1u))goto L8c6588a0;goto L8c658856; /* bt 0x8c6588a0 */
L8c658856: *stop=0x8c658856u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658870u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658858; /* mov.w 0x8c658870,r2 */
L8c658858: *stop=0x8c658858u;r[3]=36u;goto L8c65885a; /* mov #0x24,r3 */
L8c65885a: *stop=0x8c65885au;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c65885c; /* mov.l r3,@(0xc,r14) */
L8c65885c: *stop=0x8c65885eu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6588a4; /* bra 0x8c6588a4 */
L8c6588a0: *stop=0x8c6588a0u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[6],4);if(io_status)return io_status;goto L8c6588a2; /* mov.l r6,@(0xc,r14) */
L8c6588a2: *stop=0x8c6588a2u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[5],4);if(io_status)return io_status;goto L8c6588a4; /* mov.l r5,@(0x10,r14) */
L8c6588a4: *stop=0x8c6588a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658904u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6588a6; /* mov.l 0x8c658904,r3 */
L8c6588a6: *stop=0x8c6588a6u;r[2]=63u;goto L8c6588a8; /* mov #0x3f,r2 */
L8c6588a8: *stop=0x8c6588a8u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c6588aa; /* mov.l r2,@(0x18,r14) */
L8c6588aa: *stop=0x8c6588aau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6588ac; /* mov.l @r3,r0 */
L8c6588ac: *stop=0x8c6588acu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6588ae; /* tst #0x20,r0 */
L8c6588ae: take=(*sr&1u);*stop=0x8c6588b0u;r[7]=21u;if(take)goto L8c6588ba;goto L8c6588b2; /* bt/s 0x8c6588ba */
L8c6588b2: *stop=0x8c6588b2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658900u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6588b4; /* mov.w 0x8c658900,r1 */
L8c6588b4: *stop=0x8c6588b4u;r[0]=116u;goto L8c6588b6; /* mov #0x74,r0 */
L8c6588b6: *stop=0x8c6588b8u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c6588be; /* bra 0x8c6588be */
L8c6588ba: *stop=0x8c6588bau;r[0]=116u;goto L8c6588bc; /* mov #0x74,r0 */
L8c6588bc: *stop=0x8c6588bcu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6588be; /* mov.l r5,@(r0,r14) */
L8c6588be: *stop=0x8c6588beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658904u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6588c0; /* mov.l 0x8c658904,r3 */
L8c6588c0: *stop=0x8c6588c0u;r[0]=120u;goto L8c6588c2; /* mov #0x78,r0 */
L8c6588c2: *stop=0x8c6588c2u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[7],4);if(io_status)return io_status;goto L8c6588c4; /* mov.l r7,@(r0,r14) */
L8c6588c4: *stop=0x8c6588c4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6588c6; /* mov.l @r3,r0 */
L8c6588c6: *stop=0x8c6588c6u;r[0]&=r[4];goto L8c6588c8; /* and r4,r0 */
L8c6588c8: *stop=0x8c6588c8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6588ca; /* cmp/eq #0x1,r0 */
L8c6588ca: if((*sr&1u))goto L8c6588de;goto L8c6588cc; /* bt 0x8c6588de */
L8c6588cc: *stop=0x8c6588ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658904u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6588ce; /* mov.l 0x8c658904,r2 */
L8c6588ce: *stop=0x8c6588ceu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6588d0; /* mov.l @r2,r0 */
L8c6588d0: *stop=0x8c6588d0u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c6588d2; /* tst #0x2c,r0 */
L8c6588d2: if((*sr&1u))goto L8c6588de;goto L8c6588d4; /* bt 0x8c6588de */
L8c6588d4: *stop=0x8c6588d4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6588d6; /* tst r4,r4 */
L8c6588d6: if((*sr&1u))goto L8c6588de;goto L8c6588d8; /* bt 0x8c6588de */
L8c6588d8: *stop=0x8c6588d8u;r[3]=6u;goto L8c6588da; /* mov #0x6,r3 */
L8c6588da: *stop=0x8c6588dcu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c6588e2; /* bra 0x8c6588e2 */
L8c6588de: *stop=0x8c6588deu;r[1]=3u;goto L8c6588e0; /* mov #0x3,r1 */
L8c6588e0: *stop=0x8c6588e0u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c6588e2; /* mov.l r1,@(0x1c,r14) */
L8c6588e2: *stop=0x8c6588e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658904u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6588e4; /* mov.l 0x8c658904,r3 */
L8c6588e4: *stop=0x8c6588e4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6588e6; /* mov.l @r3,r0 */
L8c6588e6: *stop=0x8c6588e6u;r[0]&=r[4];goto L8c6588e8; /* and r4,r0 */
L8c6588e8: *stop=0x8c6588e8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6588ea; /* cmp/eq #0x1,r0 */
L8c6588ea: if((*sr&1u))goto L8c658920;goto L8c6588ec; /* bt 0x8c658920 */
L8c6588ec: *stop=0x8c6588ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658904u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6588ee; /* mov.l 0x8c658904,r2 */
L8c6588ee: *stop=0x8c6588eeu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6588f0; /* mov.l @r2,r0 */
L8c6588f0: *stop=0x8c6588f0u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c6588f2; /* tst #0x2c,r0 */
L8c6588f2: if((*sr&1u))goto L8c658920;goto L8c6588f4; /* bt 0x8c658920 */
L8c6588f4: *stop=0x8c6588f4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6588f6; /* tst r4,r4 */
L8c6588f6: if((*sr&1u))goto L8c658920;goto L8c6588f8; /* bt 0x8c658920 */
L8c6588f8: *stop=0x8c6588f8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658902u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6588fa; /* mov.w 0x8c658902,r3 */
L8c6588fa: *stop=0x8c6588fcu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c658924; /* bra 0x8c658924 */
L8c658920: *stop=0x8c658920u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658980u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c658922; /* mov.w 0x8c658980,r1 */
L8c658922: *stop=0x8c658922u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[1],4);if(io_status)return io_status;goto L8c658924; /* mov.l r1,@(0x20,r14) */
L8c658924: *stop=0x8c658924u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658984u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658926; /* mov.l 0x8c658984,r3 */
L8c658926: *stop=0x8c658926u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658928; /* mov.l @r3,r0 */
L8c658928: *stop=0x8c658928u;r[0]&=r[4];goto L8c65892a; /* and r4,r0 */
L8c65892a: *stop=0x8c65892au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65892c; /* cmp/eq #0x1,r0 */
L8c65892c: take=(*sr&1u);*stop=0x8c65892eu;r[7]=31u;if(take)goto L8c658940;goto L8c658930; /* bt/s 0x8c658940 */
L8c658930: *stop=0x8c658930u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658984u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658932; /* mov.l 0x8c658984,r2 */
L8c658932: *stop=0x8c658932u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658934; /* mov.l @r2,r0 */
L8c658934: *stop=0x8c658934u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658936; /* tst #0x2c,r0 */
L8c658936: if((*sr&1u))goto L8c658940;goto L8c658938; /* bt 0x8c658940 */
L8c658938: *stop=0x8c658938u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65893a; /* tst r4,r4 */
L8c65893a: if((*sr&1u))goto L8c658940;goto L8c65893c; /* bt 0x8c658940 */
L8c65893c: *stop=0x8c65893eu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[7],4);if(io_status)return io_status;goto L8c658944; /* bra 0x8c658944 */
L8c658940: *stop=0x8c658940u;r[2]=15u;goto L8c658942; /* mov #0xf,r2 */
L8c658942: *stop=0x8c658942u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[2],4);if(io_status)return io_status;goto L8c658944; /* mov.l r2,@(0x24,r14) */
L8c658944: *stop=0x8c658944u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658984u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658946; /* mov.l 0x8c658984,r3 */
L8c658946: *stop=0x8c658946u;r[5]=0u;goto L8c658948; /* mov #0x0,r5 */
L8c658948: *stop=0x8c658948u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[4],4);if(io_status)return io_status;goto L8c65894a; /* mov.l r4,@(0x28,r14) */
L8c65894a: *stop=0x8c65894au;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[5],4);if(io_status)return io_status;goto L8c65894c; /* mov.l r5,@(0x2c,r14) */
L8c65894c: *stop=0x8c65894cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65894e; /* mov.l @r3,r0 */
L8c65894e: *stop=0x8c65894eu;r[0]&=r[4];goto L8c658950; /* and r4,r0 */
L8c658950: *stop=0x8c658950u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c658952; /* cmp/eq #0x1,r0 */
L8c658952: if(!(*sr&1u))goto L8c65895c;goto L8c658954; /* bf 0x8c65895c */
L8c658954: *stop=0x8c658954u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c658956; /* tst r5,r5 */
L8c658956: if((*sr&1u))goto L8c6589a0;goto L8c658958; /* bt 0x8c6589a0 */
L8c658958: *stop=0x8c65895au;goto L8c658970; /* bra 0x8c658970 */
L8c65895c: *stop=0x8c65895cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658984u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65895e; /* mov.l 0x8c658984,r2 */
L8c65895e: *stop=0x8c65895eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658960; /* mov.l @r2,r0 */
L8c658960: *stop=0x8c658960u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658962; /* tst #0x2c,r0 */
L8c658962: if((*sr&1u))goto L8c65896c;goto L8c658964; /* bt 0x8c65896c */
L8c658964: *stop=0x8c658964u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c658966; /* tst r4,r4 */
L8c658966: if((*sr&1u))goto L8c6589a0;goto L8c658968; /* bt 0x8c6589a0 */
L8c658968: *stop=0x8c65896au;goto L8c658970; /* bra 0x8c658970 */
L8c65896c: *stop=0x8c65896cu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c65896e; /* tst r5,r5 */
L8c65896e: if((*sr&1u))goto L8c6589a0;goto L8c658970; /* bt 0x8c6589a0 */
L8c658970: *stop=0x8c658972u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[4],4);if(io_status)return io_status;goto L8c6589a2; /* bra 0x8c6589a2 */
L8c6589a0: *stop=0x8c6589a0u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[5],4);if(io_status)return io_status;goto L8c6589a2; /* mov.l r5,@(0x30,r14) */
L8c6589a2: *stop=0x8c6589a2u;r[0]=64u;goto L8c6589a4; /* mov #0x40,r0 */
L8c6589a4: *stop=0x8c6589a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658a24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6589a6; /* mov.l 0x8c658a24,r3 */
L8c6589a6: *stop=0x8c6589a6u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[5],4);if(io_status)return io_status;goto L8c6589a8; /* mov.l r5,@(0x34,r14) */
L8c6589a8: *stop=0x8c6589a8u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[5],4);if(io_status)return io_status;goto L8c6589aa; /* mov.l r5,@(0x38,r14) */
L8c6589aa: *stop=0x8c6589aau;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[5],4);if(io_status)return io_status;goto L8c6589ac; /* mov.l r5,@(0x3c,r14) */
L8c6589ac: *stop=0x8c6589acu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589ae; /* mov.l r5,@(r0,r14) */
L8c6589ae: *stop=0x8c6589aeu;r[0]=68u;goto L8c6589b0; /* mov #0x44,r0 */
L8c6589b0: *stop=0x8c6589b0u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589b2; /* mov.l r5,@(r0,r14) */
L8c6589b2: *stop=0x8c6589b2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6589b4; /* mov.l @r3,r0 */
L8c6589b4: *stop=0x8c6589b4u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c6589b6; /* cmp/eq #0x6,r0 */
L8c6589b6: if(!(*sr&1u))goto L8c6589c0;goto L8c6589b8; /* bf 0x8c6589c0 */
L8c6589b8: *stop=0x8c6589b8u;r[0]=112u;goto L8c6589ba; /* mov #0x70,r0 */
L8c6589ba: *stop=0x8c6589bau;r[1]=22u;goto L8c6589bc; /* mov #0x16,r1 */
L8c6589bc: *stop=0x8c6589beu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c6589c4; /* bra 0x8c6589c4 */
L8c6589c0: *stop=0x8c6589c0u;r[0]=112u;goto L8c6589c2; /* mov #0x70,r0 */
L8c6589c2: *stop=0x8c6589c2u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[7],4);if(io_status)return io_status;goto L8c6589c4; /* mov.l r7,@(r0,r14) */
L8c6589c4: *stop=0x8c6589c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658a28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6589c6; /* mov.l 0x8c658a28,r3 */
L8c6589c6: *stop=0x8c6589c6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6589c8; /* mov.l @r3,r0 */
L8c6589c8: *stop=0x8c6589c8u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c6589ca; /* tst #0x10,r0 */
L8c6589ca: if((*sr&1u))goto L8c6589d2;goto L8c6589cc; /* bt 0x8c6589d2 */
L8c6589cc: *stop=0x8c6589ccu;r[0]=108u;goto L8c6589ce; /* mov #0x6c,r0 */
L8c6589ce: *stop=0x8c6589d0u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589d6; /* bra 0x8c6589d6 */
L8c6589d2: *stop=0x8c6589d2u;r[0]=108u;goto L8c6589d4; /* mov #0x6c,r0 */
L8c6589d4: *stop=0x8c6589d4u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c6589d6; /* mov.l r4,@(r0,r14) */
L8c6589d6: *stop=0x8c6589d6u;r[0]=72u;goto L8c6589d8; /* mov #0x48,r0 */
L8c6589d8: *stop=0x8c6589d8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658a20u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6589da; /* mov.w 0x8c658a20,r3 */
L8c6589da: *stop=0x8c6589dau;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c6589dc; /* mov.l r4,@(r0,r14) */
L8c6589dc: *stop=0x8c6589dcu;r[0]=76u;goto L8c6589de; /* mov #0x4c,r0 */
L8c6589de: *stop=0x8c6589deu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589e0; /* mov.l r5,@(r0,r14) */
L8c6589e0: *stop=0x8c6589e0u;r[0]=80u;goto L8c6589e2; /* mov #0x50,r0 */
L8c6589e2: *stop=0x8c6589e2u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589e4; /* mov.l r5,@(r0,r14) */
L8c6589e4: *stop=0x8c6589e4u;r[0]=84u;goto L8c6589e6; /* mov #0x54,r0 */
L8c6589e6: *stop=0x8c6589e6u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589e8; /* mov.l r5,@(r0,r14) */
L8c6589e8: *stop=0x8c6589e8u;r[0]=88u;goto L8c6589ea; /* mov #0x58,r0 */
L8c6589ea: *stop=0x8c6589eau;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589ec; /* mov.l r5,@(r0,r14) */
L8c6589ec: *stop=0x8c6589ecu;r[0]=92u;goto L8c6589ee; /* mov #0x5c,r0 */
L8c6589ee: *stop=0x8c6589eeu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589f0; /* mov.l r5,@(r0,r14) */
L8c6589f0: *stop=0x8c6589f0u;r[0]=104u;goto L8c6589f2; /* mov #0x68,r0 */
L8c6589f2: *stop=0x8c6589f2u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c6589f4; /* mov.l r5,@(r0,r14) */
L8c6589f4: *stop=0x8c6589f4u;r[0]=r[3];goto L8c6589f6; /* mov r3,r0 */
L8c6589f6: *stop=0x8c6589f6u;r[0]+=84u;goto L8c6589f8; /* add #0x54,r0 */
L8c6589f8: *stop=0x8c6589f8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658a22u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6589fa; /* mov.w 0x8c658a22,r2 */
L8c6589fa: *stop=0x8c6589fau;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c6589fc; /* mov.l r3,@(r0,r14) */
L8c6589fc: *stop=0x8c6589fcu;r[0]=100u;goto L8c6589fe; /* mov #0x64,r0 */
L8c6589fe: *stop=0x8c6589feu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c658a00; /* mov.l r2,@(r0,r14) */
L8c658a00: *stop=0x8c658a00u;r[0]=96u;goto L8c658a02; /* mov #0x60,r0 */
L8c658a02: *stop=0x8c658a02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658a28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658a04; /* mov.l 0x8c658a28,r3 */
L8c658a04: *stop=0x8c658a04u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c658a06; /* mov.l r5,@(r0,r14) */
L8c658a06: *stop=0x8c658a06u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658a08; /* mov.l @r3,r0 */
L8c658a08: *stop=0x8c658a08u;r[0]&=r[4];goto L8c658a0a; /* and r4,r0 */
L8c658a0a: *stop=0x8c658a0au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c658a0c; /* cmp/eq #0x1,r0 */
L8c658a0c: if(!(*sr&1u))goto L8c658a40;goto L8c658a0e; /* bf 0x8c658a40 */
L8c658a0e: *stop=0x8c658a0eu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c658a10; /* tst r5,r5 */
L8c658a10: if((*sr&1u))goto L8c658a5a;goto L8c658a12; /* bt 0x8c658a5a */
L8c658a12: *stop=0x8c658a14u;goto L8c658a54; /* bra 0x8c658a54 */
L8c658a40: *stop=0x8c658a40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658ac4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658a42; /* mov.l 0x8c658ac4,r2 */
L8c658a42: *stop=0x8c658a42u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658a44; /* mov.l @r2,r0 */
L8c658a44: *stop=0x8c658a44u;*sr=(*sr&~1u)|!(r[0]&0x2cu);goto L8c658a46; /* tst #0x2c,r0 */
L8c658a46: if((*sr&1u))goto L8c658a50;goto L8c658a48; /* bt 0x8c658a50 */
L8c658a48: *stop=0x8c658a48u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c658a4a; /* tst r4,r4 */
L8c658a4a: if((*sr&1u))goto L8c658a5a;goto L8c658a4c; /* bt 0x8c658a5a */
L8c658a4c: *stop=0x8c658a4eu;goto L8c658a54; /* bra 0x8c658a54 */
L8c658a50: *stop=0x8c658a50u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c658a52; /* tst r5,r5 */
L8c658a52: if((*sr&1u))goto L8c658a5a;goto L8c658a54; /* bt 0x8c658a5a */
L8c658a54: *stop=0x8c658a54u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658ac0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c658a56; /* mov.w 0x8c658ac0,r0 */
L8c658a56: *stop=0x8c658a58u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c658a60; /* bra 0x8c658a60 */
L8c658a5a: *stop=0x8c658a5au;{uint32_t loaded=sx(m,bus,&io_status,0x8c658ac0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c658a5c; /* mov.w 0x8c658ac0,r0 */
L8c658a5c: *stop=0x8c658a5cu;r[2]=17u;goto L8c658a5e; /* mov #0x11,r2 */
L8c658a5e: *stop=0x8c658a5eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c658a60; /* mov.l r2,@(r0,r14) */
L8c658a60: *stop=0x8c658a60u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658ac0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c658a62; /* mov.w 0x8c658ac0,r3 */
L8c658a62: *stop=0x8c658a62u;r[0]+=4u;goto L8c658a64; /* add #0x4,r0 */
L8c658a64: *stop=0x8c658a64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658ac8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658a66; /* mov.l 0x8c658ac8,r2 */
L8c658a66: *stop=0x8c658a66u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c658a68; /* mov.l r6,@(r0,r14) */
L8c658a68: *stop=0x8c658a68u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c658a6a; /* mov.l r3,@r2 */
L8c658a6a: *stop=0x8c658a6au;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658a6c; /* mov.l @(0xc,r14),r1 */
L8c658a6c: *stop=0x8c658a6cu;r[3]=0u;goto L8c658a6e; /* mov #0x0,r3 */
L8c658a6e: *stop=0x8c658a6eu;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[1]);goto L8c658a70; /* cmp/gt r1,r3 */
L8c658a70: *stop=0x8c658a70u;{uint64_t sum=(uint64_t)r[1]+r[3]+(*sr&1u);r[1]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}goto L8c658a72; /* addc r3,r1 */
L8c658a72: *stop=0x8c658a72u;r[3]=r[2];goto L8c658a74; /* mov r2,r3 */
L8c658a74: *stop=0x8c658a74u;*sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u);goto L8c658a76; /* shar r1 */
L8c658a76: *stop=0x8c658a76u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658a78; /* mov.l @r3,r0 */
L8c658a78: *stop=0x8c658a78u;r[1]+=r[0];goto L8c658a7a; /* add r0,r1 */
L8c658a7a: *stop=0x8c658a7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c658accu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c658a7c; /* mov.l 0x8c658acc,r0 */
L8c658a7c: *stop=0x8c658a7cu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c658a7e; /* mov.l r1,@r0 */
L8c658a7e: *stop=0x8c658a7eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658ad0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658a80; /* mov.l 0x8c658ad0,r1 */
L8c658a80: *stop=0x8c658a80u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658a82; /* mov.l @(0xc,r14),r2 */
L8c658a82: *stop=0x8c658a82u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c658a84; /* mov.l r2,@r1 */
L8c658a84: *stop=0x8c658a84u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658ac2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c658a86; /* mov.w 0x8c658ac2,r0 */
L8c658a86: *stop=0x8c658a86u;r[2]=r[14];goto L8c658a88; /* mov r14,r2 */
L8c658a88: *stop=0x8c658a88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658ad4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658a8a; /* mov.l 0x8c658ad4,r3 */
L8c658a8a: *stop=0x8c658a8au;r[15]-=r[0];goto L8c658a8c; /* sub r0,r15 */
L8c658a8c: target=r[3];*pr=0x8c658a90u;*stop=0x8c658a8eu;r[1]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658a90; /* jsr @r3 */
L8c658a90: target=0x8c6582e0u;*pr=0x8c658a94u;*stop=0x8c658a92u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658a94; /* bsr 0x8c6582e0 */
L8c658a94: *stop=0x8c658a94u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658ac2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658a96; /* mov.w 0x8c658ac2,r2 */
L8c658a96: *stop=0x8c658a96u;r[15]+=r[2];goto L8c658a98; /* add r2,r15 */
L8c658a98: *stop=0x8c658a98u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658ac2u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c658a9a; /* mov.w 0x8c658ac2,r1 */
L8c658a9a: *stop=0x8c658a9au;r[15]+=r[1];goto L8c658a9c; /* add r1,r15 */
L8c658a9c: *stop=0x8c658a9cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c658a9e; /* lds.l @r15+,PR */
L8c658a9e: *stop=0x8c658a9eu;if(*pr!=entry_pr)return -3;*stop=0x8c658aa0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c658ae0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c658ae0;
L8c658ae0: *stop=0x8c658ae0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c658ae2; /* mov.l r14,@-r15 */
L8c658ae2: *stop=0x8c658ae2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c658ae4; /* sts.l PR,@-r15 */
L8c658ae4: *stop=0x8c658ae4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658be0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c658ae6; /* mov.w 0x8c658be0,r0 */
L8c658ae6: *stop=0x8c658ae6u;r[15]+=r[0];goto L8c658ae8; /* add r0,r15 */
L8c658ae8: *stop=0x8c658ae8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658be2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c658aea; /* mov.w 0x8c658be2,r3 */
L8c658aea: *stop=0x8c658aeau;r[14]=r[15];goto L8c658aec; /* mov r15,r14 */
L8c658aec: *stop=0x8c658aecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c658be4u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658aee; /* mov.w 0x8c658be4,r2 */
L8c658aee: *stop=0x8c658aeeu;r[0]=116u;goto L8c658af0; /* mov #0x74,r0 */
L8c658af0: *stop=0x8c658af0u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c658af2; /* mov.l r3,@(0x8,r14) */
L8c658af2: *stop=0x8c658af2u;r[3]=126u;goto L8c658af4; /* mov #0x7e,r3 */
L8c658af4: *stop=0x8c658af4u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c658af6; /* mov.l r2,@(0x14,r14) */
L8c658af6: *stop=0x8c658af6u;r[5]=40u;goto L8c658af8; /* mov #0x28,r5 */
L8c658af8: *stop=0x8c658af8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c658afa; /* mov.l r3,@r14 */
L8c658afa: *stop=0x8c658afau;r[3]=63u;goto L8c658afc; /* mov #0x3f,r3 */
L8c658afc: *stop=0x8c658afcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c658be8u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c658afe; /* mov.w 0x8c658be8,r4 */
L8c658afe: *stop=0x8c658afeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c658be6u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658b00; /* mov.w 0x8c658be6,r2 */
L8c658b00: *stop=0x8c658b00u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c658b02; /* mov.l r2,@(0x4,r14) */
L8c658b02: *stop=0x8c658b02u;r[2]=3u;goto L8c658b04; /* mov #0x3,r2 */
L8c658b04: *stop=0x8c658b04u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[5],4);if(io_status)return io_status;goto L8c658b06; /* mov.l r5,@(0xc,r14) */
L8c658b06: *stop=0x8c658b06u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c658b08; /* mov.l r4,@(0x10,r14) */
L8c658b08: *stop=0x8c658b08u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c658b0a; /* mov.l r3,@(0x18,r14) */
L8c658b0a: *stop=0x8c658b0au;{uint32_t loaded=sx(m,bus,&io_status,0x8c658beau,2);if(io_status)return io_status;r[3]=loaded;}goto L8c658b0c; /* mov.w 0x8c658bea,r3 */
L8c658b0c: *stop=0x8c658b0cu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c658b0e; /* mov.l r2,@(0x1c,r14) */
L8c658b0e: *stop=0x8c658b0eu;r[2]=15u;goto L8c658b10; /* mov #0xf,r2 */
L8c658b10: *stop=0x8c658b10u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c658b12; /* mov.l r3,@(0x20,r14) */
L8c658b12: *stop=0x8c658b12u;r[3]=21u;goto L8c658b14; /* mov #0x15,r3 */
L8c658b14: *stop=0x8c658b14u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[2],4);if(io_status)return io_status;goto L8c658b16; /* mov.l r2,@(0x24,r14) */
L8c658b16: *stop=0x8c658b16u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b18; /* mov.l r4,@(r0,r14) */
L8c658b18: *stop=0x8c658b18u;r[0]=120u;goto L8c658b1a; /* mov #0x78,r0 */
L8c658b1a: *stop=0x8c658b1au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658b1c; /* mov.l r3,@(r0,r14) */
L8c658b1c: *stop=0x8c658b1cu;r[0]=64u;goto L8c658b1e; /* mov #0x40,r0 */
L8c658b1e: *stop=0x8c658b1eu;r[4]=0u;goto L8c658b20; /* mov #0x0,r4 */
L8c658b20: *stop=0x8c658b20u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[4],4);if(io_status)return io_status;goto L8c658b22; /* mov.l r4,@(0x34,r14) */
L8c658b22: *stop=0x8c658b22u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[4],4);if(io_status)return io_status;goto L8c658b24; /* mov.l r4,@(0x38,r14) */
L8c658b24: *stop=0x8c658b24u;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[4],4);if(io_status)return io_status;goto L8c658b26; /* mov.l r4,@(0x3c,r14) */
L8c658b26: *stop=0x8c658b26u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b28; /* mov.l r4,@(r0,r14) */
L8c658b28: *stop=0x8c658b28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658bf4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658b2a; /* mov.l 0x8c658bf4,r3 */
L8c658b2a: *stop=0x8c658b2au;r[0]=68u;goto L8c658b2c; /* mov #0x44,r0 */
L8c658b2c: *stop=0x8c658b2cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b2e; /* mov.l r4,@(r0,r14) */
L8c658b2e: *stop=0x8c658b2eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658b30; /* mov.l @r3,r0 */
L8c658b30: *stop=0x8c658b30u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c658b32; /* cmp/eq #0x6,r0 */
L8c658b32: if(!(*sr&1u))goto L8c658b3c;goto L8c658b34; /* bf 0x8c658b3c */
L8c658b34: *stop=0x8c658b34u;r[0]=112u;goto L8c658b36; /* mov #0x70,r0 */
L8c658b36: *stop=0x8c658b36u;r[1]=22u;goto L8c658b38; /* mov #0x16,r1 */
L8c658b38: *stop=0x8c658b3au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c658b42; /* bra 0x8c658b42 */
L8c658b3c: *stop=0x8c658b3cu;r[0]=112u;goto L8c658b3e; /* mov #0x70,r0 */
L8c658b3e: *stop=0x8c658b3eu;r[2]=31u;goto L8c658b40; /* mov #0x1f,r2 */
L8c658b40: *stop=0x8c658b40u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c658b42; /* mov.l r2,@(r0,r14) */
L8c658b42: *stop=0x8c658b42u;r[0]=108u;goto L8c658b44; /* mov #0x6c,r0 */
L8c658b44: *stop=0x8c658b44u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b46; /* mov.l r4,@(r0,r14) */
L8c658b46: *stop=0x8c658b46u;r[3]=1u;goto L8c658b48; /* mov #0x1,r3 */
L8c658b48: *stop=0x8c658b48u;r[0]=72u;goto L8c658b4a; /* mov #0x48,r0 */
L8c658b4a: *stop=0x8c658b4au;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[4],4);if(io_status)return io_status;goto L8c658b4c; /* mov.l r4,@(0x28,r14) */
L8c658b4c: *stop=0x8c658b4cu;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c658b4e; /* mov.l r4,@(0x2c,r14) */
L8c658b4e: *stop=0x8c658b4eu;r[6]=35u;goto L8c658b50; /* mov #0x23,r6 */
L8c658b50: *stop=0x8c658b50u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[4],4);if(io_status)return io_status;goto L8c658b52; /* mov.l r4,@(0x30,r14) */
L8c658b52: *stop=0x8c658b52u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658b54; /* mov.l r3,@(r0,r14) */
L8c658b54: *stop=0x8c658b54u;r[0]=76u;goto L8c658b56; /* mov #0x4c,r0 */
L8c658b56: *stop=0x8c658b56u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b58; /* mov.l r4,@(r0,r14) */
L8c658b58: *stop=0x8c658b58u;r[0]=80u;goto L8c658b5a; /* mov #0x50,r0 */
L8c658b5a: *stop=0x8c658b5au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b5c; /* mov.l r4,@(r0,r14) */
L8c658b5c: *stop=0x8c658b5cu;r[0]=84u;goto L8c658b5e; /* mov #0x54,r0 */
L8c658b5e: *stop=0x8c658b5eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b60; /* mov.l r4,@(r0,r14) */
L8c658b60: *stop=0x8c658b60u;r[0]=88u;goto L8c658b62; /* mov #0x58,r0 */
L8c658b62: *stop=0x8c658b62u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b64; /* mov.l r4,@(r0,r14) */
L8c658b64: *stop=0x8c658b64u;r[0]=92u;goto L8c658b66; /* mov #0x5c,r0 */
L8c658b66: *stop=0x8c658b66u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b68; /* mov.l r4,@(r0,r14) */
L8c658b68: *stop=0x8c658b68u;r[0]=104u;goto L8c658b6a; /* mov #0x68,r0 */
L8c658b6a: *stop=0x8c658b6au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b6c; /* mov.l r4,@(r0,r14) */
L8c658b6c: *stop=0x8c658b6cu;r[0]=96u;goto L8c658b6e; /* mov #0x60,r0 */
L8c658b6e: *stop=0x8c658b6eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b70; /* mov.l r4,@(r0,r14) */
L8c658b70: *stop=0x8c658b70u;r[0]=100u;goto L8c658b72; /* mov #0x64,r0 */
L8c658b72: *stop=0x8c658b72u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658becu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c658b74; /* mov.w 0x8c658bec,r4 */
L8c658b74: *stop=0x8c658b74u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658beeu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c658b76; /* mov.w 0x8c658bee,r3 */
L8c658b76: *stop=0x8c658b76u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c658b78; /* mov.l r4,@(r0,r14) */
L8c658b78: *stop=0x8c658b78u;r[0]=r[3];goto L8c658b7a; /* mov r3,r0 */
L8c658b7a: *stop=0x8c658b7au;r[0]+=104u;goto L8c658b7c; /* add #0x68,r0 */
L8c658b7c: *stop=0x8c658b7cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658b7e; /* mov.l r3,@(r0,r14) */
L8c658b7e: *stop=0x8c658b7eu;r[0]+=4294967288u;goto L8c658b80; /* add #-0x8,r0 */
L8c658b80: *stop=0x8c658b80u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c658b82; /* mov.l r6,@(r0,r14) */
L8c658b82: *stop=0x8c658b82u;r[0]+=4u;goto L8c658b84; /* add #0x4,r0 */
L8c658b84: *stop=0x8c658b84u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c658b86; /* mov.l r6,@(r0,r14) */
L8c658b86: *stop=0x8c658b86u;r[0]+=4u;goto L8c658b88; /* add #0x4,r0 */
L8c658b88: *stop=0x8c658b88u;r[3]+=24u;goto L8c658b8a; /* add #0x18,r3 */
L8c658b8a: *stop=0x8c658b8au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c658b8c; /* mov.l r3,@(r0,r14) */
L8c658b8c: *stop=0x8c658b8cu;r[0]+=4294967288u;goto L8c658b8e; /* add #-0x8,r0 */
L8c658b8e: *stop=0x8c658b8eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c658b90; /* mov.l r5,@(r0,r14) */
L8c658b90: *stop=0x8c658b90u;r[0]+=4u;goto L8c658b92; /* add #0x4,r0 */
L8c658b92: *stop=0x8c658b92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658bf8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658b94; /* mov.l 0x8c658bf8,r3 */
L8c658b94: *stop=0x8c658b94u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c658b96; /* mov.l r5,@(r0,r14) */
L8c658b96: *stop=0x8c658b96u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c658b98; /* mov.l r4,@r3 */
L8c658b98: *stop=0x8c658b98u;r[1]=r[3];goto L8c658b9a; /* mov r3,r1 */
L8c658b9a: *stop=0x8c658b9au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658b9c; /* mov.l @r1,r2 */
L8c658b9c: *stop=0x8c658b9cu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658b9e; /* mov.l @(0xc,r14),r1 */
L8c658b9e: *stop=0x8c658b9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658bfcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c658ba0; /* mov.l 0x8c658bfc,r0 */
L8c658ba0: *stop=0x8c658ba0u;r[1]+=r[2];goto L8c658ba2; /* add r2,r1 */
L8c658ba2: *stop=0x8c658ba2u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c658ba4; /* mov.l r1,@r0 */
L8c658ba4: *stop=0x8c658ba4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658c00u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658ba6; /* mov.l 0x8c658c00,r1 */
L8c658ba6: *stop=0x8c658ba6u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658ba8; /* mov.l @(0xc,r14),r2 */
L8c658ba8: *stop=0x8c658ba8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c658baa; /* mov.l r2,@r1 */
L8c658baa: *stop=0x8c658baau;{uint32_t loaded=sx(m,bus,&io_status,0x8c658bf0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c658bac; /* mov.w 0x8c658bf0,r0 */
L8c658bac: *stop=0x8c658bacu;r[2]=r[14];goto L8c658bae; /* mov r14,r2 */
L8c658bae: *stop=0x8c658baeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658c04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658bb0; /* mov.l 0x8c658c04,r3 */
L8c658bb0: *stop=0x8c658bb0u;r[15]-=r[0];goto L8c658bb2; /* sub r0,r15 */
L8c658bb2: target=r[3];*pr=0x8c658bb6u;*stop=0x8c658bb4u;r[1]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658bb6; /* jsr @r3 */
L8c658bb6: target=0x8c6582e0u;*pr=0x8c658bbau;*stop=0x8c658bb8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658bba; /* bsr 0x8c6582e0 */
L8c658bba: *stop=0x8c658bbau;{uint32_t loaded=sx(m,bus,&io_status,0x8c658bf0u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c658bbc; /* mov.w 0x8c658bf0,r2 */
L8c658bbc: *stop=0x8c658bbcu;r[15]+=r[2];goto L8c658bbe; /* add r2,r15 */
L8c658bbe: *stop=0x8c658bbeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c658bf0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c658bc0; /* mov.w 0x8c658bf0,r1 */
L8c658bc0: *stop=0x8c658bc0u;r[15]+=r[1];goto L8c658bc2; /* add r1,r15 */
L8c658bc2: *stop=0x8c658bc2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c658bc4; /* lds.l @r15+,PR */
L8c658bc4: *stop=0x8c658bc4u;if(*pr!=entry_pr)return -3;*stop=0x8c658bc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c665a80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c665a80;
L8c665a80: *stop=0x8c665a80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c665a82; /* sts.l PR,@-r15 */
L8c665a82: *stop=0x8c665a82u;r[15]+=4294967288u;goto L8c665a84; /* add #-0x8,r15 */
L8c665a84: *stop=0x8c665a84u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c665a86; /* mov.l r5,@r15 */
L8c665a86: *stop=0x8c665a86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ac4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665a88; /* mov.l 0x8c665ac4,r3 */
L8c665a88: *stop=0x8c665a88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ac0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665a8a; /* mov.l 0x8c665ac0,r5 */
L8c665a8a: target=r[3];*pr=0x8c665a8eu;*stop=0x8c665a8cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665a8e; /* jsr @r3 */
L8c665a8e: *stop=0x8c665a8eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c665a90; /* tst r0,r0 */
L8c665a90: if(!(*sr&1u))goto L8c665aa2;goto L8c665a92; /* bf 0x8c665aa2 */
L8c665a92: *stop=0x8c665a92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ac4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665a94; /* mov.l 0x8c665ac4,r3 */
L8c665a94: *stop=0x8c665a94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ac8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665a96; /* mov.l 0x8c665ac8,r5 */
L8c665a96: target=r[3];*pr=0x8c665a9au;*stop=0x8c665a98u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665a9a; /* jsr @r3 */
L8c665a9a: *stop=0x8c665a9au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c665a9c; /* tst r0,r0 */
L8c665a9c: if(!(*sr&1u))goto L8c665aa4;goto L8c665a9e; /* bf 0x8c665aa4 */
L8c665a9e: *stop=0x8c665aa0u;r[4]=0u;goto L8c665aa4; /* bra 0x8c665aa4 */
L8c665aa2: *stop=0x8c665aa2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c665aa4; /* mov.l @(0x4,r15),r4 */
L8c665aa4: *stop=0x8c665aa4u;r[15]+=8u;goto L8c665aa6; /* add #0x8,r15 */
L8c665aa6: *stop=0x8c665aa6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c665aa8; /* lds.l @r15+,PR */
L8c665aa8: *stop=0x8c665aa8u;if(*pr!=entry_pr)return -3;*stop=0x8c665aaau;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c10d3c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d3c8;
L8c10d3c8: *stop=0x8c10d3c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d3ca; /* sts.l PR,@-r15 */
L8c10d3ca: *stop=0x8c10d3cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d3d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d3cc; /* mov.l 0x8c10d3d8,r3 */
L8c10d3cc: target=r[3];*pr=0x8c10d3d0u;*stop=0x8c10d3ceu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d3d0; /* jsr @r3 */
L8c10d3d0: *stop=0x8c10d3d0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d3d2; /* lds.l @r15+,PR */
L8c10d3d2: *stop=0x8c10d3d2u;if(*pr!=entry_pr)return -3;*stop=0x8c10d3d4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c10db1c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10db1c;
L8c10db1c: *stop=0x8c10db1cu;r[0]=r[4];goto L8c10db1e; /* mov r4,r0 */
L8c10db1e: *stop=0x8c10db1eu;r[0]|=r[5];goto L8c10db20; /* or r5,r0 */
L8c10db20: *stop=0x8c10db20u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c10db22; /* tst #0x3,r0 */
L8c10db22: if(!(*sr&1u))goto L8c10db90;goto L8c10db24; /* bf 0x8c10db90 */
L8c10db24: *stop=0x8c10db24u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}r[5]+=4;goto L8c10db26; /* mov.l @r5+,r3 */
L8c10db26: *stop=0x8c10db26u;r[2]=0u;goto L8c10db28; /* mov #0x0,r2 */
L8c10db28: *stop=0x8c10db28u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}r[4]+=4;goto L8c10db2a; /* mov.l @r4+,r0 */
L8c10db2a: *stop=0x8c10db2au;{uint32_t x=r[2]^r[3];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10db2c; /* cmp/str r2,r3 */
L8c10db2c: if((*sr&1u))goto L8c10db5a;goto L8c10db2e; /* bt 0x8c10db5a */
L8c10db2e: *stop=0x8c10db2eu;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c10db30; /* cmp/eq r0,r3 */
L8c10db30: *stop=0x8c10db30u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}r[5]+=4;goto L8c10db32; /* mov.l @r5+,r1 */
L8c10db32: if(!(*sr&1u))goto L8c10db58;goto L8c10db34; /* bf 0x8c10db58 */
L8c10db34: *stop=0x8c10db34u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}r[4]+=4;goto L8c10db36; /* mov.l @r4+,r0 */
L8c10db36: *stop=0x8c10db36u;{uint32_t x=r[2]^r[1];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10db38; /* cmp/str r2,r1 */
L8c10db38: if((*sr&1u))goto L8c10db5a;goto L8c10db3a; /* bt 0x8c10db5a */
L8c10db3a: *stop=0x8c10db3au;*sr=(*sr&~1u)|(r[0]==r[1]);goto L8c10db3c; /* cmp/eq r0,r1 */
L8c10db3c: *stop=0x8c10db3cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}r[5]+=4;goto L8c10db3e; /* mov.l @r5+,r3 */
L8c10db3e: if(!(*sr&1u))goto L8c10db58;goto L8c10db40; /* bf 0x8c10db58 */
L8c10db40: *stop=0x8c10db40u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}r[4]+=4;goto L8c10db42; /* mov.l @r4+,r0 */
L8c10db42: *stop=0x8c10db42u;{uint32_t x=r[2]^r[3];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10db44; /* cmp/str r2,r3 */
L8c10db44: if((*sr&1u))goto L8c10db5a;goto L8c10db46; /* bt 0x8c10db5a */
L8c10db46: *stop=0x8c10db46u;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c10db48; /* cmp/eq r0,r3 */
L8c10db48: *stop=0x8c10db48u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}r[5]+=4;goto L8c10db4a; /* mov.l @r5+,r1 */
L8c10db4a: if(!(*sr&1u))goto L8c10db58;goto L8c10db4c; /* bf 0x8c10db58 */
L8c10db4c: *stop=0x8c10db4cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}r[4]+=4;goto L8c10db4e; /* mov.l @r4+,r0 */
L8c10db4e: *stop=0x8c10db4eu;{uint32_t x=r[2]^r[1];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10db50; /* cmp/str r2,r1 */
L8c10db50: if((*sr&1u))goto L8c10db5a;goto L8c10db52; /* bt 0x8c10db5a */
L8c10db52: *stop=0x8c10db52u;*sr=(*sr&~1u)|(r[0]==r[1]);goto L8c10db54; /* cmp/eq r0,r1 */
L8c10db54: *stop=0x8c10db54u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}r[5]+=4;goto L8c10db56; /* mov.l @r5+,r3 */
L8c10db56: if((*sr&1u))goto L8c10db28;goto L8c10db58; /* bt 0x8c10db28 */
L8c10db58: *stop=0x8c10db58u;r[5]+=4294967292u;goto L8c10db5a; /* add #-0x4,r5 */
L8c10db5a: *stop=0x8c10db5au;r[5]+=4294967292u;goto L8c10db5c; /* add #-0x4,r5 */
L8c10db5c: *stop=0x8c10db5cu;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[1]=loaded;}r[5]+=1;goto L8c10db5e; /* mov.b @r5+,r1 */
L8c10db5e: *stop=0x8c10db5eu;r[4]+=4294967292u;goto L8c10db60; /* add #-0x4,r4 */
L8c10db60: *stop=0x8c10db60u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10db62; /* mov.b @r4+,r0 */
L8c10db62: *stop=0x8c10db62u;*sr=(*sr&~1u)|(r[2]==r[1]);goto L8c10db64; /* cmp/eq r2,r1 */
L8c10db64: if((*sr&1u))goto L8c10db86;goto L8c10db66; /* bt 0x8c10db86 */
L8c10db66: *stop=0x8c10db66u;*sr=(*sr&~1u)|(r[0]==r[1]);goto L8c10db68; /* cmp/eq r0,r1 */
L8c10db68: if(!(*sr&1u))goto L8c10db86;goto L8c10db6a; /* bf 0x8c10db86 */
L8c10db6a: *stop=0x8c10db6au;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[1]=loaded;}r[5]+=1;goto L8c10db6c; /* mov.b @r5+,r1 */
L8c10db6c: *stop=0x8c10db6cu;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10db6e; /* mov.b @r4+,r0 */
L8c10db6e: *stop=0x8c10db6eu;*sr=(*sr&~1u)|(r[2]==r[1]);goto L8c10db70; /* cmp/eq r2,r1 */
L8c10db70: if((*sr&1u))goto L8c10db86;goto L8c10db72; /* bt 0x8c10db86 */
L8c10db72: *stop=0x8c10db72u;*sr=(*sr&~1u)|(r[0]==r[1]);goto L8c10db74; /* cmp/eq r0,r1 */
L8c10db74: if(!(*sr&1u))goto L8c10db86;goto L8c10db76; /* bf 0x8c10db86 */
L8c10db76: *stop=0x8c10db76u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[1]=loaded;}r[5]+=1;goto L8c10db78; /* mov.b @r5+,r1 */
L8c10db78: *stop=0x8c10db78u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10db7a; /* mov.b @r4+,r0 */
L8c10db7a: *stop=0x8c10db7au;*sr=(*sr&~1u)|(r[2]==r[1]);goto L8c10db7c; /* cmp/eq r2,r1 */
L8c10db7c: if((*sr&1u))goto L8c10db86;goto L8c10db7e; /* bt 0x8c10db86 */
L8c10db7e: *stop=0x8c10db7eu;*sr=(*sr&~1u)|(r[0]==r[1]);goto L8c10db80; /* cmp/eq r0,r1 */
L8c10db80: if(!(*sr&1u))goto L8c10db86;goto L8c10db82; /* bf 0x8c10db86 */
L8c10db82: *stop=0x8c10db82u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10db84; /* mov.b @r4+,r0 */
L8c10db84: *stop=0x8c10db84u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[1]=loaded;}r[5]+=1;goto L8c10db86; /* mov.b @r5+,r1 */
L8c10db86: *stop=0x8c10db86u;if(*pr!=entry_pr)return -3;*stop=0x8c10db88u;r[0]-=r[1];return 0; /* rts */
L8c10db90: *stop=0x8c10db90u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10db92; /* mov.b @r4+,r0 */
L8c10db92: *stop=0x8c10db92u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10db94; /* cmp/eq #0x0,r0 */
L8c10db94: *stop=0x8c10db94u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c10db96; /* mov.b @r5+,r3 */
L8c10db96: if((*sr&1u))goto L8c10dbc0;goto L8c10db98; /* bt 0x8c10dbc0 */
L8c10db98: *stop=0x8c10db98u;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c10db9a; /* cmp/eq r0,r3 */
L8c10db9a: if(!(*sr&1u))goto L8c10dbc0;goto L8c10db9c; /* bf 0x8c10dbc0 */
L8c10db9c: *stop=0x8c10db9cu;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10db9e; /* mov.b @r4+,r0 */
L8c10db9e: *stop=0x8c10db9eu;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c10dba0; /* mov.b @r5+,r3 */
L8c10dba0: *stop=0x8c10dba0u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10dba2; /* cmp/eq #0x0,r0 */
L8c10dba2: if((*sr&1u))goto L8c10dbc0;goto L8c10dba4; /* bt 0x8c10dbc0 */
L8c10dba4: *stop=0x8c10dba4u;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c10dba6; /* cmp/eq r0,r3 */
L8c10dba6: if(!(*sr&1u))goto L8c10dbc0;goto L8c10dba8; /* bf 0x8c10dbc0 */
L8c10dba8: *stop=0x8c10dba8u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10dbaa; /* mov.b @r4+,r0 */
L8c10dbaa: *stop=0x8c10dbaau;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c10dbac; /* mov.b @r5+,r3 */
L8c10dbac: *stop=0x8c10dbacu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10dbae; /* cmp/eq #0x0,r0 */
L8c10dbae: if((*sr&1u))goto L8c10dbc0;goto L8c10dbb0; /* bt 0x8c10dbc0 */
L8c10dbb0: *stop=0x8c10dbb0u;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c10dbb2; /* cmp/eq r0,r3 */
L8c10dbb2: if(!(*sr&1u))goto L8c10dbc0;goto L8c10dbb4; /* bf 0x8c10dbc0 */
L8c10dbb4: *stop=0x8c10dbb4u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}r[4]+=1;goto L8c10dbb6; /* mov.b @r4+,r0 */
L8c10dbb6: *stop=0x8c10dbb6u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c10dbb8; /* mov.b @r5+,r3 */
L8c10dbb8: *stop=0x8c10dbb8u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10dbba; /* cmp/eq #0x0,r0 */
L8c10dbba: if((*sr&1u))goto L8c10dbc0;goto L8c10dbbc; /* bt 0x8c10dbc0 */
L8c10dbbc: *stop=0x8c10dbbcu;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c10dbbe; /* cmp/eq r0,r3 */
L8c10dbbe: if((*sr&1u))goto L8c10db90;goto L8c10dbc0; /* bt 0x8c10db90 */
L8c10dbc0: *stop=0x8c10dbc0u;if(*pr!=entry_pr)return -3;*stop=0x8c10dbc2u;r[0]-=r[3];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c657c60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c657c60;
L8c657c60: *stop=0x8c657c60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c657c62; /* mov.l r14,@-r15 */
L8c657c62: *stop=0x8c657c62u;r[0]=88u;goto L8c657c64; /* mov #0x58,r0 */
L8c657c64: *stop=0x8c657c64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c657c66; /* mov.l r13,@-r15 */
L8c657c66: *stop=0x8c657c66u;r[4]=r[15];goto L8c657c68; /* mov r15,r4 */
L8c657c68: *stop=0x8c657c68u;r[4]+=8u;goto L8c657c6a; /* add #0x8,r4 */
L8c657c6a: *stop=0x8c657c6au;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657c6c; /* mov.l @(r0,r4),r3 */
L8c657c6c: *stop=0x8c657c6cu;r[0]=84u;goto L8c657c6e; /* mov #0x54,r0 */
L8c657c6e: *stop=0x8c657c6eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657c70; /* mov.l @(r0,r4),r2 */
L8c657c70: *stop=0x8c657c70u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657d00u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c657c72; /* mov.w 0x8c657d00,r5 */
L8c657c72: *stop=0x8c657c72u;r[2]&=r[5];goto L8c657c74; /* and r5,r2 */
L8c657c74: *stop=0x8c657c74u;r[2]<<=16;goto L8c657c76; /* shll16 r2 */
L8c657c76: *stop=0x8c657c76u;r[3]&=r[5];goto L8c657c78; /* and r5,r3 */
L8c657c78: *stop=0x8c657c78u;r[3]|=r[2];goto L8c657c7a; /* or r2,r3 */
L8c657c7a: *stop=0x8c657c7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657c7c; /* mov.l 0x8c657d04,r2 */
L8c657c7c: *stop=0x8c657c7cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c657c7e; /* mov.l r3,@r2 */
L8c657c7e: *stop=0x8c657c7eu;r[0]=96u;goto L8c657c80; /* mov #0x60,r0 */
L8c657c80: *stop=0x8c657c80u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657c82; /* mov.l @(r0,r4),r1 */
L8c657c82: *stop=0x8c657c82u;r[0]=92u;goto L8c657c84; /* mov #0x5c,r0 */
L8c657c84: *stop=0x8c657c84u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657c86; /* mov.l @(r0,r4),r3 */
L8c657c86: *stop=0x8c657c86u;r[1]&=r[5];goto L8c657c88; /* and r5,r1 */
L8c657c88: *stop=0x8c657c88u;r[3]&=r[5];goto L8c657c8a; /* and r5,r3 */
L8c657c8a: *stop=0x8c657c8au;r[3]<<=16;goto L8c657c8c; /* shll16 r3 */
L8c657c8c: *stop=0x8c657c8cu;r[1]|=r[3];goto L8c657c8e; /* or r3,r1 */
L8c657c8e: *stop=0x8c657c8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657c90; /* mov.l 0x8c657d08,r3 */
L8c657c90: *stop=0x8c657c90u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c657c92; /* mov.l r1,@r3 */
L8c657c92: *stop=0x8c657c92u;r[0]=72u;goto L8c657c94; /* mov #0x48,r0 */
L8c657c94: *stop=0x8c657c94u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657c96; /* mov.l @(r0,r4),r2 */
L8c657c96: *stop=0x8c657c96u;r[0]=76u;goto L8c657c98; /* mov #0x4c,r0 */
L8c657c98: *stop=0x8c657c98u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657c9a; /* mov.l @(r0,r4),r1 */
L8c657c9a: *stop=0x8c657c9au;r[6]=31u;goto L8c657c9c; /* mov #0x1f,r6 */
L8c657c9c: *stop=0x8c657c9cu;r[5]=15u;goto L8c657c9e; /* mov #0xf,r5 */
L8c657c9e: *stop=0x8c657c9eu;r[1]&=r[6];goto L8c657ca0; /* and r6,r1 */
L8c657ca0: *stop=0x8c657ca0u;r[1]<<=2;goto L8c657ca2; /* shll2 r1 */
L8c657ca2: *stop=0x8c657ca2u;r[1]<<=2;goto L8c657ca4; /* shll2 r1 */
L8c657ca4: *stop=0x8c657ca4u;r[2]&=r[5];goto L8c657ca6; /* and r5,r2 */
L8c657ca6: *stop=0x8c657ca6u;r[0]=80u;goto L8c657ca8; /* mov #0x50,r0 */
L8c657ca8: *stop=0x8c657ca8u;r[2]|=r[1];goto L8c657caa; /* or r1,r2 */
L8c657caa: *stop=0x8c657caau;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657cac; /* mov.l @(r0,r4),r1 */
L8c657cac: *stop=0x8c657cacu;r[1]&=r[5];goto L8c657cae; /* and r5,r1 */
L8c657cae: *stop=0x8c657caeu;r[1]<<=8;goto L8c657cb0; /* shll8 r1 */
L8c657cb0: *stop=0x8c657cb0u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c657cb2; /* shll r1 */
L8c657cb2: *stop=0x8c657cb2u;r[2]|=r[1];goto L8c657cb4; /* or r1,r2 */
L8c657cb4: *stop=0x8c657cb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d0cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657cb6; /* mov.l 0x8c657d0c,r1 */
L8c657cb6: *stop=0x8c657cb6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c657cb8; /* mov.l r2,@r1 */
L8c657cb8: *stop=0x8c657cb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657cba; /* mov.l 0x8c657d10,r2 */
L8c657cba: *stop=0x8c657cbau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657cbc; /* mov.l @r4,r3 */
L8c657cbc: *stop=0x8c657cbcu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c657cbe; /* mov.l r3,@r2 */
L8c657cbe: *stop=0x8c657cbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657cc0; /* mov.l 0x8c657d14,r3 */
L8c657cc0: *stop=0x8c657cc0u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657cc2; /* mov.l @(0x4,r4),r1 */
L8c657cc2: *stop=0x8c657cc2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c657cc4; /* mov.l r1,@r3 */
L8c657cc4: *stop=0x8c657cc4u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c657cc6; /* mov.l @(0x8,r4),r14 */
L8c657cc6: *stop=0x8c657cc6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657d02u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c657cc8; /* mov.w 0x8c657d02,r5 */
L8c657cc8: *stop=0x8c657cc8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[7]=loaded;}goto L8c657cca; /* mov.l @r4,r7 */
L8c657cca: *stop=0x8c657ccau;r[14]&=r[5];goto L8c657ccc; /* and r5,r14 */
L8c657ccc: *stop=0x8c657cccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657cce; /* mov.l 0x8c657d18,r0 */
L8c657cce: *stop=0x8c657cceu;r[14]<<=16;goto L8c657cd0; /* shll16 r14 */
L8c657cd0: *stop=0x8c657cd0u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c657cd2; /* mov.l @(0x4,r4),r13 */
L8c657cd2: *stop=0x8c657cd2u;r[14]<<=2;goto L8c657cd4; /* shll2 r14 */
L8c657cd4: *stop=0x8c657cd4u;r[7]&=r[5];goto L8c657cd6; /* and r5,r7 */
L8c657cd6: *stop=0x8c657cd6u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657cd8; /* mov.l @r0,r1 */
L8c657cd8: *stop=0x8c657cd8u;*sr=(*sr&~1u)|((int32_t)r[1]>0);goto L8c657cda; /* cmp/pl r1 */
L8c657cda: take=!(*sr&1u);*stop=0x8c657cdcu;r[14]<<=2;if(take)goto L8c657d20;goto L8c657cde; /* bf/s 0x8c657d20 */
L8c657cde: *stop=0x8c657cdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657ce0; /* mov.l 0x8c657d18,r1 */
L8c657ce0: *stop=0x8c657ce0u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657ce2; /* mov.l @r1,r2 */
L8c657ce2: *stop=0x8c657ce2u;r[13]-=r[2];goto L8c657ce4; /* sub r2,r13 */
L8c657ce4: *stop=0x8c657ce4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657d1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657ce6; /* mov.l 0x8c657d1c,r3 */
L8c657ce6: *stop=0x8c657ce6u;r[13]&=r[5];goto L8c657ce8; /* and r5,r13 */
L8c657ce8: *stop=0x8c657ce8u;r[13]<<=8;goto L8c657cea; /* shll8 r13 */
L8c657cea: *stop=0x8c657ceau;r[13]<<=2;goto L8c657cec; /* shll2 r13 */
L8c657cec: *stop=0x8c657cecu;r[7]|=r[13];goto L8c657cee; /* or r13,r7 */
L8c657cee: *stop=0x8c657ceeu;r[7]|=r[14];goto L8c657cf0; /* or r14,r7 */
L8c657cf0: *stop=0x8c657cf2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[7],4);if(io_status)return io_status;goto L8c657d2e; /* bra 0x8c657d2e */
L8c657d20: *stop=0x8c657d20u;r[13]&=r[5];goto L8c657d22; /* and r5,r13 */
L8c657d22: *stop=0x8c657d22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657de4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657d24; /* mov.l 0x8c657de4,r1 */
L8c657d24: *stop=0x8c657d24u;r[13]<<=8;goto L8c657d26; /* shll8 r13 */
L8c657d26: *stop=0x8c657d26u;r[13]<<=2;goto L8c657d28; /* shll2 r13 */
L8c657d28: *stop=0x8c657d28u;r[7]|=r[13];goto L8c657d2a; /* or r13,r7 */
L8c657d2a: *stop=0x8c657d2au;r[7]|=r[14];goto L8c657d2c; /* or r14,r7 */
L8c657d2c: *stop=0x8c657d2cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[7],4);if(io_status)return io_status;goto L8c657d2e; /* mov.l r7,@r1 */
L8c657d2e: *stop=0x8c657d2eu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657d30; /* mov.l @(0x18,r4),r2 */
L8c657d30: *stop=0x8c657d30u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657d32; /* mov.l @(0x14,r4),r3 */
L8c657d32: *stop=0x8c657d32u;r[2]=r[2]&255u;goto L8c657d34; /* extu.b r2,r2 */
L8c657d34: *stop=0x8c657d34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657de8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657d36; /* mov.l 0x8c657de8,r1 */
L8c657d36: *stop=0x8c657d36u;r[2]<<=8;goto L8c657d38; /* shll8 r2 */
L8c657d38: *stop=0x8c657d38u;r[3]=r[3]&255u;goto L8c657d3a; /* extu.b r3,r3 */
L8c657d3a: *stop=0x8c657d3au;r[3]|=r[2];goto L8c657d3c; /* or r2,r3 */
L8c657d3c: *stop=0x8c657d3cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c657d3e; /* mov.l r3,@r1 */
L8c657d3e: *stop=0x8c657d3eu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657d40; /* mov.l @(0xc,r4),r2 */
L8c657d40: *stop=0x8c657d40u;r[5]=1u;goto L8c657d42; /* mov #0x1,r5 */
L8c657d42: *stop=0x8c657d42u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d44; /* mov.l @(0x1c,r4),r0 */
L8c657d44: *stop=0x8c657d44u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657d46; /* mov.l @(0x10,r4),r3 */
L8c657d46: *stop=0x8c657d46u;r[2]&=r[5];goto L8c657d48; /* and r5,r2 */
L8c657d48: *stop=0x8c657d48u;r[0]&=r[5];goto L8c657d4a; /* and r5,r0 */
L8c657d4a: *stop=0x8c657d4au;r[3]=r[3]&65535u;goto L8c657d4c; /* extu.w r3,r3 */
L8c657d4c: *stop=0x8c657d4cu;r[2]<<=16;goto L8c657d4e; /* shll16 r2 */
L8c657d4e: *stop=0x8c657d4eu;r[0]<<=16;goto L8c657d50; /* shll16 r0 */
L8c657d50: *stop=0x8c657d50u;r[3]|=r[2];goto L8c657d52; /* or r2,r3 */
L8c657d52: *stop=0x8c657d52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657decu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657d54; /* mov.l 0x8c657dec,r2 */
L8c657d54: *stop=0x8c657d54u;r[0]<<=2;goto L8c657d56; /* shll2 r0 */
L8c657d56: *stop=0x8c657d56u;r[3]|=r[0];goto L8c657d58; /* or r0,r3 */
L8c657d58: *stop=0x8c657d58u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c657d5a; /* mov.l r3,@r2 */
L8c657d5a: *stop=0x8c657d5au;r[0]=68u;goto L8c657d5c; /* mov #0x44,r0 */
L8c657d5c: *stop=0x8c657d5cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c657de0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c657d5e; /* mov.w 0x8c657de0,r3 */
L8c657d5e: *stop=0x8c657d5eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d60; /* mov.l @(r0,r4),r0 */
L8c657d60: *stop=0x8c657d60u;r[2]+=4294967236u;goto L8c657d62; /* add #-0x3c,r2 */
L8c657d62: *stop=0x8c657d62u;r[0]&=r[3];goto L8c657d64; /* and r3,r0 */
L8c657d64: *stop=0x8c657d64u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c657d66; /* mov.l r0,@r2 */
L8c657d66: *stop=0x8c657d66u;if(!valid_access(r[4]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x2cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d68; /* mov.l @(0x2c,r4),r0 */
L8c657d68: *stop=0x8c657d68u;r[7]=7u;goto L8c657d6a; /* mov #0x7,r7 */
L8c657d6a: *stop=0x8c657d6au;if(!valid_access(r[4]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x30u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657d6c; /* mov.l @(0x30,r4),r1 */
L8c657d6c: *stop=0x8c657d6cu;r[0]&=r[5];goto L8c657d6e; /* and r5,r0 */
L8c657d6e: *stop=0x8c657d6eu;r[0]<<=2;goto L8c657d70; /* shll2 r0 */
L8c657d70: *stop=0x8c657d70u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c657d72; /* shll r0 */
L8c657d72: *stop=0x8c657d72u;r[1]&=r[7];goto L8c657d74; /* and r7,r1 */
L8c657d74: *stop=0x8c657d74u;r[1]|=r[0];goto L8c657d76; /* or r0,r1 */
L8c657d76: *stop=0x8c657d76u;if(!valid_access(r[4]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d78; /* mov.l @(0x34,r4),r0 */
L8c657d78: *stop=0x8c657d78u;r[0]=r[0]&255u;goto L8c657d7a; /* extu.b r0,r0 */
L8c657d7a: *stop=0x8c657d7au;r[0]<<=8;goto L8c657d7c; /* shll8 r0 */
L8c657d7c: *stop=0x8c657d7cu;r[1]|=r[0];goto L8c657d7e; /* or r0,r1 */
L8c657d7e: *stop=0x8c657d7eu;if(!valid_access(r[4]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x38u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d80; /* mov.l @(0x38,r4),r0 */
L8c657d80: *stop=0x8c657d80u;r[0]=r[0]&255u;goto L8c657d82; /* extu.b r0,r0 */
L8c657d82: *stop=0x8c657d82u;r[0]<<=16;goto L8c657d84; /* shll16 r0 */
L8c657d84: *stop=0x8c657d84u;r[1]|=r[0];goto L8c657d86; /* or r0,r1 */
L8c657d86: *stop=0x8c657d86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657df0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d88; /* mov.l 0x8c657df0,r0 */
L8c657d88: *stop=0x8c657d88u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c657d8a; /* mov.l r1,@r0 */
L8c657d8a: *stop=0x8c657d8au;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657d8c; /* mov.l @(0x28,r4),r1 */
L8c657d8c: *stop=0x8c657d8cu;r[2]=22u;goto L8c657d8e; /* mov #0x16,r2 */
L8c657d8e: *stop=0x8c657d8eu;if(!valid_access(r[4]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x3cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657d90; /* mov.l @(0x3c,r4),r0 */
L8c657d90: *stop=0x8c657d90u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657d92; /* mov.l @(0x20,r4),r3 */
L8c657d92: *stop=0x8c657d92u;r[1]&=r[5];goto L8c657d94; /* and r5,r1 */
L8c657d94: *stop=0x8c657d94u;r[0]&=0x3u;goto L8c657d96; /* and #0x3,r0 */
L8c657d96: *stop=0x8c657d96u;r[3]&=r[5];goto L8c657d98; /* and r5,r3 */
L8c657d98: *stop=0x8c657d98u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c657d9a; /* shll r1 */
L8c657d9a: *stop=0x8c657d9au;r[0]<<=2;goto L8c657d9c; /* shll2 r0 */
L8c657d9c: *stop=0x8c657d9cu;r[3]|=r[1];goto L8c657d9e; /* or r1,r3 */
L8c657d9e: *stop=0x8c657d9eu;r[3]|=r[0];goto L8c657da0; /* or r0,r3 */
L8c657da0: *stop=0x8c657da0u;r[0]=64u;goto L8c657da2; /* mov #0x40,r0 */
L8c657da2: *stop=0x8c657da2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657da4; /* mov.l @(r0,r4),r0 */
L8c657da4: *stop=0x8c657da4u;r[7]&=r[0];goto L8c657da6; /* and r0,r7 */
L8c657da6: *stop=0x8c657da6u;r[0]=112u;goto L8c657da8; /* mov #0x70,r0 */
L8c657da8: *stop=0x8c657da8u;if(!valid_access(r[0]+r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+r[0],1);if(io_status)return io_status;r[1]=loaded;}goto L8c657daa; /* mov.b @(r0,r4),r1 */
L8c657daa: *stop=0x8c657daau;r[7]<<=2;goto L8c657dac; /* shll2 r7 */
L8c657dac: *stop=0x8c657dacu;r[7]<<=2;goto L8c657dae; /* shll2 r7 */
L8c657dae: *stop=0x8c657daeu;r[1]=r[1]&255u;goto L8c657db0; /* extu.b r1,r1 */
L8c657db0: *stop=0x8c657db0u;r[1]<<=8;goto L8c657db2; /* shll8 r1 */
L8c657db2: *stop=0x8c657db2u;r[3]|=r[7];goto L8c657db4; /* or r7,r3 */
L8c657db4: *stop=0x8c657db4u;r[0]=104u;goto L8c657db6; /* mov #0x68,r0 */
L8c657db6: *stop=0x8c657db6u;r[3]|=r[1];goto L8c657db8; /* or r1,r3 */
L8c657db8: *stop=0x8c657db8u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657dba; /* mov.l @(r0,r4),r1 */
L8c657dba: *stop=0x8c657dbau;r[0]=100u;goto L8c657dbc; /* mov #0x64,r0 */
L8c657dbc: *stop=0x8c657dbcu;r[6]&=r[1];goto L8c657dbe; /* and r1,r6 */
L8c657dbe: *stop=0x8c657dbeu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657dc0; /* mov.l @(r0,r4),r1 */
L8c657dc0: *stop=0x8c657dc0u;r[6]<<=16;goto L8c657dc2; /* shll16 r6 */
L8c657dc2: *stop=0x8c657dc2u;r[1]&=r[5];goto L8c657dc4; /* and r5,r1 */
L8c657dc4: *stop=0x8c657dc4u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c657dc6; /* shad r2,r1 */
L8c657dc6: *stop=0x8c657dc6u;r[3]|=r[6];goto L8c657dc8; /* or r6,r3 */
L8c657dc8: *stop=0x8c657dc8u;r[3]|=r[1];goto L8c657dca; /* or r1,r3 */
L8c657dca: *stop=0x8c657dcau;r[0]=108u;goto L8c657dcc; /* mov #0x6c,r0 */
L8c657dcc: *stop=0x8c657dccu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657dce; /* mov.l @(r0,r4),r1 */
L8c657dce: *stop=0x8c657dceu;r[2]=23u;goto L8c657dd0; /* mov #0x17,r2 */
L8c657dd0: *stop=0x8c657dd0u;r[1]&=r[5];goto L8c657dd2; /* and r5,r1 */
L8c657dd2: *stop=0x8c657dd2u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c657dd4; /* shad r2,r1 */
L8c657dd4: *stop=0x8c657dd4u;r[3]|=r[1];goto L8c657dd6; /* or r1,r3 */
L8c657dd6: *stop=0x8c657dd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657df4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657dd8; /* mov.l 0x8c657df4,r1 */
L8c657dd8: *stop=0x8c657dd8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c657dda; /* mov.l r3,@r1 */
L8c657dda: *stop=0x8c657ddau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c657ddc; /* mov.l @r15+,r13 */
L8c657ddc: *stop=0x8c657ddcu;if(*pr!=entry_pr)return -3;*stop=0x8c657ddeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c6582e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6582e0;
L8c6582e0: *stop=0x8c6582e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6582e2; /* mov.l r14,@-r15 */
L8c6582e2: *stop=0x8c6582e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6582e4; /* mov.l r13,@-r15 */
L8c6582e4: *stop=0x8c6582e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6582e6; /* mov.l r12,@-r15 */
L8c6582e6: *stop=0x8c6582e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6582e8; /* sts.l PR,@-r15 */
L8c6582e8: *stop=0x8c6582e8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658460u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6582ea; /* mov.w 0x8c658460,r0 */
L8c6582ea: *stop=0x8c6582eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c658468u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6582ec; /* mov.l 0x8c658468,r2 */
L8c6582ec: *stop=0x8c6582ecu;r[4]=r[15];goto L8c6582ee; /* mov r15,r4 */
L8c6582ee: *stop=0x8c6582eeu;r[4]+=16u;goto L8c6582f0; /* add #0x10,r4 */
L8c6582f0: *stop=0x8c6582f0u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6582f2; /* mov.l @(r0,r4),r3 */
L8c6582f2: *stop=0x8c6582f2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6582f4; /* mov.l r3,@r2 */
L8c6582f4: *stop=0x8c6582f4u;r[0]+=4294967288u;goto L8c6582f6; /* add #-0x8,r0 */
L8c6582f6: *stop=0x8c6582f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65846cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6582f8; /* mov.l 0x8c65846c,r3 */
L8c6582f8: *stop=0x8c6582f8u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6582fa; /* mov.l @(r0,r4),r1 */
L8c6582fa: *stop=0x8c6582fau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c6582fc; /* mov.l r1,@r3 */
L8c6582fc: *stop=0x8c6582fcu;r[0]+=4u;goto L8c6582fe; /* add #0x4,r0 */
L8c6582fe: *stop=0x8c6582feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658470u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658300; /* mov.l 0x8c658470,r1 */
L8c658300: *stop=0x8c658300u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658302; /* mov.l @(r0,r4),r2 */
L8c658302: *stop=0x8c658302u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c658304; /* mov.l r2,@r1 */
L8c658304: *stop=0x8c658304u;r[0]=84u;goto L8c658306; /* mov #0x54,r0 */
L8c658306: *stop=0x8c658306u;if(!valid_access(r[4]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658308; /* mov.l @(0x2c,r4),r1 */
L8c658308: *stop=0x8c658308u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65830a; /* mov.l @(r0,r4),r3 */
L8c65830a: *stop=0x8c65830au;r[0]=80u;goto L8c65830c; /* mov #0x50,r0 */
L8c65830c: *stop=0x8c65830cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65830e; /* mov.l @(r0,r4),r2 */
L8c65830e: *stop=0x8c65830eu;r[0]=92u;goto L8c658310; /* mov #0x5c,r0 */
L8c658310: *stop=0x8c658310u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c658312; /* shll r3 */
L8c658312: *stop=0x8c658312u;r[3]|=r[2];goto L8c658314; /* or r2,r3 */
L8c658314: *stop=0x8c658314u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658316; /* mov.l @(r0,r4),r2 */
L8c658316: *stop=0x8c658316u;r[0]=104u;goto L8c658318; /* mov #0x68,r0 */
L8c658318: *stop=0x8c658318u;r[2]<<=2;goto L8c65831a; /* shll2 r2 */
L8c65831a: *stop=0x8c65831au;r[3]|=r[2];goto L8c65831c; /* or r2,r3 */
L8c65831c: *stop=0x8c65831cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65831e; /* mov.l @(r0,r4),r2 */
L8c65831e: *stop=0x8c65831eu;r[0]=96u;goto L8c658320; /* mov #0x60,r0 */
L8c658320: *stop=0x8c658320u;r[2]<<=2;goto L8c658322; /* shll2 r2 */
L8c658322: *stop=0x8c658322u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c658324; /* shll r2 */
L8c658324: *stop=0x8c658324u;r[3]|=r[2];goto L8c658326; /* or r2,r3 */
L8c658326: *stop=0x8c658326u;if(!valid_access(r[4]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658328; /* mov.l @(0x30,r4),r2 */
L8c658328: *stop=0x8c658328u;r[2]<<=2;goto L8c65832a; /* shll2 r2 */
L8c65832a: *stop=0x8c65832au;r[2]<<=2;goto L8c65832c; /* shll2 r2 */
L8c65832c: *stop=0x8c65832cu;r[3]|=r[2];goto L8c65832e; /* or r2,r3 */
L8c65832e: *stop=0x8c65832eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658330; /* mov.l @(r0,r4),r2 */
L8c658330: *stop=0x8c658330u;r[2]<<=2;goto L8c658332; /* shll2 r2 */
L8c658332: *stop=0x8c658332u;r[2]<<=2;goto L8c658334; /* shll2 r2 */
L8c658334: *stop=0x8c658334u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c658336; /* shll r2 */
L8c658336: *stop=0x8c658336u;r[3]|=r[2];goto L8c658338; /* or r2,r3 */
L8c658338: *stop=0x8c658338u;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65833a; /* mov.l @(0x28,r4),r2 */
L8c65833a: *stop=0x8c65833au;r[2]<<=2;goto L8c65833c; /* shll2 r2 */
L8c65833c: *stop=0x8c65833cu;r[2]<<=2;goto L8c65833e; /* shll2 r2 */
L8c65833e: *stop=0x8c65833eu;r[2]<<=2;goto L8c658340; /* shll2 r2 */
L8c658340: *stop=0x8c658340u;r[3]|=r[2];goto L8c658342; /* or r2,r3 */
L8c658342: *stop=0x8c658342u;r[2]=7u;goto L8c658344; /* mov #0x7,r2 */
L8c658344: *stop=0x8c658344u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c658346; /* shad r2,r1 */
L8c658346: *stop=0x8c658346u;r[0]=72u;goto L8c658348; /* mov #0x48,r0 */
L8c658348: *stop=0x8c658348u;r[3]|=r[1];goto L8c65834a; /* or r1,r3 */
L8c65834a: *stop=0x8c65834au;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65834c; /* mov.l @(r0,r4),r1 */
L8c65834c: *stop=0x8c65834cu;r[0]=76u;goto L8c65834e; /* mov #0x4c,r0 */
L8c65834e: *stop=0x8c65834eu;r[1]<<=8;goto L8c658350; /* shll8 r1 */
L8c658350: *stop=0x8c658350u;r[3]|=r[1];goto L8c658352; /* or r1,r3 */
L8c658352: *stop=0x8c658352u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c658354; /* mov.l @(r0,r4),r1 */
L8c658354: *stop=0x8c658354u;r[1]<<=8;goto L8c658356; /* shll8 r1 */
L8c658356: *stop=0x8c658356u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c658358; /* shll r1 */
L8c658358: *stop=0x8c658358u;r[3]|=r[1];goto L8c65835a; /* or r1,r3 */
L8c65835a: *stop=0x8c65835au;{uint32_t loaded=rd(m,bus,&io_status,0x8c658474u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65835c; /* mov.l 0x8c658474,r1 */
L8c65835c: *stop=0x8c65835cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c65835e; /* mov.l r3,@r1 */
L8c65835e: *stop=0x8c65835eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658360; /* mov.l @r4,r2 */
L8c658360: *stop=0x8c658360u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658462u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c658362; /* mov.w 0x8c658462,r5 */
L8c658362: *stop=0x8c658362u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658364; /* mov.l @(0x4,r4),r3 */
L8c658364: *stop=0x8c658364u;r[2]&=r[5];goto L8c658366; /* and r5,r2 */
L8c658366: *stop=0x8c658366u;r[2]<<=16;goto L8c658368; /* shll16 r2 */
L8c658368: *stop=0x8c658368u;r[3]&=r[5];goto L8c65836a; /* and r5,r3 */
L8c65836a: *stop=0x8c65836au;r[3]|=r[2];goto L8c65836c; /* or r2,r3 */
L8c65836c: *stop=0x8c65836cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658478u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65836e; /* mov.l 0x8c658478,r2 */
L8c65836e: *stop=0x8c65836eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c658370; /* mov.l r3,@r2 */
L8c658370: *stop=0x8c658370u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658372; /* mov.l @(0x4,r4),r1 */
L8c658372: *stop=0x8c658372u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65847cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658374; /* mov.l 0x8c65847c,r3 */
L8c658374: *stop=0x8c658374u;r[1]&=r[5];goto L8c658376; /* and r5,r1 */
L8c658376: *stop=0x8c658376u;r[1]<<=16;goto L8c658378; /* shll16 r1 */
L8c658378: *stop=0x8c658378u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65837a; /* mov.l r1,@r3 */
L8c65837a: *stop=0x8c65837au;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65837c; /* mov.l @(0x14,r4),r1 */
L8c65837c: *stop=0x8c65837cu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65837e; /* mov.l @(0x8,r4),r0 */
L8c65837e: *stop=0x8c65837eu;r[1]&=r[5];goto L8c658380; /* and r5,r1 */
L8c658380: *stop=0x8c658380u;r[1]<<=16;goto L8c658382; /* shll16 r1 */
L8c658382: *stop=0x8c658382u;r[0]&=r[5];goto L8c658384; /* and r5,r0 */
L8c658384: *stop=0x8c658384u;r[0]|=r[1];goto L8c658386; /* or r1,r0 */
L8c658386: *stop=0x8c658386u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658480u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658388; /* mov.l 0x8c658480,r1 */
L8c658388: *stop=0x8c658388u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c65838a; /* mov.l r0,@r1 */
L8c65838a: *stop=0x8c65838au;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65838c; /* mov.l @(0xc,r4),r0 */
L8c65838c: *stop=0x8c65838cu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65838e; /* mov.l @(0x10,r4),r2 */
L8c65838e: *stop=0x8c65838eu;r[0]&=r[5];goto L8c658390; /* and r5,r0 */
L8c658390: *stop=0x8c658390u;r[0]<<=16;goto L8c658392; /* shll16 r0 */
L8c658392: *stop=0x8c658392u;r[2]&=r[5];goto L8c658394; /* and r5,r2 */
L8c658394: *stop=0x8c658394u;r[2]|=r[0];goto L8c658396; /* or r0,r2 */
L8c658396: *stop=0x8c658396u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658484u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c658398; /* mov.l 0x8c658484,r0 */
L8c658398: *stop=0x8c658398u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c65839a; /* mov.l r2,@r0 */
L8c65839a: *stop=0x8c65839au;r[0]=116u;goto L8c65839c; /* mov #0x74,r0 */
L8c65839c: *stop=0x8c65839cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65839e; /* mov.l @(r0,r4),r3 */
L8c65839e: *stop=0x8c65839eu;r[0]=120u;goto L8c6583a0; /* mov #0x78,r0 */
L8c6583a0: *stop=0x8c6583a0u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6583a2; /* mov.l @(r0,r4),r2 */
L8c6583a2: *stop=0x8c6583a2u;r[3]&=r[5];goto L8c6583a4; /* and r5,r3 */
L8c6583a4: *stop=0x8c6583a4u;r[2]&=r[5];goto L8c6583a6; /* and r5,r2 */
L8c6583a6: *stop=0x8c6583a6u;r[2]<<=16;goto L8c6583a8; /* shll16 r2 */
L8c6583a8: *stop=0x8c6583a8u;r[3]|=r[2];goto L8c6583aa; /* or r2,r3 */
L8c6583aa: *stop=0x8c6583aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c658488u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6583ac; /* mov.l 0x8c658488,r2 */
L8c6583ac: *stop=0x8c6583acu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6583ae; /* mov.l r3,@r2 */
L8c6583ae: *stop=0x8c6583aeu;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6583b0; /* mov.l @(0x1c,r4),r1 */
L8c6583b0: *stop=0x8c6583b0u;r[5]=15u;goto L8c6583b2; /* mov #0xf,r5 */
L8c6583b2: *stop=0x8c6583b2u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6583b4; /* mov.l @(0x18,r4),r3 */
L8c6583b4: *stop=0x8c6583b4u;r[6]=127u;goto L8c6583b6; /* mov #0x7f,r6 */
L8c6583b6: *stop=0x8c6583b6u;r[1]&=r[5];goto L8c6583b8; /* and r5,r1 */
L8c6583b8: *stop=0x8c6583b8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658464u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c6583ba; /* mov.w 0x8c658464,r13 */
L8c6583ba: *stop=0x8c6583bau;r[1]<<=8;goto L8c6583bc; /* shll8 r1 */
L8c6583bc: *stop=0x8c6583bcu;r[3]&=r[6];goto L8c6583be; /* and r6,r3 */
L8c6583be: *stop=0x8c6583beu;r[3]|=r[1];goto L8c6583c0; /* or r1,r3 */
L8c6583c0: *stop=0x8c6583c0u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6583c2; /* mov.l @(0x20,r4),r1 */
L8c6583c2: *stop=0x8c6583c2u;r[2]=22u;goto L8c6583c4; /* mov #0x16,r2 */
L8c6583c4: *stop=0x8c6583c4u;r[1]&=r[13];goto L8c6583c6; /* and r13,r1 */
L8c6583c6: *stop=0x8c6583c6u;r[1]<<=8;goto L8c6583c8; /* shll8 r1 */
L8c6583c8: *stop=0x8c6583c8u;r[1]<<=2;goto L8c6583ca; /* shll2 r1 */
L8c6583ca: *stop=0x8c6583cau;r[1]<<=2;goto L8c6583cc; /* shll2 r1 */
L8c6583cc: *stop=0x8c6583ccu;r[3]|=r[1];goto L8c6583ce; /* or r1,r3 */
L8c6583ce: *stop=0x8c6583ceu;if(!valid_access(r[4]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6583d0; /* mov.l @(0x24,r4),r1 */
L8c6583d0: *stop=0x8c6583d0u;r[1]&=r[6];goto L8c6583d2; /* and r6,r1 */
L8c6583d2: *stop=0x8c6583d2u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c6583d4; /* shad r2,r1 */
L8c6583d4: *stop=0x8c6583d4u;r[3]|=r[1];goto L8c6583d6; /* or r1,r3 */
L8c6583d6: *stop=0x8c6583d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65848cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6583d8; /* mov.l 0x8c65848c,r1 */
L8c6583d8: *stop=0x8c6583d8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6583da; /* mov.l r3,@r1 */
L8c6583da: *stop=0x8c6583dau;if(!valid_access(r[4]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6583dc; /* mov.l @(0x34,r4),r0 */
L8c6583dc: *stop=0x8c6583dcu;r[0]&=0x1u;goto L8c6583de; /* and #0x1,r0 */
L8c6583de: *stop=0x8c6583deu;r[3]=r[0];goto L8c6583e0; /* mov r0,r3 */
L8c6583e0: *stop=0x8c6583e0u;if(!valid_access(r[4]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x38u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6583e2; /* mov.l @(0x38,r4),r0 */
L8c6583e2: *stop=0x8c6583e2u;r[0]&=0x1u;goto L8c6583e4; /* and #0x1,r0 */
L8c6583e4: *stop=0x8c6583e4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c6583e6; /* shll r0 */
L8c6583e6: *stop=0x8c6583e6u;r[3]|=r[0];goto L8c6583e8; /* or r0,r3 */
L8c6583e8: *stop=0x8c6583e8u;if(!valid_access(r[4]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x3cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6583ea; /* mov.l @(0x3c,r4),r0 */
L8c6583ea: *stop=0x8c6583eau;r[0]&=0x1u;goto L8c6583ec; /* and #0x1,r0 */
L8c6583ec: *stop=0x8c6583ecu;r[0]<<=2;goto L8c6583ee; /* shll2 r0 */
L8c6583ee: *stop=0x8c6583eeu;r[3]|=r[0];goto L8c6583f0; /* or r0,r3 */
L8c6583f0: *stop=0x8c6583f0u;r[0]=64u;goto L8c6583f2; /* mov #0x40,r0 */
L8c6583f2: *stop=0x8c6583f2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6583f4; /* mov.l @(r0,r4),r0 */
L8c6583f4: *stop=0x8c6583f4u;r[0]&=0x1u;goto L8c6583f6; /* and #0x1,r0 */
L8c6583f6: *stop=0x8c6583f6u;r[0]<<=2;goto L8c6583f8; /* shll2 r0 */
L8c6583f8: *stop=0x8c6583f8u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c6583fa; /* shll r0 */
L8c6583fa: *stop=0x8c6583fau;r[3]|=r[0];goto L8c6583fc; /* or r0,r3 */
L8c6583fc: *stop=0x8c6583fcu;r[0]=68u;goto L8c6583fe; /* mov #0x44,r0 */
L8c6583fe: *stop=0x8c6583feu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658400; /* mov.l @(r0,r4),r0 */
L8c658400: *stop=0x8c658400u;r[5]&=r[0];goto L8c658402; /* and r0,r5 */
L8c658402: *stop=0x8c658402u;r[0]=108u;goto L8c658404; /* mov #0x6c,r0 */
L8c658404: *stop=0x8c658404u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658406; /* mov.l @(r0,r4),r0 */
L8c658406: *stop=0x8c658406u;r[5]<<=2;goto L8c658408; /* shll2 r5 */
L8c658408: *stop=0x8c658408u;r[5]<<=2;goto L8c65840a; /* shll2 r5 */
L8c65840a: *stop=0x8c65840au;r[0]&=0x1u;goto L8c65840c; /* and #0x1,r0 */
L8c65840c: *stop=0x8c65840cu;r[0]<<=8;goto L8c65840e; /* shll8 r0 */
L8c65840e: *stop=0x8c65840eu;r[3]|=r[5];goto L8c658410; /* or r5,r3 */
L8c658410: *stop=0x8c658410u;r[3]|=r[0];goto L8c658412; /* or r0,r3 */
L8c658412: *stop=0x8c658412u;r[0]=112u;goto L8c658414; /* mov #0x70,r0 */
L8c658414: *stop=0x8c658414u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658416; /* mov.l @(r0,r4),r0 */
L8c658416: *stop=0x8c658416u;r[0]&=0x3fu;goto L8c658418; /* and #0x3f,r0 */
L8c658418: *stop=0x8c658418u;r[0]<<=16;goto L8c65841a; /* shll16 r0 */
L8c65841a: *stop=0x8c65841au;{uint32_t loaded=rd(m,bus,&io_status,0x8c658490u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65841c; /* mov.l 0x8c658490,r2 */
L8c65841c: *stop=0x8c65841cu;r[3]|=r[0];goto L8c65841e; /* or r0,r3 */
L8c65841e: *stop=0x8c65841eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c658420; /* mov.l r3,@r2 */
L8c658420: *stop=0x8c658420u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658494u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c658422; /* mov.l 0x8c658494,r0 */
L8c658422: *stop=0x8c658422u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c658424; /* mov.l @r0,r3 */
L8c658424: *stop=0x8c658424u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658468u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c658426; /* mov.l 0x8c658468,r1 */
L8c658426: *stop=0x8c658426u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658428; /* mov.l @r1,r0 */
L8c658428: *stop=0x8c658428u;r[0]+=r[3];goto L8c65842a; /* add r3,r0 */
L8c65842a: *stop=0x8c65842au;{uint32_t loaded=rd(m,bus,&io_status,0x8c658498u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65842c; /* mov.l 0x8c658498,r3 */
L8c65842c: *stop=0x8c65842cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c65842e; /* mov.l r0,@r3 */
L8c65842e: *stop=0x8c65842eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65849cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c658430; /* mov.l 0x8c65849c,r0 */
L8c658430: *stop=0x8c658430u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c658432; /* mov.l @r0,r2 */
L8c658432: *stop=0x8c658432u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65846cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658434; /* mov.l 0x8c65846c,r3 */
L8c658434: *stop=0x8c658434u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c658436; /* mov.l @r3,r0 */
L8c658436: *stop=0x8c658436u;r[1]+=4294967292u;goto L8c658438; /* add #-0x4,r1 */
L8c658438: *stop=0x8c658438u;r[0]+=r[2];goto L8c65843a; /* add r2,r0 */
L8c65843a: *stop=0x8c65843au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65843c; /* mov.l @r1,r2 */
L8c65843c: *stop=0x8c65843cu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65843e; /* mov.l @(0x4,r3),r1 */
L8c65843e: *stop=0x8c65843eu;r[1]+=r[2];goto L8c658440; /* add r2,r1 */
L8c658440: *stop=0x8c658440u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6584a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c658442; /* mov.l 0x8c6584a0,r2 */
L8c658442: *stop=0x8c658442u;r[1]<<=16;goto L8c658444; /* shll16 r1 */
L8c658444: *stop=0x8c658444u;r[0]|=r[1];goto L8c658446; /* or r1,r0 */
L8c658446: *stop=0x8c658446u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c658448; /* mov.l r0,@r2 */
L8c658448: *stop=0x8c658448u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6584a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65844a; /* mov.l 0x8c6584a4,r1 */
L8c65844a: *stop=0x8c65844au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65844c; /* mov.l @r1,r0 */
L8c65844c: *stop=0x8c65844cu;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c65844e; /* tst #0x2,r0 */
L8c65844e: if((*sr&1u))goto L8c6584d0;goto L8c658450; /* bt 0x8c6584d0 */
L8c658450: *stop=0x8c658450u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658466u,2);if(io_status)return io_status;r[14]=loaded;}goto L8c658452; /* mov.w 0x8c658466,r14 */
L8c658452: *stop=0x8c658452u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6584a8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c658454; /* mov.l 0x8c6584a8,r12 */
L8c658454: *stop=0x8c658456u;r[4]=r[14];goto L8c6584c0; /* bra 0x8c6584c0 */
L8c6584c0: target=r[12];*pr=0x8c6584c4u;*stop=0x8c6584c2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6584c4; /* jsr @r12 */
L8c6584c4: *stop=0x8c6584c4u;*sr=(*sr&~1u)|!(r[13]&r[0]);goto L8c6584c6; /* tst r13,r0 */
L8c6584c6: if((*sr&1u))goto L8c658454;goto L8c6584c8; /* bt 0x8c658454 */
L8c6584c8: target=r[12];*pr=0x8c6584ccu;*stop=0x8c6584cau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6584cc; /* jsr @r12 */
L8c6584cc: *stop=0x8c6584ccu;*sr=(*sr&~1u)|!(r[13]&r[0]);goto L8c6584ce; /* tst r13,r0 */
L8c6584ce: if(!(*sr&1u))goto L8c6584c8;goto L8c6584d0; /* bf 0x8c6584c8 */
L8c6584d0: *stop=0x8c6584d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658578u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6584d2; /* mov.l 0x8c658578,r2 */
L8c6584d2: *stop=0x8c6584d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658574u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6584d4; /* mov.l 0x8c658574,r14 */
L8c6584d4: *stop=0x8c6584d4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6584d6; /* mov.l @r2,r0 */
L8c6584d6: *stop=0x8c6584d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658560u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6584d8; /* mov.w 0x8c658560,r4 */
L8c6584d8: *stop=0x8c6584d8u;r[0]|=0x8u;goto L8c6584da; /* or #0x8,r0 */
L8c6584da: target=r[14];*pr=0x8c6584deu;*stop=0x8c6584dcu;r[5]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6584de; /* jsr @r14 */
L8c6584de: *stop=0x8c6584deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65857cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6584e0; /* mov.l 0x8c65857c,r3 */
L8c6584e0: *stop=0x8c6584e0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6584e2; /* mov.l @r3,r0 */
L8c6584e2: *stop=0x8c6584e2u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c6584e4; /* tst #0x1,r0 */
L8c6584e4: take=(*sr&1u);*stop=0x8c6584e6u;r[4]=68u;if(take)goto L8c6584ee;goto L8c6584e8; /* bt/s 0x8c6584ee */
L8c6584e8: *stop=0x8c6584e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658580u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6584ea; /* mov.l 0x8c658580,r5 */
L8c6584ea: *stop=0x8c6584ecu;goto L8c6584f0; /* bra 0x8c6584f0 */
L8c6584ee: *stop=0x8c6584eeu;r[5]=0u;goto L8c6584f0; /* mov #0x0,r5 */
L8c6584f0: target=r[14];*pr=0x8c6584f4u;*stop=0x8c6584f2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6584f4; /* jsr @r14 */
L8c6584f4: *stop=0x8c6584f4u;r[5]=0u;goto L8c6584f6; /* mov #0x0,r5 */
L8c6584f6: target=r[14];*pr=0x8c6584fau;*stop=0x8c6584f8u;r[4]=92u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6584fa; /* jsr @r14 */
L8c6584fa: *stop=0x8c6584fau;{uint32_t loaded=sx(m,bus,&io_status,0x8c658562u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6584fc; /* mov.w 0x8c658562,r4 */
L8c6584fc: *stop=0x8c6584fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658584u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6584fe; /* mov.l 0x8c658584,r2 */
L8c6584fe: target=r[14];*pr=0x8c658502u;*stop=0x8c658500u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658502; /* jsr @r14 */
L8c658502: *stop=0x8c658502u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658564u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c658504; /* mov.w 0x8c658564,r4 */
L8c658504: *stop=0x8c658504u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658588u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658506; /* mov.l 0x8c658588,r3 */
L8c658506: target=r[14];*pr=0x8c65850au;*stop=0x8c658508u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65850a; /* jsr @r14 */
L8c65850a: *stop=0x8c65850au;{uint32_t loaded=sx(m,bus,&io_status,0x8c658566u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65850c; /* mov.w 0x8c658566,r4 */
L8c65850c: *stop=0x8c65850cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65858cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65850e; /* mov.l 0x8c65858c,r2 */
L8c65850e: target=r[14];*pr=0x8c658512u;*stop=0x8c658510u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658512; /* jsr @r14 */
L8c658512: *stop=0x8c658512u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658568u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c658514; /* mov.w 0x8c658568,r4 */
L8c658514: *stop=0x8c658514u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658590u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658516; /* mov.l 0x8c658590,r3 */
L8c658516: target=r[14];*pr=0x8c65851au;*stop=0x8c658518u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65851a; /* jsr @r14 */
L8c65851a: *stop=0x8c65851au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65856au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65851c; /* mov.w 0x8c65856a,r4 */
L8c65851c: *stop=0x8c65851cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c658594u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65851e; /* mov.l 0x8c658594,r2 */
L8c65851e: target=r[14];*pr=0x8c658522u;*stop=0x8c658520u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658522; /* jsr @r14 */
L8c658522: *stop=0x8c658522u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65856cu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c658524; /* mov.w 0x8c65856c,r4 */
L8c658524: *stop=0x8c658524u;{uint32_t loaded=rd(m,bus,&io_status,0x8c658598u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658526; /* mov.l 0x8c658598,r3 */
L8c658526: target=r[14];*pr=0x8c65852au;*stop=0x8c658528u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65852a; /* jsr @r14 */
L8c65852a: *stop=0x8c65852au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65856eu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65852c; /* mov.w 0x8c65856e,r4 */
L8c65852c: *stop=0x8c65852cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65859cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65852e; /* mov.l 0x8c65859c,r2 */
L8c65852e: target=r[14];*pr=0x8c658532u;*stop=0x8c658530u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658532; /* jsr @r14 */
L8c658532: *stop=0x8c658532u;{uint32_t loaded=sx(m,bus,&io_status,0x8c658570u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c658534; /* mov.w 0x8c658570,r4 */
L8c658534: *stop=0x8c658534u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6585a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c658536; /* mov.l 0x8c6585a0,r3 */
L8c658536: target=r[14];*pr=0x8c65853au;*stop=0x8c658538u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65853a; /* jsr @r14 */
L8c65853a: *stop=0x8c65853au;{uint32_t loaded=sx(m,bus,&io_status,0x8c658572u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65853c; /* mov.w 0x8c658572,r4 */
L8c65853c: *stop=0x8c65853cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6585a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65853e; /* mov.l 0x8c6585a4,r2 */
L8c65853e: target=r[14];*pr=0x8c658542u;*stop=0x8c658540u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c658542; /* jsr @r14 */
L8c658542: *stop=0x8c658542u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c658544; /* lds.l @r15+,PR */
L8c658544: *stop=0x8c658544u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c658546; /* mov.l @r15+,r12 */
L8c658546: *stop=0x8c658546u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c658548; /* mov.l @r15+,r13 */
L8c658548: *stop=0x8c658548u;if(*pr!=entry_pr)return -3;*stop=0x8c65854au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_config_8c6513ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6513ee;
L8c6513ee: *stop=0x8c6513eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6513f0; /* mov.l 0x8c6514cc,r3 */
L8c6513f0: target=r[3];*stop=0x8c6513f2u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}
