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
SA_STAGE_EXPORT int sa_movie_record_constructors_8c666e50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c666e50;
L8c666e50: *stop=0x8c666e50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666e52; /* mov.l r14,@-r15 */
L8c666e52: *stop=0x8c666e52u;r[7]=0u;goto L8c666e54; /* mov #0x0,r7 */
L8c666e54: *stop=0x8c666e54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ed4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c666e56; /* mov.l 0x8c666ed4,r6 */
L8c666e56: *stop=0x8c666e56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c666e58; /* mov.l r13,@-r15 */
L8c666e58: *stop=0x8c666e58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c666e5a; /* mov.l r12,@-r15 */
L8c666e5a: *stop=0x8c666e5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666e5c; /* sts.l PR,@-r15 */
L8c666e5c: *stop=0x8c666e5cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[13]=loaded;}goto L8c666e5e; /* mov.l @r6,r13 */
L8c666e5e: *stop=0x8c666e5eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c666e60; /* tst r13,r13 */
L8c666e60: take=!(*sr&1u);*stop=0x8c666e62u;r[14]=r[7];if(take)goto L8c666e6a;goto L8c666e64; /* bf/s 0x8c666e6a */
L8c666e64: *stop=0x8c666e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ed8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666e66; /* mov.l 0x8c666ed8,r14 */
L8c666e66: *stop=0x8c666e68u;goto L8c666eb8; /* bra 0x8c666eb8 */
L8c666e6a: *stop=0x8c666e6au;{uint32_t loaded=sx(m,bus,&io_status,0x8c666ed0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666e6c; /* mov.w 0x8c666ed0,r0 */
L8c666e6c: *stop=0x8c666e6cu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c666e6e; /* cmp/pl r5 */
L8c666e6e: *stop=0x8c666e6eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c666ed0u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c666e70; /* mov.w 0x8c666ed0,r2 */
L8c666e70: *stop=0x8c666e70u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[4],4);if(io_status)return io_status;goto L8c666e72; /* mov.l r4,@(r0,r13) */
L8c666e72: *stop=0x8c666e72u;r[0]+=4u;goto L8c666e74; /* add #0x4,r0 */
L8c666e74: *stop=0x8c666e74u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e76; /* mov.l @r6,r3 */
L8c666e76: *stop=0x8c666e76u;r[13]=r[7];goto L8c666e78; /* mov r7,r13 */
L8c666e78: *stop=0x8c666e78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ee0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c666e7a; /* mov.l 0x8c666ee0,r12 */
L8c666e7a: *stop=0x8c666e7au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[5],4);if(io_status)return io_status;goto L8c666e7c; /* mov.l r5,@(r0,r3) */
L8c666e7c: *stop=0x8c666e7cu;r[0]+=4u;goto L8c666e7e; /* add #0x4,r0 */
L8c666e7e: *stop=0x8c666e7eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e80; /* mov.l @r6,r3 */
L8c666e80: *stop=0x8c666e80u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666e82; /* mov.l r7,@(r0,r3) */
L8c666e82: *stop=0x8c666e82u;r[0]+=4u;goto L8c666e84; /* add #0x4,r0 */
L8c666e84: *stop=0x8c666e84u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e86; /* mov.l @r6,r3 */
L8c666e86: *stop=0x8c666e86u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666e88; /* mov.l r7,@(r0,r3) */
L8c666e88: *stop=0x8c666e88u;r[0]+=4u;goto L8c666e8a; /* add #0x4,r0 */
L8c666e8a: *stop=0x8c666e8au;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e8c; /* mov.l @r6,r3 */
L8c666e8c: *stop=0x8c666e8cu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666e8e; /* mov.l r7,@(r0,r3) */
L8c666e8e: *stop=0x8c666e8eu;r[0]+=4u;goto L8c666e90; /* add #0x4,r0 */
L8c666e90: *stop=0x8c666e90u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e92; /* mov.l @r6,r3 */
L8c666e92: *stop=0x8c666e92u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666e94; /* mov.l r7,@(r0,r3) */
L8c666e94: *stop=0x8c666e94u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e96; /* mov.l @r6,r3 */
L8c666e96: *stop=0x8c666e96u;r[2]+=r[3];goto L8c666e98; /* add r3,r2 */
L8c666e98: *stop=0x8c666e98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666edcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e9a; /* mov.l 0x8c666edc,r3 */
L8c666e9a: *stop=0x8c666e9au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c666e9c; /* mov.l r2,@r3 */
L8c666e9c: take=!(*sr&1u);*stop=0x8c666e9eu;r[4]=r[7];if(take)goto L8c666eb8;goto L8c666ea0; /* bf/s 0x8c666eb8 */
L8c666ea0: *stop=0x8c666ea0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666ed0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666ea2; /* mov.w 0x8c666ed0,r0 */
L8c666ea2: *stop=0x8c666ea2u;r[2]=r[12];goto L8c666ea4; /* mov r12,r2 */
L8c666ea4: *stop=0x8c666ea4u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666ea6; /* mov.l @r6,r1 */
L8c666ea6: *stop=0x8c666ea6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ee4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666ea8; /* mov.l 0x8c666ee4,r3 */
L8c666ea8: *stop=0x8c666ea8u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666eaa; /* mov.l @(r0,r1),r1 */
L8c666eaa: *stop=0x8c666eaau;r[1]+=r[4];goto L8c666eac; /* add r4,r1 */
L8c666eac: target=r[3];*pr=0x8c666eb0u;*stop=0x8c666eaeu;r[0]=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666eb0; /* jsr @r3 */
L8c666eb0: *stop=0x8c666eb0u;r[13]+=1u;goto L8c666eb2; /* add #0x1,r13 */
L8c666eb2: *stop=0x8c666eb2u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[5]);goto L8c666eb4; /* cmp/ge r5,r13 */
L8c666eb4: take=!(*sr&1u);*stop=0x8c666eb6u;r[4]+=32u;if(take)goto L8c666ea0;goto L8c666eb8; /* bf/s 0x8c666ea0 */
L8c666eb8: *stop=0x8c666eb8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666eba; /* tst r14,r14 */
L8c666eba: if((*sr&1u))goto L8c666ec4;goto L8c666ebc; /* bt 0x8c666ec4 */
L8c666ebc: *stop=0x8c666ebcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666eecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666ebe; /* mov.l 0x8c666eec,r2 */
L8c666ebe: *stop=0x8c666ebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ee8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666ec0; /* mov.l 0x8c666ee8,r4 */
L8c666ec0: target=r[2];*pr=0x8c666ec4u;*stop=0x8c666ec2u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666ec4; /* jsr @r2 */
L8c666ec4: *stop=0x8c666ec4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666ec6; /* lds.l @r15+,PR */
L8c666ec6: *stop=0x8c666ec6u;r[0]=r[14];goto L8c666ec8; /* mov r14,r0 */
L8c666ec8: *stop=0x8c666ec8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c666eca; /* mov.l @r15+,r12 */
L8c666eca: *stop=0x8c666ecau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666ecc; /* mov.l @r15+,r13 */
L8c666ecc: *stop=0x8c666eccu;if(*pr!=entry_pr)return -3;*stop=0x8c666eceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c666ef0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c666ef0;
L8c666ef0: *stop=0x8c666ef0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666ef2; /* mov.l r14,@-r15 */
L8c666ef2: *stop=0x8c666ef2u;r[7]=0u;goto L8c666ef4; /* mov #0x0,r7 */
L8c666ef4: *stop=0x8c666ef4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f74u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c666ef6; /* mov.l 0x8c666f74,r6 */
L8c666ef6: *stop=0x8c666ef6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c666ef8; /* mov.l r13,@-r15 */
L8c666ef8: *stop=0x8c666ef8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c666efa; /* mov.l r12,@-r15 */
L8c666efa: *stop=0x8c666efau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666efc; /* sts.l PR,@-r15 */
L8c666efc: *stop=0x8c666efcu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[14]=loaded;}goto L8c666efe; /* mov.l @r6,r14 */
L8c666efe: *stop=0x8c666efeu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666f00; /* tst r14,r14 */
L8c666f00: take=!(*sr&1u);*stop=0x8c666f02u;r[13]=r[7];if(take)goto L8c666f0a;goto L8c666f04; /* bf/s 0x8c666f0a */
L8c666f04: *stop=0x8c666f04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f78u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c666f06; /* mov.l 0x8c666f78,r13 */
L8c666f06: *stop=0x8c666f08u;goto L8c666f5a; /* bra 0x8c666f5a */
L8c666f0a: *stop=0x8c666f0au;{uint32_t loaded=sx(m,bus,&io_status,0x8c666f72u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666f0c; /* mov.w 0x8c666f72,r0 */
L8c666f0c: *stop=0x8c666f0cu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c666f0e; /* cmp/pl r5 */
L8c666f0e: *stop=0x8c666f0eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c666f72u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c666f10; /* mov.w 0x8c666f72,r2 */
L8c666f10: *stop=0x8c666f10u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c666f12; /* mov.l r4,@(r0,r14) */
L8c666f12: *stop=0x8c666f12u;r[0]+=4u;goto L8c666f14; /* add #0x4,r0 */
L8c666f14: *stop=0x8c666f14u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f16; /* mov.l @r6,r3 */
L8c666f16: *stop=0x8c666f16u;r[14]=r[7];goto L8c666f18; /* mov r7,r14 */
L8c666f18: *stop=0x8c666f18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f80u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666f1a; /* mov.l 0x8c666f80,r1 */
L8c666f1a: *stop=0x8c666f1au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[5],4);if(io_status)return io_status;goto L8c666f1c; /* mov.l r5,@(r0,r3) */
L8c666f1c: *stop=0x8c666f1cu;r[0]+=4u;goto L8c666f1e; /* add #0x4,r0 */
L8c666f1e: *stop=0x8c666f1eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f20; /* mov.l @r6,r3 */
L8c666f20: *stop=0x8c666f20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f88u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c666f22; /* mov.l 0x8c666f88,r12 */
L8c666f22: *stop=0x8c666f22u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666f24; /* mov.l r7,@(r0,r3) */
L8c666f24: *stop=0x8c666f24u;r[0]+=4u;goto L8c666f26; /* add #0x4,r0 */
L8c666f26: *stop=0x8c666f26u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f28; /* mov.l @r6,r3 */
L8c666f28: *stop=0x8c666f28u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666f2a; /* mov.l r7,@(r0,r3) */
L8c666f2a: *stop=0x8c666f2au;r[0]+=4u;goto L8c666f2c; /* add #0x4,r0 */
L8c666f2c: *stop=0x8c666f2cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f2e; /* mov.l @r6,r3 */
L8c666f2e: *stop=0x8c666f2eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[7],4);if(io_status)return io_status;goto L8c666f30; /* mov.l r7,@(r0,r3) */
L8c666f30: *stop=0x8c666f30u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f32; /* mov.l @r6,r3 */
L8c666f32: *stop=0x8c666f32u;r[2]+=r[3];goto L8c666f34; /* add r3,r2 */
L8c666f34: *stop=0x8c666f34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f36; /* mov.l 0x8c666f7c,r3 */
L8c666f36: *stop=0x8c666f36u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c666f38; /* mov.l r2,@r3 */
L8c666f38: *stop=0x8c666f38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f3a; /* mov.l 0x8c666f84,r3 */
L8c666f3a: *stop=0x8c666f3au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c666f3c; /* mov.l r4,@r1 */
L8c666f3c: *stop=0x8c666f3cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c666f3e; /* mov.l r5,@r3 */
L8c666f3e: take=!(*sr&1u);*stop=0x8c666f40u;r[4]=r[7];if(take)goto L8c666f5a;goto L8c666f42; /* bf/s 0x8c666f5a */
L8c666f42: *stop=0x8c666f42u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666f72u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666f44; /* mov.w 0x8c666f72,r0 */
L8c666f44: *stop=0x8c666f44u;r[2]=r[12];goto L8c666f46; /* mov r12,r2 */
L8c666f46: *stop=0x8c666f46u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666f48; /* mov.l @r6,r1 */
L8c666f48: *stop=0x8c666f48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666f4a; /* mov.l 0x8c666f8c,r3 */
L8c666f4a: *stop=0x8c666f4au;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666f4c; /* mov.l @(r0,r1),r1 */
L8c666f4c: *stop=0x8c666f4cu;r[1]+=r[4];goto L8c666f4e; /* add r4,r1 */
L8c666f4e: target=r[3];*pr=0x8c666f52u;*stop=0x8c666f50u;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666f52; /* jsr @r3 */
L8c666f52: *stop=0x8c666f52u;r[14]+=1u;goto L8c666f54; /* add #0x1,r14 */
L8c666f54: *stop=0x8c666f54u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[5]);goto L8c666f56; /* cmp/ge r5,r14 */
L8c666f56: take=!(*sr&1u);*stop=0x8c666f58u;r[4]+=8u;if(take)goto L8c666f42;goto L8c666f5a; /* bf/s 0x8c666f42 */
L8c666f5a: *stop=0x8c666f5au;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c666f5c; /* tst r13,r13 */
L8c666f5c: if((*sr&1u))goto L8c666f66;goto L8c666f5e; /* bt 0x8c666f66 */
L8c666f5e: *stop=0x8c666f5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666f60; /* mov.l 0x8c666f94,r2 */
L8c666f60: *stop=0x8c666f60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666f90u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666f62; /* mov.l 0x8c666f90,r4 */
L8c666f62: target=r[2];*pr=0x8c666f66u;*stop=0x8c666f64u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666f66; /* jsr @r2 */
L8c666f66: *stop=0x8c666f66u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666f68; /* lds.l @r15+,PR */
L8c666f68: *stop=0x8c666f68u;r[0]=r[13];goto L8c666f6a; /* mov r13,r0 */
L8c666f6a: *stop=0x8c666f6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c666f6c; /* mov.l @r15+,r12 */
L8c666f6c: *stop=0x8c666f6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666f6e; /* mov.l @r15+,r13 */
L8c666f6e: *stop=0x8c666f6eu;if(*pr!=entry_pr)return -3;*stop=0x8c666f70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c666b34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c666b34;
L8c666b34: *stop=0x8c666b34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666b36; /* mov.l r14,@-r15 */
L8c666b36: *stop=0x8c666b36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c666b38; /* mov.l r13,@-r15 */
L8c666b38: *stop=0x8c666b38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666b8cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c666b3a; /* mov.l 0x8c666b8c,r13 */
L8c666b3a: *stop=0x8c666b3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666b3c; /* sts.l PR,@-r15 */
L8c666b3c: *stop=0x8c666b3cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[5]=loaded;}goto L8c666b3e; /* mov.l @r13,r5 */
L8c666b3e: *stop=0x8c666b3eu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c666b40; /* tst r5,r5 */
L8c666b40: if(!(*sr&1u))goto L8c666b48;goto L8c666b42; /* bf 0x8c666b48 */
L8c666b42: *stop=0x8c666b42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666b90u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666b44; /* mov.l 0x8c666b90,r14 */
L8c666b44: *stop=0x8c666b46u;goto L8c666b6e; /* bra 0x8c666b6e */
L8c666b48: *stop=0x8c666b48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666b94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666b4a; /* mov.l 0x8c666b94,r3 */
L8c666b4a: *stop=0x8c666b4au;{uint32_t loaded=sx(m,bus,&io_status,0x8c666b84u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666b4c; /* mov.w 0x8c666b84,r0 */
L8c666b4c: *stop=0x8c666b4cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c666b86u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c666b4e; /* mov.w 0x8c666b86,r6 */
L8c666b4e: *stop=0x8c666b4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666b98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666b50; /* mov.l 0x8c666b98,r2 */
L8c666b50: *stop=0x8c666b50u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[4],4);if(io_status)return io_status;goto L8c666b52; /* mov.l r4,@(r0,r5) */
L8c666b52: *stop=0x8c666b52u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c666b54; /* mov.l r4,@r3 */
L8c666b54: target=r[2];*pr=0x8c666b58u;*stop=0x8c666b56u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666b58; /* jsr @r2 */
L8c666b58: *stop=0x8c666b58u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666b88u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666b5a; /* mov.w 0x8c666b88,r0 */
L8c666b5a: *stop=0x8c666b5au;r[2]=0u;goto L8c666b5c; /* mov #0x0,r2 */
L8c666b5c: *stop=0x8c666b5cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666b5e; /* mov.l @r13,r3 */
L8c666b5e: *stop=0x8c666b5eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c666b60; /* mov.l r2,@(r0,r3) */
L8c666b60: *stop=0x8c666b60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666b9cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666b62; /* mov.l 0x8c666b9c,r2 */
L8c666b62: *stop=0x8c666b62u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666b8au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666b64; /* mov.w 0x8c666b8a,r3 */
L8c666b64: *stop=0x8c666b64u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}goto L8c666b66; /* mov.l @r13,r4 */
L8c666b66: target=r[2];*pr=0x8c666b6au;*stop=0x8c666b68u;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666b6a; /* jsr @r2 */
L8c666b6a: *stop=0x8c666b6au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c666b6c; /* tst r0,r0 */
L8c666b6c: *stop=0x8c666b6cu;r[14]=r[0];goto L8c666b6e; /* mov r0,r14 */
L8c666b6e: *stop=0x8c666b6eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666b70; /* tst r14,r14 */
L8c666b70: if((*sr&1u))goto L8c666b7a;goto L8c666b72; /* bt 0x8c666b7a */
L8c666b72: *stop=0x8c666b72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ba4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666b74; /* mov.l 0x8c666ba4,r2 */
L8c666b74: *stop=0x8c666b74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ba0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666b76; /* mov.l 0x8c666ba0,r4 */
L8c666b76: target=r[2];*pr=0x8c666b7au;*stop=0x8c666b78u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666b7a; /* jsr @r2 */
L8c666b7a: *stop=0x8c666b7au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666b7c; /* lds.l @r15+,PR */
L8c666b7c: *stop=0x8c666b7cu;r[0]=r[14];goto L8c666b7e; /* mov r14,r0 */
L8c666b7e: *stop=0x8c666b7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666b80; /* mov.l @r15+,r13 */
L8c666b80: *stop=0x8c666b80u;if(*pr!=entry_pr)return -3;*stop=0x8c666b82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c6669c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6669c4;
L8c6669c4: *stop=0x8c6669c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6669c6; /* mov.l r14,@-r15 */
L8c6669c6: *stop=0x8c6669c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6669c8; /* mov.l r13,@-r15 */
L8c6669c8: *stop=0x8c6669c8u;r[13]=0u;goto L8c6669ca; /* mov #0x0,r13 */
L8c6669ca: *stop=0x8c6669cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a48u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6669cc; /* mov.l 0x8c666a48,r14 */
L8c6669cc: *stop=0x8c6669ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6669ce; /* mov.l r12,@-r15 */
L8c6669ce: *stop=0x8c6669ceu;r[12]=r[5];goto L8c6669d0; /* mov r5,r12 */
L8c6669d0: *stop=0x8c6669d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6669d2; /* mov.l r11,@-r15 */
L8c6669d2: *stop=0x8c6669d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6669d4; /* sts.l PR,@-r15 */
L8c6669d4: *stop=0x8c6669d4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6669d6; /* mov.l @r14,r5 */
L8c6669d6: *stop=0x8c6669d6u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6669d8; /* tst r5,r5 */
L8c6669d8: take=!(*sr&1u);*stop=0x8c6669dau;r[11]=r[13];if(take)goto L8c6669e2;goto L8c6669dc; /* bf/s 0x8c6669e2 */
L8c6669dc: *stop=0x8c6669dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a4cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6669de; /* mov.l 0x8c666a4c,r11 */
L8c6669de: *stop=0x8c6669e0u;goto L8c666a2c; /* bra 0x8c666a2c */
L8c6669e2: *stop=0x8c6669e2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666a46u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6669e4; /* mov.w 0x8c666a46,r0 */
L8c6669e4: *stop=0x8c6669e4u;r[6]=28u;goto L8c6669e6; /* mov #0x1c,r6 */
L8c6669e6: *stop=0x8c6669e6u;*macl=r[6]*r[12];goto L8c6669e8; /* mul.l r6,r12 */
L8c6669e8: *stop=0x8c6669e8u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[4],4);if(io_status)return io_status;goto L8c6669ea; /* mov.l r4,@(r0,r5) */
L8c6669ea: *stop=0x8c6669eau;r[0]+=4u;goto L8c6669ec; /* add #0x4,r0 */
L8c6669ec: *stop=0x8c6669ecu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6669ee; /* mov.l @r14,r3 */
L8c6669ee: *stop=0x8c6669eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6669f0; /* mov.l 0x8c666a54,r2 */
L8c6669f0: *stop=0x8c6669f0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[12],4);if(io_status)return io_status;goto L8c6669f2; /* mov.l r12,@(r0,r3) */
L8c6669f2: *stop=0x8c6669f2u;r[0]+=12u;goto L8c6669f4; /* add #0xc,r0 */
L8c6669f4: *stop=0x8c6669f4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6669f6; /* mov.l @r14,r3 */
L8c6669f6: *stop=0x8c6669f6u;r[6]=*macl;goto L8c6669f8; /* sts MACL,r6 */
L8c6669f8: *stop=0x8c6669f8u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c6669fa; /* mov.l r13,@(r0,r3) */
L8c6669fa: *stop=0x8c6669fau;r[0]+=4294967292u;goto L8c6669fc; /* add #-0x4,r0 */
L8c6669fc: *stop=0x8c6669fcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6669fe; /* mov.l @r14,r3 */
L8c6669fe: *stop=0x8c6669feu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c666a00; /* mov.l r13,@(r0,r3) */
L8c666a00: *stop=0x8c666a00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666a02; /* mov.l 0x8c666a50,r3 */
L8c666a02: *stop=0x8c666a02u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c666a04; /* mov.l r4,@r3 */
L8c666a04: target=r[2];*pr=0x8c666a08u;*stop=0x8c666a06u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666a08; /* jsr @r2 */
L8c666a08: *stop=0x8c666a08u;r[3]=0u;goto L8c666a0a; /* mov #0x0,r3 */
L8c666a0a: *stop=0x8c666a0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a58u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c666a0c; /* mov.l 0x8c666a58,r6 */
L8c666a0c: *stop=0x8c666a0cu;*sr=(*sr&~1u)|(r[3]>=r[12]);goto L8c666a0e; /* cmp/hs r12,r3 */
L8c666a0e: *stop=0x8c666a0eu;r[5]=r[13];goto L8c666a10; /* mov r13,r5 */
L8c666a10: take=(*sr&1u);*stop=0x8c666a12u;r[4]=r[13];if(take)goto L8c666a2c;goto L8c666a14; /* bt/s 0x8c666a2c */
L8c666a14: *stop=0x8c666a14u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666a46u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666a16; /* mov.w 0x8c666a46,r0 */
L8c666a16: *stop=0x8c666a16u;r[2]=r[6];goto L8c666a18; /* mov r6,r2 */
L8c666a18: *stop=0x8c666a18u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666a1a; /* mov.l @r14,r1 */
L8c666a1a: *stop=0x8c666a1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666a1c; /* mov.l 0x8c666a5c,r3 */
L8c666a1c: *stop=0x8c666a1cu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666a1e; /* mov.l @(r0,r1),r1 */
L8c666a1e: *stop=0x8c666a1eu;r[1]+=r[4];goto L8c666a20; /* add r4,r1 */
L8c666a20: target=r[3];*pr=0x8c666a24u;*stop=0x8c666a22u;r[0]=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666a24; /* jsr @r3 */
L8c666a24: *stop=0x8c666a24u;r[5]+=1u;goto L8c666a26; /* add #0x1,r5 */
L8c666a26: *stop=0x8c666a26u;*sr=(*sr&~1u)|(r[5]>=r[12]);goto L8c666a28; /* cmp/hs r12,r5 */
L8c666a28: take=!(*sr&1u);*stop=0x8c666a2au;r[4]+=28u;if(take)goto L8c666a14;goto L8c666a2c; /* bf/s 0x8c666a14 */
L8c666a2c: *stop=0x8c666a2cu;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c666a2e; /* tst r11,r11 */
L8c666a2e: if((*sr&1u))goto L8c666a38;goto L8c666a30; /* bt 0x8c666a38 */
L8c666a30: *stop=0x8c666a30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a64u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666a32; /* mov.l 0x8c666a64,r2 */
L8c666a32: *stop=0x8c666a32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666a60u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666a34; /* mov.l 0x8c666a60,r4 */
L8c666a34: target=r[2];*pr=0x8c666a38u;*stop=0x8c666a36u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666a38; /* jsr @r2 */
L8c666a38: *stop=0x8c666a38u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666a3a; /* lds.l @r15+,PR */
L8c666a3a: *stop=0x8c666a3au;r[0]=r[11];goto L8c666a3c; /* mov r11,r0 */
L8c666a3c: *stop=0x8c666a3cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c666a3e; /* mov.l @r15+,r11 */
L8c666a3e: *stop=0x8c666a3eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c666a40; /* mov.l @r15+,r12 */
L8c666a40: *stop=0x8c666a40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666a42; /* mov.l @r15+,r13 */
L8c666a42: *stop=0x8c666a42u;if(*pr!=entry_pr)return -3;*stop=0x8c666a44u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c667240(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c667240;
L8c667240: *stop=0x8c667240u;r[5]=0u;goto L8c667242; /* mov #0x0,r5 */
L8c667242: *stop=0x8c667242u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667358u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c667244; /* mov.l 0x8c667358,r4 */
L8c667244: *stop=0x8c667244u;{uint32_t loaded=sx(m,bus,&io_status,0x8c667354u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c667246; /* mov.w 0x8c667354,r7 */
L8c667246: *stop=0x8c667246u;r[6]=r[5];goto L8c667248; /* mov r5,r6 */
L8c667248: *stop=0x8c667248u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c66724a; /* mov.l r13,@-r15 */
L8c66724a: *stop=0x8c66724au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66724c; /* mov.l @r4,r2 */
L8c66724c: *stop=0x8c66724cu;r[0]=r[5];goto L8c66724e; /* mov r5,r0 */
L8c66724e: *stop=0x8c66724eu;r[2]+=r[6];goto L8c667250; /* add r6,r2 */
L8c667250: *stop=0x8c667250u;if(!valid_access(r[2]+1u,1u))return -3;wr(m,bus,&io_status,r[2]+0x1u,r[0],1);if(io_status)return io_status;goto L8c667252; /* mov.b r0,@(0x1,r2) */
L8c667252: *stop=0x8c667252u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c667254; /* mov.l @r4,r2 */
L8c667254: *stop=0x8c667254u;r[2]+=r[6];goto L8c667256; /* add r6,r2 */
L8c667256: *stop=0x8c667256u;r[6]+=8u;goto L8c667258; /* add #0x8,r6 */
L8c667258: *stop=0x8c667258u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c66725a; /* cmp/hs r7,r6 */
L8c66725a: take=!(*sr&1u);*stop=0x8c66725cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c66724a;goto L8c66725e; /* bf/s 0x8c66724a */
L8c66725e: *stop=0x8c66725eu;r[7]=r[5];goto L8c667260; /* mov r5,r7 */
L8c667260: *stop=0x8c667260u;r[6]=8u;goto L8c667262; /* mov #0x8,r6 */
L8c667262: *stop=0x8c667262u;r[1]=1u;goto L8c667264; /* mov #0x1,r1 */
L8c667264: *stop=0x8c667264u;r[13]=r[7];goto L8c667266; /* mov r7,r13 */
L8c667266: *stop=0x8c667266u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c667268; /* mov.l @r4,r3 */
L8c667268: *stop=0x8c667268u;r[13]<<=2;goto L8c66726a; /* shll2 r13 */
L8c66726a: *stop=0x8c66726au;r[0]=r[7];goto L8c66726c; /* mov r7,r0 */
L8c66726c: *stop=0x8c66726cu;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c66726e; /* shll r13 */
L8c66726e: *stop=0x8c66726eu;r[3]+=8u;goto L8c667270; /* add #0x8,r3 */
L8c667270: *stop=0x8c667270u;r[7]+=1u;goto L8c667272; /* add #0x1,r7 */
L8c667272: *stop=0x8c667272u;r[3]+=r[13];goto L8c667274; /* add r13,r3 */
L8c667274: *stop=0x8c667274u;*sr=(*sr&~1u)|(r[7]>=r[6]);goto L8c667276; /* cmp/hs r6,r7 */
L8c667276: *stop=0x8c667276u;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c667278; /* mov.b r0,@(0x1,r3) */
L8c667278: *stop=0x8c667278u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66727a; /* mov.l @r4,r2 */
L8c66727a: *stop=0x8c66727au;r[2]+=8u;goto L8c66727c; /* add #0x8,r2 */
L8c66727c: *stop=0x8c66727cu;r[13]+=r[2];goto L8c66727e; /* add r2,r13 */
L8c66727e: take=!(*sr&1u);*stop=0x8c667280u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[1],1);if(io_status)return io_status;if(take)goto L8c667264;goto L8c667282; /* bf/s 0x8c667264 */
L8c667282: *stop=0x8c667282u;r[7]=r[5];goto L8c667284; /* mov r5,r7 */
L8c667284: *stop=0x8c667284u;r[13]=2u;goto L8c667286; /* mov #0x2,r13 */
L8c667286: *stop=0x8c667286u;r[1]=r[7];goto L8c667288; /* mov r7,r1 */
L8c667288: *stop=0x8c667288u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66728a; /* mov.l @r4,r3 */
L8c66728a: *stop=0x8c66728au;r[1]<<=2;goto L8c66728c; /* shll2 r1 */
L8c66728c: *stop=0x8c66728cu;r[0]=r[7];goto L8c66728e; /* mov r7,r0 */
L8c66728e: *stop=0x8c66728eu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c667290; /* shll r1 */
L8c667290: *stop=0x8c667290u;r[3]+=72u;goto L8c667292; /* add #0x48,r3 */
L8c667292: *stop=0x8c667292u;r[7]+=1u;goto L8c667294; /* add #0x1,r7 */
L8c667294: *stop=0x8c667294u;r[3]+=r[1];goto L8c667296; /* add r1,r3 */
L8c667296: *stop=0x8c667296u;*sr=(*sr&~1u)|(r[7]>=r[6]);goto L8c667298; /* cmp/hs r6,r7 */
L8c667298: *stop=0x8c667298u;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c66729a; /* mov.b r0,@(0x1,r3) */
L8c66729a: *stop=0x8c66729au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66729c; /* mov.l @r4,r2 */
L8c66729c: *stop=0x8c66729cu;r[2]+=72u;goto L8c66729e; /* add #0x48,r2 */
L8c66729e: *stop=0x8c66729eu;r[1]+=r[2];goto L8c6672a0; /* add r2,r1 */
L8c6672a0: take=!(*sr&1u);*stop=0x8c6672a2u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[13],1);if(io_status)return io_status;if(take)goto L8c667286;goto L8c6672a4; /* bf/s 0x8c667286 */
L8c6672a4: *stop=0x8c6672a4u;r[6]=r[5];goto L8c6672a6; /* mov r5,r6 */
L8c6672a6: *stop=0x8c6672a6u;r[1]=3u;goto L8c6672a8; /* mov #0x3,r1 */
L8c6672a8: *stop=0x8c6672a8u;r[13]=4u;goto L8c6672aa; /* mov #0x4,r13 */
L8c6672aa: *stop=0x8c6672aau;{uint32_t loaded=sx(m,bus,&io_status,0x8c667356u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6672ac; /* mov.w 0x8c667356,r3 */
L8c6672ac: *stop=0x8c6672acu;r[7]=r[6];goto L8c6672ae; /* mov r6,r7 */
L8c6672ae: *stop=0x8c6672aeu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6672b0; /* mov.l @r4,r2 */
L8c6672b0: *stop=0x8c6672b0u;r[7]<<=2;goto L8c6672b2; /* shll2 r7 */
L8c6672b2: *stop=0x8c6672b2u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6672b4; /* shll r7 */
L8c6672b4: *stop=0x8c6672b4u;r[0]=r[6];goto L8c6672b6; /* mov r6,r0 */
L8c6672b6: *stop=0x8c6672b6u;r[3]+=r[2];goto L8c6672b8; /* add r2,r3 */
L8c6672b8: *stop=0x8c6672b8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c667356u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6672ba; /* mov.w 0x8c667356,r2 */
L8c6672ba: *stop=0x8c6672bau;r[3]+=r[7];goto L8c6672bc; /* add r7,r3 */
L8c6672bc: *stop=0x8c6672bcu;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c6672be; /* mov.b r0,@(0x1,r3) */
L8c6672be: *stop=0x8c6672beu;r[6]+=1u;goto L8c6672c0; /* add #0x1,r6 */
L8c6672c0: *stop=0x8c6672c0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6672c2; /* mov.l @r4,r3 */
L8c6672c2: *stop=0x8c6672c2u;*sr=(*sr&~1u)|(r[6]>=r[13]);goto L8c6672c4; /* cmp/hs r13,r6 */
L8c6672c4: *stop=0x8c6672c4u;r[2]+=r[3];goto L8c6672c6; /* add r3,r2 */
L8c6672c6: *stop=0x8c6672c6u;r[7]+=r[2];goto L8c6672c8; /* add r2,r7 */
L8c6672c8: take=!(*sr&1u);*stop=0x8c6672cau;if(!valid_access(r[7],1u))return -3;wr(m,bus,&io_status,r[7],r[1],1);if(io_status)return io_status;if(take)goto L8c6672aa;goto L8c6672cc; /* bf/s 0x8c6672aa */
L8c6672cc: *stop=0x8c6672ccu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6672ce; /* mov.l @r4,r2 */
L8c6672ce: *stop=0x8c6672ceu;r[0]=r[5];goto L8c6672d0; /* mov r5,r0 */
L8c6672d0: *stop=0x8c6672d0u;if(!valid_access(r[2]+1u,1u))return -3;wr(m,bus,&io_status,r[2]+0x1u,r[0],1);if(io_status)return io_status;goto L8c6672d2; /* mov.b r0,@(0x1,r2) */
L8c6672d2: *stop=0x8c6672d2u;r[2]=9u;goto L8c6672d4; /* mov #0x9,r2 */
L8c6672d4: *stop=0x8c6672d4u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6672d6; /* mov.l @r4,r3 */
L8c6672d6: *stop=0x8c6672d6u;r[0]=0u;goto L8c6672d8; /* mov #0x0,r0 */
L8c6672d8: *stop=0x8c6672d8u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c6672da; /* mov.b r2,@r3 */
L8c6672da: *stop=0x8c6672dau;if(*pr!=entry_pr)return -3;*stop=0x8c6672dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c10ec40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10ec40;
L8c10ec40: *stop=0x8c10ec40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10ec42; /* mov.l r14,@-r15 */
L8c10ec42: *stop=0x8c10ec42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10ec44; /* mov.l r13,@-r15 */
L8c10ec44: *stop=0x8c10ec44u;r[15]+=4294967284u;goto L8c10ec46; /* add #-0xc,r15 */
L8c10ec46: *stop=0x8c10ec46u;r[6]=52u;goto L8c10ec48; /* mov #0x34,r6 */
L8c10ec48: *stop=0x8c10ec48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ed4cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10ec4a; /* mov.l 0x8c10ed4c,r7 */
L8c10ec4a: *stop=0x8c10ec4au;*macl=r[6]*r[5];goto L8c10ec4c; /* mul.l r6,r5 */
L8c10ec4c: *stop=0x8c10ec4cu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec4e; /* mov.l @r7,r3 */
L8c10ec4e: *stop=0x8c10ec4eu;r[14]=0u;goto L8c10ec50; /* mov #0x0,r14 */
L8c10ec50: *stop=0x8c10ec50u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10ec52; /* mov.l r3,@(0x8,r15) */
L8c10ec52: *stop=0x8c10ec52u;r[3]+=4u;goto L8c10ec54; /* add #0x4,r3 */
L8c10ec54: *stop=0x8c10ec54u;r[5]=*macl;goto L8c10ec56; /* sts MACL,r5 */
L8c10ec56: *stop=0x8c10ec56u;r[5]+=r[3];goto L8c10ec58; /* add r3,r5 */
L8c10ec58: *stop=0x8c10ec58u;if(!valid_access(r[5]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ec5a; /* mov.l @(0x28,r5),r0 */
L8c10ec5a: *stop=0x8c10ec5au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10ec5c; /* cmp/eq #0x1,r0 */
L8c10ec5c: take=!(*sr&1u);*stop=0x8c10ec5eu;r[13]=r[14];if(take)goto L8c10ec66;goto L8c10ec60; /* bf/s 0x8c10ec66 */
L8c10ec60: *stop=0x8c10ec60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ed50u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c10ec62; /* mov.l 0x8c10ed50,r13 */
L8c10ec62: *stop=0x8c10ec64u;goto L8c10ed40; /* bra 0x8c10ed40 */
L8c10ec66: *stop=0x8c10ec66u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10ec68; /* stc SR,r0 */
L8c10ec68: *stop=0x8c10ec68u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ed4au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec6a; /* mov.w 0x8c10ed4a,r3 */
L8c10ec6a: *stop=0x8c10ec6au;r[0]>>=2;goto L8c10ec6c; /* shlr2 r0 */
L8c10ec6c: *stop=0x8c10ec6cu;r[0]>>=2;goto L8c10ec6e; /* shlr2 r0 */
L8c10ec6e: *stop=0x8c10ec6eu;r[0]&=0xfu;goto L8c10ec70; /* and #0xf,r0 */
L8c10ec70: *stop=0x8c10ec70u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10ec72; /* mov.l r0,@(0x4,r15) */
L8c10ec72: *stop=0x8c10ec72u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10ec74; /* stc SR,r0 */
L8c10ec74: *stop=0x8c10ec74u;r[0]&=r[3];goto L8c10ec76; /* and r3,r0 */
L8c10ec76: *stop=0x8c10ec76u;r[0]|=0xf0u;goto L8c10ec78; /* or #0xf0,r0 */
L8c10ec78: *stop=0x8c10ec78u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10ec7a; /* ldc r0,SR */
L8c10ec7a: *stop=0x8c10ec7au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec7c; /* mov.l @r5,r3 */
L8c10ec7c: *stop=0x8c10ec7cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ec7e; /* mov.l @(0x8,r15),r2 */
L8c10ec7e: *stop=0x8c10ec7eu;*macl=r[6]*r[3];goto L8c10ec80; /* mul.l r6,r3 */
L8c10ec80: *stop=0x8c10ec80u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10ec82; /* mov.l r3,@r15 */
L8c10ec82: *stop=0x8c10ec82u;r[2]+=4u;goto L8c10ec84; /* add #0x4,r2 */
L8c10ec84: *stop=0x8c10ec84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ec86; /* mov.l @r15,r1 */
L8c10ec86: *stop=0x8c10ec86u;r[3]=*macl;goto L8c10ec88; /* sts MACL,r3 */
L8c10ec88: *stop=0x8c10ec88u;r[3]+=r[2];goto L8c10ec8a; /* add r2,r3 */
L8c10ec8a: *stop=0x8c10ec8au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c10ec8c; /* mov.l r1,@r3 */
L8c10ec8c: *stop=0x8c10ec8cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ec8e; /* mov.l @r5,r2 */
L8c10ec8e: *stop=0x8c10ec8eu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec90; /* mov.l @r7,r3 */
L8c10ec90: *stop=0x8c10ec90u;*macl=r[6]*r[2];goto L8c10ec92; /* mul.l r6,r2 */
L8c10ec92: *stop=0x8c10ec92u;r[3]+=4u;goto L8c10ec94; /* add #0x4,r3 */
L8c10ec94: *stop=0x8c10ec94u;r[2]=*macl;goto L8c10ec96; /* sts MACL,r2 */
L8c10ec96: *stop=0x8c10ec96u;r[2]+=r[3];goto L8c10ec98; /* add r3,r2 */
L8c10ec98: *stop=0x8c10ec98u;if(!valid_access(r[2]+40u,4u))return -3;wr(m,bus,&io_status,r[2]+0x28u,r[14],4);if(io_status)return io_status;goto L8c10ec9a; /* mov.l r14,@(0x28,r2) */
L8c10ec9a: *stop=0x8c10ec9au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec9c; /* mov.l @r5,r3 */
L8c10ec9c: *stop=0x8c10ec9cu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ec9e; /* mov.l @r7,r2 */
L8c10ec9e: *stop=0x8c10ec9eu;*macl=r[6]*r[3];goto L8c10eca0; /* mul.l r6,r3 */
L8c10eca0: *stop=0x8c10eca0u;r[2]+=4u;goto L8c10eca2; /* add #0x4,r2 */
L8c10eca2: *stop=0x8c10eca2u;r[3]=*macl;goto L8c10eca4; /* sts MACL,r3 */
L8c10eca4: *stop=0x8c10eca4u;r[3]+=r[2];goto L8c10eca6; /* add r2,r3 */
L8c10eca6: *stop=0x8c10eca6u;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[14],4);if(io_status)return io_status;goto L8c10eca8; /* mov.l r14,@(0x2c,r3) */
L8c10eca8: *stop=0x8c10eca8u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ecaa; /* mov.l @r5,r2 */
L8c10ecaa: *stop=0x8c10ecaau;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ecac; /* mov.l @r7,r3 */
L8c10ecac: *stop=0x8c10ecacu;*macl=r[6]*r[2];goto L8c10ecae; /* mul.l r6,r2 */
L8c10ecae: *stop=0x8c10ecaeu;r[3]+=4u;goto L8c10ecb0; /* add #0x4,r3 */
L8c10ecb0: *stop=0x8c10ecb0u;r[2]=*macl;goto L8c10ecb2; /* sts MACL,r2 */
L8c10ecb2: *stop=0x8c10ecb2u;r[2]+=r[3];goto L8c10ecb4; /* add r3,r2 */
L8c10ecb4: *stop=0x8c10ecb4u;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[14],4);if(io_status)return io_status;goto L8c10ecb6; /* mov.l r14,@(0x30,r2) */
L8c10ecb6: *stop=0x8c10ecb6u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ecb8; /* mov.l @r5,r3 */
L8c10ecb8: *stop=0x8c10ecb8u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ecba; /* mov.l @r7,r2 */
L8c10ecba: *stop=0x8c10ecbau;*macl=r[6]*r[3];goto L8c10ecbc; /* mul.l r6,r3 */
L8c10ecbc: *stop=0x8c10ecbcu;r[2]+=4u;goto L8c10ecbe; /* add #0x4,r2 */
L8c10ecbe: *stop=0x8c10ecbeu;r[3]=*macl;goto L8c10ecc0; /* sts MACL,r3 */
L8c10ecc0: *stop=0x8c10ecc0u;r[3]+=r[2];goto L8c10ecc2; /* add r2,r3 */
L8c10ecc2: *stop=0x8c10ecc2u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[14],4);if(io_status)return io_status;goto L8c10ecc4; /* mov.l r14,@(0x4,r3) */
L8c10ecc4: *stop=0x8c10ecc4u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ecc6; /* mov.l @r5,r2 */
L8c10ecc6: *stop=0x8c10ecc6u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ecc8; /* mov.l @r7,r3 */
L8c10ecc8: *stop=0x8c10ecc8u;*macl=r[6]*r[2];goto L8c10ecca; /* mul.l r6,r2 */
L8c10ecca: *stop=0x8c10eccau;r[3]+=4u;goto L8c10eccc; /* add #0x4,r3 */
L8c10eccc: *stop=0x8c10ecccu;r[2]=*macl;goto L8c10ecce; /* sts MACL,r2 */
L8c10ecce: *stop=0x8c10ecceu;r[2]+=r[3];goto L8c10ecd0; /* add r3,r2 */
L8c10ecd0: *stop=0x8c10ecd0u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[14],4);if(io_status)return io_status;goto L8c10ecd2; /* mov.l r14,@(0x8,r2) */
L8c10ecd2: *stop=0x8c10ecd2u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ecd4; /* mov.l @r5,r3 */
L8c10ecd4: *stop=0x8c10ecd4u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ecd6; /* mov.l @r7,r2 */
L8c10ecd6: *stop=0x8c10ecd6u;*macl=r[6]*r[3];goto L8c10ecd8; /* mul.l r6,r3 */
L8c10ecd8: *stop=0x8c10ecd8u;r[2]+=4u;goto L8c10ecda; /* add #0x4,r2 */
L8c10ecda: *stop=0x8c10ecdau;r[3]=*macl;goto L8c10ecdc; /* sts MACL,r3 */
L8c10ecdc: *stop=0x8c10ecdcu;r[3]+=r[2];goto L8c10ecde; /* add r2,r3 */
L8c10ecde: *stop=0x8c10ecdeu;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[14],4);if(io_status)return io_status;goto L8c10ece0; /* mov.l r14,@(0xc,r3) */
L8c10ece0: *stop=0x8c10ece0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ece2; /* mov.l @r5,r2 */
L8c10ece2: *stop=0x8c10ece2u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ece4; /* mov.l @r7,r3 */
L8c10ece4: *stop=0x8c10ece4u;*macl=r[6]*r[2];goto L8c10ece6; /* mul.l r6,r2 */
L8c10ece6: *stop=0x8c10ece6u;r[3]+=4u;goto L8c10ece8; /* add #0x4,r3 */
L8c10ece8: *stop=0x8c10ece8u;r[2]=*macl;goto L8c10ecea; /* sts MACL,r2 */
L8c10ecea: *stop=0x8c10eceau;r[2]+=r[3];goto L8c10ecec; /* add r3,r2 */
L8c10ecec: *stop=0x8c10ececu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[14],4);if(io_status)return io_status;goto L8c10ecee; /* mov.l r14,@(0x10,r2) */
L8c10ecee: *stop=0x8c10eceeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ecf0; /* mov.l @r5,r3 */
L8c10ecf0: *stop=0x8c10ecf0u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ecf2; /* mov.l @r7,r2 */
L8c10ecf2: *stop=0x8c10ecf2u;*macl=r[6]*r[3];goto L8c10ecf4; /* mul.l r6,r3 */
L8c10ecf4: *stop=0x8c10ecf4u;r[2]+=4u;goto L8c10ecf6; /* add #0x4,r2 */
L8c10ecf6: *stop=0x8c10ecf6u;r[3]=*macl;goto L8c10ecf8; /* sts MACL,r3 */
L8c10ecf8: *stop=0x8c10ecf8u;r[3]+=r[2];goto L8c10ecfa; /* add r2,r3 */
L8c10ecfa: *stop=0x8c10ecfau;if(!valid_access(r[3]+20u,4u))return -3;wr(m,bus,&io_status,r[3]+0x14u,r[14],4);if(io_status)return io_status;goto L8c10ecfc; /* mov.l r14,@(0x14,r3) */
L8c10ecfc: *stop=0x8c10ecfcu;r[1]=3u;goto L8c10ecfe; /* mov #0x3,r1 */
L8c10ecfe: *stop=0x8c10ecfeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed00; /* mov.l @r5,r2 */
L8c10ed00: *stop=0x8c10ed00u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed02; /* mov.l @r7,r3 */
L8c10ed02: *stop=0x8c10ed02u;*macl=r[6]*r[2];goto L8c10ed04; /* mul.l r6,r2 */
L8c10ed04: *stop=0x8c10ed04u;r[3]+=4u;goto L8c10ed06; /* add #0x4,r3 */
L8c10ed06: *stop=0x8c10ed06u;r[2]=*macl;goto L8c10ed08; /* sts MACL,r2 */
L8c10ed08: *stop=0x8c10ed08u;r[2]+=r[3];goto L8c10ed0a; /* add r3,r2 */
L8c10ed0a: *stop=0x8c10ed0au;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[1],4);if(io_status)return io_status;goto L8c10ed0c; /* mov.l r1,@(0x18,r2) */
L8c10ed0c: *stop=0x8c10ed0cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed0e; /* mov.l @r5,r2 */
L8c10ed0e: *stop=0x8c10ed0eu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed10; /* mov.l @r7,r3 */
L8c10ed10: *stop=0x8c10ed10u;*macl=r[6]*r[2];goto L8c10ed12; /* mul.l r6,r2 */
L8c10ed12: *stop=0x8c10ed12u;r[3]+=4u;goto L8c10ed14; /* add #0x4,r3 */
L8c10ed14: *stop=0x8c10ed14u;r[2]=*macl;goto L8c10ed16; /* sts MACL,r2 */
L8c10ed16: *stop=0x8c10ed16u;r[2]+=r[3];goto L8c10ed18; /* add r3,r2 */
L8c10ed18: *stop=0x8c10ed18u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c10ed1a; /* mov.l r14,@(0x1c,r2) */
L8c10ed1a: *stop=0x8c10ed1au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed1c; /* mov.l @r5,r3 */
L8c10ed1c: *stop=0x8c10ed1cu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed1e; /* mov.l @r7,r2 */
L8c10ed1e: *stop=0x8c10ed1eu;*macl=r[6]*r[3];goto L8c10ed20; /* mul.l r6,r3 */
L8c10ed20: *stop=0x8c10ed20u;r[2]+=4u;goto L8c10ed22; /* add #0x4,r2 */
L8c10ed22: *stop=0x8c10ed22u;r[3]=1u;goto L8c10ed24; /* mov #0x1,r3 */
L8c10ed24: *stop=0x8c10ed24u;r[6]=*macl;goto L8c10ed26; /* sts MACL,r6 */
L8c10ed26: *stop=0x8c10ed26u;r[6]+=r[2];goto L8c10ed28; /* add r2,r6 */
L8c10ed28: *stop=0x8c10ed28u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c10ed2a; /* stc SR,r2 */
L8c10ed2a: *stop=0x8c10ed2au;if(!valid_access(r[6]+32u,4u))return -3;wr(m,bus,&io_status,r[6]+0x20u,r[14],4);if(io_status)return io_status;goto L8c10ed2c; /* mov.l r14,@(0x20,r6) */
L8c10ed2c: *stop=0x8c10ed2cu;if(!valid_access(r[5]+40u,4u))return -3;wr(m,bus,&io_status,r[5]+0x28u,r[3],4);if(io_status)return io_status;goto L8c10ed2e; /* mov.l r3,@(0x28,r5) */
L8c10ed2e: *stop=0x8c10ed2eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ed4au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed30; /* mov.w 0x8c10ed4a,r3 */
L8c10ed30: *stop=0x8c10ed30u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c10ed32; /* mov.l r5,@r4 */
L8c10ed32: *stop=0x8c10ed32u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ed34; /* mov.l @(0x4,r15),r0 */
L8c10ed34: *stop=0x8c10ed34u;r[0]&=0xfu;goto L8c10ed36; /* and #0xf,r0 */
L8c10ed36: *stop=0x8c10ed36u;r[0]<<=2;goto L8c10ed38; /* shll2 r0 */
L8c10ed38: *stop=0x8c10ed38u;r[0]<<=2;goto L8c10ed3a; /* shll2 r0 */
L8c10ed3a: *stop=0x8c10ed3au;r[2]&=r[3];goto L8c10ed3c; /* and r3,r2 */
L8c10ed3c: *stop=0x8c10ed3cu;r[0]|=r[2];goto L8c10ed3e; /* or r2,r0 */
L8c10ed3e: *stop=0x8c10ed3eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10ed40; /* ldc r0,SR */
L8c10ed40: *stop=0x8c10ed40u;r[0]=r[13];goto L8c10ed42; /* mov r13,r0 */
L8c10ed42: *stop=0x8c10ed42u;r[15]+=12u;goto L8c10ed44; /* add #0xc,r15 */
L8c10ed44: *stop=0x8c10ed44u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10ed46; /* mov.l @r15+,r13 */
L8c10ed46: *stop=0x8c10ed46u;if(*pr!=entry_pr)return -3;*stop=0x8c10ed48u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c10cd44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cd44;
L8c10cd44: *stop=0x8c10cd44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10cd46; /* mov.l r3,@-r15 */
L8c10cd46: *stop=0x8c10cd46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cd50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd48; /* mov.l 0x8c10cd50,r3 */
L8c10cd48: *stop=0x8c10cd48u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd4a; /* mov.l @(r0,r3),r3 */
L8c10cd4a: *stop=0x8c10cd4au;r[0]+=4294967292u;goto L8c10cd4c; /* add #-0x4,r0 */
L8c10cd4c: target=r[3];*stop=0x8c10cd4eu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c10cd76(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cd76;
L8c10cd76: *stop=0x8c10cd76u;if(!valid_access(r[2]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd78; /* mov.l @(0x18,r2),r3 */
L8c10cd78: *stop=0x8c10cd78u;if(!valid_access(r[1]+28u,4u))return -3;wr(m,bus,&io_status,r[1]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c10cd7a; /* mov.l r0,@(0x1c,r1) */
L8c10cd7a: *stop=0x8c10cd7au;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd7c; /* mov.l @(0x14,r2),r0 */
L8c10cd7c: *stop=0x8c10cd7cu;if(!valid_access(r[1]+24u,4u))return -3;wr(m,bus,&io_status,r[1]+0x18u,r[3],4);if(io_status)return io_status;goto L8c10cd7e; /* mov.l r3,@(0x18,r1) */
L8c10cd7e: *stop=0x8c10cd7eu;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd80; /* mov.l @(0x10,r2),r3 */
L8c10cd80: *stop=0x8c10cd80u;if(!valid_access(r[1]+20u,4u))return -3;wr(m,bus,&io_status,r[1]+0x14u,r[0],4);if(io_status)return io_status;goto L8c10cd82; /* mov.l r0,@(0x14,r1) */
L8c10cd82: *stop=0x8c10cd82u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd84; /* mov.l @(0xc,r2),r0 */
L8c10cd84: *stop=0x8c10cd84u;if(!valid_access(r[1]+16u,4u))return -3;wr(m,bus,&io_status,r[1]+0x10u,r[3],4);if(io_status)return io_status;goto L8c10cd86; /* mov.l r3,@(0x10,r1) */
L8c10cd86: *stop=0x8c10cd86u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd88; /* mov.l @(0x8,r2),r3 */
L8c10cd88: *stop=0x8c10cd88u;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10cd8a; /* mov.l r0,@(0xc,r1) */
L8c10cd8a: *stop=0x8c10cd8au;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd8c; /* mov.l @(0x4,r2),r0 */
L8c10cd8c: *stop=0x8c10cd8cu;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10cd8e; /* mov.l r3,@(0x8,r1) */
L8c10cd8e: *stop=0x8c10cd8eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd90; /* mov.l @r2,r3 */
L8c10cd90: *stop=0x8c10cd90u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10cd92; /* mov.l r0,@(0x4,r1) */
L8c10cd92: *stop=0x8c10cd92u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c10cd94; /* mov.l r3,@r1 */
L8c10cd94: *stop=0x8c10cd94u;if(*pr!=entry_pr)return -3;*stop=0x8c10cd96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_constructors_8c10cd7a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cd7a;
L8c10cd7a: *stop=0x8c10cd7au;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd7c; /* mov.l @(0x14,r2),r0 */
L8c10cd7c: *stop=0x8c10cd7cu;if(!valid_access(r[1]+24u,4u))return -3;wr(m,bus,&io_status,r[1]+0x18u,r[3],4);if(io_status)return io_status;goto L8c10cd7e; /* mov.l r3,@(0x18,r1) */
L8c10cd7e: *stop=0x8c10cd7eu;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd80; /* mov.l @(0x10,r2),r3 */
L8c10cd80: *stop=0x8c10cd80u;if(!valid_access(r[1]+20u,4u))return -3;wr(m,bus,&io_status,r[1]+0x14u,r[0],4);if(io_status)return io_status;goto L8c10cd82; /* mov.l r0,@(0x14,r1) */
L8c10cd82: *stop=0x8c10cd82u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd84; /* mov.l @(0xc,r2),r0 */
L8c10cd84: *stop=0x8c10cd84u;if(!valid_access(r[1]+16u,4u))return -3;wr(m,bus,&io_status,r[1]+0x10u,r[3],4);if(io_status)return io_status;goto L8c10cd86; /* mov.l r3,@(0x10,r1) */
L8c10cd86: *stop=0x8c10cd86u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd88; /* mov.l @(0x8,r2),r3 */
L8c10cd88: *stop=0x8c10cd88u;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10cd8a; /* mov.l r0,@(0xc,r1) */
L8c10cd8a: *stop=0x8c10cd8au;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd8c; /* mov.l @(0x4,r2),r0 */
L8c10cd8c: *stop=0x8c10cd8cu;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10cd8e; /* mov.l r3,@(0x8,r1) */
L8c10cd8e: *stop=0x8c10cd8eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd90; /* mov.l @r2,r3 */
L8c10cd90: *stop=0x8c10cd90u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10cd92; /* mov.l r0,@(0x4,r1) */
L8c10cd92: *stop=0x8c10cd92u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c10cd94; /* mov.l r3,@r1 */
L8c10cd94: *stop=0x8c10cd94u;if(*pr!=entry_pr)return -3;*stop=0x8c10cd96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
}
