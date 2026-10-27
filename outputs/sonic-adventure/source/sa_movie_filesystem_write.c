/* Original retrying block write, data/commit SDK wrappers and packet builders.
 * Explicit register state, original callbacks and nonlocal continuations.
 * No supplied parser results or saved task frames.
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
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movie_filesystem_write_run_8c6476e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64771eu)goto L8c64771e;if(resume_pc==0x8c64776cu)goto L8c64776c;goto L8c6476e8;
L8c6476e8: *stop=0x8c6476e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6476ea; /* mov.l r14,@-r15 */
L8c6476ea: *stop=0x8c6476eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6476ec; /* mov.l r13,@-r15 */
L8c6476ec: *stop=0x8c6476ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6476ee; /* mov.l r12,@-r15 */
L8c6476ee: *stop=0x8c6476eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6476f0; /* mov.l r11,@-r15 */
L8c6476f0: *stop=0x8c6476f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6476f2; /* mov.l r10,@-r15 */
L8c6476f2: *stop=0x8c6476f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6476f4; /* mov.l r9,@-r15 */
L8c6476f4: *stop=0x8c6476f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6476f6; /* mov.l r8,@-r15 */
L8c6476f6: *stop=0x8c6476f6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6476f8; /* sts.l PR,@-r15 */
L8c6476f8: *stop=0x8c6476f8u;r[15]+=4294967284u;goto L8c6476fa; /* add #-0xc,r15 */
L8c6476fa: *stop=0x8c6476fau;r[12]=sx(m,bus,&io_status,0x8c6477a8u,2);if(io_status)return io_status;goto L8c6476fc; /* mov.w 0x8c6477a8,r12 */
L8c6476fc: *stop=0x8c6476fcu;r[13]=r[4];goto L8c6476fe; /* mov r4,r13 */
L8c6476fe: *stop=0x8c6476feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647700; /* mov.l 0x8c6477b0,r3 */
L8c647700: *stop=0x8c647700u;*macl=r[12]*r[13];goto L8c647702; /* mul.l r12,r13 */
L8c647702: *stop=0x8c647702u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647704; /* mov.l 0x8c6477b4,r2 */
L8c647704: *stop=0x8c647704u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477b8u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c647706; /* mov.l 0x8c6477b8,r8 */
L8c647706: *stop=0x8c647706u;r[11]=sx(m,bus,&io_status,0x8c6477aau,2);if(io_status)return io_status;goto L8c647708; /* mov.w 0x8c6477aa,r11 */
L8c647708: *stop=0x8c647708u;r[12]=*macl;goto L8c64770a; /* sts MACL,r12 */
L8c64770a: *stop=0x8c64770au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477bcu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64770c; /* mov.l 0x8c6477bc,r10 */
L8c64770c: *stop=0x8c64770cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64770e; /* mov.l r5,@r15 */
L8c64770e: *stop=0x8c64770eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c647710; /* mov.l r6,@(0x4,r15) */
L8c647710: *stop=0x8c647710u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[9]=loaded;}goto L8c647712; /* mov.l @r3,r9 */
L8c647712: *stop=0x8c647712u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[12],4);if(io_status)return io_status;goto L8c647714; /* mov.l r12,@(0x8,r15) */
L8c647714: *stop=0x8c647714u;r[12]+=r[2];goto L8c647716; /* add r2,r12 */
L8c647716: *stop=0x8c647716u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c647718; /* mov.l @r15,r5 */
L8c647718: *stop=0x8c647718u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64771a; /* mov.l @(0x4,r15),r6 */
L8c64771a: target=r[10];*pr=0x8c64771eu;*stop=0x8c64771cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64771e; /* jsr @r10 */
L8c64771e: *stop=0x8c64771eu;r[14]=r[0];goto L8c647720; /* mov r0,r14 */
L8c647720: *stop=0x8c647720u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c647722; /* tst r14,r14 */
L8c647722: if((*sr&1u))goto L8c647742;goto L8c647724; /* bt 0x8c647742 */
L8c647724: *stop=0x8c647724u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c647726; /* mov.l @r8,r3 */
L8c647726: *stop=0x8c647726u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c647728; /* tst r3,r3 */
L8c647728: if((*sr&1u))goto L8c64772e;goto L8c64772a; /* bt 0x8c64772e */
L8c64772a: *stop=0x8c64772au;*sr=(*sr&~1u)|(r[11]==r[14]);goto L8c64772c; /* cmp/eq r11,r14 */
L8c64772c: if((*sr&1u))goto L8c647786;goto L8c64772e; /* bt 0x8c647786 */
L8c64772e: *stop=0x8c64772eu;if(!valid_access(r[12]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647730; /* mov.l @(0x34,r12),r3 */
L8c647730: *stop=0x8c647730u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c647732; /* tst r9,r9 */
L8c647732: *stop=0x8c647732u;r[3]+=1u;goto L8c647734; /* add #0x1,r3 */
L8c647734: *stop=0x8c647734u;if(!valid_access(r[12]+52u,4u))return -3;wr(m,bus,&io_status,r[12]+0x34u,r[3],4);if(io_status)return io_status;goto L8c647736; /* mov.l r3,@(0x34,r12) */
L8c647736: take=!(*sr&1u);*stop=0x8c647738u;r[9]+=4294967295u;if(take)goto L8c647716;goto L8c64773a; /* bf/s 0x8c647716 */
L8c64773a: *stop=0x8c64773au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477b0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64773c; /* mov.l 0x8c6477b0,r1 */
L8c64773c: *stop=0x8c64773cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64773e; /* mov.l @r1,r0 */
L8c64773e: *stop=0x8c64773eu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c647740; /* cmp/eq #-0x1,r0 */
L8c647740: if((*sr&1u))goto L8c647716;goto L8c647742; /* bt 0x8c647716 */
L8c647742: *stop=0x8c647742u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c647744; /* mov.l @(0x8,r15),r12 */
L8c647744: *stop=0x8c647744u;r[2]=r[13];goto L8c647746; /* mov r13,r2 */
L8c647746: *stop=0x8c647746u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647748; /* mov.l 0x8c6477b4,r3 */
L8c647748: *stop=0x8c647748u;r[2]<<=2;goto L8c64774a; /* shll2 r2 */
L8c64774a: *stop=0x8c64774au;r[2]<<=2;goto L8c64774c; /* shll2 r2 */
L8c64774c: *stop=0x8c64774cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477c0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64774e; /* mov.l 0x8c6477c0,r1 */
L8c64774e: *stop=0x8c64774eu;r[12]+=r[3];goto L8c647750; /* add r3,r12 */
L8c647750: *stop=0x8c647750u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647752; /* mov.l 0x8c6477c4,r3 */
L8c647752: *stop=0x8c647752u;r[2]<<=2;goto L8c647754; /* shll2 r2 */
L8c647754: *stop=0x8c647754u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c647756; /* shll r2 */
L8c647756: *stop=0x8c647756u;r[2]+=r[1];goto L8c647758; /* add r1,r2 */
L8c647758: *stop=0x8c647758u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64775a; /* mov.l r2,@r15 */
L8c64775a: *stop=0x8c64775au;if(!valid_access(r[12]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64775c; /* mov.l @(0x30,r12),r6 */
L8c64775c: *stop=0x8c64775cu;if(!valid_access(r[12]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64775e; /* mov.l @(0x34,r12),r7 */
L8c64775e: *stop=0x8c64775eu;r[6]+=1u;goto L8c647760; /* add #0x1,r6 */
L8c647760: *stop=0x8c647760u;if(!valid_access(r[12]+48u,4u))return -3;wr(m,bus,&io_status,r[12]+0x30u,r[6],4);if(io_status)return io_status;goto L8c647762; /* mov.l r6,@(0x30,r12) */
L8c647762: *stop=0x8c647762u;r[6]+=4294967295u;goto L8c647764; /* add #-0x1,r6 */
L8c647764: *stop=0x8c647764u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c647766; /* mov.l @r15,r5 */
L8c647766: *stop=0x8c647766u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647768; /* mov.l @(0x8,r5),r5 */
L8c647768: target=r[3];*pr=0x8c64776cu;*stop=0x8c64776au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64776c; /* jsr @r3 */
L8c64776c: *stop=0x8c64776cu;r[0]=r[14];goto L8c64776e; /* mov r14,r0 */
L8c64776e: *stop=0x8c64776eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c647770; /* cmp/eq #0x0,r0 */
L8c647770: if((*sr&1u))goto L8c647782;goto L8c647772; /* bt 0x8c647782 */
L8c647772: *stop=0x8c647772u;r[1]=sx(m,bus,&io_status,0x8c6477acu,2);if(io_status)return io_status;goto L8c647774; /* mov.w 0x8c6477ac,r1 */
L8c647774: *stop=0x8c647774u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c647776; /* cmp/eq r1,r0 */
L8c647776: if((*sr&1u))goto L8c647786;goto L8c647778; /* bt 0x8c647786 */
L8c647778: *stop=0x8c647778u;r[1]=sx(m,bus,&io_status,0x8c6477aeu,2);if(io_status)return io_status;goto L8c64777a; /* mov.w 0x8c6477ae,r1 */
L8c64777a: *stop=0x8c64777au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64777c; /* cmp/eq r1,r0 */
L8c64777c: if((*sr&1u))goto L8c64778c;goto L8c64777e; /* bt 0x8c64778c */
L8c64777e: *stop=0x8c647780u;goto L8c647792; /* bra 0x8c647792 */
L8c647782: *stop=0x8c647784u;r[0]=0u;goto L8c647794; /* bra 0x8c647794 */
L8c647786: *stop=0x8c647786u;r[0]=sx(m,bus,&io_status,0x8c6477acu,2);if(io_status)return io_status;goto L8c647788; /* mov.w 0x8c6477ac,r0 */
L8c647788: *stop=0x8c64778au;goto L8c647794; /* bra 0x8c647794 */
L8c64778c: *stop=0x8c64778cu;r[0]=sx(m,bus,&io_status,0x8c6477aeu,2);if(io_status)return io_status;goto L8c64778e; /* mov.w 0x8c6477ae,r0 */
L8c64778e: *stop=0x8c647790u;goto L8c647794; /* bra 0x8c647794 */
L8c647792: *stop=0x8c647792u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6477c8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c647794; /* mov.l 0x8c6477c8,r0 */
L8c647794: *stop=0x8c647794u;r[15]+=12u;goto L8c647796; /* add #0xc,r15 */
L8c647796: *stop=0x8c647796u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c647798; /* lds.l @r15+,PR */
L8c647798: *stop=0x8c647798u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64779a; /* mov.l @r15+,r8 */
L8c64779a: *stop=0x8c64779au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64779c; /* mov.l @r15+,r9 */
L8c64779c: *stop=0x8c64779cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64779e; /* mov.l @r15+,r10 */
L8c64779e: *stop=0x8c64779eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6477a0; /* mov.l @r15+,r11 */
L8c6477a0: *stop=0x8c6477a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6477a2; /* mov.l @r15+,r12 */
L8c6477a2: *stop=0x8c6477a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6477a4; /* mov.l @r15+,r13 */
L8c6477a4: *stop=0x8c6477a4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6477a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c6476e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6476e8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c64771e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6476e8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64771eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c64776c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6476e8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64776cu);}

static int sa_movie_filesystem_write_run_8c660bae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c660bdeu)goto L8c660bde;if(resume_pc==0x8c660be6u)goto L8c660be6;if(resume_pc==0x8c660c22u)goto L8c660c22;if(resume_pc==0x8c660c3cu)goto L8c660c3c;if(resume_pc==0x8c660c4au)goto L8c660c4a;if(resume_pc==0x8c660c5eu)goto L8c660c5e;if(resume_pc==0x8c660c6au)goto L8c660c6a;if(resume_pc==0x8c660c74u)goto L8c660c74;if(resume_pc==0x8c660c7cu)goto L8c660c7c;if(resume_pc==0x8c660c86u)goto L8c660c86;if(resume_pc==0x8c660c8cu)goto L8c660c8c;if(resume_pc==0x8c660cc6u)goto L8c660cc6;if(resume_pc==0x8c660cd8u)goto L8c660cd8;if(resume_pc==0x8c660ce4u)goto L8c660ce4;if(resume_pc==0x8c660ceeu)goto L8c660cee;if(resume_pc==0x8c660cf6u)goto L8c660cf6;if(resume_pc==0x8c660d00u)goto L8c660d00;if(resume_pc==0x8c660d06u)goto L8c660d06;if(resume_pc==0x8c660d44u)goto L8c660d44;goto L8c660bae;
L8c660bae: *stop=0x8c660baeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660bb0; /* mov.l r14,@-r15 */
L8c660bb0: *stop=0x8c660bb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c660bb2; /* mov.l r13,@-r15 */
L8c660bb2: *stop=0x8c660bb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c660bb4; /* mov.l r12,@-r15 */
L8c660bb4: *stop=0x8c660bb4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c660bb6; /* mov.l r11,@-r15 */
L8c660bb6: *stop=0x8c660bb6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c660bb8; /* mov.l r10,@-r15 */
L8c660bb8: *stop=0x8c660bb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c660bba; /* mov.l r9,@-r15 */
L8c660bba: *stop=0x8c660bbau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c660bbc; /* mov.l r8,@-r15 */
L8c660bbc: *stop=0x8c660bbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660bbe; /* sts.l PR,@-r15 */
L8c660bbe: *stop=0x8c660bbeu;r[15]+=4294967264u;goto L8c660bc0; /* add #-0x20,r15 */
L8c660bc0: *stop=0x8c660bc0u;r[3]=sx(m,bus,&io_status,0x8c660bf2u,2);if(io_status)return io_status;goto L8c660bc2; /* mov.w 0x8c660bf2,r3 */
L8c660bc2: *stop=0x8c660bc2u;r[8]=r[5];goto L8c660bc4; /* mov r5,r8 */
L8c660bc4: *stop=0x8c660bc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660c10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660bc6; /* mov.l 0x8c660c10,r1 */
L8c660bc6: *stop=0x8c660bc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660bfcu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c660bc8; /* mov.l 0x8c660bfc,r10 */
L8c660bc8: *stop=0x8c660bc8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660c0cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c660bca; /* mov.l 0x8c660c0c,r13 */
L8c660bca: *stop=0x8c660bcau;{uint32_t loaded=rd(m,bus,&io_status,0x8c660bf8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c660bcc; /* mov.l 0x8c660bf8,r11 */
L8c660bcc: *stop=0x8c660bccu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c660bce; /* mov.l r4,@(0x8,r15) */
L8c660bce: *stop=0x8c660bceu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c660bd0; /* mov.l r6,@r15 */
L8c660bd0: *stop=0x8c660bd0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660bd2; /* mov.l @(0x8,r15),r2 */
L8c660bd2: *stop=0x8c660bd2u;*macl=r[3]*r[2];goto L8c660bd4; /* mul.l r3,r2 */
L8c660bd4: *stop=0x8c660bd4u;r[3]=*macl;goto L8c660bd6; /* sts MACL,r3 */
L8c660bd6: *stop=0x8c660bd6u;r[3]+=r[1];goto L8c660bd8; /* add r1,r3 */
L8c660bd8: *stop=0x8c660bd8u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c660bda; /* mov.l r3,@(0x14,r15) */
L8c660bda: target=0x8c660decu;*pr=0x8c660bdeu;*stop=0x8c660bdcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660bde; /* bsr 0x8c660dec */
L8c660bde: *stop=0x8c660bdeu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660be0; /* tst r0,r0 */
L8c660be0: if(!(*sr&1u))goto L8c660c14;goto L8c660be2; /* bf 0x8c660c14 */
L8c660be2: target=r[13];*pr=0x8c660be6u;*stop=0x8c660be4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660be6; /* jsr @r13 */
L8c660be6: *stop=0x8c660be8u;goto L8c660d76; /* bra 0x8c660d76 */
L8c660c14: *stop=0x8c660c14u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c660c16; /* mov.l @(0x8,r15),r14 */
L8c660c16: *stop=0x8c660c16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660c18; /* mov.l 0x8c660d14,r0 */
L8c660c18: *stop=0x8c660c18u;r[14]<<=2;goto L8c660c1a; /* shll2 r14 */
L8c660c1a: *stop=0x8c660c1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660c1c; /* mov.l 0x8c660d18,r3 */
L8c660c1c: *stop=0x8c660c1cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c660c1e; /* mov.l @(r0,r14),r14 */
L8c660c1e: target=r[3];*pr=0x8c660c22u;*stop=0x8c660c20u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c22; /* jsr @r3 */
L8c660c22: *stop=0x8c660c22u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660c24; /* mov.l @r15,r2 */
L8c660c24: *stop=0x8c660c24u;r[0]=72u;goto L8c660c26; /* mov #0x48,r0 */
L8c660c26: *stop=0x8c660c26u;r[4]=0u;goto L8c660c28; /* mov #0x0,r4 */
L8c660c28: *stop=0x8c660c28u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c660c2a; /* mov.l r2,@(0x4,r15) */
L8c660c2a: *stop=0x8c660c2au;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660c2c; /* mov.l @(0x14,r15),r3 */
L8c660c2c: *stop=0x8c660c2cu;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660c2e; /* mov.l @(0x1c,r3),r2 */
L8c660c2e: *stop=0x8c660c2eu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c660c30; /* mov.l @(r0,r2),r1 */
L8c660c30: *stop=0x8c660c30u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c660c32; /* mov.l r1,@r15 */
L8c660c32: *stop=0x8c660c32u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c660c34; /* mov.l r4,@(0x1c,r15) */
L8c660c34: *stop=0x8c660c36u;r[9]=r[4];goto L8c660cb4; /* bra 0x8c660cb4 */
L8c660c38: target=r[11];*pr=0x8c660c3cu;*stop=0x8c660c3au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c3c; /* jsr @r11 */
L8c660c3c: *stop=0x8c660c3cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660c3e; /* tst r0,r0 */
L8c660c3e: if(!(*sr&1u))goto L8c660d0a;goto L8c660c40; /* bf 0x8c660d0a */
L8c660c40: *stop=0x8c660c40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660c42; /* mov.l 0x8c660d1c,r3 */
L8c660c42: *stop=0x8c660c42u;r[12]=4u;goto L8c660c44; /* mov #0x4,r12 */
L8c660c44: *stop=0x8c660c44u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c660c46; /* mov.l @r15,r1 */
L8c660c46: target=r[3];*pr=0x8c660c4au;*stop=0x8c660c48u;r[0]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c4a; /* jsr @r3 */
L8c660c4a: *stop=0x8c660c4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c660c4c; /* mov.l r0,@-r15 */
L8c660c4c: *stop=0x8c660c4cu;r[5]=2u;goto L8c660c4e; /* mov #0x2,r5 */
L8c660c4e: *stop=0x8c660c4eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660c50; /* mov.l @(0x8,r15),r3 */
L8c660c50: *stop=0x8c660c50u;r[6]=0u;goto L8c660c52; /* mov #0x0,r6 */
L8c660c52: *stop=0x8c660c52u;r[7]=r[9];goto L8c660c54; /* mov r9,r7 */
L8c660c54: *stop=0x8c660c54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660c56; /* mov.l r3,@-r15 */
L8c660c56: *stop=0x8c660c56u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660c58; /* mov.l 0x8c660d20,r3 */
L8c660c58: *stop=0x8c660c58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c660c5a; /* mov.l r8,@-r15 */
L8c660c5a: target=r[3];*pr=0x8c660c5eu;*stop=0x8c660c5cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c5e; /* jsr @r3 */
L8c660c5e: *stop=0x8c660c5eu;r[12]=r[0];goto L8c660c60; /* mov r0,r12 */
L8c660c60: *stop=0x8c660c60u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c660c62; /* cmp/pz r12 */
L8c660c62: take=(*sr&1u);*stop=0x8c660c64u;r[15]+=12u;if(take)goto L8c660c6e;goto L8c660c66; /* bt/s 0x8c660c6e */
L8c660c66: target=r[13];*pr=0x8c660c6au;*stop=0x8c660c68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c6a; /* jsr @r13 */
L8c660c6a: *stop=0x8c660c6cu;goto L8c660c38; /* bra 0x8c660c38 */
L8c660c6e: *stop=0x8c660c6eu;r[5]=r[12];goto L8c660c70; /* mov r12,r5 */
L8c660c70: target=r[10];*pr=0x8c660c74u;*stop=0x8c660c72u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c74; /* jsr @r10 */
L8c660c74: *stop=0x8c660c74u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660c76; /* tst r0,r0 */
L8c660c76: if(!(*sr&1u))goto L8c660c80;goto L8c660c78; /* bf 0x8c660c80 */
L8c660c78: target=r[13];*pr=0x8c660c7cu;*stop=0x8c660c7au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c7c; /* jsr @r13 */
L8c660c7c: *stop=0x8c660c7eu;goto L8c660c6e; /* bra 0x8c660c6e */
L8c660c80: *stop=0x8c660c80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660c82; /* mov.l 0x8c660d18,r3 */
L8c660c82: target=r[3];*pr=0x8c660c86u;*stop=0x8c660c84u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c86; /* jsr @r3 */
L8c660c86: *stop=0x8c660c86u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c660c88; /* mov.l r0,@(0x18,r15) */
L8c660c88: target=r[11];*pr=0x8c660c8cu;*stop=0x8c660c8au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660c8c; /* jsr @r11 */
L8c660c8c: *stop=0x8c660c8cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660c8e; /* tst r0,r0 */
L8c660c8e: if(!(*sr&1u))goto L8c660d0a;goto L8c660c90; /* bf 0x8c660d0a */
L8c660c90: *stop=0x8c660c90u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660c92; /* mov.l @(0x18,r15),r0 */
L8c660c92: *stop=0x8c660c92u;if(!valid_access(r[0]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x4u,1);if(io_status)return io_status;goto L8c660c94; /* mov.b @(0x4,r0),r0 */
L8c660c94: *stop=0x8c660c94u;r[0]=r[0]&255u;goto L8c660c96; /* extu.b r0,r0 */
L8c660c96: *stop=0x8c660c96u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c660c98; /* cmp/eq #0x7,r0 */
L8c660c98: if((*sr&1u))goto L8c660ca4;goto L8c660c9a; /* bt 0x8c660ca4 */
L8c660c9a: *stop=0x8c660c9au;r[1]=sx(m,bus,&io_status,0x8c660d10u,2);if(io_status)return io_status;goto L8c660c9c; /* mov.w 0x8c660d10,r1 */
L8c660c9c: *stop=0x8c660c9cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660c9e; /* cmp/eq r1,r0 */
L8c660c9e: if((*sr&1u))goto L8c660c38;goto L8c660ca0; /* bt 0x8c660c38 */
L8c660ca0: *stop=0x8c660ca2u;goto L8c660d76; /* bra 0x8c660d76 */
L8c660ca4: *stop=0x8c660ca4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660ca6; /* mov.l @(0x4,r15),r2 */
L8c660ca6: *stop=0x8c660ca6u;r[9]+=1u;goto L8c660ca8; /* add #0x1,r9 */
L8c660ca8: *stop=0x8c660ca8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c660caa; /* mov.l @r15,r3 */
L8c660caa: *stop=0x8c660caau;r[2]+=r[3];goto L8c660cac; /* add r3,r2 */
L8c660cac: *stop=0x8c660cacu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c660cae; /* mov.l r2,@(0x4,r15) */
L8c660cae: *stop=0x8c660caeu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660cb0; /* mov.l @(0x1c,r15),r1 */
L8c660cb0: *stop=0x8c660cb0u;r[1]+=1u;goto L8c660cb2; /* add #0x1,r1 */
L8c660cb2: *stop=0x8c660cb2u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c660cb4; /* mov.l r1,@(0x1c,r15) */
L8c660cb4: *stop=0x8c660cb4u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660cb6; /* mov.l @(0x14,r15),r3 */
L8c660cb6: *stop=0x8c660cb6u;r[0]=64u;goto L8c660cb8; /* mov #0x40,r0 */
L8c660cb8: *stop=0x8c660cb8u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660cba; /* mov.l @(0x1c,r3),r2 */
L8c660cba: *stop=0x8c660cbau;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c660cbc; /* mov.l @(r0,r2),r1 */
L8c660cbc: *stop=0x8c660cbcu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660cbe; /* mov.l @(0x1c,r15),r0 */
L8c660cbe: *stop=0x8c660cbeu;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[1]);goto L8c660cc0; /* cmp/ge r1,r0 */
L8c660cc0: if(!(*sr&1u))goto L8c660c38;goto L8c660cc2; /* bf 0x8c660c38 */
L8c660cc2: target=r[11];*pr=0x8c660cc6u;*stop=0x8c660cc4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660cc6; /* jsr @r11 */
L8c660cc6: *stop=0x8c660cc6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660cc8; /* tst r0,r0 */
L8c660cc8: if(!(*sr&1u))goto L8c660d0a;goto L8c660cca; /* bf 0x8c660d0a */
L8c660cca: *stop=0x8c660ccau;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d24u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c660ccc; /* mov.l 0x8c660d24,r12 */
L8c660ccc: *stop=0x8c660cccu;r[6]=0u;goto L8c660cce; /* mov #0x0,r6 */
L8c660cce: *stop=0x8c660cceu;r[5]=2u;goto L8c660cd0; /* mov #0x2,r5 */
L8c660cd0: *stop=0x8c660cd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c660cd2; /* mov.l r8,@-r15 */
L8c660cd2: *stop=0x8c660cd2u;r[7]=r[9];goto L8c660cd4; /* mov r9,r7 */
L8c660cd4: target=r[12];*pr=0x8c660cd8u;*stop=0x8c660cd6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660cd8; /* jsr @r12 */
L8c660cd8: *stop=0x8c660cd8u;r[12]=r[0];goto L8c660cda; /* mov r0,r12 */
L8c660cda: *stop=0x8c660cdau;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c660cdc; /* cmp/pz r12 */
L8c660cdc: take=(*sr&1u);*stop=0x8c660cdeu;r[15]+=4u;if(take)goto L8c660ce8;goto L8c660ce0; /* bt/s 0x8c660ce8 */
L8c660ce0: target=r[13];*pr=0x8c660ce4u;*stop=0x8c660ce2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660ce4; /* jsr @r13 */
L8c660ce4: *stop=0x8c660ce6u;goto L8c660cc2; /* bra 0x8c660cc2 */
L8c660ce8: *stop=0x8c660ce8u;r[5]=r[12];goto L8c660cea; /* mov r12,r5 */
L8c660cea: target=r[10];*pr=0x8c660ceeu;*stop=0x8c660cecu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660cee; /* jsr @r10 */
L8c660cee: *stop=0x8c660ceeu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660cf0; /* tst r0,r0 */
L8c660cf0: if(!(*sr&1u))goto L8c660cfa;goto L8c660cf2; /* bf 0x8c660cfa */
L8c660cf2: target=r[13];*pr=0x8c660cf6u;*stop=0x8c660cf4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660cf6; /* jsr @r13 */
L8c660cf6: *stop=0x8c660cf8u;goto L8c660ce8; /* bra 0x8c660ce8 */
L8c660cfa: *stop=0x8c660cfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c660d18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660cfc; /* mov.l 0x8c660d18,r3 */
L8c660cfc: target=r[3];*pr=0x8c660d00u;*stop=0x8c660cfeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660d00; /* jsr @r3 */
L8c660d00: *stop=0x8c660d00u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c660d02; /* mov.l r0,@(0x4,r15) */
L8c660d02: target=r[11];*pr=0x8c660d06u;*stop=0x8c660d04u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660d06; /* jsr @r11 */
L8c660d06: *stop=0x8c660d06u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660d08; /* tst r0,r0 */
L8c660d08: if((*sr&1u))goto L8c660d28;goto L8c660d0a; /* bt 0x8c660d28 */
L8c660d0a: *stop=0x8c660d0au;r[0]=sx(m,bus,&io_status,0x8c660d12u,2);if(io_status)return io_status;goto L8c660d0c; /* mov.w 0x8c660d12,r0 */
L8c660d0c: *stop=0x8c660d0eu;goto L8c660d78; /* bra 0x8c660d78 */
L8c660d28: *stop=0x8c660d28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660d2a; /* mov.l 0x8c660e38,r3 */
L8c660d2a: *stop=0x8c660d2au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660d2c; /* mov.l @r3,r2 */
L8c660d2c: *stop=0x8c660d2cu;r[1]=r[2];goto L8c660d2e; /* mov r2,r1 */
L8c660d2e: *stop=0x8c660d2eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c660d30; /* tst r1,r1 */
L8c660d30: take=(*sr&1u);*stop=0x8c660d32u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c660d48;goto L8c660d34; /* bt/s 0x8c660d48 */
L8c660d34: *stop=0x8c660d34u;r[6]=r[15];goto L8c660d36; /* mov r15,r6 */
L8c660d36: *stop=0x8c660d36u;r[7]=r[15];goto L8c660d38; /* mov r15,r7 */
L8c660d38: *stop=0x8c660d38u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c660d3a; /* mov.l @r15,r1 */
L8c660d3a: *stop=0x8c660d3au;r[5]=r[8];goto L8c660d3c; /* mov r8,r5 */
L8c660d3c: *stop=0x8c660d3cu;r[6]+=12u;goto L8c660d3e; /* add #0xc,r6 */
L8c660d3e: *stop=0x8c660d3eu;r[7]+=16u;goto L8c660d40; /* add #0x10,r7 */
L8c660d40: target=r[1];*pr=0x8c660d44u;*stop=0x8c660d42u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660d44; /* jsr @r1 */
L8c660d44: *stop=0x8c660d44u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660d46; /* tst r0,r0 */
L8c660d46: if(!(*sr&1u))goto L8c660d54;goto L8c660d48; /* bf 0x8c660d54 */
L8c660d48: *stop=0x8c660d48u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660d4a; /* mov.l @(0x4,r15),r0 */
L8c660d4a: *stop=0x8c660d4au;if(!valid_access(r[0]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x4u,1);if(io_status)return io_status;goto L8c660d4c; /* mov.b @(0x4,r0),r0 */
L8c660d4c: *stop=0x8c660d4cu;if(!valid_access(r[15]+12u,1u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],1);if(io_status)return io_status;goto L8c660d4e; /* mov.b r0,@(0xc,r15) */
L8c660d4e: *stop=0x8c660d4eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660d50; /* mov.l @(0x4,r15),r3 */
L8c660d50: *stop=0x8c660d50u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660d52; /* mov.l @(0xc,r3),r2 */
L8c660d52: *stop=0x8c660d52u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c660d54; /* mov.l r2,@(0x10,r15) */
L8c660d54: *stop=0x8c660d54u;if(!valid_access(r[15]+12u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xcu,1);if(io_status)return io_status;goto L8c660d56; /* mov.b @(0xc,r15),r0 */
L8c660d56: *stop=0x8c660d56u;r[0]=r[0]&255u;goto L8c660d58; /* extu.b r0,r0 */
L8c660d58: *stop=0x8c660d58u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c660d5a; /* cmp/eq #0x7,r0 */
L8c660d5a: if((*sr&1u))goto L8c660d6c;goto L8c660d5c; /* bt 0x8c660d6c */
L8c660d5c: *stop=0x8c660d5cu;r[1]=sx(m,bus,&io_status,0x8c660e2eu,2);if(io_status)return io_status;goto L8c660d5e; /* mov.w 0x8c660e2e,r1 */
L8c660d5e: *stop=0x8c660d5eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660d60; /* cmp/eq r1,r0 */
L8c660d60: if((*sr&1u))goto L8c660cc2;goto L8c660d62; /* bt 0x8c660cc2 */
L8c660d62: *stop=0x8c660d62u;r[1]=sx(m,bus,&io_status,0x8c660e30u,2);if(io_status)return io_status;goto L8c660d64; /* mov.w 0x8c660e30,r1 */
L8c660d64: *stop=0x8c660d64u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660d66; /* cmp/eq r1,r0 */
L8c660d66: if((*sr&1u))goto L8c660d70;goto L8c660d68; /* bt 0x8c660d70 */
L8c660d68: *stop=0x8c660d6au;goto L8c660d76; /* bra 0x8c660d76 */
L8c660d6c: *stop=0x8c660d6eu;r[0]=0u;goto L8c660d78; /* bra 0x8c660d78 */
L8c660d70: *stop=0x8c660d70u;r[0]=sx(m,bus,&io_status,0x8c660e32u,2);if(io_status)return io_status;goto L8c660d72; /* mov.w 0x8c660e32,r0 */
L8c660d72: *stop=0x8c660d74u;goto L8c660d78; /* bra 0x8c660d78 */
L8c660d76: *stop=0x8c660d76u;r[0]=sx(m,bus,&io_status,0x8c660e34u,2);if(io_status)return io_status;goto L8c660d78; /* mov.w 0x8c660e34,r0 */
L8c660d78: *stop=0x8c660d78u;r[15]+=32u;goto L8c660d7a; /* add #0x20,r15 */
L8c660d7a: *stop=0x8c660d7au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660d7c; /* lds.l @r15+,PR */
L8c660d7c: *stop=0x8c660d7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c660d7e; /* mov.l @r15+,r8 */
L8c660d7e: *stop=0x8c660d7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c660d80; /* mov.l @r15+,r9 */
L8c660d80: *stop=0x8c660d80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c660d82; /* mov.l @r15+,r10 */
L8c660d82: *stop=0x8c660d82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c660d84; /* mov.l @r15+,r11 */
L8c660d84: *stop=0x8c660d84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c660d86; /* mov.l @r15+,r12 */
L8c660d86: *stop=0x8c660d86u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c660d88; /* mov.l @r15+,r13 */
L8c660d88: *stop=0x8c660d88u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c660d8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c660bae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660bde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660bdeu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660be6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660be6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c22u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c3cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c4au);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c5e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c5eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c6a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c6au);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c74u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c7c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c7cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c86u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660c8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660c8cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660cc6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660cc6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660cd8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660cd8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660ce4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660ce4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660cee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660ceeu);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660cf6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660cf6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660d00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660d00u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660d06(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660d06u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660d44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660bae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660d44u);}

static int sa_movie_filesystem_write_run_8c6469f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c646a0au)goto L8c646a0a;if(resume_pc==0x8c646a1eu)goto L8c646a1e;goto L8c6469f4;
L8c6469f4: *stop=0x8c6469f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6469f6; /* sts.l PR,@-r15 */
L8c6469f6: *stop=0x8c6469f6u;r[15]+=4294967292u;goto L8c6469f8; /* add #-0x4,r15 */
L8c6469f8: *stop=0x8c6469f8u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6469fa; /* mov.l @(0x10,r15),r3 */
L8c6469fa: *stop=0x8c6469fau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6469fc; /* mov.l r3,@-r15 */
L8c6469fc: *stop=0x8c6469fcu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6469fe; /* mov.l @(0x10,r15),r2 */
L8c6469fe: *stop=0x8c6469feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c646a00; /* mov.l r2,@-r15 */
L8c646a00: *stop=0x8c646a00u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646a02; /* mov.l @(0x10,r15),r3 */
L8c646a02: *stop=0x8c646a02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c646a04; /* mov.l r3,@-r15 */
L8c646a04: *stop=0x8c646a04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646ad0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646a06; /* mov.l 0x8c646ad0,r3 */
L8c646a06: target=r[3];*pr=0x8c646a0au;*stop=0x8c646a08u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646a0a; /* jsr @r3 */
L8c646a0a: *stop=0x8c646a0au;r[15]+=12u;goto L8c646a0c; /* add #0xc,r15 */
L8c646a0c: *stop=0x8c646a0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646accu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646a0e; /* mov.l 0x8c646acc,r2 */
L8c646a0e: *stop=0x8c646a0eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c646a10; /* mov.l r0,@r15 */
L8c646a10: *stop=0x8c646a10u;r[0]=sx(m,bus,&io_status,0x8c646ac6u,2);if(io_status)return io_status;goto L8c646a12; /* mov.w 0x8c646ac6,r0 */
L8c646a12: *stop=0x8c646a12u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646a14; /* mov.l @r2,r3 */
L8c646a14: *stop=0x8c646a14u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c646a16; /* mov.l @(r0,r3),r1 */
L8c646a16: *stop=0x8c646a16u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c646a18; /* tst r1,r1 */
L8c646a18: if((*sr&1u))goto L8c646a1e;goto L8c646a1a; /* bt 0x8c646a1e */
L8c646a1a: target=0x8c646654u;*pr=0x8c646a1eu;*stop=0x8c646a1cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646a1e; /* bsr 0x8c646654 */
L8c646a1e: *stop=0x8c646a1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c646a20; /* mov.l @r15,r0 */
L8c646a20: *stop=0x8c646a20u;r[15]+=4u;goto L8c646a22; /* add #0x4,r15 */
L8c646a22: *stop=0x8c646a22u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646a24; /* lds.l @r15+,PR */
L8c646a24: *stop=0x8c646a24u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c646a26u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c6469f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6469f4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c646a0a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6469f4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c646a0au);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c646a1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6469f4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c646a1eu);}

static int sa_movie_filesystem_write_run_8c646a28(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c646a36u)goto L8c646a36;if(resume_pc==0x8c646a4au)goto L8c646a4a;goto L8c646a28;
L8c646a28: *stop=0x8c646a28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646a2a; /* sts.l PR,@-r15 */
L8c646a2a: *stop=0x8c646a2au;r[15]+=4294967292u;goto L8c646a2c; /* add #-0x4,r15 */
L8c646a2c: *stop=0x8c646a2cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646a2e; /* mov.l @(0x8,r15),r3 */
L8c646a2e: *stop=0x8c646a2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c646a30; /* mov.l r3,@-r15 */
L8c646a30: *stop=0x8c646a30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646ad4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646a32; /* mov.l 0x8c646ad4,r3 */
L8c646a32: target=r[3];*pr=0x8c646a36u;*stop=0x8c646a34u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646a36; /* jsr @r3 */
L8c646a36: *stop=0x8c646a36u;r[15]+=4u;goto L8c646a38; /* add #0x4,r15 */
L8c646a38: *stop=0x8c646a38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646accu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646a3a; /* mov.l 0x8c646acc,r2 */
L8c646a3a: *stop=0x8c646a3au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c646a3c; /* mov.l r0,@r15 */
L8c646a3c: *stop=0x8c646a3cu;r[0]=sx(m,bus,&io_status,0x8c646ac6u,2);if(io_status)return io_status;goto L8c646a3e; /* mov.w 0x8c646ac6,r0 */
L8c646a3e: *stop=0x8c646a3eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646a40; /* mov.l @r2,r3 */
L8c646a40: *stop=0x8c646a40u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c646a42; /* mov.l @(r0,r3),r1 */
L8c646a42: *stop=0x8c646a42u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c646a44; /* tst r1,r1 */
L8c646a44: if((*sr&1u))goto L8c646a4a;goto L8c646a46; /* bt 0x8c646a4a */
L8c646a46: target=0x8c646654u;*pr=0x8c646a4au;*stop=0x8c646a48u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646a4a; /* bsr 0x8c646654 */
L8c646a4a: *stop=0x8c646a4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c646a4c; /* mov.l @r15,r0 */
L8c646a4c: *stop=0x8c646a4cu;r[15]+=4u;goto L8c646a4e; /* add #0x4,r15 */
L8c646a4e: *stop=0x8c646a4eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646a50; /* lds.l @r15+,PR */
L8c646a50: *stop=0x8c646a50u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c646a52u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c646a28(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c646a28(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c646a36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c646a28(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c646a36u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c646a4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c646a28(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c646a4au);}

static int sa_movie_filesystem_write_run_8c660546(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c660558u)goto L8c660558;if(resume_pc==0x8c660570u)goto L8c660570;goto L8c660546;
L8c660546: *stop=0x8c660546u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660548; /* mov.l 0x8c660610,r3 */
L8c660548: *stop=0x8c660548u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66054a; /* sts.l PR,@-r15 */
L8c66054a: *stop=0x8c66054au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66054c; /* mov.l @r3,r0 */
L8c66054c: *stop=0x8c66054cu;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c66054e; /* tas.b @r0 */
L8c66054e: if((*sr&1u))goto L8c660560;goto L8c660550; /* bt 0x8c660560 */
L8c660550: *stop=0x8c660550u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660618u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660552; /* mov.l 0x8c660618,r2 */
L8c660552: *stop=0x8c660552u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660614u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660554; /* mov.l 0x8c660614,r1 */
L8c660554: target=r[2];*pr=0x8c660558u;*stop=0x8c660556u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660558; /* jsr @r2 */
L8c660558: *stop=0x8c660558u;r[15]+=4u;goto L8c66055a; /* add #0x4,r15 */
L8c66055a: *stop=0x8c66055au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66055c; /* lds.l @r15+,PR */
L8c66055c: *stop=0x8c66055cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66055eu;r[0]=4294967294u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c660560: *stop=0x8c660560u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660562; /* mov.l @(0xc,r15),r3 */
L8c660562: *stop=0x8c660562u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660564; /* mov.l r3,@-r15 */
L8c660564: *stop=0x8c660564u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660566; /* mov.l @(0xc,r15),r2 */
L8c660566: *stop=0x8c660566u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c660568; /* mov.l r2,@-r15 */
L8c660568: *stop=0x8c660568u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66056a; /* mov.l @(0xc,r15),r3 */
L8c66056a: *stop=0x8c66056au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c66056c; /* mov.l r3,@-r15 */
L8c66056c: target=0x8c660580u;*pr=0x8c660570u;*stop=0x8c66056eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660570; /* bsr 0x8c660580 */
L8c660570: *stop=0x8c660570u;r[15]+=12u;goto L8c660572; /* add #0xc,r15 */
L8c660572: *stop=0x8c660572u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660574; /* mov.l 0x8c660610,r3 */
L8c660574: *stop=0x8c660574u;r[1]=0u;goto L8c660576; /* mov #0x0,r1 */
L8c660576: *stop=0x8c660576u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660578; /* mov.l @r3,r2 */
L8c660578: *stop=0x8c660578u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[1],1);if(io_status)return io_status;goto L8c66057a; /* mov.b r1,@r2 */
L8c66057a: *stop=0x8c66057au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66057c; /* lds.l @r15+,PR */
L8c66057c: *stop=0x8c66057cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66057eu;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c660546(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660546(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660558(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660546(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660558u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660570(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660546(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660570u);}

static int sa_movie_filesystem_write_run_8c6605b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6605c2u)goto L8c6605c2;if(resume_pc==0x8c6605d2u)goto L8c6605d2;goto L8c6605b0;
L8c6605b0: *stop=0x8c6605b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6605b2; /* mov.l 0x8c660610,r3 */
L8c6605b2: *stop=0x8c6605b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6605b4; /* sts.l PR,@-r15 */
L8c6605b4: *stop=0x8c6605b4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6605b6; /* mov.l @r3,r0 */
L8c6605b6: *stop=0x8c6605b6u;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c6605b8; /* tas.b @r0 */
L8c6605b8: if((*sr&1u))goto L8c6605ca;goto L8c6605ba; /* bt 0x8c6605ca */
L8c6605ba: *stop=0x8c6605bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c660618u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6605bc; /* mov.l 0x8c660618,r2 */
L8c6605bc: *stop=0x8c6605bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660614u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6605be; /* mov.l 0x8c660614,r1 */
L8c6605be: target=r[2];*pr=0x8c6605c2u;*stop=0x8c6605c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6605c2; /* jsr @r2 */
L8c6605c2: *stop=0x8c6605c2u;r[15]+=4u;goto L8c6605c4; /* add #0x4,r15 */
L8c6605c4: *stop=0x8c6605c4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6605c6; /* lds.l @r15+,PR */
L8c6605c6: *stop=0x8c6605c6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6605c8u;r[0]=4294967294u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c6605ca: *stop=0x8c6605cau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6605cc; /* mov.l @(0x4,r15),r3 */
L8c6605cc: *stop=0x8c6605ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6605ce; /* mov.l r3,@-r15 */
L8c6605ce: target=0x8c6605e2u;*pr=0x8c6605d2u;*stop=0x8c6605d0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6605d2; /* bsr 0x8c6605e2 */
L8c6605d2: *stop=0x8c6605d2u;r[15]+=4u;goto L8c6605d4; /* add #0x4,r15 */
L8c6605d4: *stop=0x8c6605d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6605d6; /* mov.l 0x8c660610,r3 */
L8c6605d6: *stop=0x8c6605d6u;r[1]=0u;goto L8c6605d8; /* mov #0x0,r1 */
L8c6605d8: *stop=0x8c6605d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6605da; /* mov.l @r3,r2 */
L8c6605da: *stop=0x8c6605dau;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[1],1);if(io_status)return io_status;goto L8c6605dc; /* mov.b r1,@r2 */
L8c6605dc: *stop=0x8c6605dcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6605de; /* lds.l @r15+,PR */
L8c6605de: *stop=0x8c6605deu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6605e0u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c6605b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6605b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c6605c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6605b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6605c2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c6605d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6605b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6605d2u);}

static int sa_movie_filesystem_write_run_8c660580(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6605a8u)goto L8c6605a8;goto L8c660580;
L8c660580: *stop=0x8c660580u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660582; /* mov.l r14,@-r15 */
L8c660582: *stop=0x8c660582u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660584; /* sts.l PR,@-r15 */
L8c660584: *stop=0x8c660584u;r[15]+=4294967288u;goto L8c660586; /* add #-0x8,r15 */
L8c660586: *stop=0x8c660586u;r[6]<<=16;goto L8c660588; /* shll16 r6 */
L8c660588: *stop=0x8c660588u;r[14]=r[15];goto L8c66058a; /* mov r15,r14 */
L8c66058a: *stop=0x8c66058au;r[6]<<=8;goto L8c66058c; /* shll8 r6 */
L8c66058c: *stop=0x8c66058cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c66058e; /* mov.l r5,@r14 */
L8c66058e: *stop=0x8c66058eu;r[7]<<=16;goto L8c660590; /* shll16 r7 */
L8c660590: *stop=0x8c660590u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660592; /* mov.l @(0x10,r15),r3 */
L8c660592: *stop=0x8c660592u;r[6]|=r[7];goto L8c660594; /* or r7,r6 */
L8c660594: *stop=0x8c660594u;r[6]|=r[3];goto L8c660596; /* or r3,r6 */
L8c660596: *stop=0x8c660596u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c660598; /* mov.l r6,@(0x4,r14) */
L8c660598: *stop=0x8c660598u;r[7]=2u;goto L8c66059a; /* mov #0x2,r7 */
L8c66059a: *stop=0x8c66059au;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66059c; /* mov.l @(0x18,r15),r3 */
L8c66059c: *stop=0x8c66059cu;r[6]=r[14];goto L8c66059e; /* mov r14,r6 */
L8c66059e: *stop=0x8c66059eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6605a0; /* mov.l r3,@-r15 */
L8c6605a0: *stop=0x8c6605a0u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6605a2; /* mov.l @(0x18,r15),r2 */
L8c6605a2: *stop=0x8c6605a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6605a4; /* mov.l r2,@-r15 */
L8c6605a4: target=0x8c65f810u;*pr=0x8c6605a8u;*stop=0x8c6605a6u;r[5]=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6605a8; /* bsr 0x8c65f810 */
L8c6605a8: *stop=0x8c6605a8u;r[15]+=16u;goto L8c6605aa; /* add #0x10,r15 */
L8c6605aa: *stop=0x8c6605aau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6605ac; /* lds.l @r15+,PR */
L8c6605ac: *stop=0x8c6605acu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6605aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c660580(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660580(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c6605a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c660580(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6605a8u);}

static int sa_movie_filesystem_write_run_8c6605e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c660608u)goto L8c660608;goto L8c6605e2;
L8c6605e2: *stop=0x8c6605e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6605e4; /* mov.l r14,@-r15 */
L8c6605e4: *stop=0x8c6605e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6605e6; /* sts.l PR,@-r15 */
L8c6605e6: *stop=0x8c6605e6u;r[15]+=4294967288u;goto L8c6605e8; /* add #-0x8,r15 */
L8c6605e8: *stop=0x8c6605e8u;r[6]<<=16;goto L8c6605ea; /* shll16 r6 */
L8c6605ea: *stop=0x8c6605eau;r[14]=r[15];goto L8c6605ec; /* mov r15,r14 */
L8c6605ec: *stop=0x8c6605ecu;r[7]<<=16;goto L8c6605ee; /* shll16 r7 */
L8c6605ee: *stop=0x8c6605eeu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c6605f0; /* mov.l r5,@r14 */
L8c6605f0: *stop=0x8c6605f0u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6605f2; /* mov.l @(0x10,r15),r3 */
L8c6605f2: *stop=0x8c6605f2u;r[6]<<=8;goto L8c6605f4; /* shll8 r6 */
L8c6605f4: *stop=0x8c6605f4u;r[6]|=r[7];goto L8c6605f6; /* or r7,r6 */
L8c6605f6: *stop=0x8c6605f6u;r[6]|=r[3];goto L8c6605f8; /* or r3,r6 */
L8c6605f8: *stop=0x8c6605f8u;r[3]=0u;goto L8c6605fa; /* mov #0x0,r3 */
L8c6605fa: *stop=0x8c6605fau;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c6605fc; /* mov.l r6,@(0x4,r14) */
L8c6605fc: *stop=0x8c6605fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6605fe; /* mov.l r3,@-r15 */
L8c6605fe: *stop=0x8c6605feu;r[7]=2u;goto L8c660600; /* mov #0x2,r7 */
L8c660600: *stop=0x8c660600u;r[6]=r[14];goto L8c660602; /* mov r14,r6 */
L8c660602: *stop=0x8c660602u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660604; /* mov.l r3,@-r15 */
L8c660604: target=0x8c65f810u;*pr=0x8c660608u;*stop=0x8c660606u;r[5]=13u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660608; /* bsr 0x8c65f810 */
L8c660608: *stop=0x8c660608u;r[15]+=16u;goto L8c66060a; /* add #0x10,r15 */
L8c66060a: *stop=0x8c66060au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66060c; /* lds.l @r15+,PR */
L8c66060c: *stop=0x8c66060cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66060eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_write_8c6605e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6605e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_write_resume_8c660608(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_write_run_8c6605e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660608u);}
