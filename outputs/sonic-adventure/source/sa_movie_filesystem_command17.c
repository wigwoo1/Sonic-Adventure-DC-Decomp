/* Original command17 multiple-name deletion.
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
static int sa_movie_filesystem_command17_run_8c649e00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c649e46u)goto L8c649e46;if(resume_pc==0x8c649e50u)goto L8c649e50;if(resume_pc==0x8c649e5au)goto L8c649e5a;if(resume_pc==0x8c649e76u)goto L8c649e76;if(resume_pc==0x8c649e92u)goto L8c649e92;if(resume_pc==0x8c649eaeu)goto L8c649eae;if(resume_pc==0x8c649ec0u)goto L8c649ec0;if(resume_pc==0x8c649eccu)goto L8c649ecc;goto L8c649e00;
L8c649e00: *stop=0x8c649e00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c649e02; /* mov.l r14,@-r15 */
L8c649e02: *stop=0x8c649e02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c649e04; /* mov.l r13,@-r15 */
L8c649e04: *stop=0x8c649e04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c649e06; /* mov.l r12,@-r15 */
L8c649e06: *stop=0x8c649e06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c649e08; /* mov.l r11,@-r15 */
L8c649e08: *stop=0x8c649e08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c649e0a; /* mov.l r10,@-r15 */
L8c649e0a: *stop=0x8c649e0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c649e0c; /* mov.l r9,@-r15 */
L8c649e0c: *stop=0x8c649e0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c649e0e; /* mov.l r8,@-r15 */
L8c649e0e: *stop=0x8c649e0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c649e10; /* sts.l PR,@-r15 */
L8c649e10: *stop=0x8c649e10u;r[15]+=4294967288u;goto L8c649e12; /* add #-0x8,r15 */
L8c649e12: *stop=0x8c649e12u;r[13]=sx(m,bus,&io_status,0x8c649ee0u,2);if(io_status)return io_status;goto L8c649e14; /* mov.w 0x8c649ee0,r13 */
L8c649e14: *stop=0x8c649e14u;r[14]=r[4];goto L8c649e16; /* mov r4,r14 */
L8c649e16: *stop=0x8c649e16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ee8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649e18; /* mov.l 0x8c649ee8,r3 */
L8c649e18: *stop=0x8c649e18u;r[11]=1u;goto L8c649e1a; /* mov #0x1,r11 */
L8c649e1a: *stop=0x8c649e1au;*macl=r[13]*r[14];goto L8c649e1c; /* mul.l r13,r14 */
L8c649e1c: *stop=0x8c649e1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649eecu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c649e1e; /* mov.l 0x8c649eec,r0 */
L8c649e1e: *stop=0x8c649e1eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c649e20; /* mov.l r5,@r15 */
L8c649e20: *stop=0x8c649e20u;r[13]=*macl;goto L8c649e22; /* sts MACL,r13 */
L8c649e22: *stop=0x8c649e22u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[13],4);if(io_status)return io_status;goto L8c649e24; /* mov.l r13,@(0x4,r15) */
L8c649e24: *stop=0x8c649e24u;r[13]+=r[3];goto L8c649e26; /* add r3,r13 */
L8c649e26: *stop=0x8c649e26u;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[11],4);if(io_status)return io_status;goto L8c649e28; /* mov.l r11,@(0x30,r13) */
L8c649e28: *stop=0x8c649e28u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649e2a; /* mov.l @(0x4,r13),r2 */
L8c649e2a: *stop=0x8c649e2au;r[1]=r[2];goto L8c649e2c; /* mov r2,r1 */
L8c649e2c: *stop=0x8c649e2cu;r[2]<<=2;goto L8c649e2e; /* shll2 r2 */
L8c649e2e: *stop=0x8c649e2eu;r[2]+=r[1];goto L8c649e30; /* add r1,r2 */
L8c649e30: *stop=0x8c649e30u;r[2]<<=2;goto L8c649e32; /* shll2 r2 */
L8c649e32: *stop=0x8c649e32u;r[2]<<=2;goto L8c649e34; /* shll2 r2 */
L8c649e34: *stop=0x8c649e34u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c649e36; /* mov.l @(r0,r2),r0 */
L8c649e36: *stop=0x8c649e36u;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[0],4);if(io_status)return io_status;goto L8c649e38; /* mov.l r0,@(0x34,r13) */
L8c649e38: *stop=0x8c649e38u;if(!valid_access(r[13]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,1);if(io_status)return io_status;goto L8c649e3a; /* mov.b @(0x2,r13),r0 */
L8c649e3a: *stop=0x8c649e3au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649e3c; /* tst r0,r0 */
L8c649e3c: take=(*sr&1u);*stop=0x8c649e3eu;r[12]=r[6];if(take)goto L8c649e6a;goto L8c649e40; /* bt/s 0x8c649e6a */
L8c649e40: *stop=0x8c649e40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ef0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649e42; /* mov.l 0x8c649ef0,r3 */
L8c649e42: target=r[3];*pr=0x8c649e46u;*stop=0x8c649e44u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649e46; /* jsr @r3 */
L8c649e46: *stop=0x8c649e46u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649e48; /* tst r0,r0 */
L8c649e48: if(!(*sr&1u))goto L8c649e54;goto L8c649e4a; /* bf 0x8c649e54 */
L8c649e4a: *stop=0x8c649e4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ef4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649e4c; /* mov.l 0x8c649ef4,r3 */
L8c649e4c: target=r[3];*pr=0x8c649e50u;*stop=0x8c649e4eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649e50; /* jsr @r3 */
L8c649e50: *stop=0x8c649e52u;goto L8c649e6a; /* bra 0x8c649e6a */
L8c649e54: *stop=0x8c649e54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ef8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649e56; /* mov.l 0x8c649ef8,r2 */
L8c649e56: target=r[2];*pr=0x8c649e5au;*stop=0x8c649e58u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649e5a; /* jsr @r2 */
L8c649e5a: *stop=0x8c649e5au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649e5c; /* tst r0,r0 */
L8c649e5c: if((*sr&1u))goto L8c649e6a;goto L8c649e5e; /* bt 0x8c649e6a */
L8c649e5e: *stop=0x8c649e5eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649e60; /* mov.l @(0x4,r15),r2 */
L8c649e60: *stop=0x8c649e60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ee8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649e62; /* mov.l 0x8c649ee8,r3 */
L8c649e62: *stop=0x8c649e62u;r[2]+=r[3];goto L8c649e64; /* add r3,r2 */
L8c649e64: *stop=0x8c649e64u;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c649e66; /* mov.b @(0x8,r2),r0 */
L8c649e66: *stop=0x8c649e66u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649e68; /* tst r0,r0 */
L8c649e68: if(!(*sr&1u))goto L8c649e70;goto L8c649e6a; /* bf 0x8c649e70 */
L8c649e6a: *stop=0x8c649e6au;r[0]=sx(m,bus,&io_status,0x8c649ee2u,2);if(io_status)return io_status;goto L8c649e6c; /* mov.w 0x8c649ee2,r0 */
L8c649e6c: *stop=0x8c649e6eu;goto L8c649f0a; /* bra 0x8c649f0a */
L8c649e70: *stop=0x8c649e70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649efcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649e72; /* mov.l 0x8c649efc,r3 */
L8c649e72: target=r[3];*pr=0x8c649e76u;*stop=0x8c649e74u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649e76; /* jsr @r3 */
L8c649e76: *stop=0x8c649e76u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649e78; /* cmp/pz r0 */
L8c649e78: if((*sr&1u))goto L8c649e80;goto L8c649e7a; /* bt 0x8c649e80 */
L8c649e7a: *stop=0x8c649e7au;r[0]=sx(m,bus,&io_status,0x8c649ee4u,2);if(io_status)return io_status;goto L8c649e7c; /* mov.w 0x8c649ee4,r0 */
L8c649e7c: *stop=0x8c649e7eu;goto L8c649f0a; /* bra 0x8c649f0a */
L8c649e80: *stop=0x8c649e80u;r[8]=0u;goto L8c649e82; /* mov #0x0,r8 */
L8c649e82: *stop=0x8c649e82u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c649e84; /* cmp/pl r12 */
L8c649e84: *stop=0x8c649e84u;r[10]=r[8];goto L8c649e86; /* mov r8,r10 */
L8c649e86: take=!(*sr&1u);*stop=0x8c649e88u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}if(take)goto L8c649e9e;goto L8c649e8a; /* bf/s 0x8c649e9e */
L8c649e8a: *stop=0x8c649e8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649f00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649e8c; /* mov.l 0x8c649f00,r2 */
L8c649e8c: *stop=0x8c649e8cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649e8e; /* mov.l @r13,r5 */
L8c649e8e: target=r[2];*pr=0x8c649e92u;*stop=0x8c649e90u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649e92; /* jsr @r2 */
L8c649e92: *stop=0x8c649e92u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649e94; /* cmp/pz r0 */
L8c649e94: if(!(*sr&1u))goto L8c649eda;goto L8c649e96; /* bf 0x8c649eda */
L8c649e96: *stop=0x8c649e96u;r[10]+=1u;goto L8c649e98; /* add #0x1,r10 */
L8c649e98: *stop=0x8c649e98u;*sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[12]);goto L8c649e9a; /* cmp/ge r12,r10 */
L8c649e9a: take=!(*sr&1u);*stop=0x8c649e9cu;r[13]+=4u;if(take)goto L8c649e8a;goto L8c649e9e; /* bf/s 0x8c649e8a */
L8c649e9e: *stop=0x8c649e9eu;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c649ea0; /* cmp/pl r12 */
L8c649ea0: *stop=0x8c649ea0u;r[10]=r[8];goto L8c649ea2; /* mov r8,r10 */
L8c649ea2: *stop=0x8c649ea2u;r[9]=r[8];goto L8c649ea4; /* mov r8,r9 */
L8c649ea4: take=!(*sr&1u);*stop=0x8c649ea6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}if(take)goto L8c649ebc;goto L8c649ea8; /* bf/s 0x8c649ebc */
L8c649ea8: *stop=0x8c649ea8u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649eaa; /* mov.l @r13,r5 */
L8c649eaa: target=0x8c649c68u;*pr=0x8c649eaeu;*stop=0x8c649eacu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649eae; /* bsr 0x8c649c68 */
L8c649eae: *stop=0x8c649eaeu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649eb0; /* cmp/pz r0 */
L8c649eb0: take=(*sr&1u);*stop=0x8c649eb2u;r[10]+=1u;if(take)goto L8c649eb6;goto L8c649eb4; /* bt/s 0x8c649eb6 */
L8c649eb4: *stop=0x8c649eb4u;r[9]=r[11];goto L8c649eb6; /* mov r11,r9 */
L8c649eb6: *stop=0x8c649eb6u;*sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[12]);goto L8c649eb8; /* cmp/ge r12,r10 */
L8c649eb8: take=!(*sr&1u);*stop=0x8c649ebau;r[13]+=4u;if(take)goto L8c649ea8;goto L8c649ebc; /* bf/s 0x8c649ea8 */
L8c649ebc: target=0x8c64ab36u;*pr=0x8c649ec0u;*stop=0x8c649ebeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649ec0; /* bsr 0x8c64ab36 */
L8c649ec0: *stop=0x8c649ec0u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649ec2; /* cmp/pz r0 */
L8c649ec2: take=!(*sr&1u);*stop=0x8c649ec4u;r[4]=r[0];if(take)goto L8c649ed2;goto L8c649ec6; /* bf/s 0x8c649ed2 */
L8c649ec6: *stop=0x8c649ec6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649f04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649ec8; /* mov.l 0x8c649f04,r2 */
L8c649ec8: target=r[2];*pr=0x8c649eccu;*stop=0x8c649ecau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649ecc; /* jsr @r2 */
L8c649ecc: *stop=0x8c649eccu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649ece; /* cmp/pz r0 */
L8c649ece: take=(*sr&1u);*stop=0x8c649ed0u;r[4]=r[0];if(take)goto L8c649ed6;goto L8c649ed2; /* bt/s 0x8c649ed6 */
L8c649ed2: *stop=0x8c649ed4u;r[0]=r[4];goto L8c649f0a; /* bra 0x8c649f0a */
L8c649ed6: *stop=0x8c649ed6u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c649ed8; /* tst r9,r9 */
L8c649ed8: if((*sr&1u))goto L8c649f08;goto L8c649eda; /* bt 0x8c649f08 */
L8c649eda: *stop=0x8c649edau;r[0]=sx(m,bus,&io_status,0x8c649ee6u,2);if(io_status)return io_status;goto L8c649edc; /* mov.w 0x8c649ee6,r0 */
L8c649edc: *stop=0x8c649edeu;goto L8c649f0a; /* bra 0x8c649f0a */
L8c649f08: *stop=0x8c649f08u;r[0]=0u;goto L8c649f0a; /* mov #0x0,r0 */
L8c649f0a: *stop=0x8c649f0au;r[15]+=8u;goto L8c649f0c; /* add #0x8,r15 */
L8c649f0c: *stop=0x8c649f0cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c649f0e; /* lds.l @r15+,PR */
L8c649f0e: *stop=0x8c649f0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c649f10; /* mov.l @r15+,r8 */
L8c649f10: *stop=0x8c649f10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c649f12; /* mov.l @r15+,r9 */
L8c649f12: *stop=0x8c649f12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c649f14; /* mov.l @r15+,r10 */
L8c649f14: *stop=0x8c649f14u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c649f16; /* mov.l @r15+,r11 */
L8c649f16: *stop=0x8c649f16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c649f18; /* mov.l @r15+,r12 */
L8c649f18: *stop=0x8c649f18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c649f1a; /* mov.l @r15+,r13 */
L8c649f1a: *stop=0x8c649f1au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c649f1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_8c649e00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649e46(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649e46u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649e50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649e50u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649e5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649e5au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649e76(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649e76u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649e92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649e92u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649eae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649eaeu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649ec0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649ec0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command17_resume_8c649ecc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command17_run_8c649e00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649eccu);}
