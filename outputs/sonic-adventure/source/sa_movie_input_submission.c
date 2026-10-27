/* Input command preparation and queued-to-inflight transport submission.
 * TAS is one uninterrupted guest operation on coherent RAM, with explicit purge.
 * Single guest executor only; host multithreading/cache/TLB hardware unqualified.
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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6c04u || (a&0x1fffffffu)==0x005f6c14u || (a&0x1fffffffu)==0x005f6c18u || (a&0x1fffffffu)==0x005f6c80u))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_input_submission_8c65efda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65efda;
L8c65efda: *stop=0x8c65efdau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65efdc; /* mov.l r14,@-r15 */
L8c65efdc: *stop=0x8c65efdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f024u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65efde; /* mov.l 0x8c65f024,r14 */
L8c65efde: *stop=0x8c65efdeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65efe0; /* mov.l r13,@-r15 */
L8c65efe0: *stop=0x8c65efe0u;r[1]=sx(m,bus,&io_status,0x8c65f00cu,2);if(io_status)return io_status;goto L8c65efe2; /* mov.w 0x8c65f00c,r1 */
L8c65efe2: *stop=0x8c65efe2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65efe4; /* mov.l r12,@-r15 */
L8c65efe4: *stop=0x8c65efe4u;r[12]=1u;goto L8c65efe6; /* mov #0x1,r12 */
L8c65efe6: *stop=0x8c65efe6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65efe8; /* mov.l r8,@-r15 */
L8c65efe8: *stop=0x8c65efe8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65efea; /* sts.l PR,@-r15 */
L8c65efea: *stop=0x8c65efeau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65efec; /* mov.l @r14,r0 */
L8c65efec: *stop=0x8c65efecu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65efee; /* mov.l @(r0,r1),r0 */
L8c65efee: *stop=0x8c65efeeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65eff0; /* cmp/eq #0x1,r0 */
L8c65eff0: take=(*sr&1u);*stop=0x8c65eff2u;r[13]=0u;if(take)goto L8c65eff8;goto L8c65eff4; /* bt/s 0x8c65eff8 */
L8c65eff4: *stop=0x8c65eff6u;r[0]=4294967293u;goto L8c65f19e; /* bra 0x8c65f19e */
L8c65eff8: *stop=0x8c65eff8u;r[0]=sx(m,bus,&io_status,0x8c65f00eu,2);if(io_status)return io_status;goto L8c65effa; /* mov.w 0x8c65f00e,r0 */
L8c65effa: *stop=0x8c65effau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65effc; /* mov.l @r14,r2 */
L8c65effc: *stop=0x8c65effcu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65effe; /* mov.l @(r0,r2),r3 */
L8c65effe: *stop=0x8c65effeu;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f000; /* mov.l @(0x18,r3),r1 */
L8c65f000: *stop=0x8c65f000u;*sr=(*sr&~1u)|!(r[12]&r[1]);goto L8c65f002; /* tst r12,r1 */
L8c65f002: if((*sr&1u))goto L8c65f028;goto L8c65f004; /* bt 0x8c65f028 */
L8c65f004: *stop=0x8c65f006u;r[0]=4294967295u;goto L8c65f19e; /* bra 0x8c65f19e */
L8c65f028: *stop=0x8c65f028u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f154u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f02a; /* mov.l 0x8c65f154,r3 */
L8c65f02a: *stop=0x8c65f02au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f02c; /* mov.l @r3,r0 */
L8c65f02c: *stop=0x8c65f02cu;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c65f02e; /* tas.b @r0 */
L8c65f02e: if((*sr&1u))goto L8c65f03c;goto L8c65f030; /* bt 0x8c65f03c */
L8c65f030: *stop=0x8c65f030u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f158u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f032; /* mov.l 0x8c65f158,r1 */
L8c65f032: target=0x8c65f348u;*pr=0x8c65f036u;*stop=0x8c65f034u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f036; /* bsr 0x8c65f348 */
L8c65f036: *stop=0x8c65f036u;r[15]+=4u;goto L8c65f038; /* add #0x4,r15 */
L8c65f038: *stop=0x8c65f03au;r[0]=4294967294u;goto L8c65f19e; /* bra 0x8c65f19e */
L8c65f03c: *stop=0x8c65f03cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f03e; /* mov.l @r14,r3 */
L8c65f03e: *stop=0x8c65f03eu;r[0]=sx(m,bus,&io_status,0x8c65f13au,2);if(io_status)return io_status;goto L8c65f040; /* mov.w 0x8c65f13a,r0 */
L8c65f040: *stop=0x8c65f040u;r[2]=r[3];goto L8c65f042; /* mov r3,r2 */
L8c65f042: *stop=0x8c65f042u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f044; /* mov.l @(r0,r2),r1 */
L8c65f044: *stop=0x8c65f044u;r[0]+=4u;goto L8c65f046; /* add #0x4,r0 */
L8c65f046: *stop=0x8c65f046u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f048; /* mov.l @(r0,r3),r3 */
L8c65f048: *stop=0x8c65f048u;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c65f04a; /* cmp/eq r1,r3 */
L8c65f04a: if((*sr&1u))goto L8c65f0aa;goto L8c65f04c; /* bt 0x8c65f0aa */
L8c65f04c: *stop=0x8c65f04cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f04e; /* mov.l @r14,r3 */
L8c65f04e: *stop=0x8c65f04eu;r[0]=sx(m,bus,&io_status,0x8c65f13au,2);if(io_status)return io_status;goto L8c65f050; /* mov.w 0x8c65f13a,r0 */
L8c65f050: *stop=0x8c65f050u;r[2]=r[3];goto L8c65f052; /* mov r3,r2 */
L8c65f052: *stop=0x8c65f052u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f054; /* mov.l @(r0,r2),r1 */
L8c65f054: *stop=0x8c65f054u;r[0]+=4u;goto L8c65f056; /* add #0x4,r0 */
L8c65f056: *stop=0x8c65f056u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c65f058; /* mov.l r1,@(r0,r3) */
L8c65f058: *stop=0x8c65f058u;r[0]+=4294967292u;goto L8c65f05a; /* add #-0x4,r0 */
L8c65f05a: *stop=0x8c65f05au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f05c; /* mov.l @r14,r3 */
L8c65f05c: *stop=0x8c65f05cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f05e; /* mov.l @(r0,r3),r2 */
L8c65f05e: *stop=0x8c65f05eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65f060; /* tst r2,r2 */
L8c65f060: if((*sr&1u))goto L8c65f07e;goto L8c65f062; /* bt 0x8c65f07e */
L8c65f062: *stop=0x8c65f062u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f15cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f064; /* mov.l 0x8c65f15c,r1 */
L8c65f064: *stop=0x8c65f064u;r[3]=sx(m,bus,&io_status,0x8c65f13cu,2);if(io_status)return io_status;goto L8c65f066; /* mov.w 0x8c65f13c,r3 */
L8c65f066: *stop=0x8c65f066u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f068; /* mov.l @r1,r0 */
L8c65f068: *stop=0x8c65f068u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f06a; /* mov.l @r14,r2 */
L8c65f06a: *stop=0x8c65f06au;*macl=r[3]*r[0];goto L8c65f06c; /* mul.l r3,r0 */
L8c65f06c: *stop=0x8c65f06cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f160u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f06e; /* mov.l 0x8c65f160,r3 */
L8c65f06e: *stop=0x8c65f06eu;r[1]=*macl;goto L8c65f070; /* sts MACL,r1 */
L8c65f070: target=r[3];*pr=0x8c65f074u;*stop=0x8c65f072u;r[0]=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f074; /* jsr @r3 */
L8c65f074: *stop=0x8c65f074u;r[1]=sx(m,bus,&io_status,0x8c65f13eu,2);if(io_status)return io_status;goto L8c65f076; /* mov.w 0x8c65f13e,r1 */
L8c65f076: *stop=0x8c65f076u;r[0]<<=16;goto L8c65f078; /* shll16 r0 */
L8c65f078: *stop=0x8c65f078u;r[1]+=r[2];goto L8c65f07a; /* add r2,r1 */
L8c65f07a: *stop=0x8c65f07cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c65f086; /* bra 0x8c65f086 */
L8c65f07e: *stop=0x8c65f07eu;r[0]=sx(m,bus,&io_status,0x8c65f13eu,2);if(io_status)return io_status;goto L8c65f080; /* mov.w 0x8c65f13e,r0 */
L8c65f080: *stop=0x8c65f080u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f164u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f082; /* mov.l 0x8c65f164,r2 */
L8c65f082: *stop=0x8c65f082u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f084; /* mov.l @r14,r3 */
L8c65f084: *stop=0x8c65f084u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f086; /* mov.l r2,@(r0,r3) */
L8c65f086: *stop=0x8c65f086u;r[0]=sx(m,bus,&io_status,0x8c65f140u,2);if(io_status)return io_status;goto L8c65f088; /* mov.w 0x8c65f140,r0 */
L8c65f088: *stop=0x8c65f088u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f08a; /* mov.l @r14,r3 */
L8c65f08a: *stop=0x8c65f08au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f08c; /* mov.l @(r0,r3),r2 */
L8c65f08c: *stop=0x8c65f08cu;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[13],4);if(io_status)return io_status;goto L8c65f08e; /* mov.l r13,@(0x14,r2) */
L8c65f08e: *stop=0x8c65f08eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f090; /* mov.l @r14,r3 */
L8c65f090: *stop=0x8c65f090u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f092; /* mov.l @(r0,r3),r2 */
L8c65f092: *stop=0x8c65f092u;r[0]+=4294967260u;goto L8c65f094; /* add #-0x24,r0 */
L8c65f094: *stop=0x8c65f094u;r[1]=r[3];goto L8c65f096; /* mov r3,r1 */
L8c65f096: *stop=0x8c65f096u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f098; /* mov.l @(r0,r3),r3 */
L8c65f098: *stop=0x8c65f098u;r[0]+=4294967280u;goto L8c65f09a; /* add #-0x10,r0 */
L8c65f09a: *stop=0x8c65f09au;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f09c; /* mov.l @(r0,r1),r1 */
L8c65f09c: *stop=0x8c65f09cu;r[0]=sx(m,bus,&io_status,0x8c65f142u,2);if(io_status)return io_status;goto L8c65f09e; /* mov.w 0x8c65f142,r0 */
L8c65f09e: *stop=0x8c65f09eu;r[1]|=r[3];goto L8c65f0a0; /* or r3,r1 */
L8c65f0a0: *stop=0x8c65f0a0u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c65f0a2; /* mov.l r1,@(r0,r2) */
L8c65f0a2: *stop=0x8c65f0a2u;r[0]=sx(m,bus,&io_status,0x8c65f140u,2);if(io_status)return io_status;goto L8c65f0a4; /* mov.w 0x8c65f140,r0 */
L8c65f0a4: *stop=0x8c65f0a4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f0a6; /* mov.l @r14,r3 */
L8c65f0a6: *stop=0x8c65f0a6u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f0a8; /* mov.l @(r0,r3),r2 */
L8c65f0a8: *stop=0x8c65f0a8u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[12],4);if(io_status)return io_status;goto L8c65f0aa; /* mov.l r12,@(0x14,r2) */
L8c65f0aa: target=0x8c65eedau;*pr=0x8c65f0aeu;*stop=0x8c65f0acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f0ae; /* bsr 0x8c65eeda */
L8c65f0ae: *stop=0x8c65f0aeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f0b0; /* mov.l @r14,r2 */
L8c65f0b0: *stop=0x8c65f0b0u;r[1]=sx(m,bus,&io_status,0x8c65f144u,2);if(io_status)return io_status;goto L8c65f0b2; /* mov.w 0x8c65f144,r1 */
L8c65f0b2: *stop=0x8c65f0b2u;r[3]=r[2];goto L8c65f0b4; /* mov r2,r3 */
L8c65f0b4: *stop=0x8c65f0b4u;r[0]=r[2];goto L8c65f0b6; /* mov r2,r0 */
L8c65f0b6: *stop=0x8c65f0b6u;r[1]+=r[3];goto L8c65f0b8; /* add r3,r1 */
L8c65f0b8: *stop=0x8c65f0b8u;r[3]=sx(m,bus,&io_status,0x8c65f146u,2);if(io_status)return io_status;goto L8c65f0ba; /* mov.w 0x8c65f146,r3 */
L8c65f0ba: *stop=0x8c65f0bau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f0bc; /* mov.l @r1,r1 */
L8c65f0bc: *stop=0x8c65f0bcu;r[3]+=r[0];goto L8c65f0be; /* add r0,r3 */
L8c65f0be: *stop=0x8c65f0beu;r[1]<<=2;goto L8c65f0c0; /* shll2 r1 */
L8c65f0c0: *stop=0x8c65f0c0u;r[1]+=r[3];goto L8c65f0c2; /* add r3,r1 */
L8c65f0c2: *stop=0x8c65f0c2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f0c4; /* mov.l @r1,r0 */
L8c65f0c4: *stop=0x8c65f0c4u;r[1]=sx(m,bus,&io_status,0x8c65f148u,2);if(io_status)return io_status;goto L8c65f0c6; /* mov.w 0x8c65f148,r1 */
L8c65f0c6: *stop=0x8c65f0c6u;r[1]+=r[2];goto L8c65f0c8; /* add r2,r1 */
L8c65f0c8: *stop=0x8c65f0c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f168u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f0ca; /* mov.l 0x8c65f168,r2 */
L8c65f0ca: *stop=0x8c65f0cau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c65f0cc; /* mov.l r0,@r1 */
L8c65f0cc: *stop=0x8c65f0ccu;r[0]=sx(m,bus,&io_status,0x8c65f140u,2);if(io_status)return io_status;goto L8c65f0ce; /* mov.w 0x8c65f140,r0 */
L8c65f0ce: *stop=0x8c65f0ceu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f0d0; /* mov.l @r14,r3 */
L8c65f0d0: *stop=0x8c65f0d0u;r[1]=sx(m,bus,&io_status,0x8c65f144u,2);if(io_status)return io_status;goto L8c65f0d2; /* mov.w 0x8c65f144,r1 */
L8c65f0d2: *stop=0x8c65f0d2u;r[4]=r[3];goto L8c65f0d4; /* mov r3,r4 */
L8c65f0d4: *stop=0x8c65f0d4u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[8]=loaded;}goto L8c65f0d6; /* mov.l @(r0,r3),r8 */
L8c65f0d6: *stop=0x8c65f0d6u;r[1]+=r[4];goto L8c65f0d8; /* add r4,r1 */
L8c65f0d8: *stop=0x8c65f0d8u;r[0]=r[3];goto L8c65f0da; /* mov r3,r0 */
L8c65f0da: *stop=0x8c65f0dau;r[3]=sx(m,bus,&io_status,0x8c65f146u,2);if(io_status)return io_status;goto L8c65f0dc; /* mov.w 0x8c65f146,r3 */
L8c65f0dc: *stop=0x8c65f0dcu;r[8]+=4u;goto L8c65f0de; /* add #0x4,r8 */
L8c65f0de: *stop=0x8c65f0deu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f0e0; /* mov.l @r1,r4 */
L8c65f0e0: *stop=0x8c65f0e0u;r[3]+=r[0];goto L8c65f0e2; /* add r0,r3 */
L8c65f0e2: *stop=0x8c65f0e2u;r[4]<<=2;goto L8c65f0e4; /* shll2 r4 */
L8c65f0e4: *stop=0x8c65f0e4u;r[4]+=r[3];goto L8c65f0e6; /* add r3,r4 */
L8c65f0e6: target=r[2];*pr=0x8c65f0eau;*stop=0x8c65f0e8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f0ea; /* jsr @r2 */
L8c65f0ea: *stop=0x8c65f0eau;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c65f0ec; /* mov.l r0,@r8 */
L8c65f0ec: *stop=0x8c65f0ecu;r[0]=sx(m,bus,&io_status,0x8c65f140u,2);if(io_status)return io_status;goto L8c65f0ee; /* mov.w 0x8c65f140,r0 */
L8c65f0ee: *stop=0x8c65f0eeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f0f0; /* mov.l @r14,r3 */
L8c65f0f0: *stop=0x8c65f0f0u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f0f2; /* mov.l @(r0,r3),r2 */
L8c65f0f2: *stop=0x8c65f0f2u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[12],4);if(io_status)return io_status;goto L8c65f0f4; /* mov.l r12,@(0x18,r2) */
L8c65f0f4: *stop=0x8c65f0f4u;r[0]=sx(m,bus,&io_status,0x8c65f14au,2);if(io_status)return io_status;goto L8c65f0f6; /* mov.w 0x8c65f14a,r0 */
L8c65f0f6: *stop=0x8c65f0f6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f0f8; /* mov.l @r14,r3 */
L8c65f0f8: *stop=0x8c65f0f8u;r[1]=sx(m,bus,&io_status,0x8c65f144u,2);if(io_status)return io_status;goto L8c65f0fa; /* mov.w 0x8c65f144,r1 */
L8c65f0fa: *stop=0x8c65f0fau;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f0fc; /* mov.l r13,@(r0,r3) */
L8c65f0fc: *stop=0x8c65f0fcu;r[0]=sx(m,bus,&io_status,0x8c65f144u,2);if(io_status)return io_status;goto L8c65f0fe; /* mov.w 0x8c65f144,r0 */
L8c65f0fe: *stop=0x8c65f0feu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f100; /* mov.l @r14,r3 */
L8c65f100: *stop=0x8c65f100u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f102; /* mov.l @(r0,r3),r2 */
L8c65f102: *stop=0x8c65f102u;r[2]^=r[12];goto L8c65f104; /* xor r12,r2 */
L8c65f104: *stop=0x8c65f104u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f106; /* mov.l r2,@(r0,r3) */
L8c65f106: *stop=0x8c65f106u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f108; /* mov.l @r14,r3 */
L8c65f108: *stop=0x8c65f108u;r[2]=r[3];goto L8c65f10a; /* mov r3,r2 */
L8c65f10a: *stop=0x8c65f10au;r[1]+=r[2];goto L8c65f10c; /* add r2,r1 */
L8c65f10c: *stop=0x8c65f10cu;r[2]=sx(m,bus,&io_status,0x8c65f146u,2);if(io_status)return io_status;goto L8c65f10e; /* mov.w 0x8c65f146,r2 */
L8c65f10e: *stop=0x8c65f10eu;r[0]=r[3];goto L8c65f110; /* mov r3,r0 */
L8c65f110: *stop=0x8c65f110u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f112; /* mov.l @r1,r1 */
L8c65f112: *stop=0x8c65f112u;r[2]+=r[0];goto L8c65f114; /* add r0,r2 */
L8c65f114: *stop=0x8c65f114u;r[1]<<=2;goto L8c65f116; /* shll2 r1 */
L8c65f116: *stop=0x8c65f116u;r[1]+=r[2];goto L8c65f118; /* add r2,r1 */
L8c65f118: *stop=0x8c65f118u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f11a; /* mov.l @r1,r0 */
L8c65f11a: *stop=0x8c65f11au;r[1]=sx(m,bus,&io_status,0x8c65f14cu,2);if(io_status)return io_status;goto L8c65f11c; /* mov.w 0x8c65f14c,r1 */
L8c65f11c: *stop=0x8c65f11cu;r[1]+=r[3];goto L8c65f11e; /* add r3,r1 */
L8c65f11e: *stop=0x8c65f11eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c65f120; /* mov.l r0,@r1 */
L8c65f120: *stop=0x8c65f120u;r[0]=sx(m,bus,&io_status,0x8c65f14eu,2);if(io_status)return io_status;goto L8c65f122; /* mov.w 0x8c65f14e,r0 */
L8c65f122: *stop=0x8c65f122u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f124; /* mov.l @r14,r3 */
L8c65f124: *stop=0x8c65f124u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f126; /* mov.l @(r0,r3),r2 */
L8c65f126: *stop=0x8c65f126u;r[2]^=r[12];goto L8c65f128; /* xor r12,r2 */
L8c65f128: *stop=0x8c65f128u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f12a; /* mov.l r2,@(r0,r3) */
L8c65f12a: *stop=0x8c65f12au;r[0]=sx(m,bus,&io_status,0x8c65f150u,2);if(io_status)return io_status;goto L8c65f12c; /* mov.w 0x8c65f150,r0 */
L8c65f12c: *stop=0x8c65f12cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f12e; /* mov.l @r14,r3 */
L8c65f12e: *stop=0x8c65f12eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f130; /* mov.l r13,@(r0,r3) */
L8c65f130: *stop=0x8c65f130u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f132; /* mov.l @r14,r4 */
L8c65f132: *stop=0x8c65f132u;r[4]+=40u;goto L8c65f134; /* add #0x28,r4 */
L8c65f134: *stop=0x8c65f134u;r[5]=r[13];goto L8c65f136; /* mov r13,r5 */
L8c65f136: *stop=0x8c65f138u;r[6]=24u;goto L8c65f18c; /* bra 0x8c65f18c */
L8c65f16c: *stop=0x8c65f16cu;r[3]=sx(m,bus,&io_status,0x8c65f284u,2);if(io_status)return io_status;goto L8c65f16e; /* mov.w 0x8c65f284,r3 */
L8c65f16e: *stop=0x8c65f16eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f170; /* mov.l @r14,r0 */
L8c65f170: *stop=0x8c65f170u;r[3]+=r[0];goto L8c65f172; /* add r0,r3 */
L8c65f172: *stop=0x8c65f172u;r[3]+=r[5];goto L8c65f174; /* add r5,r3 */
L8c65f174: *stop=0x8c65f174u;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c65f176; /* mov.b @r3,r2 */
L8c65f176: *stop=0x8c65f176u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65f178; /* tst r2,r2 */
L8c65f178: take=(*sr&1u);*stop=0x8c65f17au;r[5]+=1u;if(take)goto L8c65f18a;goto L8c65f17c; /* bt/s 0x8c65f18a */
L8c65f17c: *stop=0x8c65f17cu;if(!valid_access(r[4]+6u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x6u,1);if(io_status)return io_status;goto L8c65f17e; /* mov.b @(0x6,r4),r0 */
L8c65f17e: *stop=0x8c65f17eu;if(!valid_access(r[4]+7u,1u))return -3;wr(m,bus,&io_status,r[4]+0x7u,r[0],1);if(io_status)return io_status;goto L8c65f180; /* mov.b r0,@(0x7,r4) */
L8c65f180: *stop=0x8c65f180u;r[0]=r[13];goto L8c65f182; /* mov r13,r0 */
L8c65f182: *stop=0x8c65f182u;if(!valid_access(r[4]+6u,1u))return -3;wr(m,bus,&io_status,r[4]+0x6u,r[0],1);if(io_status)return io_status;goto L8c65f184; /* mov.b r0,@(0x6,r4) */
L8c65f184: *stop=0x8c65f184u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f186; /* mov.l @(0x18,r4),r3 */
L8c65f186: *stop=0x8c65f186u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c65f188; /* mov.l r3,@(0x1c,r4) */
L8c65f188: *stop=0x8c65f188u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],4);if(io_status)return io_status;goto L8c65f18a; /* mov.l r0,@(0x18,r4) */
L8c65f18a: *stop=0x8c65f18au;r[4]+=44u;goto L8c65f18c; /* add #0x2c,r4 */
L8c65f18c: *stop=0x8c65f18cu;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c65f18e; /* cmp/ge r6,r5 */
L8c65f18e: if(!(*sr&1u))goto L8c65f16c;goto L8c65f190; /* bf 0x8c65f16c */
L8c65f190: *stop=0x8c65f190u;r[0]=sx(m,bus,&io_status,0x8c65f286u,2);if(io_status)return io_status;goto L8c65f192; /* mov.w 0x8c65f286,r0 */
L8c65f192: *stop=0x8c65f192u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f194; /* mov.l @r14,r3 */
L8c65f194: *stop=0x8c65f194u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f28cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f196; /* mov.l 0x8c65f28c,r2 */
L8c65f196: *stop=0x8c65f196u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f198; /* mov.l r13,@(r0,r3) */
L8c65f198: *stop=0x8c65f198u;r[0]=r[13];goto L8c65f19a; /* mov r13,r0 */
L8c65f19a: *stop=0x8c65f19au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f19c; /* mov.l @r2,r3 */
L8c65f19c: *stop=0x8c65f19cu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[13],1);if(io_status)return io_status;goto L8c65f19e; /* mov.b r13,@r3 */
L8c65f19e: *stop=0x8c65f19eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1a0; /* lds.l @r15+,PR */
L8c65f1a0: *stop=0x8c65f1a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65f1a2; /* mov.l @r15+,r8 */
L8c65f1a2: *stop=0x8c65f1a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65f1a4; /* mov.l @r15+,r12 */
L8c65f1a4: *stop=0x8c65f1a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65f1a6; /* mov.l @r15+,r13 */
L8c65f1a6: *stop=0x8c65f1a6u;if(*pr!=entry_pr)return -3;*stop=0x8c65f1a8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_submission_8c65eeda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65eeda;
L8c65eeda: *stop=0x8c65eedau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65eedc; /* mov.l r14,@-r15 */
L8c65eedc: *stop=0x8c65eedcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65eede; /* mov.l r13,@-r15 */
L8c65eede: *stop=0x8c65eedeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65eee0; /* mov.l r12,@-r15 */
L8c65eee0: *stop=0x8c65eee0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65eee2; /* mov.l r11,@-r15 */
L8c65eee2: *stop=0x8c65eee2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65eee4; /* mov.l r10,@-r15 */
L8c65eee4: *stop=0x8c65eee4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ef2cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c65eee6; /* mov.l 0x8c65ef2c,r10 */
L8c65eee6: *stop=0x8c65eee6u;r[0]=sx(m,bus,&io_status,0x8c65ef1au,2);if(io_status)return io_status;goto L8c65eee8; /* mov.w 0x8c65ef1a,r0 */
L8c65eee8: *stop=0x8c65eee8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65eeea; /* mov.l r9,@-r15 */
L8c65eeea: *stop=0x8c65eeeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65eeec; /* mov.l r8,@-r15 */
L8c65eeec: *stop=0x8c65eeecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65eeee; /* sts.l PR,@-r15 */
L8c65eeee: *stop=0x8c65eeeeu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65eef0; /* mov.l @r10,r14 */
L8c65eef0: *stop=0x8c65eef0u;r[11]=sx(m,bus,&io_status,0x8c65ef18u,2);if(io_status)return io_status;goto L8c65eef2; /* mov.w 0x8c65ef18,r11 */
L8c65eef2: *stop=0x8c65eef2u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65eef4; /* mov.l @(r0,r14),r3 */
L8c65eef4: *stop=0x8c65eef4u;r[12]=r[14];goto L8c65eef6; /* mov r14,r12 */
L8c65eef6: *stop=0x8c65eef6u;r[12]+=40u;goto L8c65eef8; /* add #0x28,r12 */
L8c65eef8: *stop=0x8c65eef8u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c65eefa; /* cmp/pl r3 */
L8c65eefa: take=!(*sr&1u);*stop=0x8c65eefcu;r[11]+=r[14];if(take)goto L8c65ef04;goto L8c65eefe; /* bf/s 0x8c65ef04 */
L8c65eefe: *stop=0x8c65eefeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ef30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ef00; /* mov.l 0x8c65ef30,r3 */
L8c65ef00: target=r[3];*pr=0x8c65ef04u;*stop=0x8c65ef02u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ef04; /* jsr @r3 */
L8c65ef04: *stop=0x8c65ef04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ef20u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c65ef06; /* mov.l 0x8c65ef20,r8 */
L8c65ef06: *stop=0x8c65ef06u;r[13]=1u;goto L8c65ef08; /* mov #0x1,r13 */
L8c65ef08: *stop=0x8c65ef08u;r[9]=24u;goto L8c65ef0a; /* mov #0x18,r9 */
L8c65ef0a: *stop=0x8c65ef0cu;r[14]=0u;goto L8c65efbc; /* bra 0x8c65efbc */
L8c65ef34: *stop=0x8c65ef34u;r[3]=sx(m,bus,&io_status,0x8c65f008u,2);if(io_status)return io_status;goto L8c65ef36; /* mov.w 0x8c65f008,r3 */
L8c65ef36: *stop=0x8c65ef36u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65ef38; /* mov.l @r10,r0 */
L8c65ef38: *stop=0x8c65ef38u;r[3]+=r[0];goto L8c65ef3a; /* add r0,r3 */
L8c65ef3a: *stop=0x8c65ef3au;r[3]+=r[14];goto L8c65ef3c; /* add r14,r3 */
L8c65ef3c: *stop=0x8c65ef3cu;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c65ef3e; /* mov.b @r3,r2 */
L8c65ef3e: *stop=0x8c65ef3eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65ef40; /* tst r2,r2 */
L8c65ef40: if((*sr&1u))goto L8c65efb6;goto L8c65ef42; /* bt 0x8c65efb6 */
L8c65ef42: *stop=0x8c65ef42u;if(!valid_access(r[12]+6u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x6u,1);if(io_status)return io_status;goto L8c65ef44; /* mov.b @(0x6,r12),r0 */
L8c65ef44: *stop=0x8c65ef44u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c65ef46; /* tst r0,r0 */
L8c65ef46: if(!(*sr&1u))goto L8c65efb6;goto L8c65ef48; /* bf 0x8c65efb6 */
L8c65ef48: *stop=0x8c65ef48u;if(!valid_access(r[12]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x4u,1);if(io_status)return io_status;goto L8c65ef4a; /* mov.b @(0x4,r12),r0 */
L8c65ef4a: *stop=0x8c65ef4au;r[0]=r[0]&255u;goto L8c65ef4c; /* extu.b r0,r0 */
L8c65ef4c: *stop=0x8c65ef4cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65ef4e; /* cmp/eq #0x1,r0 */
L8c65ef4e: if((*sr&1u))goto L8c65ef7e;goto L8c65ef50; /* bt 0x8c65ef7e */
L8c65ef50: *stop=0x8c65ef50u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c65ef52; /* cmp/eq #0x2,r0 */
L8c65ef52: if((*sr&1u))goto L8c65ef86;goto L8c65ef54; /* bt 0x8c65ef86 */
L8c65ef54: *stop=0x8c65ef54u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c65ef56; /* cmp/eq #0x3,r0 */
L8c65ef56: if((*sr&1u))goto L8c65ef90;goto L8c65ef58; /* bt 0x8c65ef90 */
L8c65ef58: *stop=0x8c65ef58u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c65ef5a; /* cmp/eq #0x4,r0 */
L8c65ef5a: if((*sr&1u))goto L8c65ef9a;goto L8c65ef5c; /* bt 0x8c65ef9a */
L8c65ef5c: *stop=0x8c65ef5cu;*sr=(*sr&~1u)|(r[0]==0x00000009u);goto L8c65ef5e; /* cmp/eq #0x9,r0 */
L8c65ef5e: if((*sr&1u))goto L8c65efa4;goto L8c65ef60; /* bt 0x8c65efa4 */
L8c65ef60: *stop=0x8c65ef60u;*sr=(*sr&~1u)|(r[0]==0x0000000au);goto L8c65ef62; /* cmp/eq #0xa,r0 */
L8c65ef62: if((*sr&1u))goto L8c65efb6;goto L8c65ef64; /* bt 0x8c65efb6 */
L8c65ef64: *stop=0x8c65ef64u;*sr=(*sr&~1u)|(r[0]==0x0000000bu);goto L8c65ef66; /* cmp/eq #0xb,r0 */
L8c65ef66: if((*sr&1u))goto L8c65efb6;goto L8c65ef68; /* bt 0x8c65efb6 */
L8c65ef68: *stop=0x8c65ef68u;*sr=(*sr&~1u)|(r[0]==0x0000000cu);goto L8c65ef6a; /* cmp/eq #0xc,r0 */
L8c65ef6a: if((*sr&1u))goto L8c65efb6;goto L8c65ef6c; /* bt 0x8c65efb6 */
L8c65ef6c: *stop=0x8c65ef6cu;*sr=(*sr&~1u)|(r[0]==0x0000000du);goto L8c65ef6e; /* cmp/eq #0xd,r0 */
L8c65ef6e: if((*sr&1u))goto L8c65efb6;goto L8c65ef70; /* bt 0x8c65efb6 */
L8c65ef70: *stop=0x8c65ef70u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65ef72; /* cmp/eq #0x0,r0 */
L8c65ef72: if((*sr&1u))goto L8c65efb6;goto L8c65ef74; /* bt 0x8c65efb6 */
L8c65ef74: *stop=0x8c65ef74u;r[1]=sx(m,bus,&io_status,0x8c65f00au,2);if(io_status)return io_status;goto L8c65ef76; /* mov.w 0x8c65f00a,r1 */
L8c65ef76: *stop=0x8c65ef76u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65ef78; /* cmp/eq r1,r0 */
L8c65ef78: if((*sr&1u))goto L8c65efb6;goto L8c65ef7a; /* bt 0x8c65efb6 */
L8c65ef7a: *stop=0x8c65ef7cu;goto L8c65efb6; /* bra 0x8c65efb6 */
L8c65ef7e: target=r[8];*pr=0x8c65ef82u;*stop=0x8c65ef80u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ef82; /* jsr @r8 */
L8c65ef82: *stop=0x8c65ef84u;goto L8c65efb6; /* bra 0x8c65efb6 */
L8c65ef86: *stop=0x8c65ef86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f010u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ef88; /* mov.l 0x8c65f010,r2 */
L8c65ef88: target=r[2];*pr=0x8c65ef8cu;*stop=0x8c65ef8au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ef8c; /* jsr @r2 */
L8c65ef8c: *stop=0x8c65ef8eu;goto L8c65efb6; /* bra 0x8c65efb6 */
L8c65ef90: *stop=0x8c65ef90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f014u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ef92; /* mov.l 0x8c65f014,r2 */
L8c65ef92: target=r[2];*pr=0x8c65ef96u;*stop=0x8c65ef94u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ef96; /* jsr @r2 */
L8c65ef96: *stop=0x8c65ef98u;goto L8c65efb6; /* bra 0x8c65efb6 */
L8c65ef9a: *stop=0x8c65ef9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f018u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ef9c; /* mov.l 0x8c65f018,r2 */
L8c65ef9c: target=r[2];*pr=0x8c65efa0u;*stop=0x8c65ef9eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65efa0; /* jsr @r2 */
L8c65efa0: *stop=0x8c65efa2u;goto L8c65efb6; /* bra 0x8c65efb6 */
L8c65efa4: *stop=0x8c65efa4u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65efa6; /* mov.l @r11,r5 */
L8c65efa6: *stop=0x8c65efa6u;r[2]=r[5];goto L8c65efa8; /* mov r5,r2 */
L8c65efa8: *stop=0x8c65efa8u;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c65efaa; /* tst r13,r2 */
L8c65efaa: take=(*sr&1u);*stop=0x8c65efacu;r[4]=r[14];if(take)goto L8c65efb0;goto L8c65efae; /* bt/s 0x8c65efb0 */
L8c65efae: *stop=0x8c65efaeu;r[5]=r[13];goto L8c65efb0; /* mov r13,r5 */
L8c65efb0: *stop=0x8c65efb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f01cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65efb2; /* mov.l 0x8c65f01c,r2 */
L8c65efb2: target=r[2];*pr=0x8c65efb6u;*stop=0x8c65efb4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65efb6; /* jsr @r2 */
L8c65efb6: *stop=0x8c65efb6u;r[11]+=120u;goto L8c65efb8; /* add #0x78,r11 */
L8c65efb8: *stop=0x8c65efb8u;r[12]+=44u;goto L8c65efba; /* add #0x2c,r12 */
L8c65efba: *stop=0x8c65efbau;r[14]+=1u;goto L8c65efbc; /* add #0x1,r14 */
L8c65efbc: *stop=0x8c65efbcu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[9]);goto L8c65efbe; /* cmp/ge r9,r14 */
L8c65efbe: if(!(*sr&1u))goto L8c65ef34;goto L8c65efc0; /* bf 0x8c65ef34 */
L8c65efc0: *stop=0x8c65efc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f020u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65efc2; /* mov.l 0x8c65f020,r3 */
L8c65efc2: target=r[3];*pr=0x8c65efc6u;*stop=0x8c65efc4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65efc6; /* jsr @r3 */
L8c65efc6: *stop=0x8c65efc6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65efc8; /* lds.l @r15+,PR */
L8c65efc8: *stop=0x8c65efc8u;r[0]=0u;goto L8c65efca; /* mov #0x0,r0 */
L8c65efca: *stop=0x8c65efcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65efcc; /* mov.l @r15+,r8 */
L8c65efcc: *stop=0x8c65efccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65efce; /* mov.l @r15+,r9 */
L8c65efce: *stop=0x8c65efceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65efd0; /* mov.l @r15+,r10 */
L8c65efd0: *stop=0x8c65efd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65efd2; /* mov.l @r15+,r11 */
L8c65efd2: *stop=0x8c65efd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65efd4; /* mov.l @r15+,r12 */
L8c65efd4: *stop=0x8c65efd4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65efd6; /* mov.l @r15+,r13 */
L8c65efd6: *stop=0x8c65efd6u;if(*pr!=entry_pr)return -3;*stop=0x8c65efd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_submission_8c66034e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66034e;
L8c66034e: *stop=0x8c66034eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660350; /* sts.l PR,@-r15 */
L8c660350: *stop=0x8c660350u;r[15]+=4294967292u;goto L8c660352; /* add #-0x4,r15 */
L8c660352: *stop=0x8c660352u;r[3]=0u;goto L8c660354; /* mov #0x0,r3 */
L8c660354: *stop=0x8c660354u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c660356; /* mov.l r5,@r15 */
L8c660356: *stop=0x8c660356u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660358; /* mov.l r3,@-r15 */
L8c660358: *stop=0x8c660358u;r[7]=1u;goto L8c66035a; /* mov #0x1,r7 */
L8c66035a: *stop=0x8c66035au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c66035c; /* mov.l r3,@-r15 */
L8c66035c: *stop=0x8c66035cu;r[6]=r[15];goto L8c66035e; /* mov r15,r6 */
L8c66035e: *stop=0x8c66035eu;r[6]+=8u;goto L8c660360; /* add #0x8,r6 */
L8c660360: target=0x8c65f810u;*pr=0x8c660364u;*stop=0x8c660362u;r[5]=9u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660364; /* bsr 0x8c65f810 */
L8c660364: *stop=0x8c660364u;r[15]+=12u;goto L8c660366; /* add #0xc,r15 */
L8c660366: *stop=0x8c660366u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660368; /* lds.l @r15+,PR */
L8c660368: *stop=0x8c660368u;if(*pr!=entry_pr)return -3;*stop=0x8c66036au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_submission_8c6603f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6603f4;
L8c6603f4: *stop=0x8c6603f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6603f6; /* sts.l PR,@-r15 */
L8c6603f6: *stop=0x8c6603f6u;r[3]=0u;goto L8c6603f8; /* mov #0x0,r3 */
L8c6603f8: *stop=0x8c6603f8u;r[6]=r[3];goto L8c6603fa; /* mov r3,r6 */
L8c6603fa: *stop=0x8c6603fau;r[7]=r[3];goto L8c6603fc; /* mov r3,r7 */
L8c6603fc: *stop=0x8c6603fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6603fe; /* mov.l r3,@-r15 */
L8c6603fe: *stop=0x8c6603feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660400; /* mov.l r3,@-r15 */
L8c660400: target=0x8c65f810u;*pr=0x8c660404u;*stop=0x8c660402u;r[5]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660404; /* bsr 0x8c65f810 */
L8c660404: *stop=0x8c660404u;r[15]+=8u;goto L8c660406; /* add #0x8,r15 */
L8c660406: *stop=0x8c660406u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660408; /* lds.l @r15+,PR */
L8c660408: *stop=0x8c660408u;if(*pr!=entry_pr)return -3;*stop=0x8c66040au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_submission_8c660438(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660438;
L8c660438: *stop=0x8c660438u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66043a; /* sts.l PR,@-r15 */
L8c66043a: *stop=0x8c66043au;r[3]=0u;goto L8c66043c; /* mov #0x0,r3 */
L8c66043c: *stop=0x8c66043cu;r[6]=r[3];goto L8c66043e; /* mov r3,r6 */
L8c66043e: *stop=0x8c66043eu;r[7]=r[3];goto L8c660440; /* mov r3,r7 */
L8c660440: *stop=0x8c660440u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660442; /* mov.l r3,@-r15 */
L8c660442: *stop=0x8c660442u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660444; /* mov.l r3,@-r15 */
L8c660444: target=0x8c65f810u;*pr=0x8c660448u;*stop=0x8c660446u;r[5]=3u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660448; /* bsr 0x8c65f810 */
L8c660448: *stop=0x8c660448u;r[15]+=8u;goto L8c66044a; /* add #0x8,r15 */
L8c66044a: *stop=0x8c66044au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66044c; /* lds.l @r15+,PR */
L8c66044c: *stop=0x8c66044cu;if(*pr!=entry_pr)return -3;*stop=0x8c66044eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_submission_8c66047c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66047c;
L8c66047c: *stop=0x8c66047cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66047e; /* sts.l PR,@-r15 */
L8c66047e: *stop=0x8c66047eu;r[3]=0u;goto L8c660480; /* mov #0x0,r3 */
L8c660480: *stop=0x8c660480u;r[6]=r[3];goto L8c660482; /* mov r3,r6 */
L8c660482: *stop=0x8c660482u;r[7]=r[3];goto L8c660484; /* mov r3,r7 */
L8c660484: *stop=0x8c660484u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660486; /* mov.l r3,@-r15 */
L8c660486: *stop=0x8c660486u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660488; /* mov.l r3,@-r15 */
L8c660488: target=0x8c65f810u;*pr=0x8c66048cu;*stop=0x8c66048au;r[5]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66048c; /* bsr 0x8c65f810 */
L8c66048c: *stop=0x8c66048cu;r[15]+=8u;goto L8c66048e; /* add #0x8,r15 */
L8c66048e: *stop=0x8c66048eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660490; /* lds.l @r15+,PR */
L8c660490: *stop=0x8c660490u;if(*pr!=entry_pr)return -3;*stop=0x8c660492u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_submission_8c65f1aa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f1aa;
L8c65f1aa: *stop=0x8c65f1aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f290u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1ac; /* mov.l 0x8c65f290,r3 */
L8c65f1ac: *stop=0x8c65f1acu;r[0]=sx(m,bus,&io_status,0x8c65f286u,2);if(io_status)return io_status;goto L8c65f1ae; /* mov.w 0x8c65f286,r0 */
L8c65f1ae: *stop=0x8c65f1aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f1b0; /* sts.l PR,@-r15 */
L8c65f1b0: *stop=0x8c65f1b0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f1b2; /* mov.l @r3,r4 */
L8c65f1b2: *stop=0x8c65f1b2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f1b4; /* mov.l @(r0,r4),r2 */
L8c65f1b4: *stop=0x8c65f1b4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65f1b6; /* tst r2,r2 */
L8c65f1b6: if((*sr&1u))goto L8c65f1be;goto L8c65f1b8; /* bt 0x8c65f1be */
L8c65f1b8: *stop=0x8c65f1b8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1ba; /* lds.l @r15+,PR */
L8c65f1ba: *stop=0x8c65f1bau;if(*pr!=entry_pr)return -3;*stop=0x8c65f1bcu;r[0]=4294967293u;return 0; /* rts */
L8c65f1be: *stop=0x8c65f1beu;r[0]=sx(m,bus,&io_status,0x8c65f288u,2);if(io_status)return io_status;goto L8c65f1c0; /* mov.w 0x8c65f288,r0 */
L8c65f1c0: *stop=0x8c65f1c0u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1c2; /* mov.l @(r0,r4),r3 */
L8c65f1c2: *stop=0x8c65f1c2u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f1c4; /* mov.l @(0x18,r3),r0 */
L8c65f1c4: *stop=0x8c65f1c4u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c65f1c6; /* tst #0x1,r0 */
L8c65f1c6: if((*sr&1u))goto L8c65f1ce;goto L8c65f1c8; /* bt 0x8c65f1ce */
L8c65f1c8: *stop=0x8c65f1c8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1ca; /* lds.l @r15+,PR */
L8c65f1ca: *stop=0x8c65f1cau;if(*pr!=entry_pr)return -3;*stop=0x8c65f1ccu;r[0]=4294967295u;return 0; /* rts */
L8c65f1ce: *stop=0x8c65f1ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f28cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1d0; /* mov.l 0x8c65f28c,r3 */
L8c65f1d0: *stop=0x8c65f1d0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f1d2; /* mov.l @r3,r0 */
L8c65f1d2: *stop=0x8c65f1d2u;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c65f1d4; /* tas.b @r0 */
L8c65f1d4: if((*sr&1u))goto L8c65f1e4;goto L8c65f1d6; /* bt 0x8c65f1e4 */
L8c65f1d6: *stop=0x8c65f1d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f294u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f1d8; /* mov.l 0x8c65f294,r1 */
L8c65f1d8: target=0x8c65f348u;*pr=0x8c65f1dcu;*stop=0x8c65f1dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f1dc; /* bsr 0x8c65f348 */
L8c65f1dc: *stop=0x8c65f1dcu;r[15]+=4u;goto L8c65f1de; /* add #0x4,r15 */
L8c65f1de: *stop=0x8c65f1deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1e0; /* lds.l @r15+,PR */
L8c65f1e0: *stop=0x8c65f1e0u;if(*pr!=entry_pr)return -3;*stop=0x8c65f1e2u;r[0]=4294967294u;return 0; /* rts */
L8c65f1e4: *stop=0x8c65f1e4u;r[0]=0u;goto L8c65f1e6; /* mov #0x0,r0 */
L8c65f1e6: *stop=0x8c65f1e6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1e8; /* lds.l @r15+,PR */
L8c65f1e8: *stop=0x8c65f1e8u;if(*pr!=entry_pr)return -3;*stop=0x8c65f1eau;return 0; /* rts */
}
