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
 if(n==4u && (a&0x1fffffffu)==0x005f688cu)return 1;
 if((n==1u||n==2u||n==4u) && !(a&(n-1u)) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_hardware_8c6046c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6046c0;
L8c6046c0: *stop=0x8c6046c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6046c2; /* mov.l r14,@-r15 */
L8c6046c2: *stop=0x8c6046c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6046c4; /* sts.l PR,@-r15 */
L8c6046c4: *stop=0x8c6046c4u;r[15]+=4294967288u;goto L8c6046c6; /* add #-0x8,r15 */
L8c6046c6: *stop=0x8c6046c6u;r[14]=0u;goto L8c6046c8; /* mov #0x0,r14 */
L8c6046c8: *stop=0x8c6046c8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c6046ca; /* mov.l r4,@(0x4,r15) */
L8c6046ca: *stop=0x8c6046cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6046cc; /* mov.l r14,@-r15 */
L8c6046cc: *stop=0x8c6046ccu;r[7]=1u;goto L8c6046ce; /* mov #0x1,r7 */
L8c6046ce: *stop=0x8c6046ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604778u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6046d0; /* mov.l 0x8c604778,r4 */
L8c6046d0: *stop=0x8c6046d0u;r[6]=4u;goto L8c6046d2; /* mov #0x4,r6 */
L8c6046d2: *stop=0x8c6046d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60477cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6046d4; /* mov.l 0x8c60477c,r3 */
L8c6046d4: *stop=0x8c6046d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6046d6; /* mov.l r14,@-r15 */
L8c6046d6: *stop=0x8c6046d6u;r[5]=r[15];goto L8c6046d8; /* mov r15,r5 */
L8c6046d8: target=r[3];*pr=0x8c6046dcu;*stop=0x8c6046dau;r[5]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6046dc; /* jsr @r3 */
L8c6046dc: *stop=0x8c6046dcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6046de; /* mov.l @(0x8,r15),r2 */
L8c6046de: *stop=0x8c6046deu;r[7]=1u;goto L8c6046e0; /* mov #0x1,r7 */
L8c6046e0: *stop=0x8c6046e0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c60476cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6046e2; /* mov.w 0x8c60476c,r3 */
L8c6046e2: *stop=0x8c6046e2u;r[6]=4u;goto L8c6046e4; /* mov #0x4,r6 */
L8c6046e4: *stop=0x8c6046e4u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6046e6; /* mov.l @(0xc,r15),r1 */
L8c6046e6: *stop=0x8c6046e6u;r[2]&=r[3];goto L8c6046e8; /* and r3,r2 */
L8c6046e8: *stop=0x8c6046e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604778u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6046ea; /* mov.l 0x8c604778,r5 */
L8c6046ea: *stop=0x8c6046eau;r[2]|=r[1];goto L8c6046ec; /* or r1,r2 */
L8c6046ec: *stop=0x8c6046ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604780u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6046ee; /* mov.l 0x8c604780,r3 */
L8c6046ee: *stop=0x8c6046eeu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6046f0; /* mov.l r2,@(0x8,r15) */
L8c6046f0: *stop=0x8c6046f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6046f2; /* mov.l r14,@-r15 */
L8c6046f2: *stop=0x8c6046f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6046f4; /* mov.l r14,@-r15 */
L8c6046f4: *stop=0x8c6046f4u;r[4]=r[15];goto L8c6046f6; /* mov r15,r4 */
L8c6046f6: target=r[3];*pr=0x8c6046fau;*stop=0x8c6046f8u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6046fa; /* jsr @r3 */
L8c6046fa: *stop=0x8c6046fau;r[15]+=24u;goto L8c6046fc; /* add #0x18,r15 */
L8c6046fc: *stop=0x8c6046fcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6046fe; /* lds.l @r15+,PR */
L8c6046fe: *stop=0x8c6046feu;if(*pr!=entry_pr)return -3;*stop=0x8c604700u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10d460(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d460;
L8c10d460: *stop=0x8c10d460u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10d462; /* mov.l r14,@-r15 */
L8c10d462: *stop=0x8c10d462u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10d464; /* mov.l r13,@-r15 */
L8c10d464: *stop=0x8c10d464u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10d466; /* mov.l r12,@-r15 */
L8c10d466: *stop=0x8c10d466u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10d468; /* mov.l r11,@-r15 */
L8c10d468: *stop=0x8c10d468u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10d46a; /* mov.l r10,@-r15 */
L8c10d46a: *stop=0x8c10d46au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10d46c; /* mov.l r9,@-r15 */
L8c10d46c: *stop=0x8c10d46cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d46e; /* sts.l PR,@-r15 */
L8c10d46e: *stop=0x8c10d46eu;r[15]+=4294967288u;goto L8c10d470; /* add #-0x8,r15 */
L8c10d470: *stop=0x8c10d470u;r[9]=r[5];goto L8c10d472; /* mov r5,r9 */
L8c10d472: *stop=0x8c10d472u;r[10]=r[4];goto L8c10d474; /* mov r4,r10 */
L8c10d474: *stop=0x8c10d474u;r[4]=4294967295u;goto L8c10d476; /* mov #-0x1,r4 */
L8c10d476: *stop=0x8c10d476u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c10d478; /* mov.l r6,@r15 */
L8c10d478: *stop=0x8c10d478u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d47a; /* mov.l @(0x24,r15),r0 */
L8c10d47a: *stop=0x8c10d47au;r[14]=r[7];goto L8c10d47c; /* mov r7,r14 */
L8c10d47c: *stop=0x8c10d47cu;r[5]=1u;goto L8c10d47e; /* mov #0x1,r5 */
L8c10d47e: *stop=0x8c10d47eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d480; /* cmp/eq #0x0,r0 */
L8c10d480: take=(*sr&1u);*stop=0x8c10d482u;r[11]=0u;if(take)goto L8c10d490;goto L8c10d484; /* bt/s 0x8c10d490 */
L8c10d484: *stop=0x8c10d484u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d486; /* cmp/eq #0x1,r0 */
L8c10d486: if((*sr&1u))goto L8c10d494;goto L8c10d488; /* bt 0x8c10d494 */
L8c10d488: *stop=0x8c10d488u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d48a; /* cmp/eq #0x2,r0 */
L8c10d48a: if((*sr&1u))goto L8c10d498;goto L8c10d48c; /* bt 0x8c10d498 */
L8c10d48c: *stop=0x8c10d48eu;goto L8c10d49a; /* bra 0x8c10d49a */
L8c10d490: *stop=0x8c10d492u;r[12]=r[11];goto L8c10d49a; /* bra 0x8c10d49a */
L8c10d494: *stop=0x8c10d496u;r[12]=r[5];goto L8c10d49a; /* bra 0x8c10d49a */
L8c10d498: *stop=0x8c10d498u;r[12]=r[4];goto L8c10d49a; /* mov r4,r12 */
L8c10d49a: *stop=0x8c10d49au;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d49c; /* mov.l @(0x28,r15),r0 */
L8c10d49c: *stop=0x8c10d49cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d49e; /* cmp/eq #0x0,r0 */
L8c10d49e: if((*sr&1u))goto L8c10d4ac;goto L8c10d4a0; /* bt 0x8c10d4ac */
L8c10d4a0: *stop=0x8c10d4a0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d4a2; /* cmp/eq #0x1,r0 */
L8c10d4a2: if((*sr&1u))goto L8c10d4b0;goto L8c10d4a4; /* bt 0x8c10d4b0 */
L8c10d4a4: *stop=0x8c10d4a4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d4a6; /* cmp/eq #0x2,r0 */
L8c10d4a6: if((*sr&1u))goto L8c10d4b4;goto L8c10d4a8; /* bt 0x8c10d4b4 */
L8c10d4a8: *stop=0x8c10d4aau;goto L8c10d4b6; /* bra 0x8c10d4b6 */
L8c10d4ac: *stop=0x8c10d4aeu;r[13]=r[11];goto L8c10d4b6; /* bra 0x8c10d4b6 */
L8c10d4b0: *stop=0x8c10d4b2u;r[13]=r[5];goto L8c10d4b6; /* bra 0x8c10d4b6 */
L8c10d4b4: *stop=0x8c10d4b4u;r[13]=r[4];goto L8c10d4b6; /* mov r4,r13 */
L8c10d4b6: *stop=0x8c10d4b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d538u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d4b8; /* mov.l 0x8c10d538,r3 */
L8c10d4b8: *stop=0x8c10d4b8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d532u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d4ba; /* mov.w 0x8c10d532,r1 */
L8c10d4ba: *stop=0x8c10d4bau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d4bc; /* mov.l @r3,r0 */
L8c10d4bc: *stop=0x8c10d4bcu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d4be; /* mov.l @(r0,r1),r0 */
L8c10d4be: *stop=0x8c10d4beu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d4c0; /* cmp/eq #0x1,r0 */
L8c10d4c0: if(!(*sr&1u))goto L8c10d4d6;goto L8c10d4c2; /* bf 0x8c10d4d6 */
L8c10d4c2: *stop=0x8c10d4c2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d4c4; /* stc SR,r0 */
L8c10d4c4: *stop=0x8c10d4c4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d534u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d4c6; /* mov.w 0x8c10d534,r3 */
L8c10d4c6: *stop=0x8c10d4c6u;r[0]>>=2;goto L8c10d4c8; /* shlr2 r0 */
L8c10d4c8: *stop=0x8c10d4c8u;r[0]>>=2;goto L8c10d4ca; /* shlr2 r0 */
L8c10d4ca: *stop=0x8c10d4cau;r[0]&=0xfu;goto L8c10d4cc; /* and #0xf,r0 */
L8c10d4cc: *stop=0x8c10d4ccu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10d4ce; /* mov.l r0,@(0x4,r15) */
L8c10d4ce: *stop=0x8c10d4ceu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d4d0; /* stc SR,r0 */
L8c10d4d0: *stop=0x8c10d4d0u;r[0]&=r[3];goto L8c10d4d2; /* and r3,r0 */
L8c10d4d2: *stop=0x8c10d4d2u;r[0]|=0xf0u;goto L8c10d4d4; /* or #0xf0,r0 */
L8c10d4d4: *stop=0x8c10d4d4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d4d6; /* ldc r0,SR */
L8c10d4d6: *stop=0x8c10d4d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d53cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d4d8; /* mov.l 0x8c10d53c,r2 */
L8c10d4d8: target=r[2];*pr=0x8c10d4dcu;*stop=0x8c10d4dau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d4dc; /* jsr @r2 */
L8c10d4dc: *stop=0x8c10d4dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d540u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d4de; /* mov.l 0x8c10d540,r1 */
L8c10d4de: target=r[1];*pr=0x8c10d4e2u;*stop=0x8c10d4e0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d4e2; /* jsr @r1 */
L8c10d4e2: *stop=0x8c10d4e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d4e4; /* mov.l @r15,r0 */
L8c10d4e4: *stop=0x8c10d4e4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d4e6; /* cmp/eq #0x1,r0 */
L8c10d4e6: if((*sr&1u))goto L8c10d4f4;goto L8c10d4e8; /* bt 0x8c10d4f4 */
L8c10d4e8: *stop=0x8c10d4e8u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d4ea; /* cmp/eq #0x2,r0 */
L8c10d4ea: if((*sr&1u))goto L8c10d510;goto L8c10d4ec; /* bt 0x8c10d510 */
L8c10d4ec: *stop=0x8c10d4ecu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c10d4ee; /* cmp/eq #0x4,r0 */
L8c10d4ee: if((*sr&1u))goto L8c10d544;goto L8c10d4f0; /* bt 0x8c10d544 */
L8c10d4f0: *stop=0x8c10d4f2u;goto L8c10d562; /* bra 0x8c10d562 */
L8c10d4f4: *stop=0x8c10d4f4u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c10d4f6; /* cmp/pl r14 */
L8c10d4f6: *stop=0x8c10d4f6u;r[5]=r[10];goto L8c10d4f8; /* mov r10,r5 */
L8c10d4f8: *stop=0x8c10d4f8u;r[6]=r[9];goto L8c10d4fa; /* mov r9,r6 */
L8c10d4fa: take=!(*sr&1u);*stop=0x8c10d4fcu;r[4]=r[11];if(take)goto L8c10d562;goto L8c10d4fe; /* bf/s 0x8c10d562 */
L8c10d4fe: *stop=0x8c10d4feu;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10d500; /* mov.b @r5,r2 */
L8c10d500: *stop=0x8c10d500u;r[4]+=1u;goto L8c10d502; /* add #0x1,r4 */
L8c10d502: *stop=0x8c10d502u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[14]);goto L8c10d504; /* cmp/ge r14,r4 */
L8c10d504: *stop=0x8c10d504u;r[5]+=r[12];goto L8c10d506; /* add r12,r5 */
L8c10d506: *stop=0x8c10d506u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[2],1);if(io_status)return io_status;goto L8c10d508; /* mov.b r2,@r6 */
L8c10d508: take=!(*sr&1u);*stop=0x8c10d50au;r[6]+=r[13];if(take)goto L8c10d4fe;goto L8c10d50c; /* bf/s 0x8c10d4fe */
L8c10d50c: *stop=0x8c10d50eu;goto L8c10d562; /* bra 0x8c10d562 */
L8c10d510: *stop=0x8c10d510u;r[7]=r[12];goto L8c10d512; /* mov r12,r7 */
L8c10d512: *stop=0x8c10d512u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c10d514; /* shll r7 */
L8c10d514: *stop=0x8c10d514u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c10d516; /* shll r13 */
L8c10d516: *stop=0x8c10d516u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c10d518; /* cmp/pl r14 */
L8c10d518: *stop=0x8c10d518u;r[6]=r[10];goto L8c10d51a; /* mov r10,r6 */
L8c10d51a: *stop=0x8c10d51au;r[4]=r[9];goto L8c10d51c; /* mov r9,r4 */
L8c10d51c: take=!(*sr&1u);*stop=0x8c10d51eu;r[5]=r[11];if(take)goto L8c10d562;goto L8c10d520; /* bf/s 0x8c10d562 */
L8c10d520: *stop=0x8c10d520u;if(!valid_access(r[6],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[2]=loaded;}goto L8c10d522; /* mov.w @r6,r2 */
L8c10d522: *stop=0x8c10d522u;r[5]+=1u;goto L8c10d524; /* add #0x1,r5 */
L8c10d524: *stop=0x8c10d524u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[14]);goto L8c10d526; /* cmp/ge r14,r5 */
L8c10d526: *stop=0x8c10d526u;r[6]+=r[7];goto L8c10d528; /* add r7,r6 */
L8c10d528: *stop=0x8c10d528u;if(!valid_access(r[4],2u))return -3;wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c10d52a; /* mov.w r2,@r4 */
L8c10d52a: take=!(*sr&1u);*stop=0x8c10d52cu;r[4]+=r[13];if(take)goto L8c10d520;goto L8c10d52e; /* bf/s 0x8c10d520 */
L8c10d52e: *stop=0x8c10d530u;goto L8c10d562; /* bra 0x8c10d562 */
L8c10d544: *stop=0x8c10d544u;r[7]=r[12];goto L8c10d546; /* mov r12,r7 */
L8c10d546: *stop=0x8c10d546u;r[7]<<=2;goto L8c10d548; /* shll2 r7 */
L8c10d548: *stop=0x8c10d548u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c10d54a; /* cmp/pl r14 */
L8c10d54a: *stop=0x8c10d54au;r[4]=r[9];goto L8c10d54c; /* mov r9,r4 */
L8c10d54c: *stop=0x8c10d54cu;r[5]=r[10];goto L8c10d54e; /* mov r10,r5 */
L8c10d54e: *stop=0x8c10d54eu;r[6]=r[11];goto L8c10d550; /* mov r11,r6 */
L8c10d550: take=!(*sr&1u);*stop=0x8c10d552u;r[13]<<=2;if(take)goto L8c10d562;goto L8c10d554; /* bf/s 0x8c10d562 */
L8c10d554: *stop=0x8c10d554u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d556; /* mov.l @r5,r2 */
L8c10d556: *stop=0x8c10d556u;r[6]+=1u;goto L8c10d558; /* add #0x1,r6 */
L8c10d558: *stop=0x8c10d558u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[14]);goto L8c10d55a; /* cmp/ge r14,r6 */
L8c10d55a: *stop=0x8c10d55au;r[5]+=r[7];goto L8c10d55c; /* add r7,r5 */
L8c10d55c: *stop=0x8c10d55cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c10d55e; /* mov.l r2,@r4 */
L8c10d55e: take=!(*sr&1u);*stop=0x8c10d560u;r[4]+=r[13];if(take)goto L8c10d554;goto L8c10d562; /* bf/s 0x8c10d554 */
L8c10d562: *stop=0x8c10d562u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d5a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d564; /* mov.l 0x8c10d5a0,r3 */
L8c10d564: target=r[3];*pr=0x8c10d568u;*stop=0x8c10d566u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d568; /* jsr @r3 */
L8c10d568: *stop=0x8c10d568u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d5a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d56a; /* mov.l 0x8c10d5a4,r3 */
L8c10d56a: *stop=0x8c10d56au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d59au,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d56c; /* mov.w 0x8c10d59a,r1 */
L8c10d56c: *stop=0x8c10d56cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d56e; /* mov.l @r3,r0 */
L8c10d56e: *stop=0x8c10d56eu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d570; /* mov.l @(r0,r1),r0 */
L8c10d570: *stop=0x8c10d570u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d572; /* cmp/eq #0x1,r0 */
L8c10d572: if(!(*sr&1u))goto L8c10d586;goto L8c10d574; /* bf 0x8c10d586 */
L8c10d574: *stop=0x8c10d574u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d576; /* mov.l @(0x4,r15),r0 */
L8c10d576: *stop=0x8c10d576u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c10d578; /* stc SR,r2 */
L8c10d578: *stop=0x8c10d578u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d59cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d57a; /* mov.w 0x8c10d59c,r3 */
L8c10d57a: *stop=0x8c10d57au;r[0]&=0xfu;goto L8c10d57c; /* and #0xf,r0 */
L8c10d57c: *stop=0x8c10d57cu;r[0]<<=2;goto L8c10d57e; /* shll2 r0 */
L8c10d57e: *stop=0x8c10d57eu;r[2]&=r[3];goto L8c10d580; /* and r3,r2 */
L8c10d580: *stop=0x8c10d580u;r[0]<<=2;goto L8c10d582; /* shll2 r0 */
L8c10d582: *stop=0x8c10d582u;r[0]|=r[2];goto L8c10d584; /* or r2,r0 */
L8c10d584: *stop=0x8c10d584u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d586; /* ldc r0,SR */
L8c10d586: *stop=0x8c10d586u;r[0]=0u;goto L8c10d588; /* mov #0x0,r0 */
L8c10d588: *stop=0x8c10d588u;r[15]+=8u;goto L8c10d58a; /* add #0x8,r15 */
L8c10d58a: *stop=0x8c10d58au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d58c; /* lds.l @r15+,PR */
L8c10d58c: *stop=0x8c10d58cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10d58e; /* mov.l @r15+,r9 */
L8c10d58e: *stop=0x8c10d58eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10d590; /* mov.l @r15+,r10 */
L8c10d590: *stop=0x8c10d590u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10d592; /* mov.l @r15+,r11 */
L8c10d592: *stop=0x8c10d592u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10d594; /* mov.l @r15+,r12 */
L8c10d594: *stop=0x8c10d594u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10d596; /* mov.l @r15+,r13 */
L8c10d596: *stop=0x8c10d596u;if(*pr!=entry_pr)return -3;*stop=0x8c10d598u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10d770(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d770;
L8c10d770: *stop=0x8c10d770u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10d772; /* mov.l r14,@-r15 */
L8c10d772: *stop=0x8c10d772u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10d774; /* mov.l r13,@-r15 */
L8c10d774: *stop=0x8c10d774u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10d776; /* mov.l r12,@-r15 */
L8c10d776: *stop=0x8c10d776u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10d778; /* mov.l r11,@-r15 */
L8c10d778: *stop=0x8c10d778u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10d77a; /* mov.l r10,@-r15 */
L8c10d77a: *stop=0x8c10d77au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10d77c; /* mov.l r9,@-r15 */
L8c10d77c: *stop=0x8c10d77cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d77e; /* mov.l r8,@-r15 */
L8c10d77e: *stop=0x8c10d77eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d780; /* sts.l PR,@-r15 */
L8c10d780: *stop=0x8c10d780u;r[15]+=4294967280u;goto L8c10d782; /* add #-0x10,r15 */
L8c10d782: *stop=0x8c10d782u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c10d784; /* mov.l r5,@r15 */
L8c10d784: *stop=0x8c10d784u;r[5]=1u;goto L8c10d786; /* mov #0x1,r5 */
L8c10d786: *stop=0x8c10d786u;r[8]=r[4];goto L8c10d788; /* mov r4,r8 */
L8c10d788: *stop=0x8c10d788u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c10d78a; /* mov.l r6,@(0x4,r15) */
L8c10d78a: *stop=0x8c10d78au;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d78c; /* mov.l @(0x30,r15),r0 */
L8c10d78c: *stop=0x8c10d78cu;r[13]=r[7];goto L8c10d78e; /* mov r7,r13 */
L8c10d78e: *stop=0x8c10d78eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d790; /* cmp/eq #0x0,r0 */
L8c10d790: take=(*sr&1u);*stop=0x8c10d792u;r[4]=4294967295u;if(take)goto L8c10d7a0;goto L8c10d794; /* bt/s 0x8c10d7a0 */
L8c10d794: *stop=0x8c10d794u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d796; /* cmp/eq #0x1,r0 */
L8c10d796: if((*sr&1u))goto L8c10d7a4;goto L8c10d798; /* bt 0x8c10d7a4 */
L8c10d798: *stop=0x8c10d798u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d79a; /* cmp/eq #0x2,r0 */
L8c10d79a: if((*sr&1u))goto L8c10d7a8;goto L8c10d79c; /* bt 0x8c10d7a8 */
L8c10d79c: *stop=0x8c10d79eu;goto L8c10d7aa; /* bra 0x8c10d7aa */
L8c10d7a0: *stop=0x8c10d7a2u;r[10]=0u;goto L8c10d7aa; /* bra 0x8c10d7aa */
L8c10d7a4: *stop=0x8c10d7a6u;r[10]=r[5];goto L8c10d7aa; /* bra 0x8c10d7aa */
L8c10d7a8: *stop=0x8c10d7a8u;r[10]=r[4];goto L8c10d7aa; /* mov r4,r10 */
L8c10d7aa: *stop=0x8c10d7aau;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7ac; /* mov.l @(0x34,r15),r0 */
L8c10d7ac: *stop=0x8c10d7acu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d7ae; /* cmp/eq #0x0,r0 */
L8c10d7ae: if((*sr&1u))goto L8c10d7bc;goto L8c10d7b0; /* bt 0x8c10d7bc */
L8c10d7b0: *stop=0x8c10d7b0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d7b2; /* cmp/eq #0x1,r0 */
L8c10d7b2: if((*sr&1u))goto L8c10d7c0;goto L8c10d7b4; /* bt 0x8c10d7c0 */
L8c10d7b4: *stop=0x8c10d7b4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d7b6; /* cmp/eq #0x2,r0 */
L8c10d7b6: if((*sr&1u))goto L8c10d7c4;goto L8c10d7b8; /* bt 0x8c10d7c4 */
L8c10d7b8: *stop=0x8c10d7bau;goto L8c10d7c6; /* bra 0x8c10d7c6 */
L8c10d7bc: *stop=0x8c10d7beu;r[9]=0u;goto L8c10d7c6; /* bra 0x8c10d7c6 */
L8c10d7c0: *stop=0x8c10d7c2u;r[9]=r[5];goto L8c10d7c6; /* bra 0x8c10d7c6 */
L8c10d7c4: *stop=0x8c10d7c4u;r[9]=r[4];goto L8c10d7c6; /* mov r4,r9 */
L8c10d7c6: *stop=0x8c10d7c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d844u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d7c8; /* mov.l 0x8c10d844,r3 */
L8c10d7c8: *stop=0x8c10d7c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d840u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d7ca; /* mov.w 0x8c10d840,r1 */
L8c10d7ca: *stop=0x8c10d7cau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7cc; /* mov.l @r3,r0 */
L8c10d7cc: *stop=0x8c10d7ccu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7ce; /* mov.l @(r0,r1),r0 */
L8c10d7ce: *stop=0x8c10d7ceu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d7d0; /* cmp/eq #0x1,r0 */
L8c10d7d0: if(!(*sr&1u))goto L8c10d7e6;goto L8c10d7d2; /* bf 0x8c10d7e6 */
L8c10d7d2: *stop=0x8c10d7d2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d7d4; /* stc SR,r0 */
L8c10d7d4: *stop=0x8c10d7d4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d842u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d7d6; /* mov.w 0x8c10d842,r3 */
L8c10d7d6: *stop=0x8c10d7d6u;r[0]>>=2;goto L8c10d7d8; /* shlr2 r0 */
L8c10d7d8: *stop=0x8c10d7d8u;r[0]>>=2;goto L8c10d7da; /* shlr2 r0 */
L8c10d7da: *stop=0x8c10d7dau;r[0]&=0xfu;goto L8c10d7dc; /* and #0xf,r0 */
L8c10d7dc: *stop=0x8c10d7dcu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10d7de; /* mov.l r0,@(0xc,r15) */
L8c10d7de: *stop=0x8c10d7deu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d7e0; /* stc SR,r0 */
L8c10d7e0: *stop=0x8c10d7e0u;r[0]&=r[3];goto L8c10d7e2; /* and r3,r0 */
L8c10d7e2: *stop=0x8c10d7e2u;r[0]|=0xf0u;goto L8c10d7e4; /* or #0xf0,r0 */
L8c10d7e4: *stop=0x8c10d7e4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d7e6; /* ldc r0,SR */
L8c10d7e6: *stop=0x8c10d7e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d848u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d7e8; /* mov.l 0x8c10d848,r2 */
L8c10d7e8: target=r[2];*pr=0x8c10d7ecu;*stop=0x8c10d7eau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d7ec; /* jsr @r2 */
L8c10d7ec: *stop=0x8c10d7ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d84cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c10d7ee; /* mov.l 0x8c10d84c,r12 */
L8c10d7ee: target=r[12];*pr=0x8c10d7f2u;*stop=0x8c10d7f0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d7f2; /* jsr @r12 */
L8c10d7f2: *stop=0x8c10d7f2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7f4; /* mov.l @(0x4,r15),r0 */
L8c10d7f4: *stop=0x8c10d7f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d850u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c10d7f6; /* mov.l 0x8c10d850,r11 */
L8c10d7f6: *stop=0x8c10d7f6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d7f8; /* cmp/eq #0x1,r0 */
L8c10d7f8: take=(*sr&1u);*stop=0x8c10d7fau;r[14]=7u;if(take)goto L8c10d808;goto L8c10d7fc; /* bt/s 0x8c10d808 */
L8c10d7fc: *stop=0x8c10d7fcu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d7fe; /* cmp/eq #0x2,r0 */
L8c10d7fe: if((*sr&1u))goto L8c10d854;goto L8c10d800; /* bt 0x8c10d854 */
L8c10d800: *stop=0x8c10d800u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c10d802; /* cmp/eq #0x4,r0 */
L8c10d802: if((*sr&1u))goto L8c10d890;goto L8c10d804; /* bt 0x8c10d890 */
L8c10d804: *stop=0x8c10d806u;goto L8c10d8ce; /* bra 0x8c10d8ce */
L8c10d808: *stop=0x8c10d808u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d80a; /* mov.l @r15,r3 */
L8c10d80a: *stop=0x8c10d80au;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10d80c; /* cmp/pl r13 */
L8c10d80c: *stop=0x8c10d80cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10d80e; /* mov.l r3,@(0x4,r15) */
L8c10d80e: *stop=0x8c10d80eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d810; /* mov.l r8,@r15 */
L8c10d810: take=!(*sr&1u);*stop=0x8c10d812u;r[8]=0u;if(take)goto L8c10d8ce;goto L8c10d814; /* bf/s 0x8c10d8ce */
L8c10d814: *stop=0x8c10d814u;r[2]=r[8];goto L8c10d816; /* mov r8,r2 */
L8c10d816: *stop=0x8c10d816u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c10d818; /* tst r14,r2 */
L8c10d818: if(!(*sr&1u))goto L8c10d822;goto L8c10d81a; /* bf 0x8c10d822 */
L8c10d81a: target=r[12];*pr=0x8c10d81eu;*stop=0x8c10d81cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d81e; /* jsr @r12 */
L8c10d81e: target=r[11];*pr=0x8c10d822u;*stop=0x8c10d820u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d822; /* jsr @r11 */
L8c10d822: *stop=0x8c10d822u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d824; /* mov.l @r15,r3 */
L8c10d824: *stop=0x8c10d824u;r[8]+=1u;goto L8c10d826; /* add #0x1,r8 */
L8c10d826: *stop=0x8c10d826u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d828; /* mov.l @(0x4,r15),r1 */
L8c10d828: *stop=0x8c10d828u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]);goto L8c10d82a; /* cmp/ge r13,r8 */
L8c10d82a: *stop=0x8c10d82au;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10d82c; /* mov.b @r3,r2 */
L8c10d82c: *stop=0x8c10d82cu;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[2],1);if(io_status)return io_status;goto L8c10d82e; /* mov.b r2,@r1 */
L8c10d82e: *stop=0x8c10d82eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d830; /* mov.l @r15,r3 */
L8c10d830: *stop=0x8c10d830u;r[3]+=r[10];goto L8c10d832; /* add r10,r3 */
L8c10d832: *stop=0x8c10d832u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d834; /* mov.l r3,@r15 */
L8c10d834: *stop=0x8c10d834u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d836; /* mov.l @(0x4,r15),r2 */
L8c10d836: *stop=0x8c10d836u;r[2]+=r[9];goto L8c10d838; /* add r9,r2 */
L8c10d838: take=!(*sr&1u);*stop=0x8c10d83au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;if(take)goto L8c10d814;goto L8c10d83c; /* bf/s 0x8c10d814 */
L8c10d83c: *stop=0x8c10d83eu;goto L8c10d8ce; /* bra 0x8c10d8ce */
L8c10d854: *stop=0x8c10d854u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d856; /* mov.l @r15,r2 */
L8c10d856: *stop=0x8c10d856u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c10d858; /* shll r10 */
L8c10d858: *stop=0x8c10d858u;*sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c10d85a; /* shll r9 */
L8c10d85a: *stop=0x8c10d85au;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10d85c; /* cmp/pl r13 */
L8c10d85c: *stop=0x8c10d85cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c10d85e; /* mov.l r2,@(0x4,r15) */
L8c10d85e: *stop=0x8c10d85eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d860; /* mov.l r8,@r15 */
L8c10d860: take=!(*sr&1u);*stop=0x8c10d862u;r[8]=0u;if(take)goto L8c10d8ce;goto L8c10d864; /* bf/s 0x8c10d8ce */
L8c10d864: *stop=0x8c10d864u;r[2]=r[8];goto L8c10d866; /* mov r8,r2 */
L8c10d866: *stop=0x8c10d866u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c10d868; /* tst r14,r2 */
L8c10d868: if(!(*sr&1u))goto L8c10d872;goto L8c10d86a; /* bf 0x8c10d872 */
L8c10d86a: target=r[12];*pr=0x8c10d86eu;*stop=0x8c10d86cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d86e; /* jsr @r12 */
L8c10d86e: target=r[11];*pr=0x8c10d872u;*stop=0x8c10d870u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d872; /* jsr @r11 */
L8c10d872: *stop=0x8c10d872u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d874; /* mov.l @r15,r3 */
L8c10d874: *stop=0x8c10d874u;r[8]+=1u;goto L8c10d876; /* add #0x1,r8 */
L8c10d876: *stop=0x8c10d876u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d878; /* mov.l @(0x4,r15),r1 */
L8c10d878: *stop=0x8c10d878u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]);goto L8c10d87a; /* cmp/ge r13,r8 */
L8c10d87a: *stop=0x8c10d87au;if(!valid_access(r[3],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;r[2]=loaded;}goto L8c10d87c; /* mov.w @r3,r2 */
L8c10d87c: *stop=0x8c10d87cu;if(!valid_access(r[1],2u))return -3;wr(m,bus,&io_status,r[1],r[2],2);if(io_status)return io_status;goto L8c10d87e; /* mov.w r2,@r1 */
L8c10d87e: *stop=0x8c10d87eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d880; /* mov.l @r15,r3 */
L8c10d880: *stop=0x8c10d880u;r[3]+=r[10];goto L8c10d882; /* add r10,r3 */
L8c10d882: *stop=0x8c10d882u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d884; /* mov.l r3,@r15 */
L8c10d884: *stop=0x8c10d884u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d886; /* mov.l @(0x4,r15),r2 */
L8c10d886: *stop=0x8c10d886u;r[2]+=r[9];goto L8c10d888; /* add r9,r2 */
L8c10d888: take=!(*sr&1u);*stop=0x8c10d88au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;if(take)goto L8c10d864;goto L8c10d88c; /* bf/s 0x8c10d864 */
L8c10d88c: *stop=0x8c10d88eu;goto L8c10d8ce; /* bra 0x8c10d8ce */
L8c10d890: *stop=0x8c10d890u;r[10]<<=2;goto L8c10d892; /* shll2 r10 */
L8c10d892: *stop=0x8c10d892u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d894; /* mov.l @r15,r2 */
L8c10d894: *stop=0x8c10d894u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10d896; /* cmp/pl r13 */
L8c10d896: *stop=0x8c10d896u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c10d898; /* mov.l r2,@(0x4,r15) */
L8c10d898: *stop=0x8c10d898u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d89a; /* mov.l r8,@r15 */
L8c10d89a: *stop=0x8c10d89au;r[8]=0u;goto L8c10d89c; /* mov #0x0,r8 */
L8c10d89c: *stop=0x8c10d89cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[10],4);if(io_status)return io_status;goto L8c10d89e; /* mov.l r10,@(0x8,r15) */
L8c10d89e: *stop=0x8c10d89eu;r[10]=r[9];goto L8c10d8a0; /* mov r9,r10 */
L8c10d8a0: take=!(*sr&1u);*stop=0x8c10d8a2u;r[10]<<=2;if(take)goto L8c10d8ce;goto L8c10d8a4; /* bf/s 0x8c10d8ce */
L8c10d8a4: *stop=0x8c10d8a4u;r[2]=r[8];goto L8c10d8a6; /* mov r8,r2 */
L8c10d8a6: *stop=0x8c10d8a6u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c10d8a8; /* tst r14,r2 */
L8c10d8a8: if(!(*sr&1u))goto L8c10d8b2;goto L8c10d8aa; /* bf 0x8c10d8b2 */
L8c10d8aa: target=r[12];*pr=0x8c10d8aeu;*stop=0x8c10d8acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d8ae; /* jsr @r12 */
L8c10d8ae: target=r[11];*pr=0x8c10d8b2u;*stop=0x8c10d8b0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d8b2; /* jsr @r11 */
L8c10d8b2: *stop=0x8c10d8b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8b4; /* mov.l @r15,r3 */
L8c10d8b4: *stop=0x8c10d8b4u;r[8]+=1u;goto L8c10d8b6; /* add #0x1,r8 */
L8c10d8b6: *stop=0x8c10d8b6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d8b8; /* mov.l @(0x4,r15),r1 */
L8c10d8b8: *stop=0x8c10d8b8u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]);goto L8c10d8ba; /* cmp/ge r13,r8 */
L8c10d8ba: *stop=0x8c10d8bau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d8bc; /* mov.l @r3,r2 */
L8c10d8bc: *stop=0x8c10d8bcu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10d8be; /* mov.l r2,@r1 */
L8c10d8be: *stop=0x8c10d8beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d8c0; /* mov.l @r15,r1 */
L8c10d8c0: *stop=0x8c10d8c0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8c2; /* mov.l @(0x8,r15),r3 */
L8c10d8c2: *stop=0x8c10d8c2u;r[1]+=r[3];goto L8c10d8c4; /* add r3,r1 */
L8c10d8c4: *stop=0x8c10d8c4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10d8c6; /* mov.l r1,@r15 */
L8c10d8c6: *stop=0x8c10d8c6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d8c8; /* mov.l @(0x4,r15),r2 */
L8c10d8c8: *stop=0x8c10d8c8u;r[2]+=r[10];goto L8c10d8ca; /* add r10,r2 */
L8c10d8ca: take=!(*sr&1u);*stop=0x8c10d8ccu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;if(take)goto L8c10d8a4;goto L8c10d8ce; /* bf/s 0x8c10d8a4 */
L8c10d8ce: *stop=0x8c10d8ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d90cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8d0; /* mov.l 0x8c10d90c,r3 */
L8c10d8d0: target=r[3];*pr=0x8c10d8d4u;*stop=0x8c10d8d2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d8d4; /* jsr @r3 */
L8c10d8d4: *stop=0x8c10d8d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8d6; /* mov.l 0x8c10d910,r3 */
L8c10d8d6: *stop=0x8c10d8d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d908u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d8d8; /* mov.w 0x8c10d908,r1 */
L8c10d8d8: *stop=0x8c10d8d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d8da; /* mov.l @r3,r0 */
L8c10d8da: *stop=0x8c10d8dau;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d8dc; /* mov.l @(r0,r1),r0 */
L8c10d8dc: *stop=0x8c10d8dcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d8de; /* cmp/eq #0x1,r0 */
L8c10d8de: if(!(*sr&1u))goto L8c10d8f2;goto L8c10d8e0; /* bf 0x8c10d8f2 */
L8c10d8e0: *stop=0x8c10d8e0u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d8e2; /* mov.l @(0xc,r15),r0 */
L8c10d8e2: *stop=0x8c10d8e2u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c10d8e4; /* stc SR,r2 */
L8c10d8e4: *stop=0x8c10d8e4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d90au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8e6; /* mov.w 0x8c10d90a,r3 */
L8c10d8e6: *stop=0x8c10d8e6u;r[0]&=0xfu;goto L8c10d8e8; /* and #0xf,r0 */
L8c10d8e8: *stop=0x8c10d8e8u;r[0]<<=2;goto L8c10d8ea; /* shll2 r0 */
L8c10d8ea: *stop=0x8c10d8eau;r[2]&=r[3];goto L8c10d8ec; /* and r3,r2 */
L8c10d8ec: *stop=0x8c10d8ecu;r[0]<<=2;goto L8c10d8ee; /* shll2 r0 */
L8c10d8ee: *stop=0x8c10d8eeu;r[0]|=r[2];goto L8c10d8f0; /* or r2,r0 */
L8c10d8f0: *stop=0x8c10d8f0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d8f2; /* ldc r0,SR */
L8c10d8f2: *stop=0x8c10d8f2u;r[0]=0u;goto L8c10d8f4; /* mov #0x0,r0 */
L8c10d8f4: *stop=0x8c10d8f4u;r[15]+=16u;goto L8c10d8f6; /* add #0x10,r15 */
L8c10d8f6: *stop=0x8c10d8f6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d8f8; /* lds.l @r15+,PR */
L8c10d8f8: *stop=0x8c10d8f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10d8fa; /* mov.l @r15+,r8 */
L8c10d8fa: *stop=0x8c10d8fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10d8fc; /* mov.l @r15+,r9 */
L8c10d8fc: *stop=0x8c10d8fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10d8fe; /* mov.l @r15+,r10 */
L8c10d8fe: *stop=0x8c10d8feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10d900; /* mov.l @r15+,r11 */
L8c10d900: *stop=0x8c10d900u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10d902; /* mov.l @r15+,r12 */
L8c10d902: *stop=0x8c10d902u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10d904; /* mov.l @r15+,r13 */
L8c10d904: *stop=0x8c10d904u;if(*pr!=entry_pr)return -3;*stop=0x8c10d906u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10ee9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ee9c;
L8c10ee9c: *stop=0x8c10ee9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10ee9e; /* mov.l r14,@-r15 */
L8c10ee9e: *stop=0x8c10ee9eu;r[14]=0u;goto L8c10eea0; /* mov #0x0,r14 */
L8c10eea0: *stop=0x8c10eea0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10eea2; /* mov.l r13,@-r15 */
L8c10eea2: *stop=0x8c10eea2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10eea4; /* mov.l r12,@-r15 */
L8c10eea4: *stop=0x8c10eea4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10eea6; /* mov.l r11,@-r15 */
L8c10eea6: *stop=0x8c10eea6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10eea8; /* sts.l PR,@-r15 */
L8c10eea8: *stop=0x8c10eea8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10eed0u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c10eeaa; /* mov.w 0x8c10eed0,r13 */
L8c10eeaa: *stop=0x8c10eeaau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10eed8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c10eeac; /* mov.l 0x8c10eed8,r12 */
L8c10eeac: *stop=0x8c10eeacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10eed4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c10eeae; /* mov.l 0x8c10eed4,r11 */
L8c10eeae: *stop=0x8c10eeaeu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10eeb0; /* mov.l @r12,r4 */
L8c10eeb0: *stop=0x8c10eeb0u;r[4]+=4u;goto L8c10eeb2; /* add #0x4,r4 */
L8c10eeb2: target=r[11];*pr=0x8c10eeb6u;*stop=0x8c10eeb4u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10eeb6; /* jsr @r11 */
L8c10eeb6: *stop=0x8c10eeb6u;r[14]+=52u;goto L8c10eeb8; /* add #0x34,r14 */
L8c10eeb8: *stop=0x8c10eeb8u;*sr=(*sr&~1u)|(r[14]>=r[13]);goto L8c10eeba; /* cmp/hs r13,r14 */
L8c10eeba: if(!(*sr&1u))goto L8c10eeae;goto L8c10eebc; /* bf 0x8c10eeae */
L8c10eebc: *stop=0x8c10eebcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10eedcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10eebe; /* mov.l 0x8c10eedc,r3 */
L8c10eebe: *stop=0x8c10eebeu;r[2]=1u;goto L8c10eec0; /* mov #0x1,r2 */
L8c10eec0: *stop=0x8c10eec0u;r[0]=0u;goto L8c10eec2; /* mov #0x0,r0 */
L8c10eec2: *stop=0x8c10eec2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c10eec4; /* mov.l r2,@r3 */
L8c10eec4: *stop=0x8c10eec4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10eec6; /* lds.l @r15+,PR */
L8c10eec6: *stop=0x8c10eec6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10eec8; /* mov.l @r15+,r11 */
L8c10eec8: *stop=0x8c10eec8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10eeca; /* mov.l @r15+,r12 */
L8c10eeca: *stop=0x8c10eecau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10eecc; /* mov.l @r15+,r13 */
L8c10eecc: *stop=0x8c10eeccu;if(*pr!=entry_pr)return -3;*stop=0x8c10eeceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10eee0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10eee0;
L8c10eee0: *stop=0x8c10eee0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10eee2; /* mov.l r14,@-r15 */
L8c10eee2: *stop=0x8c10eee2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10eee4; /* mov.l r13,@-r15 */
L8c10eee4: *stop=0x8c10eee4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10eee6; /* sts.l PR,@-r15 */
L8c10eee6: *stop=0x8c10eee6u;r[15]+=4294967292u;goto L8c10eee8; /* add #-0x4,r15 */
L8c10eee8: *stop=0x8c10eee8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ef18u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c10eeea; /* mov.l 0x8c10ef18,r14 */
L8c10eeea: *stop=0x8c10eeeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ef14u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c10eeec; /* mov.l 0x8c10ef14,r13 */
L8c10eeec: target=r[14];*pr=0x8c10eef0u;*stop=0x8c10eeeeu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10eef0; /* jsr @r14 */
L8c10eef0: *stop=0x8c10eef0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10eef2; /* mov.l @r15,r0 */
L8c10eef2: *stop=0x8c10eef2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10eef4; /* cmp/eq #0x1,r0 */
L8c10eef4: if(!(*sr&1u))goto L8c10eeec;goto L8c10eef6; /* bf 0x8c10eeec */
L8c10eef6: *stop=0x8c10eef6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ef12u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10eef8; /* mov.w 0x8c10ef12,r0 */
L8c10eef8: *stop=0x8c10eef8u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10eefa; /* mov.l @r13,r2 */
L8c10eefa: *stop=0x8c10eefau;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10eefc; /* mov.l @(r0,r2),r3 */
L8c10eefc: *stop=0x8c10eefcu;r[3]+=1u;goto L8c10eefe; /* add #0x1,r3 */
L8c10eefe: *stop=0x8c10eefeu;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c10ef00; /* mov.l r3,@(r0,r2) */
L8c10ef00: *stop=0x8c10ef00u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ef02; /* mov.l @r15,r0 */
L8c10ef02: *stop=0x8c10ef02u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10ef04; /* cmp/eq #0x1,r0 */
L8c10ef04: if(!(*sr&1u))goto L8c10eeec;goto L8c10ef06; /* bf 0x8c10eeec */
L8c10ef06: *stop=0x8c10ef06u;r[0]=0u;goto L8c10ef08; /* mov #0x0,r0 */
L8c10ef08: *stop=0x8c10ef08u;r[15]+=4u;goto L8c10ef0a; /* add #0x4,r15 */
L8c10ef0a: *stop=0x8c10ef0au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10ef0c; /* lds.l @r15+,PR */
L8c10ef0c: *stop=0x8c10ef0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10ef0e; /* mov.l @r15+,r13 */
L8c10ef0e: *stop=0x8c10ef0eu;if(*pr!=entry_pr)return -3;*stop=0x8c10ef10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10ee20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ee20;
L8c10ee20: *stop=0x8c10ee20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10ee22; /* mov.l r14,@-r15 */
L8c10ee22: *stop=0x8c10ee22u;r[14]=0u;goto L8c10ee24; /* mov #0x0,r14 */
L8c10ee24: *stop=0x8c10ee24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10ee26; /* mov.l r13,@-r15 */
L8c10ee26: *stop=0x8c10ee26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10ee28; /* mov.l r12,@-r15 */
L8c10ee28: *stop=0x8c10ee28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10ee2a; /* mov.l r11,@-r15 */
L8c10ee2a: *stop=0x8c10ee2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10ee2c; /* sts.l PR,@-r15 */
L8c10ee2c: *stop=0x8c10ee2cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ee54u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c10ee2e; /* mov.w 0x8c10ee54,r13 */
L8c10ee2e: *stop=0x8c10ee2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee5cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c10ee30; /* mov.l 0x8c10ee5c,r12 */
L8c10ee30: *stop=0x8c10ee30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee58u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c10ee32; /* mov.l 0x8c10ee58,r11 */
L8c10ee32: *stop=0x8c10ee32u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10ee34; /* mov.l @r12,r4 */
L8c10ee34: *stop=0x8c10ee34u;r[4]+=4u;goto L8c10ee36; /* add #0x4,r4 */
L8c10ee36: target=r[11];*pr=0x8c10ee3au;*stop=0x8c10ee38u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ee3a; /* jsr @r11 */
L8c10ee3a: *stop=0x8c10ee3au;r[14]+=52u;goto L8c10ee3c; /* add #0x34,r14 */
L8c10ee3c: *stop=0x8c10ee3cu;*sr=(*sr&~1u)|(r[14]>=r[13]);goto L8c10ee3e; /* cmp/hs r13,r14 */
L8c10ee3e: if(!(*sr&1u))goto L8c10ee32;goto L8c10ee40; /* bf 0x8c10ee32 */
L8c10ee40: *stop=0x8c10ee40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ee42; /* mov.l 0x8c10ee60,r3 */
L8c10ee42: *stop=0x8c10ee42u;r[2]=0u;goto L8c10ee44; /* mov #0x0,r2 */
L8c10ee44: *stop=0x8c10ee44u;r[0]=r[2];goto L8c10ee46; /* mov r2,r0 */
L8c10ee46: *stop=0x8c10ee46u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c10ee48; /* mov.l r2,@r3 */
L8c10ee48: *stop=0x8c10ee48u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10ee4a; /* lds.l @r15+,PR */
L8c10ee4a: *stop=0x8c10ee4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10ee4c; /* mov.l @r15+,r11 */
L8c10ee4c: *stop=0x8c10ee4cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10ee4e; /* mov.l @r15+,r12 */
L8c10ee4e: *stop=0x8c10ee4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10ee50; /* mov.l @r15+,r13 */
L8c10ee50: *stop=0x8c10ee50u;if(*pr!=entry_pr)return -3;*stop=0x8c10ee52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10ebac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ebac;
L8c10ebac: *stop=0x8c10ebacu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10ebae; /* mov.l r14,@-r15 */
L8c10ebae: *stop=0x8c10ebaeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10ebb0; /* mov.l r13,@-r15 */
L8c10ebb0: *stop=0x8c10ebb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10ebb2; /* sts.l PR,@-r15 */
L8c10ebb2: *stop=0x8c10ebb2u;r[15]+=4294967292u;goto L8c10ebb4; /* add #-0x4,r15 */
L8c10ebb4: *stop=0x8c10ebb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ebe4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c10ebb6; /* mov.l 0x8c10ebe4,r14 */
L8c10ebb6: *stop=0x8c10ebb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ebe0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c10ebb8; /* mov.l 0x8c10ebe0,r13 */
L8c10ebb8: target=r[14];*pr=0x8c10ebbcu;*stop=0x8c10ebbau;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ebbc; /* jsr @r14 */
L8c10ebbc: *stop=0x8c10ebbcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ebbe; /* mov.l @r15,r0 */
L8c10ebbe: *stop=0x8c10ebbeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10ebc0; /* cmp/eq #0x1,r0 */
L8c10ebc0: if(!(*sr&1u))goto L8c10ebb8;goto L8c10ebc2; /* bf 0x8c10ebb8 */
L8c10ebc2: *stop=0x8c10ebc2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ebdeu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10ebc4; /* mov.w 0x8c10ebde,r0 */
L8c10ebc4: *stop=0x8c10ebc4u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ebc6; /* mov.l @r13,r2 */
L8c10ebc6: *stop=0x8c10ebc6u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ebc8; /* mov.l @(r0,r2),r3 */
L8c10ebc8: *stop=0x8c10ebc8u;r[3]+=1u;goto L8c10ebca; /* add #0x1,r3 */
L8c10ebca: *stop=0x8c10ebcau;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c10ebcc; /* mov.l r3,@(r0,r2) */
L8c10ebcc: *stop=0x8c10ebccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ebce; /* mov.l @r15,r0 */
L8c10ebce: *stop=0x8c10ebceu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10ebd0; /* cmp/eq #0x1,r0 */
L8c10ebd0: if(!(*sr&1u))goto L8c10ebb8;goto L8c10ebd2; /* bf 0x8c10ebb8 */
L8c10ebd2: *stop=0x8c10ebd2u;r[0]=0u;goto L8c10ebd4; /* mov #0x0,r0 */
L8c10ebd4: *stop=0x8c10ebd4u;r[15]+=4u;goto L8c10ebd6; /* add #0x4,r15 */
L8c10ebd6: *stop=0x8c10ebd6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10ebd8; /* lds.l @r15+,PR */
L8c10ebd8: *stop=0x8c10ebd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10ebda; /* mov.l @r15+,r13 */
L8c10ebda: *stop=0x8c10ebdau;if(*pr!=entry_pr)return -3;*stop=0x8c10ebdcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10f9a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f9a8;
L8c10f9a8: *stop=0x8c10f9a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f9b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f9aa; /* mov.l 0x8c10f9b8,r2 */
L8c10f9aa: *stop=0x8c10f9aau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f9ac; /* mov.l @r2,r0 */
L8c10f9ac: *stop=0x8c10f9acu;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c10f9ae; /* tst #0x1,r0 */
L8c10f9ae: *stop=0x8c10f9aeu;r[3]=*sr&1u;goto L8c10f9b0; /* movt r3 */
L8c10f9b0: *stop=0x8c10f9b0u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c10f9b2; /* mov.l r3,@r4 */
L8c10f9b2: *stop=0x8c10f9b2u;if(*pr!=entry_pr)return -3;*stop=0x8c10f9b4u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10f9bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f9bc;
L8c10f9bc: *stop=0x8c10f9bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10f9be; /* sts.l PR,@-r15 */
L8c10f9be: *stop=0x8c10f9beu;r[15]+=4294967288u;goto L8c10f9c0; /* add #-0x8,r15 */
L8c10f9c0: *stop=0x8c10f9c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f9ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f9c2; /* mov.l 0x8c10f9ec,r3 */
L8c10f9c2: *stop=0x8c10f9c2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c10f9c4; /* mov.l r4,@(0x4,r15) */
L8c10f9c4: target=r[3];*pr=0x8c10f9c8u;*stop=0x8c10f9c6u;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f9c8; /* jsr @r3 */
L8c10f9c8: *stop=0x8c10f9c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f9ca; /* mov.l @r15,r0 */
L8c10f9ca: *stop=0x8c10f9cau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10f9cc; /* cmp/eq #0x1,r0 */
L8c10f9cc: if(!(*sr&1u))goto L8c10f9e4;goto L8c10f9ce; /* bf 0x8c10f9e4 */
L8c10f9ce: *stop=0x8c10f9ceu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f9d0; /* mov.l @(0x4,r15),r2 */
L8c10f9d0: *stop=0x8c10f9d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f9f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f9d2; /* mov.l 0x8c10f9f0,r3 */
L8c10f9d2: *stop=0x8c10f9d2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f9d4; /* mov.l @r2,r1 */
L8c10f9d4: *stop=0x8c10f9d4u;r[2]=0u;goto L8c10f9d6; /* mov #0x0,r2 */
L8c10f9d6: *stop=0x8c10f9d6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10f9d8; /* mov.l @r3,r4 */
L8c10f9d8: *stop=0x8c10f9d8u;r[1]<<=2;goto L8c10f9da; /* shll2 r1 */
L8c10f9da: *stop=0x8c10f9dau;r[1]<<=2;goto L8c10f9dc; /* shll2 r1 */
L8c10f9dc: *stop=0x8c10f9dcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10f9de; /* mov.l @r4,r4 */
L8c10f9de: *stop=0x8c10f9deu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c10f9e0; /* shll r1 */
L8c10f9e0: *stop=0x8c10f9e0u;r[4]+=r[1];goto L8c10f9e2; /* add r1,r4 */
L8c10f9e2: *stop=0x8c10f9e2u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c10f9e4; /* mov.l r2,@(0x1c,r4) */
L8c10f9e4: *stop=0x8c10f9e4u;r[15]+=8u;goto L8c10f9e6; /* add #0x8,r15 */
L8c10f9e6: *stop=0x8c10f9e6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10f9e8; /* lds.l @r15+,PR */
L8c10f9e8: *stop=0x8c10f9e8u;if(*pr!=entry_pr)return -3;*stop=0x8c10f9eau;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10fa60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10fa60;
L8c10fa60: *stop=0x8c10fa60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10fa62; /* mov.l r14,@-r15 */
L8c10fa62: *stop=0x8c10fa62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10fa64; /* sts.l PR,@-r15 */
L8c10fa64: *stop=0x8c10fa64u;r[15]+=4294967292u;goto L8c10fa66; /* add #-0x4,r15 */
L8c10fa66: *stop=0x8c10fa66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fa98u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fa68; /* mov.l 0x8c10fa98,r3 */
L8c10fa68: *stop=0x8c10fa68u;r[14]=r[4];goto L8c10fa6a; /* mov r4,r14 */
L8c10fa6a: target=r[3];*pr=0x8c10fa6eu;*stop=0x8c10fa6cu;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fa6e; /* jsr @r3 */
L8c10fa6e: *stop=0x8c10fa6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fa70; /* mov.l @r15,r0 */
L8c10fa70: *stop=0x8c10fa70u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10fa72; /* cmp/eq #0x1,r0 */
L8c10fa72: take=!(*sr&1u);*stop=0x8c10fa74u;r[0]=0u;if(take)goto L8c10fa8e;goto L8c10fa76; /* bf/s 0x8c10fa8e */
L8c10fa76: *stop=0x8c10fa76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fa9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fa78; /* mov.l 0x8c10fa9c,r3 */
L8c10fa78: *stop=0x8c10fa78u;r[1]=2u;goto L8c10fa7a; /* mov #0x2,r1 */
L8c10fa7a: *stop=0x8c10fa7au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fa7c; /* mov.l @r14,r2 */
L8c10fa7c: *stop=0x8c10fa7cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10fa7e; /* mov.l @r3,r4 */
L8c10fa7e: *stop=0x8c10fa7eu;r[2]<<=2;goto L8c10fa80; /* shll2 r2 */
L8c10fa80: *stop=0x8c10fa80u;r[2]<<=2;goto L8c10fa82; /* shll2 r2 */
L8c10fa82: *stop=0x8c10fa82u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10fa84; /* mov.l @r4,r4 */
L8c10fa84: *stop=0x8c10fa84u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c10fa86; /* shll r2 */
L8c10fa86: *stop=0x8c10fa86u;r[4]+=r[2];goto L8c10fa88; /* add r2,r4 */
L8c10fa88: *stop=0x8c10fa88u;r[2]=1u;goto L8c10fa8a; /* mov #0x1,r2 */
L8c10fa8a: *stop=0x8c10fa8au;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c10fa8c; /* mov.l r2,@(0x1c,r4) */
L8c10fa8c: *stop=0x8c10fa8cu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[1],4);if(io_status)return io_status;goto L8c10fa8e; /* mov.l r1,@(0x18,r14) */
L8c10fa8e: *stop=0x8c10fa8eu;r[15]+=4u;goto L8c10fa90; /* add #0x4,r15 */
L8c10fa90: *stop=0x8c10fa90u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10fa92; /* lds.l @r15+,PR */
L8c10fa92: *stop=0x8c10fa92u;if(*pr!=entry_pr)return -3;*stop=0x8c10fa94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c10fb48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fb48;
L8c10fb48: *stop=0x8c10fb48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fb4a; /* mov.l 0x8c10fb58,r2 */
L8c10fb4a: *stop=0x8c10fb4au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fb4c; /* mov.l @r2,r0 */
L8c10fb4c: *stop=0x8c10fb4cu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c10fb4e; /* tst #0x20,r0 */
L8c10fb4e: *stop=0x8c10fb4eu;r[3]=*sr&1u;goto L8c10fb50; /* movt r3 */
L8c10fb50: *stop=0x8c10fb50u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c10fb52; /* mov.l r3,@r4 */
L8c10fb52: *stop=0x8c10fb52u;if(*pr!=entry_pr)return -3;*stop=0x8c10fb54u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_hardware_8c1101f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c1101f0;
L8c1101f0: *stop=0x8c1101f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c110214u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c1101f2; /* mov.l 0x8c110214,r2 */
L8c1101f2: *stop=0x8c1101f2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c1101f4; /* mov.l @r4,r4 */
L8c1101f4: *stop=0x8c1101f4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c1101f6; /* mov.l @r2,r3 */
L8c1101f6: *stop=0x8c1101f6u;r[4]<<=2;goto L8c1101f8; /* shll2 r4 */
L8c1101f8: *stop=0x8c1101f8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c1101fa; /* mov.l @r3,r1 */
L8c1101fa: *stop=0x8c1101fau;r[4]<<=2;goto L8c1101fc; /* shll2 r4 */
L8c1101fc: *stop=0x8c1101fcu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c1101fe; /* shll r4 */
L8c1101fe: *stop=0x8c1101feu;r[3]=r[1];goto L8c110200; /* mov r1,r3 */
L8c110200: *stop=0x8c110200u;r[4]+=r[3];goto L8c110202; /* add r3,r4 */
L8c110202: *stop=0x8c110202u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c110204; /* mov.l @(0x18,r4),r0 */
L8c110204: *stop=0x8c110204u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c110206; /* tst #0x1,r0 */
L8c110206: if((*sr&1u))goto L8c11020c;goto L8c110208; /* bt 0x8c11020c */
L8c110208: *stop=0x8c11020au;r[1]=1u;goto L8c11020e; /* bra 0x8c11020e */
L8c11020c: *stop=0x8c11020cu;r[1]=0u;goto L8c11020e; /* mov #0x0,r1 */
L8c11020e: *stop=0x8c11020eu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c110210; /* mov.l r1,@r5 */
L8c110210: *stop=0x8c110210u;if(*pr!=entry_pr)return -3;*stop=0x8c110212u;r[0]=0u;return 0; /* rts */
}
