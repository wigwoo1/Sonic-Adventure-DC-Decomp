/* Original command12 normal and readback-verified file writes.
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
static int sa_movie_filesystem_command12_run_8c64b71c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b73cu)goto L8c64b73c;if(resume_pc==0x8c64b746u)goto L8c64b746;if(resume_pc==0x8c64b774u)goto L8c64b774;if(resume_pc==0x8c64b7a8u)goto L8c64b7a8;if(resume_pc==0x8c64b7eau)goto L8c64b7ea;if(resume_pc==0x8c64b802u)goto L8c64b802;goto L8c64b71c;
L8c64b71c: *stop=0x8c64b71cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b71e; /* mov.l r14,@-r15 */
L8c64b71e: *stop=0x8c64b71eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b720; /* mov.l r13,@-r15 */
L8c64b720: *stop=0x8c64b720u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b722; /* mov.l r12,@-r15 */
L8c64b722: *stop=0x8c64b722u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b724; /* mov.l r11,@-r15 */
L8c64b724: *stop=0x8c64b724u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b726; /* mov.l r10,@-r15 */
L8c64b726: *stop=0x8c64b726u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b728; /* sts.l PR,@-r15 */
L8c64b728: *stop=0x8c64b728u;r[15]+=4294967288u;goto L8c64b72a; /* add #-0x8,r15 */
L8c64b72a: *stop=0x8c64b72au;r[11]=r[5];goto L8c64b72c; /* mov r5,r11 */
L8c64b72c: *stop=0x8c64b72cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b790u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b72e; /* mov.l 0x8c64b790,r3 */
L8c64b72e: *stop=0x8c64b72eu;r[5]=0u;goto L8c64b730; /* mov #0x0,r5 */
L8c64b730: *stop=0x8c64b730u;r[12]=r[4];goto L8c64b732; /* mov r4,r12 */
L8c64b732: *stop=0x8c64b732u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c64b734; /* mov.l r6,@(0x4,r15) */
L8c64b734: *stop=0x8c64b734u;r[13]=r[7];goto L8c64b736; /* mov r7,r13 */
L8c64b736: *stop=0x8c64b736u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64b738; /* mov.l @(0x24,r15),r10 */
L8c64b738: target=r[3];*pr=0x8c64b73cu;*stop=0x8c64b73au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b73c; /* jsr @r3 */
L8c64b73c: *stop=0x8c64b73cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b794u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b73e; /* mov.l 0x8c64b794,r2 */
L8c64b73e: *stop=0x8c64b73eu;r[5]=r[11];goto L8c64b740; /* mov r11,r5 */
L8c64b740: *stop=0x8c64b740u;r[14]=r[0];goto L8c64b742; /* mov r0,r14 */
L8c64b742: target=r[2];*pr=0x8c64b746u;*stop=0x8c64b744u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b746; /* jsr @r2 */
L8c64b746: *stop=0x8c64b746u;r[4]=r[0];goto L8c64b748; /* mov r0,r4 */
L8c64b748: *stop=0x8c64b748u;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c64b74a; /* cmp/pl r4 */
L8c64b74a: if(!(*sr&1u))goto L8c64b74e;goto L8c64b74c; /* bf 0x8c64b74e */
L8c64b74c: *stop=0x8c64b74cu;r[14]+=r[4];goto L8c64b74e; /* add r4,r14 */
L8c64b74e: *stop=0x8c64b74eu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b750; /* cmp/pz r14 */
L8c64b750: if(!(*sr&1u))goto L8c64b802;goto L8c64b752; /* bf 0x8c64b802 */
L8c64b752: *stop=0x8c64b752u;*sr=(*sr&~1u)|(r[14]>=r[13]);goto L8c64b754; /* cmp/hs r13,r14 */
L8c64b754: if((*sr&1u))goto L8c64b75c;goto L8c64b756; /* bt 0x8c64b75c */
L8c64b756: *stop=0x8c64b756u;r[0]=sx(m,bus,&io_status,0x8c64b77cu,2);if(io_status)return io_status;goto L8c64b758; /* mov.w 0x8c64b77c,r0 */
L8c64b758: *stop=0x8c64b75au;goto L8c64b804; /* bra 0x8c64b804 */
L8c64b75c: *stop=0x8c64b75cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b798u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b75e; /* mov.l 0x8c64b798,r4 */
L8c64b75e: *stop=0x8c64b75eu;r[4]&=r[10];goto L8c64b760; /* and r10,r4 */
L8c64b760: *stop=0x8c64b760u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64b762; /* tst r4,r4 */
L8c64b762: if((*sr&1u))goto L8c64b79c;goto L8c64b764; /* bt 0x8c64b79c */
L8c64b764: *stop=0x8c64b764u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b766; /* mov.l r10,@-r15 */
L8c64b766: *stop=0x8c64b766u;r[5]=r[11];goto L8c64b768; /* mov r11,r5 */
L8c64b768: *stop=0x8c64b768u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b76a; /* mov.l @(0x24,r15),r3 */
L8c64b76a: *stop=0x8c64b76au;r[7]=r[13];goto L8c64b76c; /* mov r13,r7 */
L8c64b76c: *stop=0x8c64b76cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64b76e; /* mov.l r3,@-r15 */
L8c64b76e: *stop=0x8c64b76eu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64b770; /* mov.l @(0xc,r15),r6 */
L8c64b770: target=0x8c64b82cu;*pr=0x8c64b774u;*stop=0x8c64b772u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b774; /* bsr 0x8c64b82c */
L8c64b774: *stop=0x8c64b776u;r[15]+=8u;goto L8c64b804; /* bra 0x8c64b804 */
L8c64b79c: *stop=0x8c64b79cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b79e; /* mov.l 0x8c64b818,r3 */
L8c64b79e: *stop=0x8c64b79eu;r[7]=r[15];goto L8c64b7a0; /* mov r15,r7 */
L8c64b7a0: *stop=0x8c64b7a0u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64b7a2; /* mov.l @(0x20,r15),r6 */
L8c64b7a2: *stop=0x8c64b7a2u;r[5]=r[11];goto L8c64b7a4; /* mov r11,r5 */
L8c64b7a4: target=r[3];*pr=0x8c64b7a8u;*stop=0x8c64b7a6u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b7a8; /* jsr @r3 */
L8c64b7a8: *stop=0x8c64b7a8u;r[14]=r[0];goto L8c64b7aa; /* mov r0,r14 */
L8c64b7aa: *stop=0x8c64b7aau;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b7ac; /* cmp/pz r14 */
L8c64b7ac: if(!(*sr&1u))goto L8c64b802;goto L8c64b7ae; /* bf 0x8c64b802 */
L8c64b7ae: *stop=0x8c64b7aeu;r[4]=sx(m,bus,&io_status,0x8c64b814u,2);if(io_status)return io_status;goto L8c64b7b0; /* mov.w 0x8c64b814,r4 */
L8c64b7b0: *stop=0x8c64b7b0u;r[0]=r[10];goto L8c64b7b2; /* mov r10,r0 */
L8c64b7b2: *stop=0x8c64b7b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b81cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b7b4; /* mov.l 0x8c64b81c,r3 */
L8c64b7b4: *stop=0x8c64b7b4u;*macl=r[4]*r[12];goto L8c64b7b6; /* mul.l r4,r12 */
L8c64b7b6: *stop=0x8c64b7b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b7b8; /* mov.l @r15,r2 */
L8c64b7b8: *stop=0x8c64b7b8u;r[12]=0u;goto L8c64b7ba; /* mov #0x0,r12 */
L8c64b7ba: *stop=0x8c64b7bau;if(!valid_access(r[2]+1u,1u))return -3;wr(m,bus,&io_status,r[2]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64b7bc; /* mov.b r0,@(0x1,r2) */
L8c64b7bc: *stop=0x8c64b7bcu;r[2]=1u;goto L8c64b7be; /* mov #0x1,r2 */
L8c64b7be: *stop=0x8c64b7beu;r[4]=*macl;goto L8c64b7c0; /* sts MACL,r4 */
L8c64b7c0: *stop=0x8c64b7c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b820u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b7c2; /* mov.l 0x8c64b820,r0 */
L8c64b7c2: *stop=0x8c64b7c2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64b7c4; /* mov.l @(0x4,r15),r11 */
L8c64b7c4: *stop=0x8c64b7c4u;r[4]+=r[3];goto L8c64b7c6; /* add r3,r4 */
L8c64b7c6: *stop=0x8c64b7c6u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64b7c8; /* mov.l r2,@(0x30,r4) */
L8c64b7c8: *stop=0x8c64b7c8u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b7ca; /* mov.l @(0x4,r4),r1 */
L8c64b7ca: *stop=0x8c64b7cau;r[2]=r[1];goto L8c64b7cc; /* mov r1,r2 */
L8c64b7cc: *stop=0x8c64b7ccu;r[1]<<=2;goto L8c64b7ce; /* shll2 r1 */
L8c64b7ce: *stop=0x8c64b7ceu;r[1]+=r[2];goto L8c64b7d0; /* add r2,r1 */
L8c64b7d0: *stop=0x8c64b7d0u;r[2]=r[12];goto L8c64b7d2; /* mov r12,r2 */
L8c64b7d2: *stop=0x8c64b7d2u;r[1]<<=2;goto L8c64b7d4; /* shll2 r1 */
L8c64b7d4: *stop=0x8c64b7d4u;*sr=(*sr&~1u)|(r[2]>=r[13]);goto L8c64b7d6; /* cmp/hs r13,r2 */
L8c64b7d6: *stop=0x8c64b7d6u;r[1]<<=2;goto L8c64b7d8; /* shll2 r1 */
L8c64b7d8: *stop=0x8c64b7d8u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b7da; /* mov.l @(r0,r1),r0 */
L8c64b7da: *stop=0x8c64b7dau;r[0]+=r[13];goto L8c64b7dc; /* add r13,r0 */
L8c64b7dc: take=(*sr&1u);*stop=0x8c64b7deu;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[0],4);if(io_status)return io_status;if(take)goto L8c64b7f8;goto L8c64b7e0; /* bt/s 0x8c64b7f8 */
L8c64b7e0: *stop=0x8c64b7e0u;r[10]=sx(m,bus,&io_status,0x8c64b816u,2);if(io_status)return io_status;goto L8c64b7e2; /* mov.w 0x8c64b816,r10 */
L8c64b7e2: *stop=0x8c64b7e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b824u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b7e4; /* mov.l 0x8c64b824,r3 */
L8c64b7e4: *stop=0x8c64b7e4u;r[5]=r[11];goto L8c64b7e6; /* mov r11,r5 */
L8c64b7e6: target=r[3];*pr=0x8c64b7eau;*stop=0x8c64b7e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b7ea; /* jsr @r3 */
L8c64b7ea: *stop=0x8c64b7eau;r[14]=r[0];goto L8c64b7ec; /* mov r0,r14 */
L8c64b7ec: *stop=0x8c64b7ecu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b7ee; /* cmp/pz r14 */
L8c64b7ee: if(!(*sr&1u))goto L8c64b7fa;goto L8c64b7f0; /* bf 0x8c64b7fa */
L8c64b7f0: *stop=0x8c64b7f0u;r[12]+=1u;goto L8c64b7f2; /* add #0x1,r12 */
L8c64b7f2: *stop=0x8c64b7f2u;*sr=(*sr&~1u)|(r[12]>=r[13]);goto L8c64b7f4; /* cmp/hs r13,r12 */
L8c64b7f4: take=!(*sr&1u);*stop=0x8c64b7f6u;r[11]+=r[10];if(take)goto L8c64b7e2;goto L8c64b7f8; /* bf/s 0x8c64b7e2 */
L8c64b7f8: *stop=0x8c64b7f8u;r[14]=0u;goto L8c64b7fa; /* mov #0x0,r14 */
L8c64b7fa: *stop=0x8c64b7fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b828u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b7fc; /* mov.l 0x8c64b828,r3 */
L8c64b7fc: *stop=0x8c64b7fcu;r[5]=0u;goto L8c64b7fe; /* mov #0x0,r5 */
L8c64b7fe: target=r[3];*pr=0x8c64b802u;*stop=0x8c64b800u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b802; /* jsr @r3 */
L8c64b802: *stop=0x8c64b802u;r[0]=r[14];goto L8c64b804; /* mov r14,r0 */
L8c64b804: *stop=0x8c64b804u;r[15]+=8u;goto L8c64b806; /* add #0x8,r15 */
L8c64b806: *stop=0x8c64b806u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b808; /* lds.l @r15+,PR */
L8c64b808: *stop=0x8c64b808u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64b80a; /* mov.l @r15+,r10 */
L8c64b80a: *stop=0x8c64b80au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b80c; /* mov.l @r15+,r11 */
L8c64b80c: *stop=0x8c64b80cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b80e; /* mov.l @r15+,r12 */
L8c64b80e: *stop=0x8c64b80eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b810; /* mov.l @r15+,r13 */
L8c64b810: *stop=0x8c64b810u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b812u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64b71c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b73c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b73cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b746(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b746u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b774(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b774u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b7a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b7a8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b7ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b7eau);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b802(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b71c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b802u);}

static int sa_movie_filesystem_command12_run_8c648b40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c648b6au)goto L8c648b6a;if(resume_pc==0x8c648b72u)goto L8c648b72;if(resume_pc==0x8c648b7cu)goto L8c648b7c;if(resume_pc==0x8c648b98u)goto L8c648b98;if(resume_pc==0x8c648bbau)goto L8c648bba;if(resume_pc==0x8c648c04u)goto L8c648c04;if(resume_pc==0x8c648c12u)goto L8c648c12;if(resume_pc==0x8c648c32u)goto L8c648c32;goto L8c648b40;
L8c648b40: *stop=0x8c648b40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648b42; /* mov.l r14,@-r15 */
L8c648b42: *stop=0x8c648b42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c648b44; /* mov.l r13,@-r15 */
L8c648b44: *stop=0x8c648b44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c648b46; /* mov.l r12,@-r15 */
L8c648b46: *stop=0x8c648b46u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c648b48; /* mov.l r11,@-r15 */
L8c648b48: *stop=0x8c648b48u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c648b4a; /* mov.l r10,@-r15 */
L8c648b4a: *stop=0x8c648b4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c648b4c; /* sts.l PR,@-r15 */
L8c648b4c: *stop=0x8c648b4cu;r[15]+=4294967288u;goto L8c648b4e; /* add #-0x8,r15 */
L8c648b4e: *stop=0x8c648b4eu;r[12]=sx(m,bus,&io_status,0x8c648bceu,2);if(io_status)return io_status;goto L8c648b50; /* mov.w 0x8c648bce,r12 */
L8c648b50: *stop=0x8c648b50u;r[14]=r[4];goto L8c648b52; /* mov r4,r14 */
L8c648b52: *stop=0x8c648b52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648bd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648b54; /* mov.l 0x8c648bd8,r3 */
L8c648b54: *stop=0x8c648b54u;*macl=r[12]*r[14];goto L8c648b56; /* mul.l r12,r14 */
L8c648b56: *stop=0x8c648b56u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c648b58; /* mov.l r5,@r15 */
L8c648b58: *stop=0x8c648b58u;r[12]=*macl;goto L8c648b5a; /* sts MACL,r12 */
L8c648b5a: *stop=0x8c648b5au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[12],4);if(io_status)return io_status;goto L8c648b5c; /* mov.l r12,@(0x4,r15) */
L8c648b5c: *stop=0x8c648b5cu;r[12]+=r[3];goto L8c648b5e; /* add r3,r12 */
L8c648b5e: *stop=0x8c648b5eu;if(!valid_access(r[12]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x2u,1);if(io_status)return io_status;goto L8c648b60; /* mov.b @(0x2,r12),r0 */
L8c648b60: *stop=0x8c648b60u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648b62; /* tst r0,r0 */
L8c648b62: if((*sr&1u))goto L8c648b8c;goto L8c648b64; /* bt 0x8c648b8c */
L8c648b64: *stop=0x8c648b64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648bdcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648b66; /* mov.l 0x8c648bdc,r3 */
L8c648b66: target=r[3];*pr=0x8c648b6au;*stop=0x8c648b68u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648b6a; /* jsr @r3 */
L8c648b6a: *stop=0x8c648b6au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648b6c; /* tst r0,r0 */
L8c648b6c: if(!(*sr&1u))goto L8c648b76;goto L8c648b6e; /* bf 0x8c648b76 */
L8c648b6e: target=0x8c64850au;*pr=0x8c648b72u;*stop=0x8c648b70u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648b72; /* bsr 0x8c64850a */
L8c648b72: *stop=0x8c648b74u;goto L8c648b8c; /* bra 0x8c648b8c */
L8c648b76: *stop=0x8c648b76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648be0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648b78; /* mov.l 0x8c648be0,r2 */
L8c648b78: target=r[2];*pr=0x8c648b7cu;*stop=0x8c648b7au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648b7c; /* jsr @r2 */
L8c648b7c: *stop=0x8c648b7cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648b7e; /* tst r0,r0 */
L8c648b7e: if((*sr&1u))goto L8c648b8c;goto L8c648b80; /* bt 0x8c648b8c */
L8c648b80: *stop=0x8c648b80u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648b82; /* mov.l @(0x4,r15),r2 */
L8c648b82: *stop=0x8c648b82u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648bd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648b84; /* mov.l 0x8c648bd8,r3 */
L8c648b84: *stop=0x8c648b84u;r[2]+=r[3];goto L8c648b86; /* add r3,r2 */
L8c648b86: *stop=0x8c648b86u;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c648b88; /* mov.b @(0x8,r2),r0 */
L8c648b88: *stop=0x8c648b88u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648b8a; /* tst r0,r0 */
L8c648b8a: if(!(*sr&1u))goto L8c648b92;goto L8c648b8c; /* bf 0x8c648b92 */
L8c648b8c: *stop=0x8c648b8cu;r[0]=sx(m,bus,&io_status,0x8c648bd0u,2);if(io_status)return io_status;goto L8c648b8e; /* mov.w 0x8c648bd0,r0 */
L8c648b8e: *stop=0x8c648b90u;goto L8c648c46; /* bra 0x8c648c46 */
L8c648b92: *stop=0x8c648b92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648be4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648b94; /* mov.l 0x8c648be4,r3 */
L8c648b94: target=r[3];*pr=0x8c648b98u;*stop=0x8c648b96u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648b98; /* jsr @r3 */
L8c648b98: *stop=0x8c648b98u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648b9a; /* cmp/pz r0 */
L8c648b9a: if((*sr&1u))goto L8c648ba2;goto L8c648b9c; /* bt 0x8c648ba2 */
L8c648b9c: *stop=0x8c648b9cu;r[0]=sx(m,bus,&io_status,0x8c648bd2u,2);if(io_status)return io_status;goto L8c648b9e; /* mov.w 0x8c648bd2,r0 */
L8c648b9e: *stop=0x8c648ba0u;goto L8c648c46; /* bra 0x8c648c46 */
L8c648ba2: *stop=0x8c648ba2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c648ba4; /* mov.l @r15,r0 */
L8c648ba4: *stop=0x8c648ba4u;r[4]=0u;goto L8c648ba6; /* mov #0x0,r4 */
L8c648ba6: *stop=0x8c648ba6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648be8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c648ba8; /* mov.l 0x8c648be8,r13 */
L8c648ba8: *stop=0x8c648ba8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c648baa; /* cmp/eq #0x1,r0 */
L8c648baa: take=!(*sr&1u);*stop=0x8c648bacu;r[10]=r[4];if(take)goto L8c648bf0;goto L8c648bae; /* bf/s 0x8c648bf0 */
L8c648bae: *stop=0x8c648bb0u;r[11]=r[4];goto L8c648bc2; /* bra 0x8c648bc2 */
L8c648bb2: *stop=0x8c648bb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648becu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648bb4; /* mov.l 0x8c648bec,r3 */
L8c648bb4: *stop=0x8c648bb4u;r[5]=r[11];goto L8c648bb6; /* mov r11,r5 */
L8c648bb6: target=r[3];*pr=0x8c648bbau;*stop=0x8c648bb8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648bba; /* jsr @r3 */
L8c648bba: *stop=0x8c648bbau;r[0]=r[0]&65535u;goto L8c648bbc; /* extu.w r0,r0 */
L8c648bbc: *stop=0x8c648bbcu;*sr=(*sr&~1u)|(r[13]==r[0]);goto L8c648bbe; /* cmp/eq r13,r0 */
L8c648bbe: if(!(*sr&1u))goto L8c648bca;goto L8c648bc0; /* bf 0x8c648bca */
L8c648bc0: *stop=0x8c648bc0u;r[11]+=1u;goto L8c648bc2; /* add #0x1,r11 */
L8c648bc2: *stop=0x8c648bc2u;if(!valid_access(r[12]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648bc4; /* mov.l @(0x1c,r12),r2 */
L8c648bc4: *stop=0x8c648bc4u;if(!valid_access(r[2]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648bc6; /* mov.l @(0x38,r2),r3 */
L8c648bc6: *stop=0x8c648bc6u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c648bc8; /* cmp/ge r3,r11 */
L8c648bc8: if(!(*sr&1u))goto L8c648bb2;goto L8c648bca; /* bf 0x8c648bb2 */
L8c648bca: *stop=0x8c648bccu;r[10]=r[11];goto L8c648c44; /* bra 0x8c648c44 */
L8c648bf0: *stop=0x8c648bf0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648cc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648bf2; /* mov.l 0x8c648cc8,r3 */
L8c648bf2: *stop=0x8c648bf2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c648bf4; /* mov.l @r3,r2 */
L8c648bf4: *stop=0x8c648bf4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c648bf6; /* tst r2,r2 */
L8c648bf6: if((*sr&1u))goto L8c648c26;goto L8c648bf8; /* bt 0x8c648c26 */
L8c648bf8: *stop=0x8c648bfau;r[11]=r[4];goto L8c648c1a; /* bra 0x8c648c1a */
L8c648bfc: *stop=0x8c648bfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648cccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648bfe; /* mov.l 0x8c648ccc,r3 */
L8c648bfe: *stop=0x8c648bfeu;r[5]=r[11];goto L8c648c00; /* mov r11,r5 */
L8c648c00: target=r[3];*pr=0x8c648c04u;*stop=0x8c648c02u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648c04; /* jsr @r3 */
L8c648c04: *stop=0x8c648c04u;r[0]=r[0]&65535u;goto L8c648c06; /* extu.w r0,r0 */
L8c648c06: *stop=0x8c648c06u;*sr=(*sr&~1u)|(r[13]==r[0]);goto L8c648c08; /* cmp/eq r13,r0 */
L8c648c08: if(!(*sr&1u))goto L8c648c18;goto L8c648c0a; /* bf 0x8c648c18 */
L8c648c0a: *stop=0x8c648c0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648cd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648c0c; /* mov.l 0x8c648cd0,r3 */
L8c648c0c: *stop=0x8c648c0cu;r[5]=r[11];goto L8c648c0e; /* mov r11,r5 */
L8c648c0e: target=r[3];*pr=0x8c648c12u;*stop=0x8c648c10u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648c12; /* jsr @r3 */
L8c648c12: *stop=0x8c648c12u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648c14; /* cmp/pz r0 */
L8c648c14: if(!(*sr&1u))goto L8c648c18;goto L8c648c16; /* bf 0x8c648c18 */
L8c648c16: *stop=0x8c648c16u;r[10]+=1u;goto L8c648c18; /* add #0x1,r10 */
L8c648c18: *stop=0x8c648c18u;r[11]+=1u;goto L8c648c1a; /* add #0x1,r11 */
L8c648c1a: *stop=0x8c648c1au;if(!valid_access(r[12]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648c1c; /* mov.l @(0x1c,r12),r3 */
L8c648c1c: *stop=0x8c648c1cu;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648c1e; /* mov.l @(0x30,r3),r2 */
L8c648c1e: *stop=0x8c648c1eu;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[2]);goto L8c648c20; /* cmp/ge r2,r11 */
L8c648c20: if(!(*sr&1u))goto L8c648bfc;goto L8c648c22; /* bf 0x8c648bfc */
L8c648c22: *stop=0x8c648c24u;goto L8c648c44; /* bra 0x8c648c44 */
L8c648c26: *stop=0x8c648c28u;r[11]=r[4];goto L8c648c3c; /* bra 0x8c648c3c */
L8c648c2a: *stop=0x8c648c2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648cccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648c2c; /* mov.l 0x8c648ccc,r3 */
L8c648c2c: *stop=0x8c648c2cu;r[5]=r[11];goto L8c648c2e; /* mov r11,r5 */
L8c648c2e: target=r[3];*pr=0x8c648c32u;*stop=0x8c648c30u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648c32; /* jsr @r3 */
L8c648c32: *stop=0x8c648c32u;r[0]=r[0]&65535u;goto L8c648c34; /* extu.w r0,r0 */
L8c648c34: *stop=0x8c648c34u;*sr=(*sr&~1u)|(r[13]==r[0]);goto L8c648c36; /* cmp/eq r13,r0 */
L8c648c36: take=!(*sr&1u);*stop=0x8c648c38u;r[11]+=1u;if(take)goto L8c648c3c;goto L8c648c3a; /* bf/s 0x8c648c3c */
L8c648c3a: *stop=0x8c648c3au;r[10]+=1u;goto L8c648c3c; /* add #0x1,r10 */
L8c648c3c: *stop=0x8c648c3cu;if(!valid_access(r[12]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648c3e; /* mov.l @(0x1c,r12),r3 */
L8c648c3e: *stop=0x8c648c3eu;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648c40; /* mov.l @(0x30,r3),r2 */
L8c648c40: *stop=0x8c648c40u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[2]);goto L8c648c42; /* cmp/ge r2,r11 */
L8c648c42: if(!(*sr&1u))goto L8c648c2a;goto L8c648c44; /* bf 0x8c648c2a */
L8c648c44: *stop=0x8c648c44u;r[0]=r[10];goto L8c648c46; /* mov r10,r0 */
L8c648c46: *stop=0x8c648c46u;r[15]+=8u;goto L8c648c48; /* add #0x8,r15 */
L8c648c48: *stop=0x8c648c48u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648c4a; /* lds.l @r15+,PR */
L8c648c4a: *stop=0x8c648c4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c648c4c; /* mov.l @r15+,r10 */
L8c648c4c: *stop=0x8c648c4cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648c4e; /* mov.l @r15+,r11 */
L8c648c4e: *stop=0x8c648c4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c648c50; /* mov.l @r15+,r12 */
L8c648c50: *stop=0x8c648c50u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c648c52; /* mov.l @r15+,r13 */
L8c648c52: *stop=0x8c648c52u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c648c54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c648b40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648b6a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648b6au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648b72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648b72u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648b7c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648b7cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648b98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648b98u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648bba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648bbau);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648c04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648c04u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648c12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648c12u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648c32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648b40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648c32u);}

static int sa_movie_filesystem_command12_run_8c648f9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c648fcau)goto L8c648fca;if(resume_pc==0x8c648fd2u)goto L8c648fd2;if(resume_pc==0x8c648fdcu)goto L8c648fdc;if(resume_pc==0x8c649036u)goto L8c649036;if(resume_pc==0x8c649048u)goto L8c649048;if(resume_pc==0x8c649058u)goto L8c649058;if(resume_pc==0x8c649062u)goto L8c649062;if(resume_pc==0x8c649072u)goto L8c649072;if(resume_pc==0x8c649086u)goto L8c649086;if(resume_pc==0x8c64908cu)goto L8c64908c;if(resume_pc==0x8c6490ccu)goto L8c6490cc;goto L8c648f9a;
L8c648f9a: *stop=0x8c648f9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648f9c; /* mov.l r14,@-r15 */
L8c648f9c: *stop=0x8c648f9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c648f9e; /* mov.l r13,@-r15 */
L8c648f9e: *stop=0x8c648f9eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c648fa0; /* mov.l r12,@-r15 */
L8c648fa0: *stop=0x8c648fa0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c648fa2; /* mov.l r11,@-r15 */
L8c648fa2: *stop=0x8c648fa2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c648fa4; /* mov.l r10,@-r15 */
L8c648fa4: *stop=0x8c648fa4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c648fa6; /* sts.l PR,@-r15 */
L8c648fa6: *stop=0x8c648fa6u;r[15]+=4294967244u;goto L8c648fa8; /* add #-0x34,r15 */
L8c648fa8: *stop=0x8c648fa8u;r[11]=0u;goto L8c648faa; /* mov #0x0,r11 */
L8c648faa: *stop=0x8c648faau;r[3]=r[7];goto L8c648fac; /* mov r7,r3 */
L8c648fac: *stop=0x8c648facu;r[14]=r[4];goto L8c648fae; /* mov r4,r14 */
L8c648fae: *stop=0x8c648faeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649020u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c648fb0; /* mov.l 0x8c649020,r0 */
L8c648fb0: *stop=0x8c648fb0u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c648fb2; /* mov.l r6,@(0x4,r15) */
L8c648fb2: *stop=0x8c648fb2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c648fb4; /* mov.l r7,@r15 */
L8c648fb4: *stop=0x8c648fb4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[11],4);if(io_status)return io_status;goto L8c648fb6; /* mov.l r11,@r3 */
L8c648fb6: *stop=0x8c648fb6u;r[3]=sx(m,bus,&io_status,0x8c648ffau,2);if(io_status)return io_status;goto L8c648fb8; /* mov.w 0x8c648ffa,r3 */
L8c648fb8: *stop=0x8c648fb8u;*macl=r[3]*r[14];goto L8c648fba; /* mul.l r3,r14 */
L8c648fba: *stop=0x8c648fbau;r[3]=*macl;goto L8c648fbc; /* sts MACL,r3 */
L8c648fbc: *stop=0x8c648fbcu;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c648fbe; /* mov.b @(r0,r3),r2 */
L8c648fbe: *stop=0x8c648fbeu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c648fc0; /* tst r2,r2 */
L8c648fc0: take=(*sr&1u);*stop=0x8c648fc2u;r[12]=r[5];if(take)goto L8c648fee;goto L8c648fc4; /* bt/s 0x8c648fee */
L8c648fc4: *stop=0x8c648fc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649024u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648fc6; /* mov.l 0x8c649024,r3 */
L8c648fc6: target=r[3];*pr=0x8c648fcau;*stop=0x8c648fc8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648fca; /* jsr @r3 */
L8c648fca: *stop=0x8c648fcau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648fcc; /* tst r0,r0 */
L8c648fcc: if(!(*sr&1u))goto L8c648fd6;goto L8c648fce; /* bf 0x8c648fd6 */
L8c648fce: target=0x8c64850au;*pr=0x8c648fd2u;*stop=0x8c648fd0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648fd2; /* bsr 0x8c64850a */
L8c648fd2: *stop=0x8c648fd4u;goto L8c648fee; /* bra 0x8c648fee */
L8c648fd6: *stop=0x8c648fd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649028u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648fd8; /* mov.l 0x8c649028,r2 */
L8c648fd8: target=r[2];*pr=0x8c648fdcu;*stop=0x8c648fdau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648fdc; /* jsr @r2 */
L8c648fdc: *stop=0x8c648fdcu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648fde; /* tst r0,r0 */
L8c648fde: if((*sr&1u))goto L8c648fee;goto L8c648fe0; /* bt 0x8c648fee */
L8c648fe0: *stop=0x8c648fe0u;r[3]=sx(m,bus,&io_status,0x8c648ffau,2);if(io_status)return io_status;goto L8c648fe2; /* mov.w 0x8c648ffa,r3 */
L8c648fe2: *stop=0x8c648fe2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64902cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c648fe4; /* mov.l 0x8c64902c,r0 */
L8c648fe4: *stop=0x8c648fe4u;*macl=r[3]*r[14];goto L8c648fe6; /* mul.l r3,r14 */
L8c648fe6: *stop=0x8c648fe6u;r[3]=*macl;goto L8c648fe8; /* sts MACL,r3 */
L8c648fe8: *stop=0x8c648fe8u;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c648fea; /* mov.b @(r0,r3),r2 */
L8c648fea: *stop=0x8c648feau;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c648fec; /* tst r2,r2 */
L8c648fec: if(!(*sr&1u))goto L8c649030;goto L8c648fee; /* bf 0x8c649030 */
L8c648fee: *stop=0x8c648feeu;r[0]=sx(m,bus,&io_status,0x8c648ffcu,2);if(io_status)return io_status;goto L8c648ff0; /* mov.w 0x8c648ffc,r0 */
L8c648ff0: *stop=0x8c648ff2u;goto L8c6490d2; /* bra 0x8c6490d2 */
L8c649030: *stop=0x8c649030u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64912cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649032; /* mov.l 0x8c64912c,r3 */
L8c649032: target=r[3];*pr=0x8c649036u;*stop=0x8c649034u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649036; /* jsr @r3 */
L8c649036: *stop=0x8c649036u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649038; /* cmp/pz r0 */
L8c649038: if((*sr&1u))goto L8c649040;goto L8c64903a; /* bt 0x8c649040 */
L8c64903a: *stop=0x8c64903au;r[0]=sx(m,bus,&io_status,0x8c649120u,2);if(io_status)return io_status;goto L8c64903c; /* mov.w 0x8c649120,r0 */
L8c64903c: *stop=0x8c64903eu;goto L8c6490d2; /* bra 0x8c6490d2 */
L8c649040: *stop=0x8c649040u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649130u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649042; /* mov.l 0x8c649130,r3 */
L8c649042: *stop=0x8c649042u;r[5]=r[12];goto L8c649044; /* mov r12,r5 */
L8c649044: target=r[3];*pr=0x8c649048u;*stop=0x8c649046u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649048; /* jsr @r3 */
L8c649048: *stop=0x8c649048u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64904a; /* cmp/pz r0 */
L8c64904a: if(!(*sr&1u))goto L8c649078;goto L8c64904c; /* bf 0x8c649078 */
L8c64904c: *stop=0x8c64904cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649134u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64904e; /* mov.l 0x8c649134,r2 */
L8c64904e: *stop=0x8c64904eu;r[6]=r[15];goto L8c649050; /* mov r15,r6 */
L8c649050: *stop=0x8c649050u;r[5]=r[12];goto L8c649052; /* mov r12,r5 */
L8c649052: *stop=0x8c649052u;r[6]+=8u;goto L8c649054; /* add #0x8,r6 */
L8c649054: target=r[2];*pr=0x8c649058u;*stop=0x8c649056u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649058; /* jsr @r2 */
L8c649058: *stop=0x8c649058u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64905a; /* cmp/pz r0 */
L8c64905a: if(!(*sr&1u))goto L8c64906c;goto L8c64905c; /* bf 0x8c64906c */
L8c64905c: *stop=0x8c64905cu;r[5]=r[12];goto L8c64905e; /* mov r12,r5 */
L8c64905e: target=0x8c649c68u;*pr=0x8c649062u;*stop=0x8c649060u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649062; /* bsr 0x8c649c68 */
L8c649062: *stop=0x8c649062u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649064; /* cmp/pz r0 */
L8c649064: take=(*sr&1u);*stop=0x8c649066u;r[4]=r[0];if(take)goto L8c64906c;goto L8c649068; /* bt/s 0x8c64906c */
L8c649068: *stop=0x8c64906au;r[0]=r[4];goto L8c6490d2; /* bra 0x8c6490d2 */
L8c64906c: *stop=0x8c64906cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649138u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64906e; /* mov.l 0x8c649138,r2 */
L8c64906e: target=r[2];*pr=0x8c649072u;*stop=0x8c649070u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649072; /* jsr @r2 */
L8c649072: *stop=0x8c649072u;r[10]=r[0];goto L8c649074; /* mov r0,r10 */
L8c649074: *stop=0x8c649074u;*sr=(*sr&~1u)|((int32_t)r[10]>=0);goto L8c649076; /* cmp/pz r10 */
L8c649076: if((*sr&1u))goto L8c64907e;goto L8c649078; /* bt 0x8c64907e */
L8c649078: *stop=0x8c649078u;r[0]=sx(m,bus,&io_status,0x8c649122u,2);if(io_status)return io_status;goto L8c64907a; /* mov.w 0x8c649122,r0 */
L8c64907a: *stop=0x8c64907cu;goto L8c6490d2; /* bra 0x8c6490d2 */
L8c64907e: *stop=0x8c64907eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64913cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649080; /* mov.l 0x8c64913c,r2 */
L8c649080: *stop=0x8c649080u;r[5]=r[10];goto L8c649082; /* mov r10,r5 */
L8c649082: target=r[2];*pr=0x8c649086u;*stop=0x8c649084u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649086; /* jsr @r2 */
L8c649086: *stop=0x8c649086u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649140u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649088; /* mov.l 0x8c649140,r3 */
L8c649088: target=r[3];*pr=0x8c64908cu;*stop=0x8c64908au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64908c; /* jsr @r3 */
L8c64908c: *stop=0x8c64908cu;r[13]=r[0];goto L8c64908e; /* mov r0,r13 */
L8c64908e: *stop=0x8c64908eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c649090; /* tst r13,r13 */
L8c649090: if(!(*sr&1u))goto L8c649098;goto L8c649092; /* bf 0x8c649098 */
L8c649092: *stop=0x8c649092u;r[0]=sx(m,bus,&io_status,0x8c649124u,2);if(io_status)return io_status;goto L8c649094; /* mov.w 0x8c649124,r0 */
L8c649094: *stop=0x8c649096u;goto L8c6490d2; /* bra 0x8c6490d2 */
L8c649098: *stop=0x8c649098u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64909a; /* mov.l 0x8c649144,r2 */
L8c64909a: *stop=0x8c64909au;r[0]=r[11];goto L8c64909c; /* mov r11,r0 */
L8c64909c: *stop=0x8c64909cu;r[3]=1u;goto L8c64909e; /* mov #0x1,r3 */
L8c64909e: *stop=0x8c64909eu;r[4]=r[13];goto L8c6490a0; /* mov r13,r4 */
L8c6490a0: *stop=0x8c6490a0u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[11],4);if(io_status)return io_status;goto L8c6490a2; /* mov.l r11,@(0x8,r13) */
L8c6490a2: *stop=0x8c6490a2u;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6490a4; /* mov.l r3,@(0x4,r13) */
L8c6490a4: *stop=0x8c6490a4u;if(!valid_access(r[13]+44u,4u))return -3;wr(m,bus,&io_status,r[13]+0x2cu,r[10],4);if(io_status)return io_status;goto L8c6490a6; /* mov.l r10,@(0x2c,r13) */
L8c6490a6: *stop=0x8c6490a6u;if(!valid_access(r[13]+56u,4u))return -3;wr(m,bus,&io_status,r[13]+0x38u,r[2],4);if(io_status)return io_status;goto L8c6490a8; /* mov.l r2,@(0x38,r13) */
L8c6490a8: *stop=0x8c6490a8u;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[11],4);if(io_status)return io_status;goto L8c6490aa; /* mov.l r11,@(0x30,r13) */
L8c6490aa: *stop=0x8c6490aau;if(!valid_access(r[13]+40u,4u))return -3;wr(m,bus,&io_status,r[13]+0x28u,r[14],4);if(io_status)return io_status;goto L8c6490ac; /* mov.l r14,@(0x28,r13) */
L8c6490ac: *stop=0x8c6490acu;if(!valid_access(r[13]+2u,2u))return -3;wr(m,bus,&io_status,r[13]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6490ae; /* mov.w r0,@(0x2,r13) */
L8c6490ae: *stop=0x8c6490aeu;if(!valid_access(r[13]+1u,1u))return -3;wr(m,bus,&io_status,r[13]+0x1u,r[0],1);if(io_status)return io_status;goto L8c6490b0; /* mov.b r0,@(0x1,r13) */
L8c6490b0: *stop=0x8c6490b2u;r[4]+=16u;goto L8c6490ba; /* bra 0x8c6490ba */
L8c6490b4: *stop=0x8c6490b4u;if(!valid_access(r[12],1u))return -3;r[3]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[12]+=1;goto L8c6490b6; /* mov.b @r12+,r3 */
L8c6490b6: *stop=0x8c6490b6u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c6490b8; /* mov.b r3,@r4 */
L8c6490b8: *stop=0x8c6490b8u;r[4]+=1u;goto L8c6490ba; /* add #0x1,r4 */
L8c6490ba: *stop=0x8c6490bau;if(!valid_access(r[12],1u))return -3;r[2]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c6490bc; /* mov.b @r12,r2 */
L8c6490bc: *stop=0x8c6490bcu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6490be; /* tst r2,r2 */
L8c6490be: if(!(*sr&1u))goto L8c6490b4;goto L8c6490c0; /* bf 0x8c6490b4 */
L8c6490c0: *stop=0x8c6490c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649148u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6490c2; /* mov.l 0x8c649148,r3 */
L8c6490c2: *stop=0x8c6490c2u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[11],1);if(io_status)return io_status;goto L8c6490c4; /* mov.b r11,@r4 */
L8c6490c4: *stop=0x8c6490c4u;r[4]=r[13];goto L8c6490c6; /* mov r13,r4 */
L8c6490c6: *stop=0x8c6490c6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6490c8; /* mov.l @(0x4,r15),r5 */
L8c6490c8: target=r[3];*pr=0x8c6490ccu;*stop=0x8c6490cau;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6490cc; /* jsr @r3 */
L8c6490cc: *stop=0x8c6490ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6490ce; /* mov.l @r15,r2 */
L8c6490ce: *stop=0x8c6490ceu;r[0]=0u;goto L8c6490d0; /* mov #0x0,r0 */
L8c6490d0: *stop=0x8c6490d0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c6490d2; /* mov.l r13,@r2 */
L8c6490d2: *stop=0x8c6490d2u;r[15]+=52u;goto L8c6490d4; /* add #0x34,r15 */
L8c6490d4: *stop=0x8c6490d4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6490d6; /* lds.l @r15+,PR */
L8c6490d6: *stop=0x8c6490d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6490d8; /* mov.l @r15+,r10 */
L8c6490d8: *stop=0x8c6490d8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6490da; /* mov.l @r15+,r11 */
L8c6490da: *stop=0x8c6490dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6490dc; /* mov.l @r15+,r12 */
L8c6490dc: *stop=0x8c6490dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6490de; /* mov.l @r15+,r13 */
L8c6490de: *stop=0x8c6490deu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6490e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c648f9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648fca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648fcau);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648fd2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648fd2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c648fdc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648fdcu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649036(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649036u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649048(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649048u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649058(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649058u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649062(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649062u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649072(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649072u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649086(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649086u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64908c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64908cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6490cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c648f9a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6490ccu);}

static int sa_movie_filesystem_command12_run_8c6493c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c6493e0u)goto L8c6493e0;if(resume_pc==0x8c6493e8u)goto L8c6493e8;if(resume_pc==0x8c6493f2u)goto L8c6493f2;if(resume_pc==0x8c649412u)goto L8c649412;if(resume_pc==0x8c649424u)goto L8c649424;if(resume_pc==0x8c649434u)goto L8c649434;if(resume_pc==0x8c649450u)goto L8c649450;if(resume_pc==0x8c64945au)goto L8c64945a;if(resume_pc==0x8c6494a0u)goto L8c6494a0;if(resume_pc==0x8c6494b0u)goto L8c6494b0;if(resume_pc==0x8c6494c6u)goto L8c6494c6;goto L8c6493c6;
L8c6493c6: *stop=0x8c6493c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6493c8; /* mov.l r14,@-r15 */
L8c6493c8: *stop=0x8c6493c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6493ca; /* mov.l r13,@-r15 */
L8c6493ca: *stop=0x8c6493cau;r[13]=r[4];goto L8c6493cc; /* mov r4,r13 */
L8c6493cc: *stop=0x8c6493ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6493ce; /* mov.l r12,@-r15 */
L8c6493ce: *stop=0x8c6493ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6493d0; /* mov.l r11,@-r15 */
L8c6493d0: *stop=0x8c6493d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6493d2; /* mov.l r10,@-r15 */
L8c6493d2: *stop=0x8c6493d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6493d4; /* mov.l r9,@-r15 */
L8c6493d4: *stop=0x8c6493d4u;r[9]=r[5];goto L8c6493d6; /* mov r5,r9 */
L8c6493d6: *stop=0x8c6493d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6493d8; /* mov.l r8,@-r15 */
L8c6493d8: *stop=0x8c6493d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6493da; /* sts.l PR,@-r15 */
L8c6493da: *stop=0x8c6493dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c649470u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6493dc; /* mov.l 0x8c649470,r3 */
L8c6493dc: target=r[3];*pr=0x8c6493e0u;*stop=0x8c6493deu;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6493e0; /* jsr @r3 */
L8c6493e0: *stop=0x8c6493e0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6493e2; /* tst r0,r0 */
L8c6493e2: if(!(*sr&1u))goto L8c6493ec;goto L8c6493e4; /* bf 0x8c6493ec */
L8c6493e4: target=0x8c64850au;*pr=0x8c6493e8u;*stop=0x8c6493e6u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6493e8; /* bsr 0x8c64850a */
L8c6493e8: *stop=0x8c6493eau;goto L8c649406; /* bra 0x8c649406 */
L8c6493ec: *stop=0x8c6493ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649474u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6493ee; /* mov.l 0x8c649474,r2 */
L8c6493ee: target=r[2];*pr=0x8c6493f2u;*stop=0x8c6493f0u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6493f2; /* jsr @r2 */
L8c6493f2: *stop=0x8c6493f2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6493f4; /* tst r0,r0 */
L8c6493f4: if((*sr&1u))goto L8c649406;goto L8c6493f6; /* bt 0x8c649406 */
L8c6493f6: *stop=0x8c6493f6u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6493f8; /* mov.l @(0x28,r13),r2 */
L8c6493f8: *stop=0x8c6493f8u;r[3]=sx(m,bus,&io_status,0x8c649468u,2);if(io_status)return io_status;goto L8c6493fa; /* mov.w 0x8c649468,r3 */
L8c6493fa: *stop=0x8c6493fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c649478u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6493fc; /* mov.l 0x8c649478,r0 */
L8c6493fc: *stop=0x8c6493fcu;*macl=r[3]*r[2];goto L8c6493fe; /* mul.l r3,r2 */
L8c6493fe: *stop=0x8c6493feu;r[3]=*macl;goto L8c649400; /* sts MACL,r3 */
L8c649400: *stop=0x8c649400u;if(!valid_access(r[0]+r[3],1u))return -3;r[1]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c649402; /* mov.b @(r0,r3),r1 */
L8c649402: *stop=0x8c649402u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c649404; /* tst r1,r1 */
L8c649404: if(!(*sr&1u))goto L8c64940c;goto L8c649406; /* bf 0x8c64940c */
L8c649406: *stop=0x8c649406u;r[0]=sx(m,bus,&io_status,0x8c64946au,2);if(io_status)return io_status;goto L8c649408; /* mov.w 0x8c64946a,r0 */
L8c649408: *stop=0x8c64940au;goto L8c6494d0; /* bra 0x8c6494d0 */
L8c64940c: *stop=0x8c64940cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64947cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64940e; /* mov.l 0x8c64947c,r3 */
L8c64940e: target=r[3];*pr=0x8c649412u;*stop=0x8c649410u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649412; /* jsr @r3 */
L8c649412: *stop=0x8c649412u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649414; /* cmp/pz r0 */
L8c649414: if((*sr&1u))goto L8c64941c;goto L8c649416; /* bt 0x8c64941c */
L8c649416: *stop=0x8c649416u;r[0]=sx(m,bus,&io_status,0x8c64946cu,2);if(io_status)return io_status;goto L8c649418; /* mov.w 0x8c64946c,r0 */
L8c649418: *stop=0x8c64941au;goto L8c6494d0; /* bra 0x8c6494d0 */
L8c64941c: *stop=0x8c64941cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649480u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64941e; /* mov.l 0x8c649480,r3 */
L8c64941e: *stop=0x8c64941eu;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c649420; /* mov.l @(0x28,r13),r12 */
L8c649420: target=r[3];*pr=0x8c649424u;*stop=0x8c649422u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649424; /* jsr @r3 */
L8c649424: *stop=0x8c649424u;r[14]=r[0];goto L8c649426; /* mov r0,r14 */
L8c649426: *stop=0x8c649426u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c649428; /* cmp/pz r14 */
L8c649428: if(!(*sr&1u))goto L8c649460;goto L8c64942a; /* bf 0x8c649460 */
L8c64942a: *stop=0x8c64942au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649484u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64942c; /* mov.l 0x8c649484,r10 */
L8c64942c: *stop=0x8c64942cu;r[5]=r[14];goto L8c64942e; /* mov r14,r5 */
L8c64942e: *stop=0x8c64942eu;r[6]=r[9];goto L8c649430; /* mov r9,r6 */
L8c649430: target=r[10];*pr=0x8c649434u;*stop=0x8c649432u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649434; /* jsr @r10 */
L8c649434: *stop=0x8c649434u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649488u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c649436; /* mov.l 0x8c649488,r11 */
L8c649436: *stop=0x8c649436u;r[4]=r[0];goto L8c649438; /* mov r0,r4 */
L8c649438: *stop=0x8c649438u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64943a; /* cmp/pz r4 */
L8c64943a: if((*sr&1u))goto L8c6494a6;goto L8c64943c; /* bt 0x8c6494a6 */
L8c64943c: *stop=0x8c64943cu;r[3]=sx(m,bus,&io_status,0x8c64946au,2);if(io_status)return io_status;goto L8c64943e; /* mov.w 0x8c64946a,r3 */
L8c64943e: *stop=0x8c64943eu;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c649440; /* cmp/eq r3,r4 */
L8c649440: if(!(*sr&1u))goto L8c649446;goto L8c649442; /* bf 0x8c649446 */
L8c649442: *stop=0x8c649444u;r[0]=r[4];goto L8c6494d0; /* bra 0x8c6494d0 */
L8c649446: *stop=0x8c649446u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64948cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649448; /* mov.l 0x8c64948c,r2 */
L8c649448: *stop=0x8c649448u;r[5]=r[14];goto L8c64944a; /* mov r14,r5 */
L8c64944a: *stop=0x8c64944au;r[6]=r[11];goto L8c64944c; /* mov r11,r6 */
L8c64944c: target=r[2];*pr=0x8c649450u;*stop=0x8c64944eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649450; /* jsr @r2 */
L8c649450: *stop=0x8c649450u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649490u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c649452; /* mov.l 0x8c649490,r8 */
L8c649452: *stop=0x8c649452u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649494u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649454; /* mov.l 0x8c649494,r3 */
L8c649454: *stop=0x8c649454u;r[5]=r[8];goto L8c649456; /* mov r8,r5 */
L8c649456: target=r[3];*pr=0x8c64945au;*stop=0x8c649458u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64945a; /* jsr @r3 */
L8c64945a: *stop=0x8c64945au;r[14]=r[0];goto L8c64945c; /* mov r0,r14 */
L8c64945c: *stop=0x8c64945cu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64945e; /* cmp/pz r14 */
L8c64945e: if((*sr&1u))goto L8c649498;goto L8c649460; /* bt 0x8c649498 */
L8c649460: *stop=0x8c649460u;r[0]=sx(m,bus,&io_status,0x8c64946eu,2);if(io_status)return io_status;goto L8c649462; /* mov.w 0x8c64946e,r0 */
L8c649462: *stop=0x8c649464u;goto L8c6494d0; /* bra 0x8c6494d0 */
L8c649498: *stop=0x8c649498u;r[5]=r[14];goto L8c64949a; /* mov r14,r5 */
L8c64949a: *stop=0x8c64949au;r[6]=r[9];goto L8c64949c; /* mov r9,r6 */
L8c64949c: target=r[10];*pr=0x8c6494a0u;*stop=0x8c64949eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6494a0; /* jsr @r10 */
L8c6494a0: *stop=0x8c6494a0u;r[4]=r[0];goto L8c6494a2; /* mov r0,r4 */
L8c6494a2: *stop=0x8c6494a2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6494a4; /* tst r4,r4 */
L8c6494a4: if(!(*sr&1u))goto L8c649446;goto L8c6494a6; /* bf 0x8c649446 */
L8c6494a6: *stop=0x8c6494a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649588u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6494a8; /* mov.l 0x8c649588,r3 */
L8c6494a8: *stop=0x8c6494a8u;r[5]=r[14];goto L8c6494aa; /* mov r14,r5 */
L8c6494aa: *stop=0x8c6494aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c649584u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6494ac; /* mov.l 0x8c649584,r6 */
L8c6494ac: target=r[3];*pr=0x8c6494b0u;*stop=0x8c6494aeu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6494b0; /* jsr @r3 */
L8c6494b0: *stop=0x8c6494b0u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6494b2; /* mov.l @(0x38,r13),r2 */
L8c6494b2: *stop=0x8c6494b2u;*sr=(*sr&~1u)|(r[11]==r[2]);goto L8c6494b4; /* cmp/eq r11,r2 */
L8c6494b4: take=!(*sr&1u);*stop=0x8c6494b6u;r[10]=(uint32_t)(int32_t)(int16_t)r[14];if(take)goto L8c6494bc;goto L8c6494b8; /* bf/s 0x8c6494bc */
L8c6494b8: *stop=0x8c6494bau;if(!valid_access(r[13]+56u,4u))return -3;wr(m,bus,&io_status,r[13]+0x38u,r[10],4);if(io_status)return io_status;goto L8c6494c6; /* bra 0x8c6494c6 */
L8c6494bc: *stop=0x8c6494bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649588u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6494be; /* mov.l 0x8c649588,r3 */
L8c6494be: *stop=0x8c6494beu;r[6]=r[14];goto L8c6494c0; /* mov r14,r6 */
L8c6494c0: *stop=0x8c6494c0u;if(!valid_access(r[13]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x3cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6494c2; /* mov.l @(0x3c,r13),r5 */
L8c6494c2: target=r[3];*pr=0x8c6494c6u;*stop=0x8c6494c4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6494c6; /* jsr @r3 */
L8c6494c6: *stop=0x8c6494c6u;r[0]=0u;goto L8c6494c8; /* mov #0x0,r0 */
L8c6494c8: *stop=0x8c6494c8u;if(!valid_access(r[13]+60u,4u))return -3;wr(m,bus,&io_status,r[13]+0x3cu,r[10],4);if(io_status)return io_status;goto L8c6494ca; /* mov.l r10,@(0x3c,r13) */
L8c6494ca: *stop=0x8c6494cau;if(!valid_access(r[13]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6494cc; /* mov.l @(0x30,r13),r2 */
L8c6494cc: *stop=0x8c6494ccu;r[2]+=1u;goto L8c6494ce; /* add #0x1,r2 */
L8c6494ce: *stop=0x8c6494ceu;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[2],4);if(io_status)return io_status;goto L8c6494d0; /* mov.l r2,@(0x30,r13) */
L8c6494d0: *stop=0x8c6494d0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6494d2; /* lds.l @r15+,PR */
L8c6494d2: *stop=0x8c6494d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6494d4; /* mov.l @r15+,r8 */
L8c6494d4: *stop=0x8c6494d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6494d6; /* mov.l @r15+,r9 */
L8c6494d6: *stop=0x8c6494d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6494d8; /* mov.l @r15+,r10 */
L8c6494d8: *stop=0x8c6494d8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6494da; /* mov.l @r15+,r11 */
L8c6494da: *stop=0x8c6494dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6494dc; /* mov.l @r15+,r12 */
L8c6494dc: *stop=0x8c6494dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6494de; /* mov.l @r15+,r13 */
L8c6494de: *stop=0x8c6494deu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6494e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c6493c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6493e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6493e0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6493e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6493e8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6493f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6493f2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649412(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649412u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649424(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649424u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649434(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649434u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c649450(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649450u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64945a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64945au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6494a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6494a0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6494b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6494b0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c6494c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c6493c6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6494c6u);}

static int sa_movie_filesystem_command12_run_8c64b82c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b852u)goto L8c64b852;if(resume_pc==0x8c64b898u)goto L8c64b898;if(resume_pc==0x8c64b8aeu)goto L8c64b8ae;if(resume_pc==0x8c64b8b8u)goto L8c64b8b8;if(resume_pc==0x8c64b8deu)goto L8c64b8de;if(resume_pc==0x8c64b8ecu)goto L8c64b8ec;if(resume_pc==0x8c64b906u)goto L8c64b906;goto L8c64b82c;
L8c64b82c: *stop=0x8c64b82cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b82e; /* mov.l r14,@-r15 */
L8c64b82e: *stop=0x8c64b82eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b830; /* mov.l r13,@-r15 */
L8c64b830: *stop=0x8c64b830u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b832; /* mov.l r12,@-r15 */
L8c64b832: *stop=0x8c64b832u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b834; /* mov.l r11,@-r15 */
L8c64b834: *stop=0x8c64b834u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b836; /* mov.l r10,@-r15 */
L8c64b836: *stop=0x8c64b836u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64b838; /* mov.l r9,@-r15 */
L8c64b838: *stop=0x8c64b838u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64b83a; /* mov.l r8,@-r15 */
L8c64b83a: *stop=0x8c64b83au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b83c; /* sts.l PR,@-r15 */
L8c64b83c: *stop=0x8c64b83cu;r[15]+=4294967284u;goto L8c64b83e; /* add #-0xc,r15 */
L8c64b83e: *stop=0x8c64b83eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64b840; /* mov.l r5,@(0x4,r15) */
L8c64b840: *stop=0x8c64b840u;r[12]=r[7];goto L8c64b842; /* mov r7,r12 */
L8c64b842: *stop=0x8c64b842u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c64b844; /* mov.l r6,@(0x8,r15) */
L8c64b844: *stop=0x8c64b844u;r[9]=r[4];goto L8c64b846; /* mov r4,r9 */
L8c64b846: *stop=0x8c64b846u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64b848; /* mov.l @(0x2c,r15),r6 */
L8c64b848: *stop=0x8c64b848u;r[7]=r[15];goto L8c64b84a; /* mov r15,r7 */
L8c64b84a: *stop=0x8c64b84au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b84c; /* mov.l @(0x4,r15),r5 */
L8c64b84c: *stop=0x8c64b84cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b94cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b84e; /* mov.l 0x8c64b94c,r3 */
L8c64b84e: target=r[3];*pr=0x8c64b852u;*stop=0x8c64b850u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b852; /* jsr @r3 */
L8c64b852: *stop=0x8c64b852u;r[14]=r[0];goto L8c64b854; /* mov r0,r14 */
L8c64b854: *stop=0x8c64b854u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b856; /* cmp/pz r14 */
L8c64b856: if(!(*sr&1u))goto L8c64b906;goto L8c64b858; /* bf 0x8c64b906 */
L8c64b858: *stop=0x8c64b858u;r[4]=sx(m,bus,&io_status,0x8c64b946u,2);if(io_status)return io_status;goto L8c64b85a; /* mov.w 0x8c64b946,r4 */
L8c64b85a: *stop=0x8c64b85au;r[2]=1u;goto L8c64b85c; /* mov #0x1,r2 */
L8c64b85c: *stop=0x8c64b85cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b85e; /* mov.l @r15,r3 */
L8c64b85e: *stop=0x8c64b85eu;r[1]=r[12];goto L8c64b860; /* mov r12,r1 */
L8c64b860: *stop=0x8c64b860u;*macl=r[4]*r[9];goto L8c64b862; /* mul.l r4,r9 */
L8c64b862: *stop=0x8c64b862u;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b864; /* mov.l @(0x30,r15),r0 */
L8c64b864: *stop=0x8c64b864u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c64b866; /* shll r1 */
L8c64b866: *stop=0x8c64b866u;r[10]=0u;goto L8c64b868; /* mov #0x0,r10 */
L8c64b868: *stop=0x8c64b868u;r[13]=sx(m,bus,&io_status,0x8c64b948u,2);if(io_status)return io_status;goto L8c64b86a; /* mov.w 0x8c64b948,r13 */
L8c64b86a: *stop=0x8c64b86au;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64b86c; /* mov.b r0,@(0x1,r3) */
L8c64b86c: *stop=0x8c64b86cu;r[4]=*macl;goto L8c64b86e; /* sts MACL,r4 */
L8c64b86e: *stop=0x8c64b86eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b950u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b870; /* mov.l 0x8c64b950,r3 */
L8c64b870: *stop=0x8c64b870u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64b872; /* mov.l @(0x8,r15),r11 */
L8c64b872: *stop=0x8c64b872u;r[4]+=r[3];goto L8c64b874; /* add r3,r4 */
L8c64b874: *stop=0x8c64b874u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64b876; /* mov.l r2,@(0x30,r4) */
L8c64b876: *stop=0x8c64b876u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b878; /* mov.l @(0x4,r4),r2 */
L8c64b878: *stop=0x8c64b878u;r[0]=r[2];goto L8c64b87a; /* mov r2,r0 */
L8c64b87a: *stop=0x8c64b87au;r[2]<<=2;goto L8c64b87c; /* shll2 r2 */
L8c64b87c: *stop=0x8c64b87cu;r[2]+=r[0];goto L8c64b87e; /* add r0,r2 */
L8c64b87e: *stop=0x8c64b87eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b954u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b880; /* mov.l 0x8c64b954,r0 */
L8c64b880: *stop=0x8c64b880u;r[2]<<=2;goto L8c64b882; /* shll2 r2 */
L8c64b882: *stop=0x8c64b882u;r[2]<<=2;goto L8c64b884; /* shll2 r2 */
L8c64b884: *stop=0x8c64b884u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b886; /* mov.l @(r0,r2),r2 */
L8c64b886: *stop=0x8c64b886u;r[1]+=r[2];goto L8c64b888; /* add r2,r1 */
L8c64b888: *stop=0x8c64b888u;r[2]=r[10];goto L8c64b88a; /* mov r10,r2 */
L8c64b88a: *stop=0x8c64b88au;*sr=(*sr&~1u)|(r[2]>=r[12]);goto L8c64b88c; /* cmp/hs r12,r2 */
L8c64b88c: take=(*sr&1u);*stop=0x8c64b88eu;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[1],4);if(io_status)return io_status;if(take)goto L8c64b8a6;goto L8c64b890; /* bt/s 0x8c64b8a6 */
L8c64b890: *stop=0x8c64b890u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b958u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b892; /* mov.l 0x8c64b958,r3 */
L8c64b892: *stop=0x8c64b892u;r[5]=r[11];goto L8c64b894; /* mov r11,r5 */
L8c64b894: target=r[3];*pr=0x8c64b898u;*stop=0x8c64b896u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b898; /* jsr @r3 */
L8c64b898: *stop=0x8c64b898u;r[14]=r[0];goto L8c64b89a; /* mov r0,r14 */
L8c64b89a: *stop=0x8c64b89au;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b89c; /* cmp/pz r14 */
L8c64b89c: if(!(*sr&1u))goto L8c64b8fe;goto L8c64b89e; /* bf 0x8c64b8fe */
L8c64b89e: *stop=0x8c64b89eu;r[10]+=1u;goto L8c64b8a0; /* add #0x1,r10 */
L8c64b8a0: *stop=0x8c64b8a0u;*sr=(*sr&~1u)|(r[10]>=r[12]);goto L8c64b8a2; /* cmp/hs r12,r10 */
L8c64b8a2: take=!(*sr&1u);*stop=0x8c64b8a4u;r[11]+=r[13];if(take)goto L8c64b890;goto L8c64b8a6; /* bf/s 0x8c64b890 */
L8c64b8a6: *stop=0x8c64b8a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b95cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b8a8; /* mov.l 0x8c64b95c,r3 */
L8c64b8a8: *stop=0x8c64b8a8u;r[5]=0u;goto L8c64b8aa; /* mov #0x0,r5 */
L8c64b8aa: target=r[3];*pr=0x8c64b8aeu;*stop=0x8c64b8acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b8ae; /* jsr @r3 */
L8c64b8ae: *stop=0x8c64b8aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b960u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b8b0; /* mov.l 0x8c64b960,r3 */
L8c64b8b0: *stop=0x8c64b8b0u;r[6]=r[15];goto L8c64b8b2; /* mov r15,r6 */
L8c64b8b2: *stop=0x8c64b8b2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b8b4; /* mov.l @(0x4,r15),r5 */
L8c64b8b4: target=r[3];*pr=0x8c64b8b8u;*stop=0x8c64b8b6u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b8b8; /* jsr @r3 */
L8c64b8b8: *stop=0x8c64b8b8u;r[14]=r[0];goto L8c64b8ba; /* mov r0,r14 */
L8c64b8ba: *stop=0x8c64b8bau;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b8bc; /* cmp/pz r14 */
L8c64b8bc: if(!(*sr&1u))goto L8c64b906;goto L8c64b8be; /* bf 0x8c64b906 */
L8c64b8be: *stop=0x8c64b8beu;r[8]=r[9];goto L8c64b8c0; /* mov r9,r8 */
L8c64b8c0: *stop=0x8c64b8c0u;r[8]<<=8;goto L8c64b8c2; /* shll8 r8 */
L8c64b8c2: *stop=0x8c64b8c2u;r[11]=0u;goto L8c64b8c4; /* mov #0x0,r11 */
L8c64b8c4: *stop=0x8c64b8c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b964u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b8c6; /* mov.l 0x8c64b964,r3 */
L8c64b8c6: *stop=0x8c64b8c6u;r[2]=r[11];goto L8c64b8c8; /* mov r11,r2 */
L8c64b8c8: *stop=0x8c64b8c8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64b8ca; /* mov.l @(0x8,r15),r10 */
L8c64b8ca: *stop=0x8c64b8cau;r[8]<<=2;goto L8c64b8cc; /* shll2 r8 */
L8c64b8cc: *stop=0x8c64b8ccu;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto L8c64b8ce; /* shll r8 */
L8c64b8ce: *stop=0x8c64b8ceu;*sr=(*sr&~1u)|(r[2]>=r[12]);goto L8c64b8d0; /* cmp/hs r12,r2 */
L8c64b8d0: take=(*sr&1u);*stop=0x8c64b8d2u;r[8]+=r[3];if(take)goto L8c64b8fc;goto L8c64b8d4; /* bt/s 0x8c64b8fc */
L8c64b8d4: *stop=0x8c64b8d4u;r[9]=sx(m,bus,&io_status,0x8c64b94au,2);if(io_status)return io_status;goto L8c64b8d6; /* mov.w 0x8c64b94a,r9 */
L8c64b8d6: *stop=0x8c64b8d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b968u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b8d8; /* mov.l 0x8c64b968,r3 */
L8c64b8d8: *stop=0x8c64b8d8u;r[5]=r[8];goto L8c64b8da; /* mov r8,r5 */
L8c64b8da: target=r[3];*pr=0x8c64b8deu;*stop=0x8c64b8dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b8de; /* jsr @r3 */
L8c64b8de: *stop=0x8c64b8deu;r[14]=r[0];goto L8c64b8e0; /* mov r0,r14 */
L8c64b8e0: *stop=0x8c64b8e0u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b8e2; /* cmp/pz r14 */
L8c64b8e2: if(!(*sr&1u))goto L8c64b8fe;goto L8c64b8e4; /* bf 0x8c64b8fe */
L8c64b8e4: *stop=0x8c64b8e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b96cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b8e6; /* mov.l 0x8c64b96c,r2 */
L8c64b8e6: *stop=0x8c64b8e6u;r[5]=r[10];goto L8c64b8e8; /* mov r10,r5 */
L8c64b8e8: target=r[2];*pr=0x8c64b8ecu;*stop=0x8c64b8eau;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b8ec; /* jsr @r2 */
L8c64b8ec: *stop=0x8c64b8ecu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64b8ee; /* tst r0,r0 */
L8c64b8ee: if((*sr&1u))goto L8c64b8f4;goto L8c64b8f0; /* bt 0x8c64b8f4 */
L8c64b8f0: *stop=0x8c64b8f2u;r[14]=r[9];goto L8c64b8fe; /* bra 0x8c64b8fe */
L8c64b8f4: *stop=0x8c64b8f4u;r[11]+=1u;goto L8c64b8f6; /* add #0x1,r11 */
L8c64b8f6: *stop=0x8c64b8f6u;*sr=(*sr&~1u)|(r[11]>=r[12]);goto L8c64b8f8; /* cmp/hs r12,r11 */
L8c64b8f8: take=!(*sr&1u);*stop=0x8c64b8fau;r[10]+=r[13];if(take)goto L8c64b8d6;goto L8c64b8fc; /* bf/s 0x8c64b8d6 */
L8c64b8fc: *stop=0x8c64b8fcu;r[14]=0u;goto L8c64b8fe; /* mov #0x0,r14 */
L8c64b8fe: *stop=0x8c64b8feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b95cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b900; /* mov.l 0x8c64b95c,r3 */
L8c64b900: *stop=0x8c64b900u;r[5]=0u;goto L8c64b902; /* mov #0x0,r5 */
L8c64b902: target=r[3];*pr=0x8c64b906u;*stop=0x8c64b904u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b906; /* jsr @r3 */
L8c64b906: *stop=0x8c64b906u;r[0]=r[14];goto L8c64b908; /* mov r14,r0 */
L8c64b908: *stop=0x8c64b908u;r[15]+=12u;goto L8c64b90a; /* add #0xc,r15 */
L8c64b90a: *stop=0x8c64b90au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b90c; /* lds.l @r15+,PR */
L8c64b90c: *stop=0x8c64b90cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64b90e; /* mov.l @r15+,r8 */
L8c64b90e: *stop=0x8c64b90eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64b910; /* mov.l @r15+,r9 */
L8c64b910: *stop=0x8c64b910u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64b912; /* mov.l @r15+,r10 */
L8c64b912: *stop=0x8c64b912u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b914; /* mov.l @r15+,r11 */
L8c64b914: *stop=0x8c64b914u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b916; /* mov.l @r15+,r12 */
L8c64b916: *stop=0x8c64b916u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b918; /* mov.l @r15+,r13 */
L8c64b918: *stop=0x8c64b918u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b91au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64b82c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b852(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b852u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b898(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b898u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b8ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b8aeu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b8b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b8b8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b8de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b8deu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b8ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b8ecu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b906(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b82c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b906u);}

static int sa_movie_filesystem_command12_run_8c64b12a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b150u)goto L8c64b150;if(resume_pc==0x8c64b16au)goto L8c64b16a;goto L8c64b12a;
L8c64b12a: *stop=0x8c64b12au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b12c; /* mov.l r14,@-r15 */
L8c64b12c: *stop=0x8c64b12cu;r[14]=0u;goto L8c64b12e; /* mov #0x0,r14 */
L8c64b12e: *stop=0x8c64b12eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b130; /* mov.l r13,@-r15 */
L8c64b130: *stop=0x8c64b130u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b132; /* mov.l r12,@-r15 */
L8c64b132: *stop=0x8c64b132u;r[12]=r[4];goto L8c64b134; /* mov r4,r12 */
L8c64b134: *stop=0x8c64b134u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b136; /* mov.l r11,@-r15 */
L8c64b136: *stop=0x8c64b136u;r[11]=sx(m,bus,&io_status,0x8c64b1e0u,2);if(io_status)return io_status;goto L8c64b138; /* mov.w 0x8c64b1e0,r11 */
L8c64b138: *stop=0x8c64b138u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b13a; /* mov.l r10,@-r15 */
L8c64b13a: *stop=0x8c64b13au;*macl=r[11]*r[12];goto L8c64b13c; /* mul.l r11,r12 */
L8c64b13c: *stop=0x8c64b13cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b13e; /* mov.l 0x8c64b1e8,r3 */
L8c64b13e: *stop=0x8c64b13eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b140; /* sts.l PR,@-r15 */
L8c64b140: *stop=0x8c64b140u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1ecu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64b142; /* mov.l 0x8c64b1ec,r10 */
L8c64b142: *stop=0x8c64b142u;r[11]=*macl;goto L8c64b144; /* sts MACL,r11 */
L8c64b144: *stop=0x8c64b144u;r[11]+=r[3];goto L8c64b146; /* add r3,r11 */
L8c64b146: *stop=0x8c64b148u;r[13]=r[5]&65535u;goto L8c64b176; /* bra 0x8c64b176 */
L8c64b14a: *stop=0x8c64b14au;r[5]=r[14];goto L8c64b14c; /* mov r14,r5 */
L8c64b14c: target=0x8c64b274u;*pr=0x8c64b150u;*stop=0x8c64b14eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b150; /* bsr 0x8c64b274 */
L8c64b150: *stop=0x8c64b150u;r[4]=r[0];goto L8c64b152; /* mov r0,r4 */
L8c64b152: *stop=0x8c64b152u;if(!valid_access(r[4],1u))return -3;r[5]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c64b154; /* mov.b @r4,r5 */
L8c64b154: *stop=0x8c64b154u;r[0]=r[5]&255u;goto L8c64b156; /* extu.b r5,r0 */
L8c64b156: *stop=0x8c64b156u;*sr=(*sr&~1u)|(r[0]==0x00000033u);goto L8c64b158; /* cmp/eq #0x33,r0 */
L8c64b158: take=(*sr&1u);*stop=0x8c64b15au;r[5]=r[0];if(take)goto L8c64b162;goto L8c64b15c; /* bt/s 0x8c64b162 */
L8c64b15c: *stop=0x8c64b15cu;r[3]=sx(m,bus,&io_status,0x8c64b1e2u,2);if(io_status)return io_status;goto L8c64b15e; /* mov.w 0x8c64b1e2,r3 */
L8c64b15e: *stop=0x8c64b15eu;*sr=(*sr&~1u)|(r[3]==r[5]);goto L8c64b160; /* cmp/eq r3,r5 */
L8c64b160: if(!(*sr&1u))goto L8c64b174;goto L8c64b162; /* bf 0x8c64b174 */
L8c64b162: *stop=0x8c64b162u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b164; /* mov.l @r10,r1 */
L8c64b164: *stop=0x8c64b164u;if(!valid_access(r[1]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b166; /* mov.l @(0xc,r1),r3 */
L8c64b166: target=r[3];*pr=0x8c64b16au;*stop=0x8c64b168u;r[5]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b16a; /* jsr @r3 */
L8c64b16a: *stop=0x8c64b16au;r[4]=r[0]&65535u;goto L8c64b16c; /* extu.w r0,r4 */
L8c64b16c: *stop=0x8c64b16cu;*sr=(*sr&~1u)|(r[4]==r[13]);goto L8c64b16e; /* cmp/eq r4,r13 */
L8c64b16e: if(!(*sr&1u))goto L8c64b174;goto L8c64b170; /* bf 0x8c64b174 */
L8c64b170: *stop=0x8c64b172u;r[0]=4294967295u;goto L8c64b180; /* bra 0x8c64b180 */
L8c64b174: *stop=0x8c64b174u;r[14]+=1u;goto L8c64b176; /* add #0x1,r14 */
L8c64b176: *stop=0x8c64b176u;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b178; /* mov.l @(0x1c,r11),r3 */
L8c64b178: *stop=0x8c64b178u;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b17a; /* mov.l @(0x30,r3),r2 */
L8c64b17a: *stop=0x8c64b17au;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[2]);goto L8c64b17c; /* cmp/ge r2,r14 */
L8c64b17c: if(!(*sr&1u))goto L8c64b14a;goto L8c64b17e; /* bf 0x8c64b14a */
L8c64b17e: *stop=0x8c64b17eu;r[0]=r[13];goto L8c64b180; /* mov r13,r0 */
L8c64b180: *stop=0x8c64b180u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b182; /* lds.l @r15+,PR */
L8c64b182: *stop=0x8c64b182u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64b184; /* mov.l @r15+,r10 */
L8c64b184: *stop=0x8c64b184u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b186; /* mov.l @r15+,r11 */
L8c64b186: *stop=0x8c64b186u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b188; /* mov.l @r15+,r12 */
L8c64b188: *stop=0x8c64b188u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b18a; /* mov.l @r15+,r13 */
L8c64b18a: *stop=0x8c64b18au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b18cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64b12a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b12a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b150(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b12a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b150u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b16a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b12a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b16au);}

SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64ae04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64ae04;
L8c64ae04: *stop=0x8c64ae04u;r[6]=sx(m,bus,&io_status,0x8c64aebau,2);if(io_status)return io_status;goto L8c64ae06; /* mov.w 0x8c64aeba,r6 */
L8c64ae06: *stop=0x8c64ae06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ae08; /* mov.l 0x8c64aec0,r3 */
L8c64ae08: *stop=0x8c64ae08u;*macl=r[6]*r[4];goto L8c64ae0a; /* mul.l r6,r4 */
L8c64ae0a: *stop=0x8c64ae0au;r[6]=*macl;goto L8c64ae0c; /* sts MACL,r6 */
L8c64ae0c: *stop=0x8c64ae0cu;r[6]+=r[3];goto L8c64ae0e; /* add r3,r6 */
L8c64ae0e: *stop=0x8c64ae0eu;if(!valid_access(r[6]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x28u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae10; /* mov.l @(0x28,r6),r5 */
L8c64ae10: *stop=0x8c64ae12u;r[4]=0u;goto L8c64ae24; /* bra 0x8c64ae24 */
L8c64ae14: *stop=0x8c64ae14u;if(!valid_access(r[5],1u))return -3;r[7]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c64ae16; /* mov.b @r5,r7 */
L8c64ae16: *stop=0x8c64ae16u;r[7]=r[7]&255u;goto L8c64ae18; /* extu.b r7,r7 */
L8c64ae18: *stop=0x8c64ae18u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c64ae1a; /* tst r7,r7 */
L8c64ae1a: if(!(*sr&1u))goto L8c64ae20;goto L8c64ae1c; /* bf 0x8c64ae20 */
L8c64ae1c: *stop=0x8c64ae1cu;if(*pr!=entry_pr)return -3;*stop=0x8c64ae1eu;r[0]=r[4];return 0; /* rts */
L8c64ae20: *stop=0x8c64ae20u;r[5]+=32u;goto L8c64ae22; /* add #0x20,r5 */
L8c64ae22: *stop=0x8c64ae22u;r[4]+=1u;goto L8c64ae24; /* add #0x1,r4 */
L8c64ae24: *stop=0x8c64ae24u;if(!valid_access(r[6]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ae26; /* mov.l @(0x1c,r6),r2 */
L8c64ae26: *stop=0x8c64ae26u;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ae28; /* mov.l @(0x30,r2),r3 */
L8c64ae28: *stop=0x8c64ae28u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c64ae2a; /* cmp/ge r3,r4 */
L8c64ae2a: if(!(*sr&1u))goto L8c64ae14;goto L8c64ae2c; /* bf 0x8c64ae14 */
L8c64ae2c: *stop=0x8c64ae2cu;r[0]=4294967295u;goto L8c64ae2e; /* mov #-0x1,r0 */
L8c64ae2e: *stop=0x8c64ae2eu;if(*pr!=entry_pr)return -3;*stop=0x8c64ae30u;return 0; /* rts */
}

static int sa_movie_filesystem_command12_run_8c64b3ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b406u)goto L8c64b406;if(resume_pc==0x8c64b40au)goto L8c64b40a;if(resume_pc==0x8c64b41au)goto L8c64b41a;if(resume_pc==0x8c64b41eu)goto L8c64b41e;if(resume_pc==0x8c64b42au)goto L8c64b42a;if(resume_pc==0x8c64b436u)goto L8c64b436;if(resume_pc==0x8c64b442u)goto L8c64b442;if(resume_pc==0x8c64b44eu)goto L8c64b44e;if(resume_pc==0x8c64b45au)goto L8c64b45a;goto L8c64b3ec;
L8c64b3ec: *stop=0x8c64b3ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b3ee; /* mov.l r14,@-r15 */
L8c64b3ee: *stop=0x8c64b3eeu;r[14]=r[4];goto L8c64b3f0; /* mov r4,r14 */
L8c64b3f0: *stop=0x8c64b3f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b3f2; /* mov.l r13,@-r15 */
L8c64b3f2: *stop=0x8c64b3f2u;r[13]=r[5];goto L8c64b3f4; /* mov r5,r13 */
L8c64b3f4: *stop=0x8c64b3f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64b3f6; /* mov.l r8,@-r15 */
L8c64b3f6: *stop=0x8c64b3f6u;r[8]=r[4];goto L8c64b3f8; /* mov r4,r8 */
L8c64b3f8: *stop=0x8c64b3f8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b3fa; /* sts.l PR,@-r15 */
L8c64b3fa: *stop=0x8c64b3fau;if(!valid_access(r[13],2u))return -3;r[1]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c64b3fc; /* mov.w @r13,r1 */
L8c64b3fc: *stop=0x8c64b3fcu;r[14]+=1u;goto L8c64b3fe; /* add #0x1,r14 */
L8c64b3fe: *stop=0x8c64b3feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b4d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b400; /* mov.l 0x8c64b4d4,r3 */
L8c64b400: *stop=0x8c64b400u;r[1]=r[1]&65535u;goto L8c64b402; /* extu.w r1,r1 */
L8c64b402: target=r[3];*pr=0x8c64b406u;*stop=0x8c64b404u;r[0]=100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b406; /* jsr @r3 */
L8c64b406: target=0x8c64b390u;*pr=0x8c64b40au;*stop=0x8c64b408u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b40a; /* bsr 0x8c64b390 */
L8c64b40a: *stop=0x8c64b40au;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b40c; /* mov.b r0,@r8 */
L8c64b40c: *stop=0x8c64b40cu;r[8]=r[14];goto L8c64b40e; /* mov r14,r8 */
L8c64b40e: *stop=0x8c64b40eu;if(!valid_access(r[13],2u))return -3;r[1]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c64b410; /* mov.w @r13,r1 */
L8c64b410: *stop=0x8c64b410u;r[14]+=1u;goto L8c64b412; /* add #0x1,r14 */
L8c64b412: *stop=0x8c64b412u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b4d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b414; /* mov.l 0x8c64b4d8,r3 */
L8c64b414: *stop=0x8c64b414u;r[1]=r[1]&65535u;goto L8c64b416; /* extu.w r1,r1 */
L8c64b416: target=r[3];*pr=0x8c64b41au;*stop=0x8c64b418u;r[0]=100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b41a; /* jsr @r3 */
L8c64b41a: target=0x8c64b390u;*pr=0x8c64b41eu;*stop=0x8c64b41cu;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b41e; /* bsr 0x8c64b390 */
L8c64b41e: *stop=0x8c64b41eu;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b420; /* mov.b r0,@r8 */
L8c64b420: *stop=0x8c64b420u;r[8]=r[14];goto L8c64b422; /* mov r14,r8 */
L8c64b422: *stop=0x8c64b422u;if(!valid_access(r[13]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,1);if(io_status)return io_status;goto L8c64b424; /* mov.b @(0x2,r13),r0 */
L8c64b424: *stop=0x8c64b424u;r[14]+=1u;goto L8c64b426; /* add #0x1,r14 */
L8c64b426: target=0x8c64b390u;*pr=0x8c64b42au;*stop=0x8c64b428u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b42a; /* bsr 0x8c64b390 */
L8c64b42a: *stop=0x8c64b42au;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b42c; /* mov.b r0,@r8 */
L8c64b42c: *stop=0x8c64b42cu;r[8]=r[14];goto L8c64b42e; /* mov r14,r8 */
L8c64b42e: *stop=0x8c64b42eu;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c64b430; /* mov.b @(0x3,r13),r0 */
L8c64b430: *stop=0x8c64b430u;r[14]+=1u;goto L8c64b432; /* add #0x1,r14 */
L8c64b432: target=0x8c64b390u;*pr=0x8c64b436u;*stop=0x8c64b434u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b436; /* bsr 0x8c64b390 */
L8c64b436: *stop=0x8c64b436u;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b438; /* mov.b r0,@r8 */
L8c64b438: *stop=0x8c64b438u;r[8]=r[14];goto L8c64b43a; /* mov r14,r8 */
L8c64b43a: *stop=0x8c64b43au;if(!valid_access(r[13]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,1);if(io_status)return io_status;goto L8c64b43c; /* mov.b @(0x4,r13),r0 */
L8c64b43c: *stop=0x8c64b43cu;r[14]+=1u;goto L8c64b43e; /* add #0x1,r14 */
L8c64b43e: target=0x8c64b390u;*pr=0x8c64b442u;*stop=0x8c64b440u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b442; /* bsr 0x8c64b390 */
L8c64b442: *stop=0x8c64b442u;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b444; /* mov.b r0,@r8 */
L8c64b444: *stop=0x8c64b444u;r[8]=r[14];goto L8c64b446; /* mov r14,r8 */
L8c64b446: *stop=0x8c64b446u;if(!valid_access(r[13]+5u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x5u,1);if(io_status)return io_status;goto L8c64b448; /* mov.b @(0x5,r13),r0 */
L8c64b448: *stop=0x8c64b448u;r[14]+=1u;goto L8c64b44a; /* add #0x1,r14 */
L8c64b44a: target=0x8c64b390u;*pr=0x8c64b44eu;*stop=0x8c64b44cu;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b44e; /* bsr 0x8c64b390 */
L8c64b44e: *stop=0x8c64b44eu;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b450; /* mov.b r0,@r8 */
L8c64b450: *stop=0x8c64b450u;r[8]=r[14];goto L8c64b452; /* mov r14,r8 */
L8c64b452: *stop=0x8c64b452u;if(!valid_access(r[13]+6u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x6u,1);if(io_status)return io_status;goto L8c64b454; /* mov.b @(0x6,r13),r0 */
L8c64b454: *stop=0x8c64b454u;r[14]+=1u;goto L8c64b456; /* add #0x1,r14 */
L8c64b456: target=0x8c64b390u;*pr=0x8c64b45au;*stop=0x8c64b458u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b45a; /* bsr 0x8c64b390 */
L8c64b45a: *stop=0x8c64b45au;if(!valid_access(r[8],1u))return -3;wr(m,bus,&io_status,r[8],r[0],1);if(io_status)return io_status;goto L8c64b45c; /* mov.b r0,@r8 */
L8c64b45c: *stop=0x8c64b45cu;if(!valid_access(r[13]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x7u,1);if(io_status)return io_status;goto L8c64b45e; /* mov.b @(0x7,r13),r0 */
L8c64b45e: *stop=0x8c64b45eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b4dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b460; /* mov.l 0x8c64b4dc,r1 */
L8c64b460: *stop=0x8c64b460u;r[0]=r[0]&255u;goto L8c64b462; /* extu.b r0,r0 */
L8c64b462: *stop=0x8c64b462u;r[0]&=0x7u;goto L8c64b464; /* and #0x7,r0 */
L8c64b464: *stop=0x8c64b464u;if(!valid_access(r[0]+r[1],1u))return -3;r[3]=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;goto L8c64b466; /* mov.b @(r0,r1),r3 */
L8c64b466: *stop=0x8c64b466u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[3],1);if(io_status)return io_status;goto L8c64b468; /* mov.b r3,@r14 */
L8c64b468: *stop=0x8c64b468u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b46a; /* lds.l @r15+,PR */
L8c64b46a: *stop=0x8c64b46au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64b46c; /* mov.l @r15+,r8 */
L8c64b46c: *stop=0x8c64b46cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b46e; /* mov.l @r15+,r13 */
L8c64b46e: *stop=0x8c64b46eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b470u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64b3ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b406(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b406u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b40a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b40au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b41a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b41au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b41e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b41eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b42a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b42au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b436(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b436u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b442(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b442u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b44e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b44eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b45a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b3ec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b45au);}

static int sa_movie_filesystem_command12_run_8c64b0e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b0fcu)goto L8c64b0fc;if(resume_pc==0x8c64b10cu)goto L8c64b10c;if(resume_pc==0x8c64b118u)goto L8c64b118;goto L8c64b0e4;
L8c64b0e4: *stop=0x8c64b0e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b0e6; /* mov.l r14,@-r15 */
L8c64b0e6: *stop=0x8c64b0e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b0e8; /* mov.l r13,@-r15 */
L8c64b0e8: *stop=0x8c64b0e8u;r[13]=r[4];goto L8c64b0ea; /* mov r4,r13 */
L8c64b0ea: *stop=0x8c64b0eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b0ec; /* mov.l r12,@-r15 */
L8c64b0ec: *stop=0x8c64b0ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b0ee; /* mov.l r11,@-r15 */
L8c64b0ee: *stop=0x8c64b0eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1e4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64b0f0; /* mov.l 0x8c64b1e4,r11 */
L8c64b0f0: *stop=0x8c64b0f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b0f2; /* sts.l PR,@-r15 */
L8c64b0f2: *stop=0x8c64b0f2u;r[12]=r[11];goto L8c64b0f4; /* mov r11,r12 */
L8c64b0f4: *stop=0x8c64b0f4u;r[12]+=4294967294u;goto L8c64b0f6; /* add #-0x2,r12 */
L8c64b0f6: *stop=0x8c64b0f6u;r[5]=r[11];goto L8c64b0f8; /* mov r11,r5 */
L8c64b0f8: target=0x8c64b080u;*pr=0x8c64b0fcu;*stop=0x8c64b0fau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b0fc; /* bsr 0x8c64b080 */
L8c64b0fc: *stop=0x8c64b0fcu;r[14]=r[0];goto L8c64b0fe; /* mov r0,r14 */
L8c64b0fe: *stop=0x8c64b0feu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b100; /* cmp/pz r14 */
L8c64b100: if((*sr&1u))goto L8c64b106;goto L8c64b102; /* bt 0x8c64b106 */
L8c64b102: *stop=0x8c64b104u;r[0]=4294967295u;goto L8c64b11e; /* bra 0x8c64b11e */
L8c64b106: *stop=0x8c64b106u;r[5]=r[14];goto L8c64b108; /* mov r14,r5 */
L8c64b108: target=0x8c64b12au;*pr=0x8c64b10cu;*stop=0x8c64b10au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b10c; /* bsr 0x8c64b12a */
L8c64b10c: *stop=0x8c64b10cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64b10e; /* cmp/pz r0 */
L8c64b10e: if((*sr&1u))goto L8c64b11c;goto L8c64b110; /* bt 0x8c64b11c */
L8c64b110: *stop=0x8c64b110u;r[5]=r[14];goto L8c64b112; /* mov r14,r5 */
L8c64b112: *stop=0x8c64b112u;r[6]=r[12];goto L8c64b114; /* mov r12,r6 */
L8c64b114: target=0x8c64b18eu;*pr=0x8c64b118u;*stop=0x8c64b116u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b118; /* bsr 0x8c64b18e */
L8c64b118: *stop=0x8c64b11au;goto L8c64b0f6; /* bra 0x8c64b0f6 */
L8c64b11c: *stop=0x8c64b11cu;r[0]=r[14];goto L8c64b11e; /* mov r14,r0 */
L8c64b11e: *stop=0x8c64b11eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b120; /* lds.l @r15+,PR */
L8c64b120: *stop=0x8c64b120u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b122; /* mov.l @r15+,r11 */
L8c64b122: *stop=0x8c64b122u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b124; /* mov.l @r15+,r12 */
L8c64b124: *stop=0x8c64b124u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b126; /* mov.l @r15+,r13 */
L8c64b126: *stop=0x8c64b126u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b128u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64b0e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b0e4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b0fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b0e4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b0fcu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b10c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b0e4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b10cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b118(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b0e4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b118u);}

SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c660d8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c660d8c;
L8c660d8c: *stop=0x8c660d8cu;r[1]=sx(m,bus,&io_status,0x8c660e36u,2);if(io_status)return io_status;goto L8c660d8e; /* mov.w 0x8c660e36,r1 */
L8c660d8e: *stop=0x8c660d8eu;r[6]=0u;goto L8c660d90; /* mov #0x0,r6 */
L8c660d90: *stop=0x8c660d90u;r[0]=4294967295u;goto L8c660d92; /* mov #-0x1,r0 */
L8c660d92: *stop=0x8c660d92u;r[7]=0u;goto L8c660d94; /* mov #0x0,r7 */
L8c660d94: *stop=0x8c660d94u;if(!valid_access(r[5],1u))return -3;r[2]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c660d96; /* mov.b @r5,r2 */
L8c660d96: *stop=0x8c660d96u;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c660d98; /* mov.b @r4,r3 */
L8c660d98: *stop=0x8c660d98u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c660d9a; /* cmp/eq r2,r3 */
L8c660d9a: take=(*sr&1u);*stop=0x8c660d9cu;r[6]+=1u;if(take)goto L8c660da0;goto L8c660d9e; /* bt/s 0x8c660da0 */
L8c660d9e: *stop=0x8c660d9eu;r[7]=r[0];goto L8c660da0; /* mov r0,r7 */
L8c660da0: *stop=0x8c660da0u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[1]);goto L8c660da2; /* cmp/ge r1,r6 */
L8c660da2: *stop=0x8c660da2u;r[4]+=1u;goto L8c660da4; /* add #0x1,r4 */
L8c660da4: take=!(*sr&1u);*stop=0x8c660da6u;r[5]+=1u;if(take)goto L8c660d94;goto L8c660da8; /* bf/s 0x8c660d94 */
L8c660da8: *stop=0x8c660da8u;if(*pr!=entry_pr)return -3;*stop=0x8c660daau;r[0]=r[7];return 0; /* rts */
}

static int sa_movie_filesystem_command12_run_8c64b390(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b39cu)goto L8c64b39c;if(resume_pc==0x8c64b3a6u)goto L8c64b3a6;goto L8c64b390;
L8c64b390: *stop=0x8c64b390u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b392; /* sts.l PR,@-r15 */
L8c64b392: *stop=0x8c64b392u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b3e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b394; /* mov.l 0x8c64b3e4,r3 */
L8c64b394: *stop=0x8c64b394u;r[1]=r[4];goto L8c64b396; /* mov r4,r1 */
L8c64b396: *stop=0x8c64b396u;r[6]=10u;goto L8c64b398; /* mov #0xa,r6 */
L8c64b398: target=r[3];*pr=0x8c64b39cu;*stop=0x8c64b39au;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b39c; /* jsr @r3 */
L8c64b39c: *stop=0x8c64b39cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b3e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b39e; /* mov.l 0x8c64b3e8,r2 */
L8c64b39e: *stop=0x8c64b39eu;r[1]=r[4];goto L8c64b3a0; /* mov r4,r1 */
L8c64b3a0: *stop=0x8c64b3a0u;r[5]=r[0];goto L8c64b3a2; /* mov r0,r5 */
L8c64b3a2: target=r[2];*pr=0x8c64b3a6u;*stop=0x8c64b3a4u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b3a6; /* jsr @r2 */
L8c64b3a6: *stop=0x8c64b3a6u;r[4]=r[0];goto L8c64b3a8; /* mov r0,r4 */
L8c64b3a8: *stop=0x8c64b3a8u;r[0]=r[5];goto L8c64b3aa; /* mov r5,r0 */
L8c64b3aa: *stop=0x8c64b3aau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b3ac; /* lds.l @r15+,PR */
L8c64b3ac: *stop=0x8c64b3acu;r[0]<<=2;goto L8c64b3ae; /* shll2 r0 */
L8c64b3ae: *stop=0x8c64b3aeu;r[0]<<=2;goto L8c64b3b0; /* shll2 r0 */
L8c64b3b0: *stop=0x8c64b3b0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b3b2u;r[0]+=r[4];if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_8c64b390(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b390(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b39c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b390(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b39cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command12_resume_8c64b3a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command12_run_8c64b390(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b3a6u);}
