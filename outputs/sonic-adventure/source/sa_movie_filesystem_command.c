/* Original command20 and file information dependencies.
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
static int sa_movie_filesystem_command_run_8c64bcd6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64bd0cu)goto L8c64bd0c;if(resume_pc==0x8c64bd42u)goto L8c64bd42;if(resume_pc==0x8c64bd5cu)goto L8c64bd5c;goto L8c64bcd6;
L8c64bcd6: *stop=0x8c64bcd6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64bcd8; /* mov.l r14,@-r15 */
L8c64bcd8: *stop=0x8c64bcd8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64bcda; /* mov.l r13,@-r15 */
L8c64bcda: *stop=0x8c64bcdau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64bcdc; /* mov.l r12,@-r15 */
L8c64bcdc: *stop=0x8c64bcdcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64bcde; /* mov.l r11,@-r15 */
L8c64bcde: *stop=0x8c64bcdeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64bce0; /* mov.l r10,@-r15 */
L8c64bce0: *stop=0x8c64bce0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64bce2; /* mov.l r9,@-r15 */
L8c64bce2: *stop=0x8c64bce2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64bce4; /* sts.l PR,@-r15 */
L8c64bce4: *stop=0x8c64bce4u;r[15]+=4294967272u;goto L8c64bce6; /* add #-0x18,r15 */
L8c64bce6: *stop=0x8c64bce6u;r[13]=sx(m,bus,&io_status,0x8c64bd68u,2);if(io_status)return io_status;goto L8c64bce8; /* mov.w 0x8c64bd68,r13 */
L8c64bce8: *stop=0x8c64bce8u;r[12]=r[4];goto L8c64bcea; /* mov r4,r12 */
L8c64bcea: *stop=0x8c64bceau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bd70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bcec; /* mov.l 0x8c64bd70,r3 */
L8c64bcec: *stop=0x8c64bcecu;r[2]=1u;goto L8c64bcee; /* mov #0x1,r2 */
L8c64bcee: *stop=0x8c64bceeu;*macl=r[13]*r[12];goto L8c64bcf0; /* mul.l r13,r12 */
L8c64bcf0: *stop=0x8c64bcf0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64bcf2; /* mov.l r5,@r15 */
L8c64bcf2: *stop=0x8c64bcf2u;r[11]=r[6];goto L8c64bcf4; /* mov r6,r11 */
L8c64bcf4: *stop=0x8c64bcf4u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64bcf6; /* mov.l @(0x34,r15),r14 */
L8c64bcf6: *stop=0x8c64bcf6u;r[6]=r[15];goto L8c64bcf8; /* mov r15,r6 */
L8c64bcf8: *stop=0x8c64bcf8u;r[9]=r[7];goto L8c64bcfa; /* mov r7,r9 */
L8c64bcfa: *stop=0x8c64bcfau;r[6]+=4u;goto L8c64bcfc; /* add #0x4,r6 */
L8c64bcfc: *stop=0x8c64bcfcu;r[13]=*macl;goto L8c64bcfe; /* sts MACL,r13 */
L8c64bcfe: *stop=0x8c64bcfeu;r[13]+=r[3];goto L8c64bd00; /* add r3,r13 */
L8c64bd00: *stop=0x8c64bd00u;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64bd02; /* mov.l r2,@(0x30,r13) */
L8c64bd02: *stop=0x8c64bd02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bd74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64bd04; /* mov.l 0x8c64bd74,r2 */
L8c64bd04: *stop=0x8c64bd04u;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[14],4);if(io_status)return io_status;goto L8c64bd06; /* mov.l r14,@(0x34,r13) */
L8c64bd06: *stop=0x8c64bd06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bd08; /* mov.l @r15,r5 */
L8c64bd08: target=r[2];*pr=0x8c64bd0cu;*stop=0x8c64bd0au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bd0c; /* jsr @r2 */
L8c64bd0c: *stop=0x8c64bd0cu;r[4]=r[0];goto L8c64bd0e; /* mov r0,r4 */
L8c64bd0e: *stop=0x8c64bd0eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64bd10; /* cmp/pz r4 */
L8c64bd10: if(!(*sr&1u))goto L8c64bd62;goto L8c64bd12; /* bf 0x8c64bd62 */
L8c64bd12: *stop=0x8c64bd12u;if(!valid_access(r[15]+10u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xau,1);if(io_status)return io_status;goto L8c64bd14; /* mov.b @(0xa,r15),r0 */
L8c64bd14: *stop=0x8c64bd14u;r[0]=r[0]&255u;goto L8c64bd16; /* extu.b r0,r0 */
L8c64bd16: *stop=0x8c64bd16u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64bd18; /* cmp/eq #0x1,r0 */
L8c64bd18: if((*sr&1u))goto L8c64bd20;goto L8c64bd1a; /* bt 0x8c64bd20 */
L8c64bd1a: *stop=0x8c64bd1au;r[0]=sx(m,bus,&io_status,0x8c64bd6au,2);if(io_status)return io_status;goto L8c64bd1c; /* mov.w 0x8c64bd6a,r0 */
L8c64bd1c: *stop=0x8c64bd1eu;goto L8c64bd8a; /* bra 0x8c64bd8a */
L8c64bd20: *stop=0x8c64bd20u;if(!valid_access(r[15]+8u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;goto L8c64bd22; /* mov.w @(0x8,r15),r0 */
L8c64bd22: *stop=0x8c64bd22u;r[4]=r[0];goto L8c64bd24; /* mov r0,r4 */
L8c64bd24: *stop=0x8c64bd24u;r[4]=r[4]&65535u;goto L8c64bd26; /* extu.w r4,r4 */
L8c64bd26: *stop=0x8c64bd26u;*sr=(*sr&~1u)|(r[9]>=r[4]);goto L8c64bd28; /* cmp/hs r4,r9 */
L8c64bd28: if((*sr&1u))goto L8c64bd36;goto L8c64bd2a; /* bt 0x8c64bd36 */
L8c64bd2a: *stop=0x8c64bd2au;*sr=(*sr&~1u)|(r[14]>r[4]);goto L8c64bd2c; /* cmp/hi r4,r14 */
L8c64bd2c: if((*sr&1u))goto L8c64bd36;goto L8c64bd2e; /* bt 0x8c64bd36 */
L8c64bd2e: *stop=0x8c64bd2eu;r[2]=r[9];goto L8c64bd30; /* mov r9,r2 */
L8c64bd30: *stop=0x8c64bd30u;r[2]+=r[14];goto L8c64bd32; /* add r14,r2 */
L8c64bd32: *stop=0x8c64bd32u;*sr=(*sr&~1u)|(r[2]>r[4]);goto L8c64bd34; /* cmp/hi r4,r2 */
L8c64bd34: if(!(*sr&1u))goto L8c64bd3c;goto L8c64bd36; /* bf 0x8c64bd3c */
L8c64bd36: *stop=0x8c64bd36u;r[0]=sx(m,bus,&io_status,0x8c64bd6cu,2);if(io_status)return io_status;goto L8c64bd38; /* mov.w 0x8c64bd6c,r0 */
L8c64bd38: *stop=0x8c64bd3au;goto L8c64bd8a; /* bra 0x8c64bd8a */
L8c64bd3c: *stop=0x8c64bd3cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bd3e; /* mov.l @r15,r5 */
L8c64bd3e: target=0x8c64b300u;*pr=0x8c64bd42u;*stop=0x8c64bd40u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bd42; /* bsr 0x8c64b300 */
L8c64bd42: *stop=0x8c64bd42u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64bd44; /* cmp/pz r0 */
L8c64bd44: take=!(*sr&1u);*stop=0x8c64bd46u;r[4]=r[0];if(take)goto L8c64bd62;goto L8c64bd48; /* bf/s 0x8c64bd62 */
L8c64bd48: *stop=0x8c64bd48u;r[10]=0u;goto L8c64bd4a; /* mov #0x0,r10 */
L8c64bd4a: *stop=0x8c64bd4au;r[2]=r[10];goto L8c64bd4c; /* mov r10,r2 */
L8c64bd4c: *stop=0x8c64bd4cu;*sr=(*sr&~1u)|(r[2]>=r[14]);goto L8c64bd4e; /* cmp/hs r14,r2 */
L8c64bd4e: take=(*sr&1u);*stop=0x8c64bd50u;r[13]=r[9];if(take)goto L8c64bd88;goto L8c64bd52; /* bt/s 0x8c64bd88 */
L8c64bd52: *stop=0x8c64bd52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bd78u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64bd54; /* mov.l 0x8c64bd78,r9 */
L8c64bd54: *stop=0x8c64bd54u;r[5]=r[13];goto L8c64bd56; /* mov r13,r5 */
L8c64bd56: *stop=0x8c64bd56u;r[6]=r[11];goto L8c64bd58; /* mov r11,r6 */
L8c64bd58: target=r[9];*pr=0x8c64bd5cu;*stop=0x8c64bd5au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bd5c; /* jsr @r9 */
L8c64bd5c: *stop=0x8c64bd5cu;r[4]=r[0];goto L8c64bd5e; /* mov r0,r4 */
L8c64bd5e: *stop=0x8c64bd5eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64bd60; /* cmp/pz r4 */
L8c64bd60: if((*sr&1u))goto L8c64bd7c;goto L8c64bd62; /* bt 0x8c64bd7c */
L8c64bd62: *stop=0x8c64bd64u;r[0]=r[4];goto L8c64bd8a; /* bra 0x8c64bd8a */
L8c64bd7c: *stop=0x8c64bd7cu;r[4]=sx(m,bus,&io_status,0x8c64bd9cu,2);if(io_status)return io_status;goto L8c64bd7e; /* mov.w 0x8c64bd9c,r4 */
L8c64bd7e: *stop=0x8c64bd7eu;r[10]+=1u;goto L8c64bd80; /* add #0x1,r10 */
L8c64bd80: *stop=0x8c64bd80u;*sr=(*sr&~1u)|(r[10]>=r[14]);goto L8c64bd82; /* cmp/hs r14,r10 */
L8c64bd82: *stop=0x8c64bd82u;r[11]+=r[4];goto L8c64bd84; /* add r4,r11 */
L8c64bd84: take=!(*sr&1u);*stop=0x8c64bd86u;r[13]+=1u;if(take)goto L8c64bd52;goto L8c64bd88; /* bf/s 0x8c64bd52 */
L8c64bd88: *stop=0x8c64bd88u;r[0]=0u;goto L8c64bd8a; /* mov #0x0,r0 */
L8c64bd8a: *stop=0x8c64bd8au;r[15]+=24u;goto L8c64bd8c; /* add #0x18,r15 */
L8c64bd8c: *stop=0x8c64bd8cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64bd8e; /* lds.l @r15+,PR */
L8c64bd8e: *stop=0x8c64bd8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64bd90; /* mov.l @r15+,r9 */
L8c64bd90: *stop=0x8c64bd90u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64bd92; /* mov.l @r15+,r10 */
L8c64bd92: *stop=0x8c64bd92u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64bd94; /* mov.l @r15+,r11 */
L8c64bd94: *stop=0x8c64bd94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64bd96; /* mov.l @r15+,r12 */
L8c64bd96: *stop=0x8c64bd96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64bd98; /* mov.l @r15+,r13 */
L8c64bd98: *stop=0x8c64bd98u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64bd9au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64bcd6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64bcd6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64bd0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64bcd6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bd0cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64bd42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64bcd6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bd42u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64bd5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64bcd6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bd5cu);}

static int sa_movie_filesystem_command_run_8c648534(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64855eu)goto L8c64855e;if(resume_pc==0x8c648566u)goto L8c648566;if(resume_pc==0x8c648570u)goto L8c648570;if(resume_pc==0x8c64858cu)goto L8c64858c;if(resume_pc==0x8c6485a2u)goto L8c6485a2;if(resume_pc==0x8c648614u)goto L8c648614;goto L8c648534;
L8c648534: *stop=0x8c648534u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648536; /* mov.l r14,@-r15 */
L8c648536: *stop=0x8c648536u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c648538; /* mov.l r13,@-r15 */
L8c648538: *stop=0x8c648538u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64853a; /* sts.l PR,@-r15 */
L8c64853a: *stop=0x8c64853au;r[15]+=4294967240u;goto L8c64853c; /* add #-0x38,r15 */
L8c64853c: *stop=0x8c64853cu;r[3]=sx(m,bus,&io_status,0x8c6485aeu,2);if(io_status)return io_status;goto L8c64853e; /* mov.w 0x8c6485ae,r3 */
L8c64853e: *stop=0x8c64853eu;r[13]=r[4];goto L8c648540; /* mov r4,r13 */
L8c648540: *stop=0x8c648540u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648542; /* mov.l 0x8c6485c8,r2 */
L8c648542: *stop=0x8c648542u;*macl=r[3]*r[13];goto L8c648544; /* mul.l r3,r13 */
L8c648544: *stop=0x8c648544u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c648546; /* mov.l r5,@(0x4,r15) */
L8c648546: *stop=0x8c648546u;r[3]=*macl;goto L8c648548; /* sts MACL,r3 */
L8c648548: *stop=0x8c648548u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c64854a; /* mov.l r3,@(0x8,r15) */
L8c64854a: *stop=0x8c64854au;r[3]+=r[2];goto L8c64854c; /* add r2,r3 */
L8c64854c: *stop=0x8c64854cu;r[1]=r[3];goto L8c64854e; /* mov r3,r1 */
L8c64854e: *stop=0x8c64854eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c648550; /* mov.l r3,@r15 */
L8c648550: *stop=0x8c648550u;if(!valid_access(r[1]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[1]+0x2u,1);if(io_status)return io_status;goto L8c648552; /* mov.b @(0x2,r1),r0 */
L8c648552: *stop=0x8c648552u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648554; /* tst r0,r0 */
L8c648554: take=(*sr&1u);*stop=0x8c648556u;r[14]=r[6];if(take)goto L8c648580;goto L8c648558; /* bt/s 0x8c648580 */
L8c648558: *stop=0x8c648558u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64855a; /* mov.l 0x8c6485cc,r2 */
L8c64855a: target=r[2];*pr=0x8c64855eu;*stop=0x8c64855cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64855e; /* jsr @r2 */
L8c64855e: *stop=0x8c64855eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648560; /* tst r0,r0 */
L8c648560: if(!(*sr&1u))goto L8c64856a;goto L8c648562; /* bf 0x8c64856a */
L8c648562: target=0x8c64850au;*pr=0x8c648566u;*stop=0x8c648564u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648566; /* bsr 0x8c64850a */
L8c648566: *stop=0x8c648568u;goto L8c648580; /* bra 0x8c648580 */
L8c64856a: *stop=0x8c64856au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64856c; /* mov.l 0x8c6485d0,r3 */
L8c64856c: target=r[3];*pr=0x8c648570u;*stop=0x8c64856eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648570; /* jsr @r3 */
L8c648570: *stop=0x8c648570u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648572; /* tst r0,r0 */
L8c648572: if((*sr&1u))goto L8c648580;goto L8c648574; /* bt 0x8c648580 */
L8c648574: *stop=0x8c648574u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648576; /* mov.l @(0x8,r15),r2 */
L8c648576: *stop=0x8c648576u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648578; /* mov.l 0x8c6485c8,r3 */
L8c648578: *stop=0x8c648578u;r[2]+=r[3];goto L8c64857a; /* add r3,r2 */
L8c64857a: *stop=0x8c64857au;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c64857c; /* mov.b @(0x8,r2),r0 */
L8c64857c: *stop=0x8c64857cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64857e; /* tst r0,r0 */
L8c64857e: if(!(*sr&1u))goto L8c648586;goto L8c648580; /* bf 0x8c648586 */
L8c648580: *stop=0x8c648580u;r[0]=sx(m,bus,&io_status,0x8c6485b0u,2);if(io_status)return io_status;goto L8c648582; /* mov.w 0x8c6485b0,r0 */
L8c648582: *stop=0x8c648584u;goto L8c648616; /* bra 0x8c648616 */
L8c648586: *stop=0x8c648586u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648588; /* mov.l 0x8c6485d4,r3 */
L8c648588: target=r[3];*pr=0x8c64858cu;*stop=0x8c64858au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64858c; /* jsr @r3 */
L8c64858c: *stop=0x8c64858cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64858e; /* cmp/pz r0 */
L8c64858e: if((*sr&1u))goto L8c648596;goto L8c648590; /* bt 0x8c648596 */
L8c648590: *stop=0x8c648590u;r[0]=sx(m,bus,&io_status,0x8c6485b2u,2);if(io_status)return io_status;goto L8c648592; /* mov.w 0x8c6485b2,r0 */
L8c648592: *stop=0x8c648594u;goto L8c648616; /* bra 0x8c648616 */
L8c648596: *stop=0x8c648596u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c648598; /* mov.l @(0x4,r15),r5 */
L8c648598: *stop=0x8c648598u;r[6]=r[15];goto L8c64859a; /* mov r15,r6 */
L8c64859a: *stop=0x8c64859au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64859c; /* mov.l 0x8c6485d8,r3 */
L8c64859c: *stop=0x8c64859cu;r[6]+=12u;goto L8c64859e; /* add #0xc,r6 */
L8c64859e: target=r[3];*pr=0x8c6485a2u;*stop=0x8c6485a0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6485a2; /* jsr @r3 */
L8c6485a2: *stop=0x8c6485a2u;r[4]=r[0];goto L8c6485a4; /* mov r0,r4 */
L8c6485a4: *stop=0x8c6485a4u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c6485a6; /* cmp/pz r4 */
L8c6485a6: if((*sr&1u))goto L8c6485dc;goto L8c6485a8; /* bt 0x8c6485dc */
L8c6485a8: *stop=0x8c6485a8u;r[0]=sx(m,bus,&io_status,0x8c6485b4u,2);if(io_status)return io_status;goto L8c6485aa; /* mov.w 0x8c6485b4,r0 */
L8c6485aa: *stop=0x8c6485acu;goto L8c648616; /* bra 0x8c648616 */
L8c6485dc: *stop=0x8c6485dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6485de; /* mov.l @r15,r3 */
L8c6485de: *stop=0x8c6485deu;r[5]=r[15];goto L8c6485e0; /* mov r15,r5 */
L8c6485e0: *stop=0x8c6485e0u;r[4]=r[14];goto L8c6485e2; /* mov r14,r4 */
L8c6485e2: *stop=0x8c6485e2u;r[5]+=44u;goto L8c6485e4; /* add #0x2c,r5 */
L8c6485e4: *stop=0x8c6485e4u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6485e6; /* mov.l @(0x1c,r3),r2 */
L8c6485e6: *stop=0x8c6485e6u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6485e8; /* mov.l @(0x34,r15),r3 */
L8c6485e8: *stop=0x8c6485e8u;if(!valid_access(r[2]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6485ea; /* mov.l @(0x2c,r2),r1 */
L8c6485ea: *stop=0x8c6485eau;r[1]+=1u;goto L8c6485ec; /* add #0x1,r1 */
L8c6485ec: *stop=0x8c6485ecu;*macl=r[1]*r[3];goto L8c6485ee; /* mul.l r1,r3 */
L8c6485ee: *stop=0x8c6485eeu;r[3]=sx(m,bus,&io_status,0x8c6486fau,2);if(io_status)return io_status;goto L8c6485f0; /* mov.w 0x8c6486fa,r3 */
L8c6485f0: *stop=0x8c6485f0u;r[1]=*macl;goto L8c6485f2; /* sts MACL,r1 */
L8c6485f2: *stop=0x8c6485f2u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c6485f4; /* mov.l r1,@r14 */
L8c6485f4: *stop=0x8c6485f4u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6485f6; /* mov.l @(0x34,r15),r0 */
L8c6485f6: *stop=0x8c6485f6u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c6485f8; /* mov.w r0,@(0x4,r14) */
L8c6485f8: *stop=0x8c6485f8u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6485fa; /* mov.l @(0x10,r15),r0 */
L8c6485fa: *stop=0x8c6485fau;if(!valid_access(r[14]+6u,1u))return -3;wr(m,bus,&io_status,r[14]+0x6u,r[0],1);if(io_status)return io_status;goto L8c6485fc; /* mov.b r0,@(0x6,r14) */
L8c6485fc: *stop=0x8c6485fcu;r[0]=22u;goto L8c6485fe; /* mov #0x16,r0 */
L8c6485fe: *stop=0x8c6485feu;if(!valid_access(r[0]+r[15],1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+r[0],1);if(io_status)return io_status;goto L8c648600; /* mov.b @(r0,r15),r0 */
L8c648600: *stop=0x8c648600u;if(!valid_access(r[14]+7u,1u))return -3;wr(m,bus,&io_status,r[14]+0x7u,r[0],1);if(io_status)return io_status;goto L8c648602; /* mov.b r0,@(0x7,r14) */
L8c648602: *stop=0x8c648602u;if(!valid_access(r[15]+20u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x14u,2);if(io_status)return io_status;goto L8c648604; /* mov.w @(0x14,r15),r0 */
L8c648604: *stop=0x8c648604u;if(!valid_access(r[14]+8u,2u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],2);if(io_status)return io_status;goto L8c648606; /* mov.w r0,@(0x8,r14) */
L8c648606: *stop=0x8c648606u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648608; /* mov.l @(0x10,r15),r2 */
L8c648608: *stop=0x8c648608u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c64860a; /* cmp/eq r3,r2 */
L8c64860a: *stop=0x8c64860au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648704u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64860c; /* mov.l 0x8c648704,r3 */
L8c64860c: *stop=0x8c64860cu;r[0]=*sr&1u;goto L8c64860e; /* movt r0 */
L8c64860e: *stop=0x8c64860eu;if(!valid_access(r[14]+6u,1u))return -3;wr(m,bus,&io_status,r[14]+0x6u,r[0],1);if(io_status)return io_status;goto L8c648610; /* mov.b r0,@(0x6,r14) */
L8c648610: target=r[3];*pr=0x8c648614u;*stop=0x8c648612u;r[4]+=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648614; /* jsr @r3 */
L8c648614: *stop=0x8c648614u;r[0]=0u;goto L8c648616; /* mov #0x0,r0 */
L8c648616: *stop=0x8c648616u;r[15]+=56u;goto L8c648618; /* add #0x38,r15 */
L8c648618: *stop=0x8c648618u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64861a; /* lds.l @r15+,PR */
L8c64861a: *stop=0x8c64861au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64861c; /* mov.l @r15+,r13 */
L8c64861c: *stop=0x8c64861cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64861eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command_8c648534(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64855e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64855eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c648566(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648566u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c648570(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648570u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64858c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64858cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c6485a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6485a2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c648614(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c648534(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648614u);}

SA_STAGE_EXPORT int sa_movie_filesystem_command_8c602e9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602e9c;
L8c602e9c: *stop=0x8c602e9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602e9e; /* mov.l 0x8c602f50,r3 */
L8c602e9e: target=r[3];*stop=0x8c602ea0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

static int sa_movie_filesystem_command_run_8c64ad56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64ad92u)goto L8c64ad92;if(resume_pc==0x8c64ad9cu)goto L8c64ad9c;goto L8c64ad56;
L8c64ad56: *stop=0x8c64ad56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ad58; /* mov.l r14,@-r15 */
L8c64ad58: *stop=0x8c64ad58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ad5a; /* mov.l r13,@-r15 */
L8c64ad5a: *stop=0x8c64ad5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ad5c; /* mov.l r12,@-r15 */
L8c64ad5c: *stop=0x8c64ad5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ad5e; /* sts.l PR,@-r15 */
L8c64ad5e: *stop=0x8c64ad5eu;r[15]+=4294967288u;goto L8c64ad60; /* add #-0x8,r15 */
L8c64ad60: *stop=0x8c64ad60u;r[12]=sx(m,bus,&io_status,0x8c64ada0u,2);if(io_status)return io_status;goto L8c64ad62; /* mov.w 0x8c64ada0,r12 */
L8c64ad62: *stop=0x8c64ad62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ada8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ad64; /* mov.l 0x8c64ada8,r3 */
L8c64ad64: *stop=0x8c64ad64u;*macl=r[12]*r[4];goto L8c64ad66; /* mul.l r12,r4 */
L8c64ad66: *stop=0x8c64ad66u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64ad68; /* mov.l r5,@(0x4,r15) */
L8c64ad68: *stop=0x8c64ad68u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c64ad6a; /* mov.l r6,@r15 */
L8c64ad6a: *stop=0x8c64ad6au;r[12]=*macl;goto L8c64ad6c; /* sts MACL,r12 */
L8c64ad6c: *stop=0x8c64ad6cu;r[12]+=r[3];goto L8c64ad6e; /* add r3,r12 */
L8c64ad6e: *stop=0x8c64ad6eu;if(!valid_access(r[12]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x28u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64ad70; /* mov.l @(0x28,r12),r14 */
L8c64ad70: *stop=0x8c64ad72u;r[13]=0u;goto L8c64adb0; /* bra 0x8c64adb0 */
L8c64ad74: *stop=0x8c64ad74u;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c64ad76; /* mov.b @r14,r4 */
L8c64ad76: *stop=0x8c64ad76u;r[0]=r[4]&255u;goto L8c64ad78; /* extu.b r4,r0 */
L8c64ad78: *stop=0x8c64ad78u;*sr=(*sr&~1u)|(r[0]==0x00000033u);goto L8c64ad7a; /* cmp/eq #0x33,r0 */
L8c64ad7a: if((*sr&1u))goto L8c64ad8a;goto L8c64ad7c; /* bt 0x8c64ad8a */
L8c64ad7c: *stop=0x8c64ad7cu;r[1]=sx(m,bus,&io_status,0x8c64ada2u,2);if(io_status)return io_status;goto L8c64ad7e; /* mov.w 0x8c64ada2,r1 */
L8c64ad7e: *stop=0x8c64ad7eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ad80; /* cmp/eq r1,r0 */
L8c64ad80: if((*sr&1u))goto L8c64ad8a;goto L8c64ad82; /* bt 0x8c64ad8a */
L8c64ad82: *stop=0x8c64ad82u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64ad84; /* cmp/eq #0x0,r0 */
L8c64ad84: if((*sr&1u))goto L8c64adac;goto L8c64ad86; /* bt 0x8c64adac */
L8c64ad86: *stop=0x8c64ad88u;goto L8c64adac; /* bra 0x8c64adac */
L8c64ad8a: *stop=0x8c64ad8au;r[5]=r[14];goto L8c64ad8c; /* mov r14,r5 */
L8c64ad8c: *stop=0x8c64ad8cu;r[5]+=4u;goto L8c64ad8e; /* add #0x4,r5 */
L8c64ad8e: target=0x8c64ac90u;*pr=0x8c64ad92u;*stop=0x8c64ad90u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ad92; /* bsr 0x8c64ac90 */
L8c64ad92: *stop=0x8c64ad92u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64ad94; /* tst r0,r0 */
L8c64ad94: if((*sr&1u))goto L8c64adac;goto L8c64ad96; /* bt 0x8c64adac */
L8c64ad96: *stop=0x8c64ad96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ad98; /* mov.l @r15,r5 */
L8c64ad98: target=0x8c64acdau;*pr=0x8c64ad9cu;*stop=0x8c64ad9au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ad9c; /* bsr 0x8c64acda */
L8c64ad9c: *stop=0x8c64ad9eu;r[0]=r[13];goto L8c64adba; /* bra 0x8c64adba */
L8c64adac: *stop=0x8c64adacu;r[14]+=32u;goto L8c64adae; /* add #0x20,r14 */
L8c64adae: *stop=0x8c64adaeu;r[13]+=1u;goto L8c64adb0; /* add #0x1,r13 */
L8c64adb0: *stop=0x8c64adb0u;if(!valid_access(r[12]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64adb2; /* mov.l @(0x1c,r12),r3 */
L8c64adb2: *stop=0x8c64adb2u;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64adb4; /* mov.l @(0x30,r3),r2 */
L8c64adb4: *stop=0x8c64adb4u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[2]);goto L8c64adb6; /* cmp/ge r2,r13 */
L8c64adb6: if(!(*sr&1u))goto L8c64ad74;goto L8c64adb8; /* bf 0x8c64ad74 */
L8c64adb8: *stop=0x8c64adb8u;r[0]=4294967295u;goto L8c64adba; /* mov #-0x1,r0 */
L8c64adba: *stop=0x8c64adbau;r[15]+=8u;goto L8c64adbc; /* add #0x8,r15 */
L8c64adbc: *stop=0x8c64adbcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64adbe; /* lds.l @r15+,PR */
L8c64adbe: *stop=0x8c64adbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64adc0; /* mov.l @r15+,r12 */
L8c64adc0: *stop=0x8c64adc0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64adc2; /* mov.l @r15+,r13 */
L8c64adc2: *stop=0x8c64adc2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64adc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64ad56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64ad56(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64ad92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64ad56(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ad92u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64ad9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64ad56(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ad9cu);}

static int sa_movie_filesystem_command_run_8c64ac90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ac9eu)goto L8c64ac9e;if(resume_pc==0x8c64aca4u)goto L8c64aca4;goto L8c64ac90;
L8c64ac90: *stop=0x8c64ac90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ac92; /* mov.l r14,@-r15 */
L8c64ac92: *stop=0x8c64ac92u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ac94; /* sts.l PR,@-r15 */
L8c64ac94: *stop=0x8c64ac94u;r[15]+=4294967288u;goto L8c64ac96; /* add #-0x8,r15 */
L8c64ac96: *stop=0x8c64ac96u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c64ac98; /* mov.l r4,@r15 */
L8c64ac98: *stop=0x8c64ac98u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64ac9a; /* mov.l r5,@(0x4,r15) */
L8c64ac9a: target=0x8c64ac62u;*pr=0x8c64ac9eu;*stop=0x8c64ac9cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ac9e; /* bsr 0x8c64ac62 */
L8c64ac9e: *stop=0x8c64ac9eu;r[14]=r[0];goto L8c64aca0; /* mov r0,r14 */
L8c64aca0: target=0x8c64ac62u;*pr=0x8c64aca4u;*stop=0x8c64aca2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64aca4; /* bsr 0x8c64ac62 */
L8c64aca4: *stop=0x8c64aca4u;r[4]=r[0];goto L8c64aca6; /* mov r0,r4 */
L8c64aca6: *stop=0x8c64aca6u;*sr=(*sr&~1u)|(r[4]==r[14]);goto L8c64aca8; /* cmp/eq r4,r14 */
L8c64aca8: if(!(*sr&1u))goto L8c64acbc;goto L8c64acaa; /* bf 0x8c64acbc */
L8c64acaa: *stop=0x8c64acaau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64acac; /* mov.l @(0x4,r15),r4 */
L8c64acac: *stop=0x8c64acacu;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c64acae; /* cmp/pl r14 */
L8c64acae: *stop=0x8c64acaeu;r[6]=0u;goto L8c64acb0; /* mov #0x0,r6 */
L8c64acb0: take=!(*sr&1u);*stop=0x8c64acb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c64acd0;goto L8c64acb4; /* bf/s 0x8c64acd0 */
L8c64acb4: *stop=0x8c64acb4u;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c64acb6; /* mov.b @r4,r3 */
L8c64acb6: *stop=0x8c64acb6u;if(!valid_access(r[5],1u))return -3;r[2]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c64acb8; /* mov.b @r5,r2 */
L8c64acb8: *stop=0x8c64acb8u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c64acba; /* cmp/eq r3,r2 */
L8c64acba: if((*sr&1u))goto L8c64acc6;goto L8c64acbc; /* bt 0x8c64acc6 */
L8c64acbc: *stop=0x8c64acbcu;r[0]=0u;goto L8c64acbe; /* mov #0x0,r0 */
L8c64acbe: *stop=0x8c64acbeu;r[15]+=8u;goto L8c64acc0; /* add #0x8,r15 */
L8c64acc0: *stop=0x8c64acc0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64acc2; /* lds.l @r15+,PR */
L8c64acc2: *stop=0x8c64acc2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64acc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64acc6: *stop=0x8c64acc6u;r[6]+=1u;goto L8c64acc8; /* add #0x1,r6 */
L8c64acc8: *stop=0x8c64acc8u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[14]);goto L8c64acca; /* cmp/ge r14,r6 */
L8c64acca: *stop=0x8c64accau;r[4]+=1u;goto L8c64accc; /* add #0x1,r4 */
L8c64accc: take=!(*sr&1u);*stop=0x8c64acceu;r[5]+=1u;if(take)goto L8c64acb4;goto L8c64acd0; /* bf/s 0x8c64acb4 */
L8c64acd0: *stop=0x8c64acd0u;r[0]=1u;goto L8c64acd2; /* mov #0x1,r0 */
L8c64acd2: *stop=0x8c64acd2u;r[15]+=8u;goto L8c64acd4; /* add #0x8,r15 */
L8c64acd4: *stop=0x8c64acd4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64acd6; /* lds.l @r15+,PR */
L8c64acd6: *stop=0x8c64acd6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64acd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64ac90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64ac90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64ac9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64ac90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ac9eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64aca4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64ac90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64aca4u);}

static int sa_movie_filesystem_command_run_8c64acda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64ad2au)goto L8c64ad2a;if(resume_pc==0x8c64ad38u)goto L8c64ad38;if(resume_pc==0x8c64ad4au)goto L8c64ad4a;goto L8c64acda;
L8c64acda: *stop=0x8c64acdau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64acdc; /* mov.l r14,@-r15 */
L8c64acdc: *stop=0x8c64acdcu;r[14]=r[5];goto L8c64acde; /* mov r5,r14 */
L8c64acde: *stop=0x8c64acdeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ace0; /* mov.l r13,@-r15 */
L8c64ace0: *stop=0x8c64ace0u;r[13]=r[4];goto L8c64ace2; /* mov r4,r13 */
L8c64ace2: *stop=0x8c64ace2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ace4; /* mov.l r12,@-r15 */
L8c64ace4: *stop=0x8c64ace4u;r[5]=r[13];goto L8c64ace6; /* mov r13,r5 */
L8c64ace6: *stop=0x8c64ace6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ace8; /* sts.l PR,@-r15 */
L8c64ace8: *stop=0x8c64ace8u;if(!valid_access(r[13],1u))return -3;r[3]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c64acea; /* mov.b @r13,r3 */
L8c64acea: *stop=0x8c64aceau;r[4]=r[14];goto L8c64acec; /* mov r14,r4 */
L8c64acec: *stop=0x8c64acecu;r[4]+=16u;goto L8c64acee; /* add #0x10,r4 */
L8c64acee: *stop=0x8c64aceeu;r[3]=r[3]&255u;goto L8c64acf0; /* extu.b r3,r3 */
L8c64acf0: *stop=0x8c64acf0u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c64acf2; /* mov.l r3,@(0x4,r14) */
L8c64acf2: *stop=0x8c64acf2u;r[5]+=4u;goto L8c64acf4; /* add #0x4,r5 */
L8c64acf4: *stop=0x8c64acf4u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c64acf6; /* mov.b @r5+,r3 */
L8c64acf6: *stop=0x8c64acf6u;r[2]=r[14];goto L8c64acf8; /* mov r14,r2 */
L8c64acf8: *stop=0x8c64acf8u;r[2]+=28u;goto L8c64acfa; /* add #0x1c,r2 */
L8c64acfa: *stop=0x8c64acfau;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c64acfc; /* mov.b r3,@r4 */
L8c64acfc: *stop=0x8c64acfcu;r[4]+=1u;goto L8c64acfe; /* add #0x1,r4 */
L8c64acfe: *stop=0x8c64acfeu;*sr=(*sr&~1u)|(r[4]>=r[2]);goto L8c64ad00; /* cmp/hs r2,r4 */
L8c64ad00: if(!(*sr&1u))goto L8c64acf4;goto L8c64ad02; /* bf 0x8c64acf4 */
L8c64ad02: *stop=0x8c64ad02u;r[5]=r[13];goto L8c64ad04; /* mov r13,r5 */
L8c64ad04: *stop=0x8c64ad04u;r[4]=r[14];goto L8c64ad06; /* mov r14,r4 */
L8c64ad06: *stop=0x8c64ad06u;r[2]=0u;goto L8c64ad08; /* mov #0x0,r2 */
L8c64ad08: *stop=0x8c64ad08u;r[0]=28u;goto L8c64ad0a; /* mov #0x1c,r0 */
L8c64ad0a: *stop=0x8c64ad0au;r[4]+=32u;goto L8c64ad0c; /* add #0x20,r4 */
L8c64ad0c: *stop=0x8c64ad0cu;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],1);if(io_status)return io_status;goto L8c64ad0e; /* mov.b r2,@(r0,r14) */
L8c64ad0e: *stop=0x8c64ad0eu;r[5]+=16u;goto L8c64ad10; /* add #0x10,r5 */
L8c64ad10: *stop=0x8c64ad10u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c64ad12; /* mov.b @r5+,r3 */
L8c64ad12: *stop=0x8c64ad12u;r[2]=r[14];goto L8c64ad14; /* mov r14,r2 */
L8c64ad14: *stop=0x8c64ad14u;r[2]+=40u;goto L8c64ad16; /* add #0x28,r2 */
L8c64ad16: *stop=0x8c64ad16u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c64ad18; /* mov.b r3,@r4 */
L8c64ad18: *stop=0x8c64ad18u;r[4]+=1u;goto L8c64ad1a; /* add #0x1,r4 */
L8c64ad1a: *stop=0x8c64ad1au;*sr=(*sr&~1u)|(r[4]>=r[2]);goto L8c64ad1c; /* cmp/hs r2,r4 */
L8c64ad1c: if(!(*sr&1u))goto L8c64ad10;goto L8c64ad1e; /* bf 0x8c64ad10 */
L8c64ad1e: *stop=0x8c64ad1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ada4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64ad20; /* mov.l 0x8c64ada4,r12 */
L8c64ad20: *stop=0x8c64ad20u;r[5]=2u;goto L8c64ad22; /* mov #0x2,r5 */
L8c64ad22: *stop=0x8c64ad22u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ad24; /* mov.l @r12,r2 */
L8c64ad24: *stop=0x8c64ad24u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ad26; /* mov.l @(0xc,r2),r3 */
L8c64ad26: target=r[3];*pr=0x8c64ad2au;*stop=0x8c64ad28u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ad2a; /* jsr @r3 */
L8c64ad2a: *stop=0x8c64ad2au;r[0]=r[0]&65535u;goto L8c64ad2c; /* extu.w r0,r0 */
L8c64ad2c: *stop=0x8c64ad2cu;r[5]=24u;goto L8c64ad2e; /* mov #0x18,r5 */
L8c64ad2e: *stop=0x8c64ad2eu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],4);if(io_status)return io_status;goto L8c64ad30; /* mov.l r0,@(0xc,r14) */
L8c64ad30: *stop=0x8c64ad30u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ad32; /* mov.l @r12,r2 */
L8c64ad32: *stop=0x8c64ad32u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ad34; /* mov.l @(0xc,r2),r3 */
L8c64ad34: target=r[3];*pr=0x8c64ad38u;*stop=0x8c64ad36u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ad38; /* jsr @r3 */
L8c64ad38: *stop=0x8c64ad38u;r[0]=r[0]&65535u;goto L8c64ad3a; /* extu.w r0,r0 */
L8c64ad3a: *stop=0x8c64ad3au;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[0],4);if(io_status)return io_status;goto L8c64ad3c; /* mov.l r0,@(0x28,r14) */
L8c64ad3c: *stop=0x8c64ad3cu;r[5]=26u;goto L8c64ad3e; /* mov #0x1a,r5 */
L8c64ad3e: *stop=0x8c64ad3eu;if(!valid_access(r[13]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x1u,1);if(io_status)return io_status;goto L8c64ad40; /* mov.b @(0x1,r13),r0 */
L8c64ad40: *stop=0x8c64ad40u;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c64ad42; /* mov.b r0,@(0xa,r14) */
L8c64ad42: *stop=0x8c64ad42u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ad44; /* mov.l @r12,r0 */
L8c64ad44: *stop=0x8c64ad44u;if(!valid_access(r[0]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ad46; /* mov.l @(0xc,r0),r0 */
L8c64ad46: target=r[0];*pr=0x8c64ad4au;*stop=0x8c64ad48u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ad4a; /* jsr @r0 */
L8c64ad4a: *stop=0x8c64ad4au;if(!valid_access(r[14]+8u,2u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],2);if(io_status)return io_status;goto L8c64ad4c; /* mov.w r0,@(0x8,r14) */
L8c64ad4c: *stop=0x8c64ad4cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ad4e; /* lds.l @r15+,PR */
L8c64ad4e: *stop=0x8c64ad4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64ad50; /* mov.l @r15+,r12 */
L8c64ad50: *stop=0x8c64ad50u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ad52; /* mov.l @r15+,r13 */
L8c64ad52: *stop=0x8c64ad52u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ad54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64acda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64acda(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64ad2a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64acda(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ad2au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64ad38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64acda(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ad38u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64ad4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64acda(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ad4au);}

static int sa_movie_filesystem_command_run_8c64b472(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b484u)goto L8c64b484;if(resume_pc==0x8c64b492u)goto L8c64b492;if(resume_pc==0x8c64b49cu)goto L8c64b49c;if(resume_pc==0x8c64b4a4u)goto L8c64b4a4;if(resume_pc==0x8c64b4acu)goto L8c64b4ac;if(resume_pc==0x8c64b4b4u)goto L8c64b4b4;if(resume_pc==0x8c64b4bcu)goto L8c64b4bc;goto L8c64b472;
L8c64b472: *stop=0x8c64b472u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b474; /* mov.l r14,@-r15 */
L8c64b474: *stop=0x8c64b474u;r[14]=r[5];goto L8c64b476; /* mov r5,r14 */
L8c64b476: *stop=0x8c64b476u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b478; /* mov.l r13,@-r15 */
L8c64b478: *stop=0x8c64b478u;r[13]=r[4];goto L8c64b47a; /* mov r4,r13 */
L8c64b47a: *stop=0x8c64b47au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64b47c; /* mov.l r8,@-r15 */
L8c64b47c: *stop=0x8c64b47cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b47e; /* sts.l PR,@-r15 */
L8c64b47e: *stop=0x8c64b47eu;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c64b480; /* mov.b @r14,r4 */
L8c64b480: target=0x8c64b3b4u;*pr=0x8c64b484u;*stop=0x8c64b482u;r[4]=r[4]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b484; /* bsr 0x8c64b3b4 */
L8c64b484: *stop=0x8c64b484u;r[2]=100u;goto L8c64b486; /* mov #0x64,r2 */
L8c64b486: *stop=0x8c64b486u;*macl=(r[2]&65535u)*(r[0]&65535u);goto L8c64b488; /* mulu.w r2,r0 */
L8c64b488: *stop=0x8c64b488u;r[0]=*macl;goto L8c64b48a; /* sts MACL,r0 */
L8c64b48a: *stop=0x8c64b48au;r[8]=r[0];goto L8c64b48c; /* mov r0,r8 */
L8c64b48c: *stop=0x8c64b48cu;if(!valid_access(r[14]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;goto L8c64b48e; /* mov.b @(0x1,r14),r0 */
L8c64b48e: target=0x8c64b3b4u;*pr=0x8c64b492u;*stop=0x8c64b490u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b492; /* bsr 0x8c64b3b4 */
L8c64b492: *stop=0x8c64b492u;r[8]+=r[0];goto L8c64b494; /* add r0,r8 */
L8c64b494: *stop=0x8c64b494u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[8],2);if(io_status)return io_status;goto L8c64b496; /* mov.w r8,@r13 */
L8c64b496: *stop=0x8c64b496u;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c64b498; /* mov.b @(0x2,r14),r0 */
L8c64b498: target=0x8c64b3b4u;*pr=0x8c64b49cu;*stop=0x8c64b49au;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b49c; /* bsr 0x8c64b3b4 */
L8c64b49c: *stop=0x8c64b49cu;if(!valid_access(r[13]+2u,1u))return -3;wr(m,bus,&io_status,r[13]+0x2u,r[0],1);if(io_status)return io_status;goto L8c64b49e; /* mov.b r0,@(0x2,r13) */
L8c64b49e: *stop=0x8c64b49eu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c64b4a0; /* mov.b @(0x3,r14),r0 */
L8c64b4a0: target=0x8c64b3b4u;*pr=0x8c64b4a4u;*stop=0x8c64b4a2u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b4a4; /* bsr 0x8c64b3b4 */
L8c64b4a4: *stop=0x8c64b4a4u;if(!valid_access(r[13]+3u,1u))return -3;wr(m,bus,&io_status,r[13]+0x3u,r[0],1);if(io_status)return io_status;goto L8c64b4a6; /* mov.b r0,@(0x3,r13) */
L8c64b4a6: *stop=0x8c64b4a6u;if(!valid_access(r[14]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,1);if(io_status)return io_status;goto L8c64b4a8; /* mov.b @(0x4,r14),r0 */
L8c64b4a8: target=0x8c64b3b4u;*pr=0x8c64b4acu;*stop=0x8c64b4aau;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b4ac; /* bsr 0x8c64b3b4 */
L8c64b4ac: *stop=0x8c64b4acu;if(!valid_access(r[13]+4u,1u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],1);if(io_status)return io_status;goto L8c64b4ae; /* mov.b r0,@(0x4,r13) */
L8c64b4ae: *stop=0x8c64b4aeu;if(!valid_access(r[14]+5u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x5u,1);if(io_status)return io_status;goto L8c64b4b0; /* mov.b @(0x5,r14),r0 */
L8c64b4b0: target=0x8c64b3b4u;*pr=0x8c64b4b4u;*stop=0x8c64b4b2u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b4b4; /* bsr 0x8c64b3b4 */
L8c64b4b4: *stop=0x8c64b4b4u;if(!valid_access(r[13]+5u,1u))return -3;wr(m,bus,&io_status,r[13]+0x5u,r[0],1);if(io_status)return io_status;goto L8c64b4b6; /* mov.b r0,@(0x5,r13) */
L8c64b4b6: *stop=0x8c64b4b6u;if(!valid_access(r[14]+6u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x6u,1);if(io_status)return io_status;goto L8c64b4b8; /* mov.b @(0x6,r14),r0 */
L8c64b4b8: target=0x8c64b3b4u;*pr=0x8c64b4bcu;*stop=0x8c64b4bau;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b4bc; /* bsr 0x8c64b3b4 */
L8c64b4bc: *stop=0x8c64b4bcu;if(!valid_access(r[13]+6u,1u))return -3;wr(m,bus,&io_status,r[13]+0x6u,r[0],1);if(io_status)return io_status;goto L8c64b4be; /* mov.b r0,@(0x6,r13) */
L8c64b4be: *stop=0x8c64b4beu;if(!valid_access(r[14]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x7u,1);if(io_status)return io_status;goto L8c64b4c0; /* mov.b @(0x7,r14),r0 */
L8c64b4c0: *stop=0x8c64b4c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b4e0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b4c2; /* mov.l 0x8c64b4e0,r1 */
L8c64b4c2: *stop=0x8c64b4c2u;r[0]=r[0]&255u;goto L8c64b4c4; /* extu.b r0,r0 */
L8c64b4c4: *stop=0x8c64b4c4u;r[0]&=0x7u;goto L8c64b4c6; /* and #0x7,r0 */
L8c64b4c6: *stop=0x8c64b4c6u;if(!valid_access(r[0]+r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;goto L8c64b4c8; /* mov.b @(r0,r1),r0 */
L8c64b4c8: *stop=0x8c64b4c8u;if(!valid_access(r[13]+7u,1u))return -3;wr(m,bus,&io_status,r[13]+0x7u,r[0],1);if(io_status)return io_status;goto L8c64b4ca; /* mov.b r0,@(0x7,r13) */
L8c64b4ca: *stop=0x8c64b4cau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b4cc; /* lds.l @r15+,PR */
L8c64b4cc: *stop=0x8c64b4ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64b4ce; /* mov.l @r15+,r8 */
L8c64b4ce: *stop=0x8c64b4ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b4d0; /* mov.l @r15+,r13 */
L8c64b4d0: *stop=0x8c64b4d0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b4d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64b472(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b484(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b484u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b492(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b492u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b49c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b49cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b4a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b4a4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b4ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b4acu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b4b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b4b4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command_resume_8c64b4bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command_run_8c64b472(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b4bcu);}

SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64b3b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b3b4;
L8c64b3b4: *stop=0x8c64b3b4u;r[5]=r[4];goto L8c64b3b6; /* mov r4,r5 */
L8c64b3b6: *stop=0x8c64b3b6u;r[5]>>=2;goto L8c64b3b8; /* shlr2 r5 */
L8c64b3b8: *stop=0x8c64b3b8u;r[5]>>=2;goto L8c64b3ba; /* shlr2 r5 */
L8c64b3ba: *stop=0x8c64b3bau;r[0]=r[5];goto L8c64b3bc; /* mov r5,r0 */
L8c64b3bc: *stop=0x8c64b3bcu;r[3]=r[5];goto L8c64b3be; /* mov r5,r3 */
L8c64b3be: *stop=0x8c64b3beu;r[0]<<=2;goto L8c64b3c0; /* shll2 r0 */
L8c64b3c0: *stop=0x8c64b3c0u;r[2]=15u;goto L8c64b3c2; /* mov #0xf,r2 */
L8c64b3c2: *stop=0x8c64b3c2u;r[0]+=r[3];goto L8c64b3c4; /* add r3,r0 */
L8c64b3c4: *stop=0x8c64b3c4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c64b3c6; /* shll r0 */
L8c64b3c6: *stop=0x8c64b3c6u;r[4]&=r[2];goto L8c64b3c8; /* and r2,r4 */
L8c64b3c8: *stop=0x8c64b3c8u;if(*pr!=entry_pr)return -3;*stop=0x8c64b3cau;r[0]+=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_filesystem_command_8c64ac62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64ac62;
L8c64ac62: *stop=0x8c64ac62u;r[6]=0u;goto L8c64ac64; /* mov #0x0,r6 */
L8c64ac64: *stop=0x8c64ac64u;r[7]=12u;goto L8c64ac66; /* mov #0xc,r7 */
L8c64ac66: *stop=0x8c64ac66u;r[5]=r[6];goto L8c64ac68; /* mov r6,r5 */
L8c64ac68: *stop=0x8c64ac68u;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c64ac6a; /* mov.b @r4,r3 */
L8c64ac6a: *stop=0x8c64ac6au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64ac6c; /* tst r3,r3 */
L8c64ac6c: if((*sr&1u))goto L8c64ac78;goto L8c64ac6e; /* bt 0x8c64ac78 */
L8c64ac6e: *stop=0x8c64ac6eu;r[6]+=1u;goto L8c64ac70; /* add #0x1,r6 */
L8c64ac70: *stop=0x8c64ac70u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c64ac72; /* cmp/ge r7,r6 */
L8c64ac72: *stop=0x8c64ac72u;r[5]+=1u;goto L8c64ac74; /* add #0x1,r5 */
L8c64ac74: take=!(*sr&1u);*stop=0x8c64ac76u;r[4]+=1u;if(take)goto L8c64ac68;goto L8c64ac78; /* bf/s 0x8c64ac68 */
L8c64ac78: *stop=0x8c64ac78u;if(*pr!=entry_pr)return -3;*stop=0x8c64ac7au;r[0]=r[5];return 0; /* rts */
}
