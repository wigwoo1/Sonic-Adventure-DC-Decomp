/* Original buffer initialization, upload and wait; explicit complete state.
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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x05000000u && (a&0x1fffffffu)<0x05800000u)return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x04000000u && (a&0x1fffffffu)<0x04800000u)return 1;
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6900u || (a&0x1fffffffu)==0x005f6908u))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c65393a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65393a;
L8c65393a: *stop=0x8c65393au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65393c; /* mov.l r14,@-r15 */
L8c65393c: *stop=0x8c65393cu;r[0]=r[6];goto L8c65393e; /* mov r6,r0 */
L8c65393e: *stop=0x8c65393eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c653940; /* sts.l PR,@-r15 */
L8c653940: *stop=0x8c653940u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c653942; /* cmp/eq #0x1,r0 */
L8c653942: take=(*sr&1u);*stop=0x8c653944u;r[14]=r[4];if(take)goto L8c65394c;goto L8c653946; /* bt/s 0x8c65394c */
L8c653946: *stop=0x8c653946u;r[0]=r[7];goto L8c653948; /* mov r7,r0 */
L8c653948: *stop=0x8c653948u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65394a; /* cmp/eq #0x1,r0 */
L8c65394a: if(!(*sr&1u))goto L8c653954;goto L8c65394c; /* bf 0x8c653954 */
L8c65394c: *stop=0x8c65394cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65394e; /* lds.l @r15+,PR */
L8c65394e: *stop=0x8c65394eu;r[0]=4u;goto L8c653950; /* mov #0x4,r0 */
L8c653950: *stop=0x8c653950u;if(*pr!=entry_pr)return -3;*stop=0x8c653952u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c653954: *stop=0x8c653954u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653956; /* mov.l @r14,r0 */
L8c653956: *stop=0x8c653956u;r[0]=r[0]&65535u;goto L8c653958; /* extu.w r0,r0 */
L8c653958: *stop=0x8c653958u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c65395a; /* cmp/eq #0x3,r0 */
L8c65395a: if(!(*sr&1u))goto L8c653968;goto L8c65395c; /* bf 0x8c653968 */
L8c65395c: *stop=0x8c65395cu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65395e; /* mov.l @(0xc,r14),r6 */
L8c65395e: *stop=0x8c65395eu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c653960; /* mov.l @(0x18,r14),r7 */
L8c653960: target=0x8c654520u;*pr=0x8c653964u;*stop=0x8c653962u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653964; /* bsr 0x8c654520 */
L8c653964: *stop=0x8c653966u;goto L8c653994; /* bra 0x8c653994 */
L8c653968: *stop=0x8c653968u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65396a; /* mov.l @(0x18,r14),r4 */
L8c65396a: *stop=0x8c65396au;r[2]=4u;goto L8c65396c; /* mov #0x4,r2 */
L8c65396c: *stop=0x8c65396cu;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c65396e; /* tst r4,r2 */
L8c65396e: if((*sr&1u))goto L8c65398c;goto L8c653970; /* bt 0x8c65398c */
L8c653970: *stop=0x8c653970u;r[1]=1u;goto L8c653972; /* mov #0x1,r1 */
L8c653972: *stop=0x8c653972u;*sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c653974; /* tst r4,r1 */
L8c653974: if((*sr&1u))goto L8c65398c;goto L8c653976; /* bt 0x8c65398c */
L8c653976: *stop=0x8c653976u;r[2]=8u;goto L8c653978; /* mov #0x8,r2 */
L8c653978: *stop=0x8c653978u;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c65397a; /* tst r4,r2 */
L8c65397a: if(!(*sr&1u))goto L8c65398c;goto L8c65397c; /* bf 0x8c65398c */
L8c65397c: *stop=0x8c65397cu;r[1]=32u;goto L8c65397e; /* mov #0x20,r1 */
L8c65397e: *stop=0x8c65397eu;*sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c653980; /* tst r4,r1 */
L8c653980: if(!(*sr&1u))goto L8c65398c;goto L8c653982; /* bf 0x8c65398c */
L8c653982: *stop=0x8c653982u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c653984; /* mov.l @(0xc,r14),r6 */
L8c653984: target=0x8c6544a0u;*pr=0x8c653988u;*stop=0x8c653986u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653988; /* bsr 0x8c6544a0 */
L8c653988: *stop=0x8c65398au;goto L8c653994; /* bra 0x8c653994 */
L8c65398c: *stop=0x8c65398cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6539ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65398e; /* mov.l 0x8c6539cc,r2 */
L8c65398e: *stop=0x8c65398eu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c653990; /* mov.l @(0x14,r14),r6 */
L8c653990: target=r[2];*pr=0x8c653994u;*stop=0x8c653992u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653994; /* jsr @r2 */
L8c653994: *stop=0x8c653994u;r[0]=0u;goto L8c653996; /* mov #0x0,r0 */
L8c653996: *stop=0x8c653996u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c653998; /* lds.l @r15+,PR */
L8c653998: *stop=0x8c653998u;if(*pr!=entry_pr)return -3;*stop=0x8c65399au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c654520(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c654520;
L8c654520: *stop=0x8c654520u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c654522; /* mov.l r14,@-r15 */
L8c654522: *stop=0x8c654522u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c654524; /* mov.l r13,@-r15 */
L8c654524: *stop=0x8c654524u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c654526; /* mov.l r12,@-r15 */
L8c654526: *stop=0x8c654526u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c654528; /* mov.l r10,@-r15 */
L8c654528: *stop=0x8c654528u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65452a; /* sts.l PR,@-r15 */
L8c65452a: *stop=0x8c65452au;r[15]+=4294967288u;goto L8c65452c; /* add #-0x8,r15 */
L8c65452c: *stop=0x8c65452cu;r[13]=r[5];goto L8c65452e; /* mov r5,r13 */
L8c65452e: *stop=0x8c65452eu;r[14]=r[4];goto L8c654530; /* mov r4,r14 */
L8c654530: *stop=0x8c654530u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c654532; /* mov.l r6,@r15 */
L8c654532: *stop=0x8c654532u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[7],4);if(io_status)return io_status;goto L8c654534; /* mov.l r7,@(0x4,r15) */
L8c654534: *stop=0x8c654534u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c654536; /* mov.l @r15,r5 */
L8c654536: target=0x8c6541e0u;*pr=0x8c65453au;*stop=0x8c654538u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65453a; /* bsr 0x8c6541e0 */
L8c65453a: *stop=0x8c65453au;r[2]=r[0];goto L8c65453c; /* mov r0,r2 */
L8c65453c: *stop=0x8c65453cu;r[4]=r[0];goto L8c65453e; /* mov r0,r4 */
L8c65453e: *stop=0x8c65453eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654540; /* mov.l @(0x4,r15),r0 */
L8c654540: *stop=0x8c654540u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c654542; /* tst #0x1,r0 */
L8c654542: take=(*sr&1u);*stop=0x8c654544u;r[2]<<=2;if(take)goto L8c654552;goto L8c654546; /* bt/s 0x8c654552 */
L8c654546: *stop=0x8c654546u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6545c4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654548; /* mov.l 0x8c6545c4,r0 */
L8c654548: *stop=0x8c654548u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[12]=loaded;}goto L8c65454a; /* mov.l @(r0,r2),r12 */
L8c65454a: *stop=0x8c65454au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6545c8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65454c; /* mov.l 0x8c6545c8,r0 */
L8c65454c: *stop=0x8c65454cu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[10]=loaded;}goto L8c65454e; /* mov.l @(r0,r2),r10 */
L8c65454e: *stop=0x8c654550u;r[10]+=10u;goto L8c65455a; /* bra 0x8c65455a */
L8c654552: *stop=0x8c654552u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6545ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654554; /* mov.l 0x8c6545cc,r0 */
L8c654554: *stop=0x8c654554u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[12]=loaded;}goto L8c654556; /* mov.l @(r0,r2),r12 */
L8c654556: *stop=0x8c654556u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6545d0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654558; /* mov.l 0x8c6545d0,r0 */
L8c654558: *stop=0x8c654558u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[10]=loaded;}goto L8c65455a; /* mov.l @(r0,r2),r10 */
L8c65455a: *stop=0x8c65455au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6545d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65455c; /* mov.l 0x8c6545d4,r2 */
L8c65455c: *stop=0x8c65455cu;r[5]=r[13];goto L8c65455e; /* mov r13,r5 */
L8c65455e: *stop=0x8c65455eu;r[6]=r[12];goto L8c654560; /* mov r12,r6 */
L8c654560: target=r[2];*pr=0x8c654564u;*stop=0x8c654562u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c654564; /* jsr @r2 */
L8c654564: *stop=0x8c654564u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6545c0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c654566; /* mov.w 0x8c6545c0,r3 */
L8c654566: *stop=0x8c654566u;r[13]+=r[12];goto L8c654568; /* add r12,r13 */
L8c654568: *stop=0x8c654568u;r[5]=r[13];goto L8c65456a; /* mov r13,r5 */
L8c65456a: *stop=0x8c65456au;r[6]=r[10];goto L8c65456c; /* mov r10,r6 */
L8c65456c: *stop=0x8c65456cu;r[14]+=r[3];goto L8c65456e; /* add r3,r14 */
L8c65456e: *stop=0x8c65456eu;r[4]=r[14];goto L8c654570; /* mov r14,r4 */
L8c654570: *stop=0x8c654570u;r[15]+=8u;goto L8c654572; /* add #0x8,r15 */
L8c654572: *stop=0x8c654572u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c654574; /* lds.l @r15+,PR */
L8c654574: *stop=0x8c654574u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6545d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c654576; /* mov.l 0x8c6545d4,r2 */
L8c654576: *stop=0x8c654576u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c654578; /* mov.l @r15+,r10 */
L8c654578: *stop=0x8c654578u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65457a; /* mov.l @r15+,r12 */
L8c65457a: *stop=0x8c65457au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65457c; /* mov.l @r15+,r13 */
L8c65457c: target=r[2];*stop=0x8c65457eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c6544a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6544a0;
L8c6544a0: *stop=0x8c6544a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6544a2; /* sts.l PR,@-r15 */
L8c6544a2: *stop=0x8c6544a2u;r[15]+=4294967284u;goto L8c6544a4; /* add #-0xc,r15 */
L8c6544a4: *stop=0x8c6544a4u;r[4]+=4u;goto L8c6544a6; /* add #0x4,r4 */
L8c6544a6: *stop=0x8c6544a6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c6544a8; /* mov.l r6,@r15 */
L8c6544a8: *stop=0x8c6544a8u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c6544aa; /* mov.l r5,@(0x8,r15) */
L8c6544aa: *stop=0x8c6544aau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c6544ac; /* mov.l r4,@(0x4,r15) */
L8c6544ac: *stop=0x8c6544acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6544ae; /* mov.l @r15,r5 */
L8c6544ae: target=0x8c6541e0u;*pr=0x8c6544b2u;*stop=0x8c6544b0u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6544b2; /* bsr 0x8c6541e0 */
L8c6544b2: *stop=0x8c6544b2u;r[6]=r[0];goto L8c6544b4; /* mov r0,r6 */
L8c6544b4: *stop=0x8c6544b4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6544b6; /* mov.l r0,@r15 */
L8c6544b6: *stop=0x8c6544b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65450cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6544b8; /* mov.l 0x8c65450c,r0 */
L8c6544b8: *stop=0x8c6544b8u;r[6]<<=2;goto L8c6544ba; /* shll2 r6 */
L8c6544ba: *stop=0x8c6544bau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6544bc; /* mov.l @(0x8,r15),r5 */
L8c6544bc: *stop=0x8c6544bcu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6544be; /* mov.l @(r0,r6),r6 */
L8c6544be: *stop=0x8c6544beu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6544c0; /* mov.l @(0x4,r15),r4 */
L8c6544c0: *stop=0x8c6544c0u;r[6]+=4u;goto L8c6544c2; /* add #0x4,r6 */
L8c6544c2: *stop=0x8c6544c2u;r[15]+=12u;goto L8c6544c4; /* add #0xc,r15 */
L8c6544c4: *stop=0x8c6544c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c654510u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6544c6; /* mov.l 0x8c654510,r3 */
L8c6544c6: target=r[3];*stop=0x8c6544c8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c6534e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6534e0;
L8c6534e0: *stop=0x8c6534e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6534e2; /* mov.l r14,@-r15 */
L8c6534e2: *stop=0x8c6534e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6534e4; /* mov.l r13,@-r15 */
L8c6534e4: *stop=0x8c6534e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6534e6; /* mov.l r12,@-r15 */
L8c6534e6: *stop=0x8c6534e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6534e8; /* mov.l r11,@-r15 */
L8c6534e8: *stop=0x8c6534e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6534ea; /* mov.l r10,@-r15 */
L8c6534ea: *stop=0x8c6534eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6534ec; /* mov.l r9,@-r15 */
L8c6534ec: *stop=0x8c6534ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6534ee; /* mov.l r8,@-r15 */
L8c6534ee: *stop=0x8c6534eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6534f0; /* sts.l PR,@-r15 */
L8c6534f0: *stop=0x8c6534f0u;r[15]+=4294967292u;goto L8c6534f2; /* add #-0x4,r15 */
L8c6534f2: *stop=0x8c6534f2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6534f4; /* stc SR,r0 */
L8c6534f4: *stop=0x8c6534f4u;r[12]=r[4];goto L8c6534f6; /* mov r4,r12 */
L8c6534f6: *stop=0x8c6534f6u;r[10]=31u;goto L8c6534f8; /* mov #0x1f,r10 */
L8c6534f8: *stop=0x8c6534f8u;r[3]=r[12];goto L8c6534fa; /* mov r12,r3 */
L8c6534fa: *stop=0x8c6534fau;r[11]=r[6];goto L8c6534fc; /* mov r6,r11 */
L8c6534fc: *stop=0x8c6534fcu;r[0]>>=2;goto L8c6534fe; /* shlr2 r0 */
L8c6534fe: *stop=0x8c6534feu;r[14]=r[5];goto L8c653500; /* mov r5,r14 */
L8c653500: *stop=0x8c653500u;r[0]>>=2;goto L8c653502; /* shlr2 r0 */
L8c653502: *stop=0x8c653502u;*sr=(*sr&~1u)|!(r[10]&r[3]);goto L8c653504; /* tst r10,r3 */
L8c653504: *stop=0x8c653504u;r[0]&=0xfu;goto L8c653506; /* and #0xf,r0 */
L8c653506: take=!(*sr&1u);*stop=0x8c653508u;r[4]=r[0];if(take)goto L8c6535a0;goto L8c65350a; /* bf/s 0x8c6535a0 */
L8c65350a: *stop=0x8c65350au;r[1]=r[14];goto L8c65350c; /* mov r14,r1 */
L8c65350c: *stop=0x8c65350cu;*sr=(*sr&~1u)|!(r[10]&r[1]);goto L8c65350e; /* tst r10,r1 */
L8c65350e: if(!(*sr&1u))goto L8c6535a0;goto L8c653510; /* bf 0x8c6535a0 */
L8c653510: *stop=0x8c653510u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c653512; /* tst r4,r4 */
L8c653512: if(!(*sr&1u))goto L8c6535a0;goto L8c653514; /* bf 0x8c6535a0 */
L8c653514: *stop=0x8c653514u;r[5]=r[11];goto L8c653516; /* mov r11,r5 */
L8c653516: target=0x8c65344eu;*pr=0x8c65351au;*stop=0x8c653518u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65351a; /* bsr 0x8c65344e */
L8c65351a: *stop=0x8c65351au;r[13]=4294967264u;goto L8c65351c; /* mov #-0x20,r13 */
L8c65351c: *stop=0x8c65351cu;r[13]&=r[11];goto L8c65351e; /* and r11,r13 */
L8c65351e: *stop=0x8c65351eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c653520; /* tst r13,r13 */
L8c653520: if((*sr&1u))goto L8c653548;goto L8c653522; /* bt 0x8c653548 */
L8c653522: *stop=0x8c653522u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653584u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c653524; /* mov.l 0x8c653584,r8 */
L8c653524: *stop=0x8c653524u;{uint32_t loaded=sx(m,bus,&io_status,0x8c653580u,2);if(io_status)return io_status;r[9]=loaded;}goto L8c653526; /* mov.w 0x8c653580,r9 */
L8c653526: *stop=0x8c653526u;r[8]+=r[12];goto L8c653528; /* add r12,r8 */
L8c653528: *stop=0x8c653528u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653588u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65352a; /* mov.l 0x8c653588,r3 */
L8c65352a: *stop=0x8c65352au;r[2]=r[15];goto L8c65352c; /* mov r15,r2 */
L8c65352c: *stop=0x8c65352cu;r[5]=r[14];goto L8c65352e; /* mov r14,r5 */
L8c65352e: *stop=0x8c65352eu;r[6]=r[13];goto L8c653530; /* mov r13,r6 */
L8c653530: *stop=0x8c653530u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c653532; /* mov.l r2,@-r15 */
L8c653532: *stop=0x8c653532u;r[7]=24u;goto L8c653534; /* mov #0x18,r7 */
L8c653534: target=r[3];*pr=0x8c653538u;*stop=0x8c653536u;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653538; /* jsr @r3 */
L8c653538: *stop=0x8c653538u;*sr=(*sr&~1u)|(r[9]==r[0]);goto L8c65353a; /* cmp/eq r9,r0 */
L8c65353a: take=(*sr&1u);*stop=0x8c65353cu;r[15]+=4u;if(take)goto L8c653528;goto L8c65353e; /* bt/s 0x8c653528 */
L8c65353e: *stop=0x8c65353eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c653540; /* mov.l @r15,r3 */
L8c653540: *stop=0x8c653540u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65358cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c653542; /* mov.l 0x8c65358c,r1 */
L8c653542: *stop=0x8c653542u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c653544; /* mov.l @r3,r2 */
L8c653544: *stop=0x8c653546u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c65354e; /* bra 0x8c65354e */
L8c653548: *stop=0x8c653548u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65358cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65354a; /* mov.l 0x8c65358c,r3 */
L8c65354a: *stop=0x8c65354au;r[0]=0u;goto L8c65354c; /* mov #0x0,r0 */
L8c65354c: *stop=0x8c65354cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c65354e; /* mov.l r0,@r3 */
L8c65354e: *stop=0x8c65354eu;r[6]=r[11];goto L8c653550; /* mov r11,r6 */
L8c653550: *stop=0x8c653550u;r[6]&=r[10];goto L8c653552; /* and r10,r6 */
L8c653552: *stop=0x8c653552u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c653554; /* tst r6,r6 */
L8c653554: if((*sr&1u))goto L8c653564;goto L8c653556; /* bt 0x8c653564 */
L8c653556: *stop=0x8c653556u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653590u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c653558; /* mov.l 0x8c653590,r4 */
L8c653558: *stop=0x8c653558u;r[14]+=r[13];goto L8c65355a; /* add r13,r14 */
L8c65355a: *stop=0x8c65355au;{uint32_t loaded=rd(m,bus,&io_status,0x8c653594u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65355c; /* mov.l 0x8c653594,r3 */
L8c65355c: *stop=0x8c65355cu;r[12]+=r[13];goto L8c65355e; /* add r13,r12 */
L8c65355e: *stop=0x8c65355eu;r[5]=r[14];goto L8c653560; /* mov r14,r5 */
L8c653560: target=r[3];*pr=0x8c653564u;*stop=0x8c653562u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653564; /* jsr @r3 */
L8c653564: *stop=0x8c653564u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653598u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653566; /* mov.l 0x8c653598,r2 */
L8c653566: *stop=0x8c653566u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653568; /* mov.l @r2,r0 */
L8c653568: *stop=0x8c653568u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65356a; /* cmp/eq #0x1,r0 */
L8c65356a: if(!(*sr&1u))goto L8c6535ac;goto L8c65356c; /* bf 0x8c6535ac */
L8c65356c: *stop=0x8c65356cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65359cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65356e; /* mov.l 0x8c65359c,r3 */
L8c65356e: *stop=0x8c65356eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65358cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c653570; /* mov.l 0x8c65358c,r1 */
L8c653570: target=r[3];*pr=0x8c653574u;*stop=0x8c653572u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653574; /* jsr @r3 */
L8c653574: *stop=0x8c653576u;goto L8c6535ac; /* bra 0x8c6535ac */
L8c6535a0: *stop=0x8c6535a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653620u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6535a2; /* mov.l 0x8c653620,r4 */
L8c6535a2: *stop=0x8c6535a2u;r[6]=r[11];goto L8c6535a4; /* mov r11,r6 */
L8c6535a4: *stop=0x8c6535a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653624u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6535a6; /* mov.l 0x8c653624,r3 */
L8c6535a6: *stop=0x8c6535a6u;r[5]=r[14];goto L8c6535a8; /* mov r14,r5 */
L8c6535a8: target=r[3];*pr=0x8c6535acu;*stop=0x8c6535aau;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6535ac; /* jsr @r3 */
L8c6535ac: *stop=0x8c6535acu;r[0]=1u;goto L8c6535ae; /* mov #0x1,r0 */
L8c6535ae: *stop=0x8c6535aeu;r[15]+=4u;goto L8c6535b0; /* add #0x4,r15 */
L8c6535b0: *stop=0x8c6535b0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6535b2; /* lds.l @r15+,PR */
L8c6535b2: *stop=0x8c6535b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6535b4; /* mov.l @r15+,r8 */
L8c6535b4: *stop=0x8c6535b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6535b6; /* mov.l @r15+,r9 */
L8c6535b6: *stop=0x8c6535b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6535b8; /* mov.l @r15+,r10 */
L8c6535b8: *stop=0x8c6535b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6535ba; /* mov.l @r15+,r11 */
L8c6535ba: *stop=0x8c6535bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6535bc; /* mov.l @r15+,r12 */
L8c6535bc: *stop=0x8c6535bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6535be; /* mov.l @r15+,r13 */
L8c6535be: *stop=0x8c6535beu;if(*pr!=entry_pr)return -3;*stop=0x8c6535c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c65344e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65344e;
L8c65344e: *stop=0x8c65344eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c653450; /* mov.l r14,@-r15 */
L8c653450: *stop=0x8c653450u;r[14]=r[4];goto L8c653452; /* mov r4,r14 */
L8c653452: *stop=0x8c653452u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c653454; /* mov.l r13,@-r15 */
L8c653454: *stop=0x8c653454u;r[13]=r[4];goto L8c653456; /* mov r4,r13 */
L8c653456: *stop=0x8c653456u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c653458; /* mov.l r12,@-r15 */
L8c653458: *stop=0x8c653458u;r[13]+=r[5];goto L8c65345a; /* add r5,r13 */
L8c65345a: *stop=0x8c65345au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65345c; /* sts.l PR,@-r15 */
L8c65345c: *stop=0x8c65345cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6534c4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c65345e; /* mov.l 0x8c6534c4,r12 */
L8c65345e: *stop=0x8c65345eu;*sr=(*sr&~1u)|(r[14]>r[13]);goto L8c653460; /* cmp/hi r13,r14 */
L8c653460: if((*sr&1u))goto L8c65346c;goto L8c653462; /* bt 0x8c65346c */
L8c653462: target=r[12];*pr=0x8c653466u;*stop=0x8c653464u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653466; /* jsr @r12 */
L8c653466: *stop=0x8c653466u;r[14]+=32u;goto L8c653468; /* add #0x20,r14 */
L8c653468: *stop=0x8c653468u;*sr=(*sr&~1u)|(r[14]>r[13]);goto L8c65346a; /* cmp/hi r13,r14 */
L8c65346a: if(!(*sr&1u))goto L8c653462;goto L8c65346c; /* bf 0x8c653462 */
L8c65346c: *stop=0x8c65346cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65346e; /* lds.l @r15+,PR */
L8c65346e: *stop=0x8c65346eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c653470; /* mov.l @r15+,r12 */
L8c653470: *stop=0x8c653470u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c653472; /* mov.l @r15+,r13 */
L8c653472: *stop=0x8c653472u;if(*pr!=entry_pr)return -3;*stop=0x8c653474u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c651000(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned buffer_wait_visits=0;(void)buffer_wait_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c651000;
L8c651000: *stop=0x8c651000u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c651002; /* sts.l PR,@-r15 */
L8c651002: *stop=0x8c651002u;r[15]+=4294967292u;goto L8c651004; /* add #-0x4,r15 */
L8c651004: *stop=0x8c651004u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c651006; /* mov.l r4,@r15 */
L8c651006: target=0x8c650f78u;*pr=0x8c65100au;*stop=0x8c651008u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65100a; /* bsr 0x8c650f78 */
L8c65100a: *stop=0x8c65100au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65100c; /* cmp/eq #0x1,r0 */
L8c65100c: 
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c65100cu;if(!(*sr&1u)&&++buffer_wait_visits>=2u)return -4;
#endif
 if(!(*sr&1u))goto L8c651006;goto L8c65100e; /* bf 0x8c651006 */
L8c65100e: *stop=0x8c65100eu;r[15]+=4u;goto L8c651010; /* add #0x4,r15 */
L8c651010: *stop=0x8c651010u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c651012; /* lds.l @r15+,PR */
L8c651012: *stop=0x8c651012u;if(*pr!=entry_pr)return -3;*stop=0x8c651014u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c65253e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65253e;
L8c65253e: *stop=0x8c65253eu;r[6]>>=2;goto L8c652540; /* shlr2 r6 */
L8c652540: *stop=0x8c652540u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}r[5]+=4;goto L8c652542; /* mov.l @r5+,r0 */
L8c652542: *stop=0x8c652542u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c652544; /* dt r6 */
L8c652544: *stop=0x8c652544u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c652546; /* mov.l r0,@r4 */
L8c652546: take=!(*sr&1u);*stop=0x8c652548u;r[4]+=4u;if(take)goto L8c652540;goto L8c65254a; /* bf/s 0x8c652540 */
L8c65254a: *stop=0x8c65254au;if(*pr!=entry_pr)return -3;*stop=0x8c65254cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c65254e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65254e;
L8c65254e: *stop=0x8c65254eu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c652550; /* ocbwb @r4 */
L8c652550: *stop=0x8c652550u;if(*pr!=entry_pr)return -3;*stop=0x8c652552u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c650e40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c650e40;
L8c650e40: *stop=0x8c650e40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f30u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650e42; /* mov.l 0x8c650f30,r1 */
L8c650e42: *stop=0x8c650e42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650e44; /* mov.l 0x8c650f2c,r3 */
L8c650e44: *stop=0x8c650e44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c650e46; /* mov.l r14,@-r15 */
L8c650e46: *stop=0x8c650e46u;r[14]=2u;goto L8c650e48; /* mov #0x2,r14 */
L8c650e48: *stop=0x8c650e48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f28u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650e4a; /* mov.l 0x8c650f28,r5 */
L8c650e4a: *stop=0x8c650e4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c650e4c; /* mov.l r13,@-r15 */
L8c650e4c: *stop=0x8c650e4cu;r[13]=1u;goto L8c650e4e; /* mov #0x1,r13 */
L8c650e4e: *stop=0x8c650e4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c650e50; /* mov.l r12,@-r15 */
L8c650e50: *stop=0x8c650e50u;r[12]=3u;goto L8c650e52; /* mov #0x3,r12 */
L8c650e52: *stop=0x8c650e52u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c650e54; /* mov.l r11,@-r15 */
L8c650e54: *stop=0x8c650e54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c650e56; /* mov.l r10,@-r15 */
L8c650e56: *stop=0x8c650e56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c650e58; /* mov.l r9,@-r15 */
L8c650e58: *stop=0x8c650e58u;r[9]=4294967292u;goto L8c650e5a; /* mov #-0x4,r9 */
L8c650e5a: *stop=0x8c650e5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c650e5c; /* mov.l r8,@-r15 */
L8c650e5c: *stop=0x8c650e5cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c650e5e; /* mov.l @r1,r2 */
L8c650e5e: *stop=0x8c650e5eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c650e60; /* mov.l @r3,r4 */
L8c650e60: *stop=0x8c650e60u;r[2]<<=2;goto L8c650e62; /* shll2 r2 */
L8c650e62: *stop=0x8c650e62u;{uint32_t loaded=sx(m,bus,&io_status,0x8c650f20u,2);if(io_status)return io_status;r[8]=loaded;}goto L8c650e64; /* mov.w 0x8c650f20,r8 */
L8c650e64: *stop=0x8c650e64u;r[4]<<=8;goto L8c650e66; /* shll8 r4 */
L8c650e66: *stop=0x8c650e66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f34u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c650e68; /* mov.l 0x8c650f34,r10 */
L8c650e68: *stop=0x8c650e68u;r[2]<<=2;goto L8c650e6a; /* shll2 r2 */
L8c650e6a: *stop=0x8c650e6au;r[4]+=r[5];goto L8c650e6c; /* add r5,r4 */
L8c650e6c: *stop=0x8c650e6cu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c650e6e; /* shll r2 */
L8c650e6e: *stop=0x8c650e6eu;r[4]+=r[2];goto L8c650e70; /* add r2,r4 */
L8c650e70: *stop=0x8c650e70u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650e72; /* mov.l @(0x18,r4),r2 */
L8c650e72: *stop=0x8c650e72u;r[2]&=r[9];goto L8c650e74; /* and r9,r2 */
L8c650e74: *stop=0x8c650e74u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c650e76; /* mov.l r2,@(0x18,r4) */
L8c650e76: *stop=0x8c650e76u;r[4]=0u;goto L8c650e78; /* mov #0x0,r4 */
L8c650e78: *stop=0x8c650e78u;r[6]=r[4];goto L8c650e7a; /* mov r4,r6 */
L8c650e7a: *stop=0x8c650e7au;r[0]=r[4];goto L8c650e7c; /* mov r4,r0 */
L8c650e7c: *stop=0x8c650e7cu;r[0]<<=2;goto L8c650e7e; /* shll2 r0 */
L8c650e7e: *stop=0x8c650e7eu;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c650e80; /* mov.l @(r0,r10),r7 */
L8c650e80: *stop=0x8c650e80u;r[0]=r[4];goto L8c650e82; /* mov r4,r0 */
L8c650e82: *stop=0x8c650e82u;r[0]<<=8;goto L8c650e84; /* shll8 r0 */
L8c650e84: *stop=0x8c650e84u;r[3]=r[7];goto L8c650e86; /* mov r7,r3 */
L8c650e86: *stop=0x8c650e86u;r[3]<<=2;goto L8c650e88; /* shll2 r3 */
L8c650e88: *stop=0x8c650e88u;r[3]<<=2;goto L8c650e8a; /* shll2 r3 */
L8c650e8a: *stop=0x8c650e8au;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c650e8c; /* shll r3 */
L8c650e8c: *stop=0x8c650e8cu;r[0]+=r[5];goto L8c650e8e; /* add r5,r0 */
L8c650e8e: *stop=0x8c650e8eu;r[0]+=r[3];goto L8c650e90; /* add r3,r0 */
L8c650e90: *stop=0x8c650e90u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650e92; /* mov.l @(0x18,r0),r0 */
L8c650e92: *stop=0x8c650e92u;r[0]&=r[12];goto L8c650e94; /* and r12,r0 */
L8c650e94: *stop=0x8c650e94u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c650e96; /* cmp/eq #0x1,r0 */
L8c650e96: if(!(*sr&1u))goto L8c650f60;goto L8c650e98; /* bf 0x8c650f60 */
L8c650e98: *stop=0x8c650e98u;r[11]=r[7];goto L8c650e9a; /* mov r7,r11 */
L8c650e9a: *stop=0x8c650e9au;r[12]=r[4];goto L8c650e9c; /* mov r4,r12 */
L8c650e9c: *stop=0x8c650e9cu;r[11]<<=2;goto L8c650e9e; /* shll2 r11 */
L8c650e9e: *stop=0x8c650e9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f38u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650ea0; /* mov.l 0x8c650f38,r1 */
L8c650ea0: *stop=0x8c650ea0u;r[12]<<=8;goto L8c650ea2; /* shll8 r12 */
L8c650ea2: *stop=0x8c650ea2u;r[6]=r[12];goto L8c650ea4; /* mov r12,r6 */
L8c650ea4: *stop=0x8c650ea4u;r[11]<<=2;goto L8c650ea6; /* shll2 r11 */
L8c650ea6: *stop=0x8c650ea6u;*sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c650ea8; /* shll r11 */
L8c650ea8: *stop=0x8c650ea8u;r[6]+=r[5];goto L8c650eaa; /* add r5,r6 */
L8c650eaa: *stop=0x8c650eaau;r[6]+=r[11];goto L8c650eac; /* add r11,r6 */
L8c650eac: *stop=0x8c650eacu;if(!valid_access(r[6]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650eae; /* mov.l @(0x18,r6),r3 */
L8c650eae: *stop=0x8c650eaeu;r[3]&=r[9];goto L8c650eb0; /* and r9,r3 */
L8c650eb0: *stop=0x8c650eb0u;r[3]|=r[14];goto L8c650eb2; /* or r14,r3 */
L8c650eb2: *stop=0x8c650eb2u;if(!valid_access(r[6]+24u,4u))return -3;wr(m,bus,&io_status,r[6]+0x18u,r[3],4);if(io_status)return io_status;goto L8c650eb4; /* mov.l r3,@(0x18,r6) */
L8c650eb4: *stop=0x8c650eb4u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650eb6; /* mov.l @(0x4,r6),r2 */
L8c650eb6: *stop=0x8c650eb6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c650eb8; /* mov.l r2,@r1 */
L8c650eb8: *stop=0x8c650eb8u;if(!valid_access(r[6]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650eba; /* mov.l @(0xc,r6),r0 */
L8c650eba: *stop=0x8c650ebau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650ebc; /* mov.l 0x8c650f3c,r3 */
L8c650ebc: *stop=0x8c650ebcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650ebe; /* mov.l 0x8c650f40,r2 */
L8c650ebe: *stop=0x8c650ebeu;r[0]+=r[3];goto L8c650ec0; /* add r3,r0 */
L8c650ec0: *stop=0x8c650ec0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c650ec2; /* shll r0 */
L8c650ec2: *stop=0x8c650ec2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c650ec4; /* mov.l r0,@r2 */
L8c650ec4: *stop=0x8c650ec4u;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650ec6; /* mov.l @(0x14,r6),r1 */
L8c650ec6: *stop=0x8c650ec6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f44u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650ec8; /* mov.l 0x8c650f44,r0 */
L8c650ec8: *stop=0x8c650ec8u;r[1]>>=2;goto L8c650eca; /* shlr2 r1 */
L8c650eca: *stop=0x8c650ecau;r[1]>>=2;goto L8c650ecc; /* shlr2 r1 */
L8c650ecc: *stop=0x8c650eccu;*sr=(*sr&~1u)|(r[1]&1u);r[1]>>=1;goto L8c650ece; /* shlr r1 */
L8c650ece: *stop=0x8c650eceu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c650ed0; /* mov.l r1,@r0 */
L8c650ed0: *stop=0x8c650ed0u;if(!valid_access(r[6]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650ed2; /* mov.l @(0x18,r6),r0 */
L8c650ed2: *stop=0x8c650ed2u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c650ed4; /* tst #0x4,r0 */
L8c650ed4: take=!(*sr&1u);*stop=0x8c650ed6u;r[14]=r[8];if(take)goto L8c650eda;goto L8c650ed8; /* bf/s 0x8c650eda */
L8c650ed8: *stop=0x8c650ed8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c650f22u,2);if(io_status)return io_status;r[14]=loaded;}goto L8c650eda; /* mov.w 0x8c650f22,r14 */
L8c650eda: *stop=0x8c650edau;r[0]=r[12];goto L8c650edc; /* mov r12,r0 */
L8c650edc: *stop=0x8c650edcu;r[0]+=r[5];goto L8c650ede; /* add r5,r0 */
L8c650ede: *stop=0x8c650edeu;r[0]+=r[11];goto L8c650ee0; /* add r11,r0 */
L8c650ee0: *stop=0x8c650ee0u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650ee2; /* mov.l @(0x18,r0),r0 */
L8c650ee2: *stop=0x8c650ee2u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c650ee4; /* tst #0x10,r0 */
L8c650ee4: if((*sr&1u))goto L8c650eea;goto L8c650ee6; /* bt 0x8c650eea */
L8c650ee6: *stop=0x8c650ee6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c650f24u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c650ee8; /* mov.w 0x8c650f24,r2 */
L8c650ee8: *stop=0x8c650ee8u;r[14]|=r[2];goto L8c650eea; /* or r2,r14 */
L8c650eea: *stop=0x8c650eeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650eec; /* mov.l 0x8c650f48,r1 */
L8c650eec: *stop=0x8c650eecu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c650eee; /* mov.l r14,@r1 */
L8c650eee: *stop=0x8c650eeeu;r[6]=r[12];goto L8c650ef0; /* mov r12,r6 */
L8c650ef0: *stop=0x8c650ef0u;r[6]+=r[5];goto L8c650ef2; /* add r5,r6 */
L8c650ef2: *stop=0x8c650ef2u;r[6]+=r[11];goto L8c650ef4; /* add r11,r6 */
L8c650ef4: *stop=0x8c650ef4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650ef6; /* mov.l 0x8c650f4c,r2 */
L8c650ef6: *stop=0x8c650ef6u;if(!valid_access(r[6]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650ef8; /* mov.l @(0xc,r6),r3 */
L8c650ef8: *stop=0x8c650ef8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c650efa; /* mov.l r3,@r2 */
L8c650efa: *stop=0x8c650efau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650efc; /* mov.l 0x8c650f50,r3 */
L8c650efc: *stop=0x8c650efcu;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650efe; /* mov.l @(0x14,r6),r0 */
L8c650efe: *stop=0x8c650efeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c650f00; /* mov.l r0,@r3 */
L8c650f00: *stop=0x8c650f00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f54u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650f02; /* mov.l 0x8c650f54,r1 */
L8c650f02: *stop=0x8c650f02u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c650f04; /* mov.l r13,@r1 */
L8c650f04: *stop=0x8c650f04u;r[5]=r[4];goto L8c650f06; /* mov r4,r5 */
L8c650f06: *stop=0x8c650f06u;r[5]<<=2;goto L8c650f08; /* shll2 r5 */
L8c650f08: *stop=0x8c650f08u;r[5]+=r[10];goto L8c650f0a; /* add r10,r5 */
L8c650f0a: *stop=0x8c650f0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650f0c; /* mov.l 0x8c650f2c,r3 */
L8c650f0c: *stop=0x8c650f0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650f30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650f0e; /* mov.l 0x8c650f30,r2 */
L8c650f0e: *stop=0x8c650f0eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[7],4);if(io_status)return io_status;goto L8c650f10; /* mov.l r7,@r2 */
L8c650f10: *stop=0x8c650f10u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c650f12; /* mov.l r4,@r3 */
L8c650f12: *stop=0x8c650f12u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}goto L8c650f14; /* mov.l @r5,r0 */
L8c650f14: *stop=0x8c650f14u;r[0]+=1u;goto L8c650f16; /* add #0x1,r0 */
L8c650f16: *stop=0x8c650f16u;r[0]&=0x7u;goto L8c650f18; /* and #0x7,r0 */
L8c650f18: *stop=0x8c650f18u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c650f1a; /* mov.l r0,@r5 */
L8c650f1a: *stop=0x8c650f1cu;r[6]=r[13];goto L8c650f66; /* bra 0x8c650f66 */
L8c650f60: *stop=0x8c650f60u;r[4]+=1u;goto L8c650f62; /* add #0x1,r4 */
L8c650f62: *stop=0x8c650f62u;*sr=(*sr&~1u)|(r[4]>=r[14]);goto L8c650f64; /* cmp/hs r14,r4 */
L8c650f64: if(!(*sr&1u))goto L8c650e7a;goto L8c650f66; /* bf 0x8c650e7a */
L8c650f66: *stop=0x8c650f66u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c650f68; /* mov.l @r15+,r8 */
L8c650f68: *stop=0x8c650f68u;r[0]=r[6];goto L8c650f6a; /* mov r6,r0 */
L8c650f6a: *stop=0x8c650f6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c650f6c; /* mov.l @r15+,r9 */
L8c650f6c: *stop=0x8c650f6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c650f6e; /* mov.l @r15+,r10 */
L8c650f6e: *stop=0x8c650f6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c650f70; /* mov.l @r15+,r11 */
L8c650f70: *stop=0x8c650f70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c650f72; /* mov.l @r15+,r12 */
L8c650f72: *stop=0x8c650f72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c650f74; /* mov.l @r15+,r13 */
L8c650f74: *stop=0x8c650f74u;if(*pr!=entry_pr)return -3;*stop=0x8c650f76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_buffer_initialize_8c652210(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c652210;
L8c652210: *stop=0x8c652210u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c652212; /* mov.l r14,@-r15 */
L8c652212: *stop=0x8c652212u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c652214; /* mov.l r13,@-r15 */
L8c652214: *stop=0x8c652214u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c652216; /* mov.l r12,@-r15 */
L8c652216: *stop=0x8c652216u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c652218; /* mov.l r11,@-r15 */
L8c652218: *stop=0x8c652218u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65221a; /* mov.l r10,@-r15 */
L8c65221a: *stop=0x8c65221au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65221c; /* mov.l r9,@-r15 */
L8c65221c: *stop=0x8c65221cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65221e; /* mov.l r8,@-r15 */
L8c65221e: *stop=0x8c65221eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652220; /* sts.l PR,@-r15 */
L8c652220: *stop=0x8c652220u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522dcu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c652222; /* mov.l 0x8c6522dc,r12 */
L8c652222: *stop=0x8c652222u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[13]=loaded;}goto L8c652224; /* mov.l @r12,r13 */
L8c652224: *stop=0x8c652224u;r[3]=r[13];goto L8c652226; /* mov r13,r3 */
L8c652226: *stop=0x8c652226u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522c0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c652228; /* mov.l 0x8c6522c0,r14 */
L8c652228: *stop=0x8c652228u;r[8]=8u;goto L8c65222a; /* mov #0x8,r8 */
L8c65222a: *stop=0x8c65222au;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c65222c; /* tst r8,r3 */
L8c65222c: if((*sr&1u))goto L8c65223c;goto L8c65222e; /* bt 0x8c65223c */
L8c65222e: target=0x8c651bc0u;*pr=0x8c652232u;*stop=0x8c652230u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652232; /* bsr 0x8c651bc0 */
L8c652232: *stop=0x8c652232u;r[4]=r[0];goto L8c652234; /* mov r0,r4 */
L8c652234: *stop=0x8c652234u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c652236; /* tst r4,r4 */
L8c652236: if((*sr&1u))goto L8c65223c;goto L8c652238; /* bt 0x8c65223c */
L8c652238: target=r[14];*pr=0x8c65223cu;*stop=0x8c65223au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65223c; /* jsr @r14 */
L8c65223c: *stop=0x8c65223cu;r[2]=r[13];goto L8c65223e; /* mov r13,r2 */
L8c65223e: *stop=0x8c65223eu;r[4]=32u;goto L8c652240; /* mov #0x20,r4 */
L8c652240: *stop=0x8c652240u;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c652242; /* tst r4,r2 */
L8c652242: take=(*sr&1u);*stop=0x8c652244u;r[9]=4u;if(take)goto L8c652250;goto L8c652246; /* bt/s 0x8c652250 */
L8c652246: *stop=0x8c652246u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c652248; /* mov.l r4,@r12 */
L8c652248: *stop=0x8c652248u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65224a; /* mov.l 0x8c6522d8,r3 */
L8c65224a: *stop=0x8c65224au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65224c; /* mov.l @r3,r2 */
L8c65224c: *stop=0x8c65224cu;r[2]|=r[9];goto L8c65224e; /* or r9,r2 */
L8c65224e: *stop=0x8c65224eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c652250; /* mov.l r2,@r3 */
L8c652250: *stop=0x8c652250u;r[3]=r[13];goto L8c652252; /* mov r13,r3 */
L8c652252: *stop=0x8c652252u;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c652254; /* tst r9,r3 */
L8c652254: *stop=0x8c652254u;r[10]=0u;goto L8c652256; /* mov #0x0,r10 */
L8c652256: take=(*sr&1u);*stop=0x8c652258u;r[11]=1u;if(take)goto L8c65233a;goto L8c65225a; /* bt/s 0x8c65233a */
L8c65225a: *stop=0x8c65225au;r[1]=7u;goto L8c65225c; /* mov #0x7,r1 */
L8c65225c: *stop=0x8c65225cu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[1],4);if(io_status)return io_status;goto L8c65225e; /* mov.l r1,@r12 */
L8c65225e: *stop=0x8c65225eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652260; /* mov.l 0x8c6522e0,r3 */
L8c652260: *stop=0x8c652260u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c652262; /* mov.l r10,@r3 */
L8c652262: *stop=0x8c652262u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522e4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c652264; /* mov.l 0x8c6522e4,r0 */
L8c652264: *stop=0x8c652264u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522c8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c652266; /* mov.l 0x8c6522c8,r4 */
L8c652266: *stop=0x8c652266u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[10],4);if(io_status)return io_status;goto L8c652268; /* mov.l r10,@r4 */
L8c652268: *stop=0x8c652268u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65226a; /* mov.l @r0,r2 */
L8c65226a: *stop=0x8c65226au;r[0]=64u;goto L8c65226c; /* mov #0x40,r0 */
L8c65226c: *stop=0x8c65226cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65226e; /* mov.l @(r0,r4),r1 */
L8c65226e: *stop=0x8c65226eu;r[0]=68u;goto L8c652270; /* mov #0x44,r0 */
L8c652270: *stop=0x8c652270u;r[1]-=r[2];goto L8c652272; /* sub r2,r1 */
L8c652272: *stop=0x8c652272u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[1],4);if(io_status)return io_status;goto L8c652274; /* mov.l r1,@(r0,r4) */
L8c652274: *stop=0x8c652274u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522e8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c652276; /* mov.l 0x8c6522e8,r1 */
L8c652276: *stop=0x8c652276u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652278; /* mov.l @r1,r2 */
L8c652278: *stop=0x8c652278u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65227a; /* tst r2,r2 */
L8c65227a: if((*sr&1u))goto L8c652320;goto L8c65227c; /* bt 0x8c652320 */
L8c65227c: *stop=0x8c65227cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65227e; /* mov.l 0x8c6522e8,r3 */
L8c65227e: *stop=0x8c65227eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c652280; /* mov.l r10,@r3 */
L8c652280: *stop=0x8c652280u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522ecu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c652282; /* mov.l 0x8c6522ec,r0 */
L8c652282: *stop=0x8c652282u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[10],4);if(io_status)return io_status;goto L8c652284; /* mov.l r10,@(0x18,r4) */
L8c652284: *stop=0x8c652284u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652286; /* mov.l @r0,r2 */
L8c652286: *stop=0x8c652286u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c652288; /* mov.l 0x8c6522f0,r1 */
L8c652288: *stop=0x8c652288u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c65228a; /* mov.l r2,@r1 */
L8c65228a: *stop=0x8c65228au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65228c; /* mov.l 0x8c6522f4,r2 */
L8c65228c: *stop=0x8c65228cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65228e; /* mov.l @r2,r3 */
L8c65228e: *stop=0x8c65228eu;r[1]+=4u;goto L8c652290; /* add #0x4,r1 */
L8c652290: *stop=0x8c652290u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c652292; /* mov.l r3,@r1 */
L8c652292: *stop=0x8c652292u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652294; /* mov.l 0x8c6522f8,r3 */
L8c652294: *stop=0x8c652294u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c652296; /* mov.l @r3,r0 */
L8c652296: *stop=0x8c652296u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6522fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652298; /* mov.l 0x8c6522fc,r2 */
L8c652298: *stop=0x8c652298u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c65229a; /* mov.l r0,@r2 */
L8c65229a: *stop=0x8c65229au;{uint32_t loaded=rd(m,bus,&io_status,0x8c652300u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65229c; /* mov.l 0x8c652300,r0 */
L8c65229c: *stop=0x8c65229cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65229e; /* mov.l @r0,r1 */
L8c65229e: *stop=0x8c65229eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652304u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6522a0; /* mov.l 0x8c652304,r3 */
L8c6522a0: *stop=0x8c6522a2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65232e; /* bra 0x8c65232e */
L8c652320: *stop=0x8c652320u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652428u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c652322; /* mov.l 0x8c652428,r0 */
L8c652322: *stop=0x8c652322u;r[1]=5u;goto L8c652324; /* mov #0x5,r1 */
L8c652324: *stop=0x8c652324u;r[2]=r[0];goto L8c652326; /* mov r0,r2 */
L8c652326: *stop=0x8c652326u;r[2]+=20u;goto L8c652328; /* add #0x14,r2 */
L8c652328: *stop=0x8c652328u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65232a; /* mov.l @r2,r3 */
L8c65232a: *stop=0x8c65232au;r[3]<<=2;goto L8c65232c; /* shll2 r3 */
L8c65232c: *stop=0x8c65232cu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c65232e; /* mov.l r1,@(r0,r3) */
L8c65232e: *stop=0x8c65232eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65242cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652330; /* mov.l 0x8c65242c,r3 */
L8c652330: *stop=0x8c652330u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652332; /* mov.l @r3,r2 */
L8c652332: *stop=0x8c652332u;r[2]|=r[11];goto L8c652334; /* or r11,r2 */
L8c652334: *stop=0x8c652334u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c652336; /* mov.l r2,@r3 */
L8c652336: *stop=0x8c652336u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652430u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652338; /* mov.l 0x8c652430,r3 */
L8c652338: *stop=0x8c652338u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c65233a; /* mov.l r10,@r3 */
L8c65233a: *stop=0x8c65233au;{uint32_t loaded=rd(m,bus,&io_status,0x8c652434u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65233c; /* mov.l 0x8c652434,r5 */
L8c65233c: *stop=0x8c65233cu;r[3]=r[13];goto L8c65233e; /* mov r13,r3 */
L8c65233e: *stop=0x8c65233eu;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c652340; /* tst r5,r3 */
L8c652340: if((*sr&1u))goto L8c652352;goto L8c652342; /* bt 0x8c652352 */
L8c652342: *stop=0x8c652342u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652438u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c652344; /* mov.l 0x8c652438,r6 */
L8c652344: *stop=0x8c652344u;r[3]=4294967292u;goto L8c652346; /* mov #-0x4,r3 */
L8c652346: *stop=0x8c652346u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[4]=loaded;}goto L8c652348; /* mov.l @r6,r4 */
L8c652348: *stop=0x8c652348u;r[4]&=r[3];goto L8c65234a; /* and r3,r4 */
L8c65234a: *stop=0x8c65234au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c65234c; /* mov.l r4,@r6 */
L8c65234c: *stop=0x8c65234cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65243cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65234e; /* mov.l 0x8c65243c,r2 */
L8c65234e: target=r[2];*pr=0x8c652352u;*stop=0x8c652350u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[5],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652352; /* jsr @r2 */
L8c652352: *stop=0x8c652352u;r[3]=r[13];goto L8c652354; /* mov r13,r3 */
L8c652354: *stop=0x8c652354u;r[4]=64u;goto L8c652356; /* mov #0x40,r4 */
L8c652356: *stop=0x8c652356u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c652358; /* tst r4,r3 */
L8c652358: if((*sr&1u))goto L8c652364;goto L8c65235a; /* bt 0x8c652364 */
L8c65235a: *stop=0x8c65235au;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c65235c; /* mov.l r4,@r12 */
L8c65235c: *stop=0x8c65235cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65242cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65235e; /* mov.l 0x8c65242c,r3 */
L8c65235e: *stop=0x8c65235eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652360; /* mov.l @r3,r2 */
L8c652360: *stop=0x8c652360u;r[2]|=r[4];goto L8c652362; /* or r4,r2 */
L8c652362: *stop=0x8c652362u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c652364; /* mov.l r2,@r3 */
L8c652364: *stop=0x8c652364u;{uint32_t loaded=sx(m,bus,&io_status,0x8c652420u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c652366; /* mov.w 0x8c652420,r4 */
L8c652366: *stop=0x8c652366u;r[3]=r[13];goto L8c652368; /* mov r13,r3 */
L8c652368: *stop=0x8c652368u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c65236a; /* tst r4,r3 */
L8c65236a: if((*sr&1u))goto L8c652386;goto L8c65236c; /* bt 0x8c652386 */
L8c65236c: *stop=0x8c65236cu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c65236e; /* mov.l r4,@r12 */
L8c65236e: *stop=0x8c65236eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652370; /* mov.l 0x8c652440,r3 */
L8c652370: *stop=0x8c652370u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652372; /* mov.l @r3,r2 */
L8c652372: *stop=0x8c652372u;r[2]|=r[11];goto L8c652374; /* or r11,r2 */
L8c652374: *stop=0x8c652374u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c652376; /* mov.l r2,@r3 */
L8c652376: *stop=0x8c652376u;r[0]=r[3];goto L8c652378; /* mov r3,r0 */
L8c652378: *stop=0x8c652378u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65237a; /* mov.l @r0,r3 */
L8c65237a: *stop=0x8c65237au;{uint32_t loaded=rd(m,bus,&io_status,0x8c652444u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65237c; /* mov.l 0x8c652444,r2 */
L8c65237c: *stop=0x8c65237cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65237e; /* mov.l @r2,r1 */
L8c65237e: *stop=0x8c65237eu;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c652380; /* cmp/eq r3,r1 */
L8c652380: if(!(*sr&1u))goto L8c652386;goto L8c652382; /* bf 0x8c652386 */
L8c652382: target=0x8c651f1eu;*pr=0x8c652386u;*stop=0x8c652384u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652386; /* bsr 0x8c651f1e */
L8c652386: *stop=0x8c652386u;{uint32_t loaded=sx(m,bus,&io_status,0x8c652422u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c652388; /* mov.w 0x8c652422,r4 */
L8c652388: *stop=0x8c652388u;r[3]=r[13];goto L8c65238a; /* mov r13,r3 */
L8c65238a: *stop=0x8c65238au;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c65238c; /* tst r4,r3 */
L8c65238c: if((*sr&1u))goto L8c6523a8;goto L8c65238e; /* bt 0x8c6523a8 */
L8c65238e: *stop=0x8c65238eu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c652390; /* mov.l r4,@r12 */
L8c652390: *stop=0x8c652390u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652392; /* mov.l 0x8c652440,r3 */
L8c652392: *stop=0x8c652392u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c652394; /* mov.l @r3,r0 */
L8c652394: *stop=0x8c652394u;r[0]|=0x2u;goto L8c652396; /* or #0x2,r0 */
L8c652396: *stop=0x8c652396u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c652398; /* mov.l r0,@r3 */
L8c652398: *stop=0x8c652398u;r[1]=r[3];goto L8c65239a; /* mov r3,r1 */
L8c65239a: *stop=0x8c65239au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65239c; /* mov.l @r1,r2 */
L8c65239c: *stop=0x8c65239cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652444u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65239e; /* mov.l 0x8c652444,r0 */
L8c65239e: *stop=0x8c65239eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6523a0; /* mov.l @r0,r3 */
L8c6523a0: *stop=0x8c6523a0u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c6523a2; /* cmp/eq r2,r3 */
L8c6523a2: if(!(*sr&1u))goto L8c6523a8;goto L8c6523a4; /* bf 0x8c6523a8 */
L8c6523a4: target=0x8c651f1eu;*pr=0x8c6523a8u;*stop=0x8c6523a6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6523a8; /* bsr 0x8c651f1e */
L8c6523a8: *stop=0x8c6523a8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c652424u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6523aa; /* mov.w 0x8c652424,r4 */
L8c6523aa: *stop=0x8c6523aau;r[3]=r[13];goto L8c6523ac; /* mov r13,r3 */
L8c6523ac: *stop=0x8c6523acu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6523ae; /* tst r4,r3 */
L8c6523ae: if((*sr&1u))goto L8c6523ca;goto L8c6523b0; /* bt 0x8c6523ca */
L8c6523b0: *stop=0x8c6523b0u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c6523b2; /* mov.l r4,@r12 */
L8c6523b2: *stop=0x8c6523b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6523b4; /* mov.l 0x8c652440,r3 */
L8c6523b4: *stop=0x8c6523b4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6523b6; /* mov.l @r3,r2 */
L8c6523b6: *stop=0x8c6523b6u;r[2]|=r[9];goto L8c6523b8; /* or r9,r2 */
L8c6523b8: *stop=0x8c6523b8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6523ba; /* mov.l r2,@r3 */
L8c6523ba: *stop=0x8c6523bau;r[0]=r[3];goto L8c6523bc; /* mov r3,r0 */
L8c6523bc: *stop=0x8c6523bcu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6523be; /* mov.l @r0,r3 */
L8c6523be: *stop=0x8c6523beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652444u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6523c0; /* mov.l 0x8c652444,r2 */
L8c6523c0: *stop=0x8c6523c0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6523c2; /* mov.l @r2,r1 */
L8c6523c2: *stop=0x8c6523c2u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c6523c4; /* cmp/eq r3,r1 */
L8c6523c4: if(!(*sr&1u))goto L8c6523ca;goto L8c6523c6; /* bf 0x8c6523ca */
L8c6523c6: target=0x8c651f1eu;*pr=0x8c6523cau;*stop=0x8c6523c8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6523ca; /* bsr 0x8c651f1e */
L8c6523ca: *stop=0x8c6523cau;{uint32_t loaded=sx(m,bus,&io_status,0x8c652426u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6523cc; /* mov.w 0x8c652426,r4 */
L8c6523cc: *stop=0x8c6523ccu;r[3]=r[13];goto L8c6523ce; /* mov r13,r3 */
L8c6523ce: *stop=0x8c6523ceu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6523d0; /* tst r4,r3 */
L8c6523d0: if((*sr&1u))goto L8c6523ec;goto L8c6523d2; /* bt 0x8c6523ec */
L8c6523d2: *stop=0x8c6523d2u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c6523d4; /* mov.l r4,@r12 */
L8c6523d4: *stop=0x8c6523d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6523d6; /* mov.l 0x8c652440,r3 */
L8c6523d6: *stop=0x8c6523d6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6523d8; /* mov.l @r3,r2 */
L8c6523d8: *stop=0x8c6523d8u;r[2]|=r[8];goto L8c6523da; /* or r8,r2 */
L8c6523da: *stop=0x8c6523dau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6523dc; /* mov.l r2,@r3 */
L8c6523dc: *stop=0x8c6523dcu;r[0]=r[3];goto L8c6523de; /* mov r3,r0 */
L8c6523de: *stop=0x8c6523deu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6523e0; /* mov.l @r0,r3 */
L8c6523e0: *stop=0x8c6523e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652444u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6523e2; /* mov.l 0x8c652444,r2 */
L8c6523e2: *stop=0x8c6523e2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6523e4; /* mov.l @r2,r1 */
L8c6523e4: *stop=0x8c6523e4u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c6523e6; /* cmp/eq r3,r1 */
L8c6523e6: if(!(*sr&1u))goto L8c6523ec;goto L8c6523e8; /* bf 0x8c6523ec */
L8c6523e8: target=0x8c651f1eu;*pr=0x8c6523ecu;*stop=0x8c6523eau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6523ec; /* bsr 0x8c651f1e */
L8c6523ec: *stop=0x8c6523ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652448u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6523ee; /* mov.l 0x8c652448,r4 */
L8c6523ee: *stop=0x8c6523eeu;*sr=(*sr&~1u)|!(r[4]&r[13]);goto L8c6523f0; /* tst r4,r13 */
L8c6523f0: if((*sr&1u))goto L8c65240c;goto L8c6523f2; /* bt 0x8c65240c */
L8c6523f2: *stop=0x8c6523f2u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[4],4);if(io_status)return io_status;goto L8c6523f4; /* mov.l r4,@r12 */
L8c6523f4: *stop=0x8c6523f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6523f6; /* mov.l 0x8c652440,r3 */
L8c6523f6: *stop=0x8c6523f6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6523f8; /* mov.l @r3,r0 */
L8c6523f8: *stop=0x8c6523f8u;r[0]|=0x10u;goto L8c6523fa; /* or #0x10,r0 */
L8c6523fa: *stop=0x8c6523fau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c6523fc; /* mov.l r0,@r3 */
L8c6523fc: *stop=0x8c6523fcu;r[1]=r[3];goto L8c6523fe; /* mov r3,r1 */
L8c6523fe: *stop=0x8c6523feu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652400; /* mov.l @r1,r2 */
L8c652400: *stop=0x8c652400u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652444u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c652402; /* mov.l 0x8c652444,r0 */
L8c652402: *stop=0x8c652402u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c652404; /* mov.l @r0,r3 */
L8c652404: *stop=0x8c652404u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c652406; /* cmp/eq r2,r3 */
L8c652406: if(!(*sr&1u))goto L8c65240c;goto L8c652408; /* bf 0x8c65240c */
L8c652408: target=0x8c651f1eu;*pr=0x8c65240cu;*stop=0x8c65240au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65240c; /* bsr 0x8c651f1e */
L8c65240c: *stop=0x8c65240cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65244cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65240e; /* mov.l 0x8c65244c,r3 */
L8c65240e: *stop=0x8c65240eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[13]=loaded;}goto L8c652410; /* mov.l @r3,r13 */
L8c652410: *stop=0x8c652410u;*sr=(*sr&~1u)|!(r[13]&r[9]);goto L8c652412; /* tst r13,r9 */
L8c652412: if((*sr&1u))goto L8c652466;goto L8c652414; /* bt 0x8c652466 */
L8c652414: target=0x8c652196u;*pr=0x8c652418u;*stop=0x8c652416u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652418; /* bsr 0x8c652196 */
L8c652418: *stop=0x8c652418u;r[4]=r[0];goto L8c65241a; /* mov r0,r4 */
L8c65241a: *stop=0x8c65241au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65241c; /* tst r4,r4 */
L8c65241c: *stop=0x8c65241eu;goto L8c652460; /* bra 0x8c652460 */
L8c652460: if((*sr&1u))goto L8c652466;goto L8c652462; /* bt 0x8c652466 */
L8c652462: target=r[14];*pr=0x8c652466u;*stop=0x8c652464u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652466; /* jsr @r14 */
L8c652466: *stop=0x8c652466u;*sr=(*sr&~1u)|!(r[13]&r[8]);goto L8c652468; /* tst r13,r8 */
L8c652468: if((*sr&1u))goto L8c652478;goto L8c65246a; /* bt 0x8c652478 */
L8c65246a: target=0x8c6521cau;*pr=0x8c65246eu;*stop=0x8c65246cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65246e; /* bsr 0x8c6521ca */
L8c65246e: *stop=0x8c65246eu;r[4]=r[0];goto L8c652470; /* mov r0,r4 */
L8c652470: *stop=0x8c652470u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c652472; /* tst r4,r4 */
L8c652472: if((*sr&1u))goto L8c652478;goto L8c652474; /* bt 0x8c652478 */
L8c652474: target=r[14];*pr=0x8c652478u;*stop=0x8c652476u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652478; /* jsr @r14 */
L8c652478: *stop=0x8c652478u;r[3]=2u;goto L8c65247a; /* mov #0x2,r3 */
L8c65247a: *stop=0x8c65247au;*sr=(*sr&~1u)|!(r[3]&r[13]);goto L8c65247c; /* tst r3,r13 */
L8c65247c: if((*sr&1u))goto L8c65248c;goto L8c65247e; /* bt 0x8c65248c */
L8c65247e: target=0x8c6521f2u;*pr=0x8c652482u;*stop=0x8c652480u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652482; /* bsr 0x8c6521f2 */
L8c652482: *stop=0x8c652482u;r[4]=r[0];goto L8c652484; /* mov r0,r4 */
L8c652484: *stop=0x8c652484u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c652486; /* tst r4,r4 */
L8c652486: if((*sr&1u))goto L8c65248c;goto L8c652488; /* bt 0x8c65248c */
L8c652488: target=r[14];*pr=0x8c65248cu;*stop=0x8c65248au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65248c; /* jsr @r14 */
L8c65248c: *stop=0x8c65248cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652500u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c65248e; /* mov.l 0x8c652500,r9 */
L8c65248e: *stop=0x8c65248eu;r[12]=9u;goto L8c652490; /* mov #0x9,r12 */
L8c652490: *stop=0x8c652492u;r[14]=r[10];goto L8c6524ba; /* bra 0x8c6524ba */
L8c652494: *stop=0x8c652494u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652504u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652496; /* mov.l 0x8c652504,r2 */
L8c652496: *stop=0x8c652496u;r[4]=r[11];goto L8c652498; /* mov r11,r4 */
L8c652498: *stop=0x8c652498u;{uint32_t shift=r[14];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}goto L8c65249a; /* shad r14,r4 */
L8c65249a: *stop=0x8c65249au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65249c; /* mov.l @r2,r3 */
L8c65249c: *stop=0x8c65249cu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c65249e; /* tst r4,r3 */
L8c65249e: if((*sr&1u))goto L8c6524b8;goto L8c6524a0; /* bt 0x8c6524b8 */
L8c6524a0: *stop=0x8c6524a0u;r[4]=~r[4];goto L8c6524a2; /* not r4,r4 */
L8c6524a2: *stop=0x8c6524a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6524a4; /* mov.l @r2,r3 */
L8c6524a4: *stop=0x8c6524a4u;r[3]&=r[4];goto L8c6524a6; /* and r4,r3 */
L8c6524a6: *stop=0x8c6524a6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6524a8; /* mov.l r3,@r2 */
L8c6524a8: *stop=0x8c6524a8u;r[13]=r[14];goto L8c6524aa; /* mov r14,r13 */
L8c6524aa: *stop=0x8c6524aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c652508u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6524ac; /* mov.l 0x8c652508,r0 */
L8c6524ac: *stop=0x8c6524acu;r[13]<<=2;goto L8c6524ae; /* shll2 r13 */
L8c6524ae: *stop=0x8c6524aeu;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6524b0; /* shll r13 */
L8c6524b0: *stop=0x8c6524b0u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6524b2; /* mov.l @(r0,r13),r5 */
L8c6524b2: *stop=0x8c6524b2u;r[0]+=4294967292u;goto L8c6524b4; /* add #-0x4,r0 */
L8c6524b4: target=r[9];*pr=0x8c6524b8u;*stop=0x8c6524b6u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6524b8; /* jsr @r9 */
L8c6524b8: *stop=0x8c6524b8u;r[14]+=1u;goto L8c6524ba; /* add #0x1,r14 */
L8c6524ba: *stop=0x8c6524bau;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]);goto L8c6524bc; /* cmp/ge r12,r14 */
L8c6524bc: if(!(*sr&1u))goto L8c652494;goto L8c6524be; /* bf 0x8c652494 */
L8c6524be: *stop=0x8c6524beu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6524c0; /* lds.l @r15+,PR */
L8c6524c0: *stop=0x8c6524c0u;r[0]=0u;goto L8c6524c2; /* mov #0x0,r0 */
L8c6524c2: *stop=0x8c6524c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6524c4; /* mov.l @r15+,r8 */
L8c6524c4: *stop=0x8c6524c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6524c6; /* mov.l @r15+,r9 */
L8c6524c6: *stop=0x8c6524c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6524c8; /* mov.l @r15+,r10 */
L8c6524c8: *stop=0x8c6524c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6524ca; /* mov.l @r15+,r11 */
L8c6524ca: *stop=0x8c6524cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6524cc; /* mov.l @r15+,r12 */
L8c6524cc: *stop=0x8c6524ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6524ce; /* mov.l @r15+,r13 */
L8c6524ce: *stop=0x8c6524ceu;if(*pr!=entry_pr)return -3;*stop=0x8c6524d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
