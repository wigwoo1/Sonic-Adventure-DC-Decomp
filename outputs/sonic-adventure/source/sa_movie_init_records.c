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
SA_STAGE_EXPORT int sa_movie_init_records_8c092fa0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c092fa0;
L8c092fa0: *stop=0x8c092fa0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c092fa2; /* mov.l r14,@-r15 */
L8c092fa2: *stop=0x8c092fa2u;r[14]=0u;goto L8c092fa4; /* mov #0x0,r14 */
L8c092fa4: *stop=0x8c092fa4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c092fa6; /* sts.l PR,@-r15 */
L8c092fa6: *stop=0x8c092fa6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c092fa8; /* mov.l 0x8c0930e0,r3 */
L8c092fa8: *stop=0x8c092fa8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c092faa; /* mov.l 0x8c0930e4,r2 */
L8c092faa: target=r[2];*pr=0x8c092faeu;*stop=0x8c092facu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c092fae; /* jsr @r2 */
L8c092fae: target=0x8c093c74u;*pr=0x8c092fb2u;*stop=0x8c092fb0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c092fb2; /* bsr 0x8c093c74 */
L8c092fb2: target=0x8c092fe8u;*pr=0x8c092fb6u;*stop=0x8c092fb4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c092fb6; /* bsr 0x8c092fe8 */
L8c092fb6: *stop=0x8c092fb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930e8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c092fb8; /* mov.l 0x8c0930e8,r1 */
L8c092fb8: target=r[1];*pr=0x8c092fbcu;*stop=0x8c092fbau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c092fbc; /* jsr @r1 */
L8c092fbc: *stop=0x8c092fbcu;r[5]=8u;goto L8c092fbe; /* mov #0x8,r5 */
L8c092fbe: *stop=0x8c092fbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930f4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c092fc0; /* mov.l 0x8c0930f4,r6 */
L8c092fc0: *stop=0x8c092fc0u;r[5]<<=8;goto L8c092fc2; /* shll8 r5 */
L8c092fc2: *stop=0x8c092fc2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c092fc4; /* mov.l 0x8c0930ec,r2 */
L8c092fc4: *stop=0x8c092fc4u;r[5]+=4294967224u;goto L8c092fc6; /* add #-0x48,r5 */
L8c092fc6: *stop=0x8c092fc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c092fc8; /* mov.l 0x8c0930f0,r1 */
L8c092fc8: *stop=0x8c092fc8u;r[5]+=r[6];goto L8c092fca; /* add r6,r5 */
L8c092fca: *stop=0x8c092fcau;r[4]=r[6];goto L8c092fcc; /* mov r6,r4 */
L8c092fcc: *stop=0x8c092fccu;r[3]=32u;goto L8c092fce; /* mov #0x20,r3 */
L8c092fce: *stop=0x8c092fceu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c092fd0; /* cmp/hs r5,r4 */
L8c092fd0: *stop=0x8c092fd0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c092fd2; /* mov.l r14,@r2 */
L8c092fd2: *stop=0x8c092fd2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c092fd4; /* mov.l r3,@r1 */
L8c092fd4: if((*sr&1u))goto L8c092fe2;goto L8c092fd6; /* bt 0x8c092fe2 */
L8c092fd6: *stop=0x8c092fd6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c092fd8; /* mov.l r14,@(0x4,r4) */
L8c092fd8: *stop=0x8c092fd8u;r[4]+=52u;goto L8c092fda; /* add #0x34,r4 */
L8c092fda: *stop=0x8c092fdau;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c092fdc; /* mov.l r14,@(0x4,r4) */
L8c092fdc: *stop=0x8c092fdcu;r[4]+=52u;goto L8c092fde; /* add #0x34,r4 */
L8c092fde: *stop=0x8c092fdeu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c092fe0; /* cmp/hs r5,r4 */
L8c092fe0: if(!(*sr&1u))goto L8c092fd6;goto L8c092fe2; /* bf 0x8c092fd6 */
L8c092fe2: *stop=0x8c092fe2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c092fe4; /* lds.l @r15+,PR */
L8c092fe4: *stop=0x8c092fe6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,0x8c09306cu,m,f,sr,r,pr); /* bra 0x8c09306c */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c093e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c093e60;
L8c093e60: *stop=0x8c093e60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c093e62; /* mov.l r14,@-r15 */
L8c093e62: *stop=0x8c093e62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c093e64; /* sts.l PR,@-r15 */
L8c093e64: *stop=0x8c093e64u;r[15]+=4294967288u;goto L8c093e66; /* add #-0x8,r15 */
L8c093e66: *stop=0x8c093e66u;r[3]=0u;goto L8c093e68; /* mov #0x0,r3 */
L8c093e68: *stop=0x8c093e68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094040u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c093e6a; /* mov.l 0x8c094040,r4 */
L8c093e6a: *stop=0x8c093e6au;r[5]=r[3];goto L8c093e6c; /* mov r3,r5 */
L8c093e6c: *stop=0x8c093e6cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c093e6e; /* mov.l r3,@r15 */
L8c093e6e: *stop=0x8c093e6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c094044u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093e70; /* mov.l 0x8c094044,r3 */
L8c093e70: target=r[3];*pr=0x8c093e74u;*stop=0x8c093e72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093e74; /* jsr @r3 */
L8c093e74: *stop=0x8c093e74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094048u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c093e76; /* mov.l 0x8c094048,r2 */
L8c093e76: *stop=0x8c093e76u;r[5]=r[15];goto L8c093e78; /* mov r15,r5 */
L8c093e78: *stop=0x8c093e78u;r[14]=r[0];goto L8c093e7a; /* mov r0,r14 */
L8c093e7a: *stop=0x8c093e7au;r[5]+=4u;goto L8c093e7c; /* add #0x4,r5 */
L8c093e7c: target=r[2];*pr=0x8c093e80u;*stop=0x8c093e7eu;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093e80; /* jsr @r2 */
L8c093e80: *stop=0x8c093e80u;r[3]=8u;goto L8c093e82; /* mov #0x8,r3 */
L8c093e82: *stop=0x8c093e82u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c093e84; /* mov.l @(0x4,r15),r5 */
L8c093e84: *stop=0x8c093e84u;r[3]<<=8;goto L8c093e86; /* shll8 r3 */
L8c093e86: *stop=0x8c093e86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09404cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c093e88; /* mov.l 0x8c09404c,r6 */
L8c093e88: *stop=0x8c093e88u;r[3]+=4294967295u;goto L8c093e8a; /* add #-0x1,r3 */
L8c093e8a: *stop=0x8c093e8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c094050u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c093e8c; /* mov.l 0x8c094050,r1 */
L8c093e8c: *stop=0x8c093e8cu;r[5]+=r[3];goto L8c093e8e; /* add r3,r5 */
L8c093e8e: *stop=0x8c093e8eu;r[2]=4294967285u;goto L8c093e90; /* mov #-0xb,r2 */
L8c093e90: *stop=0x8c093e90u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}}goto L8c093e92; /* shad r2,r5 */
L8c093e92: target=r[1];*pr=0x8c093e96u;*stop=0x8c093e94u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093e96; /* jsr @r1 */
L8c093e96: *stop=0x8c093e96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094054u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093e98; /* mov.l 0x8c094054,r3 */
L8c093e98: target=r[3];*pr=0x8c093e9cu;*stop=0x8c093e9au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093e9c; /* jsr @r3 */
L8c093e9c: *stop=0x8c093e9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c094058u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c093e9e; /* mov.l 0x8c094058,r2 */
L8c093e9e: *stop=0x8c093e9eu;r[6]=0u;goto L8c093ea0; /* mov #0x0,r6 */
L8c093ea0: *stop=0x8c093ea0u;r[5]=r[6];goto L8c093ea2; /* mov r6,r5 */
L8c093ea2: target=r[2];*pr=0x8c093ea6u;*stop=0x8c093ea4u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093ea6; /* jsr @r2 */
L8c093ea6: *stop=0x8c093ea6u;r[4]=r[0];goto L8c093ea8; /* mov r0,r4 */
L8c093ea8: *stop=0x8c093ea8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c093eaa; /* tst r4,r4 */
L8c093eaa: if(!(*sr&1u))goto L8c093ee0;goto L8c093eac; /* bf 0x8c093ee0 */
L8c093eac: *stop=0x8c093eacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09405cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093eae; /* mov.l 0x8c09405c,r3 */
L8c093eae: target=r[3];*pr=0x8c093eb2u;*stop=0x8c093eb0u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093eb2; /* jsr @r3 */
L8c093eb2: *stop=0x8c093eb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094060u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c093eb4; /* mov.l 0x8c094060,r2 */
L8c093eb4: target=r[2];*pr=0x8c093eb8u;*stop=0x8c093eb6u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093eb8; /* jsr @r2 */
L8c093eb8: *stop=0x8c093eb8u;r[4]=r[0];goto L8c093eba; /* mov r0,r4 */
L8c093eba: *stop=0x8c093ebau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c093ebc; /* tst r4,r4 */
L8c093ebc: if(!(*sr&1u))goto L8c093ee0;goto L8c093ebe; /* bf 0x8c093ee0 */
L8c093ebe: *stop=0x8c093ebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09404cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c093ec0; /* mov.l 0x8c09404c,r5 */
L8c093ec0: *stop=0x8c093ec0u;r[3]=0u;goto L8c093ec2; /* mov #0x0,r3 */
L8c093ec2: *stop=0x8c093ec2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094064u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c093ec4; /* mov.l 0x8c094064,r2 */
L8c093ec4: *stop=0x8c093ec4u;r[6]=r[3];goto L8c093ec6; /* mov r3,r6 */
L8c093ec6: *stop=0x8c093ec6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c093ec8; /* mov.l r3,@-r15 */
L8c093ec8: *stop=0x8c093ec8u;r[7]=4294967295u;goto L8c093eca; /* mov #-0x1,r7 */
L8c093eca: target=r[2];*pr=0x8c093eceu;*stop=0x8c093eccu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093ece; /* jsr @r2 */
L8c093ece: *stop=0x8c093eceu;r[4]=r[0];goto L8c093ed0; /* mov r0,r4 */
L8c093ed0: *stop=0x8c093ed0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c093ed2; /* tst r4,r4 */
L8c093ed2: take=!(*sr&1u);*stop=0x8c093ed4u;r[15]+=4u;if(take)goto L8c093ee0;goto L8c093ed6; /* bf/s 0x8c093ee0 */
L8c093ed6: *stop=0x8c093ed6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094068u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093ed8; /* mov.l 0x8c094068,r3 */
L8c093ed8: target=r[3];*pr=0x8c093edcu;*stop=0x8c093edau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093edc; /* jsr @r3 */
L8c093edc: *stop=0x8c093edcu;r[4]=r[0];goto L8c093ede; /* mov r0,r4 */
L8c093ede: *stop=0x8c093edeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c093ee0; /* tst r4,r4 */
L8c093ee0: *stop=0x8c093ee0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c093ee2; /* mov.l @r15,r2 */
L8c093ee2: *stop=0x8c093ee2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c093ee4; /* tst r2,r2 */
L8c093ee4: if((*sr&1u))goto L8c093eec;goto L8c093ee6; /* bt 0x8c093eec */
L8c093ee6: *stop=0x8c093ee6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09406cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093ee8; /* mov.l 0x8c09406c,r3 */
L8c093ee8: target=r[3];*pr=0x8c093eecu;*stop=0x8c093eeau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093eec; /* jsr @r3 */
L8c093eec: *stop=0x8c093eecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c094068u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093eee; /* mov.l 0x8c094068,r3 */
L8c093eee: target=r[3];*pr=0x8c093ef2u;*stop=0x8c093ef0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093ef2; /* jsr @r3 */
L8c093ef2: *stop=0x8c093ef2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094074u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c093ef4; /* mov.l 0x8c094074,r2 */
L8c093ef4: *stop=0x8c093ef4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094070u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c093ef6; /* mov.l 0x8c094070,r4 */
L8c093ef6: target=r[2];*pr=0x8c093efau;*stop=0x8c093ef8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093efa; /* jsr @r2 */
L8c093efa: *stop=0x8c093efau;r[15]+=8u;goto L8c093efc; /* add #0x8,r15 */
L8c093efc: *stop=0x8c093efcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c093efe; /* lds.l @r15+,PR */
L8c093efe: *stop=0x8c093efeu;if(*pr!=entry_pr)return -3;*stop=0x8c093f00u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c093c74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c093c74;
L8c093c74: *stop=0x8c093c74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c093c76; /* mov.l r14,@-r15 */
L8c093c76: *stop=0x8c093c76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093cc0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c093c78; /* mov.l 0x8c093cc0,r4 */
L8c093c78: *stop=0x8c093c78u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c093c7a; /* mov.l r13,@-r15 */
L8c093c7a: *stop=0x8c093c7au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c093c7c; /* mov.l r12,@-r15 */
L8c093c7c: *stop=0x8c093c7cu;r[12]=r[4];goto L8c093c7e; /* mov r4,r12 */
L8c093c7e: *stop=0x8c093c7eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c093c80; /* sts.l PR,@-r15 */
L8c093c80: *stop=0x8c093c80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093cd0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c093c82; /* mov.l 0x8c093cd0,r13 */
L8c093c82: *stop=0x8c093c82u;r[12]+=32u;goto L8c093c84; /* add #0x20,r12 */
L8c093c84: *stop=0x8c093c84u;r[14]=r[4];goto L8c093c86; /* mov r4,r14 */
L8c093c86: *stop=0x8c093c86u;*sr=(*sr&~1u)|(r[14]>=r[12]);goto L8c093c88; /* cmp/hs r12,r14 */
L8c093c88: if((*sr&1u))goto L8c093c9a;goto L8c093c8a; /* bt 0x8c093c9a */
L8c093c8a: target=r[13];*pr=0x8c093c8eu;*stop=0x8c093c8cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093c8e; /* jsr @r13 */
L8c093c8e: *stop=0x8c093c8eu;r[14]+=4u;goto L8c093c90; /* add #0x4,r14 */
L8c093c90: target=r[13];*pr=0x8c093c94u;*stop=0x8c093c92u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093c94; /* jsr @r13 */
L8c093c94: *stop=0x8c093c94u;r[14]+=4u;goto L8c093c96; /* add #0x4,r14 */
L8c093c96: *stop=0x8c093c96u;*sr=(*sr&~1u)|(r[14]>=r[12]);goto L8c093c98; /* cmp/hs r12,r14 */
L8c093c98: if(!(*sr&1u))goto L8c093c8a;goto L8c093c9a; /* bf 0x8c093c8a */
L8c093c9a: *stop=0x8c093c9au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c093c9c; /* lds.l @r15+,PR */
L8c093c9c: *stop=0x8c093c9cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c093c9e; /* mov.l @r15+,r12 */
L8c093c9e: *stop=0x8c093c9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c093ca0; /* mov.l @r15+,r13 */
L8c093ca0: *stop=0x8c093ca0u;if(*pr!=entry_pr)return -3;*stop=0x8c093ca2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c092fe8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c092fe8;
L8c092fe8: *stop=0x8c092fe8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c092fea; /* mov.l r14,@-r15 */
L8c092fea: *stop=0x8c092feau;r[0]=4294967232u;goto L8c092fec; /* mov #-0x40,r0 */
L8c092fec: *stop=0x8c092fecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c092fee; /* mov.l r13,@-r15 */
L8c092fee: *stop=0x8c092feeu;r[0]<<=8;goto L8c092ff0; /* shll8 r0 */
L8c092ff0: *stop=0x8c092ff0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c092ff2; /* mov.l r12,@-r15 */
L8c092ff2: *stop=0x8c092ff2u;r[0]+=4294967292u;goto L8c092ff4; /* add #-0x4,r0 */
L8c092ff4: *stop=0x8c092ff4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c092ff6; /* mov.l r11,@-r15 */
L8c092ff6: *stop=0x8c092ff6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c092ff8; /* mov.l r10,@-r15 */
L8c092ff8: *stop=0x8c092ff8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c092ffa; /* sts.l PR,@-r15 */
L8c092ffa: *stop=0x8c092ffau;r[15]+=r[0];goto L8c092ffc; /* add r0,r15 */
L8c092ffc: *stop=0x8c092ffcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930f8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c092ffe; /* mov.l 0x8c0930f8,r12 */
L8c092ffe: *stop=0x8c092ffeu;r[11]=r[15];goto L8c093000; /* mov r15,r11 */
L8c093000: *stop=0x8c093000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0930fcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c093002; /* mov.l 0x8c0930fc,r14 */
L8c093002: *stop=0x8c093002u;r[13]=0u;goto L8c093004; /* mov #0x0,r13 */
L8c093004: *stop=0x8c093004u;r[10]=r[13];goto L8c093006; /* mov r13,r10 */
L8c093006: *stop=0x8c093006u;r[11]+=4u;goto L8c093008; /* add #0x4,r11 */
L8c093008: target=r[12];*pr=0x8c09300cu;*stop=0x8c09300au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09300c; /* jsr @r12 */
L8c09300c: *stop=0x8c09300cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09300e; /* tst r0,r0 */
L8c09300e: if((*sr&1u))goto L8c09301e;goto L8c093010; /* bt 0x8c09301e */
L8c093010: *stop=0x8c093010u;*sr=(*sr&~1u)|((int32_t)r[10]>(int32_t)r[14]);goto L8c093012; /* cmp/gt r14,r10 */
L8c093012: take=(*sr&1u);*stop=0x8c093014u;r[10]+=1u;if(take)goto L8c09301e;goto L8c093016; /* bt/s 0x8c09301e */
L8c093016: target=r[12];*pr=0x8c09301au;*stop=0x8c093018u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09301a; /* jsr @r12 */
L8c09301a: *stop=0x8c09301au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09301c; /* tst r0,r0 */
L8c09301c: if(!(*sr&1u))goto L8c093010;goto L8c09301e; /* bf 0x8c093010 */
L8c09301e: *stop=0x8c09301eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c093100u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c093020; /* mov.l 0x8c093100,r12 */
L8c093020: *stop=0x8c093020u;r[11]=r[13];goto L8c093022; /* mov r13,r11 */
L8c093022: target=r[12];*pr=0x8c093026u;*stop=0x8c093024u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093026; /* jsr @r12 */
L8c093026: *stop=0x8c093026u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c093028; /* tst r0,r0 */
L8c093028: if((*sr&1u))goto L8c09303c;goto L8c09302a; /* bt 0x8c09303c */
L8c09302a: *stop=0x8c09302au;*sr=(*sr&~1u)|((int32_t)r[11]>(int32_t)r[14]);goto L8c09302c; /* cmp/gt r14,r11 */
L8c09302c: take=!(*sr&1u);*stop=0x8c09302eu;r[11]+=1u;if(take)goto L8c093034;goto L8c093030; /* bf/s 0x8c093034 */
L8c093030: *stop=0x8c093032u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c09303c; /* bra 0x8c09303c */
L8c093034: target=r[12];*pr=0x8c093038u;*stop=0x8c093036u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093038; /* jsr @r12 */
L8c093038: *stop=0x8c093038u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09303a; /* tst r0,r0 */
L8c09303a: if(!(*sr&1u))goto L8c09302a;goto L8c09303c; /* bf 0x8c09302a */
L8c09303c: *stop=0x8c09303cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c09303e; /* mov.l @r15,r3 */
L8c09303e: *stop=0x8c09303eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c093040; /* tst r3,r3 */
L8c093040: if(!(*sr&1u))goto L8c093048;goto L8c093042; /* bf 0x8c093048 */
L8c093042: *stop=0x8c093042u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093104u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c093044; /* mov.l 0x8c093104,r4 */
L8c093044: *stop=0x8c093046u;goto L8c09304a; /* bra 0x8c09304a */
L8c093048: *stop=0x8c093048u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093108u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c09304a; /* mov.l 0x8c093108,r4 */
L8c09304a: *stop=0x8c09304au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09310cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09304c; /* mov.l 0x8c09310c,r3 */
L8c09304c: target=r[3];*pr=0x8c093050u;*stop=0x8c09304eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093050; /* jsr @r3 */
L8c093050: *stop=0x8c093050u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093110u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c093052; /* mov.l 0x8c093110,r2 */
L8c093052: target=r[2];*pr=0x8c093056u;*stop=0x8c093054u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093056; /* jsr @r2 */
L8c093056: *stop=0x8c093056u;r[1]=64u;goto L8c093058; /* mov #0x40,r1 */
L8c093058: *stop=0x8c093058u;r[1]<<=8;goto L8c09305a; /* shll8 r1 */
L8c09305a: *stop=0x8c09305au;r[1]+=4u;goto L8c09305c; /* add #0x4,r1 */
L8c09305c: *stop=0x8c09305cu;r[15]+=r[1];goto L8c09305e; /* add r1,r15 */
L8c09305e: *stop=0x8c09305eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c093060; /* lds.l @r15+,PR */
L8c093060: *stop=0x8c093060u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c093062; /* mov.l @r15+,r10 */
L8c093062: *stop=0x8c093062u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c093064; /* mov.l @r15+,r11 */
L8c093064: *stop=0x8c093064u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c093066; /* mov.l @r15+,r12 */
L8c093066: *stop=0x8c093066u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c093068; /* mov.l @r15+,r13 */
L8c093068: *stop=0x8c093068u;if(*pr!=entry_pr)return -3;*stop=0x8c09306au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c094e20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c094e20;
L8c094e20: *stop=0x8c094e20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c094e22; /* sts.l PR,@-r15 */
L8c094e22: *stop=0x8c094e22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c094e24; /* mov.l 0x8c094e64,r3 */
L8c094e24: *stop=0x8c094e24u;r[5]=0u;goto L8c094e26; /* mov #0x0,r5 */
L8c094e26: target=r[3];*pr=0x8c094e2au;*stop=0x8c094e28u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c094e2a; /* jsr @r3 */
L8c094e2a: target=0x8c094dd0u;*pr=0x8c094e2eu;*stop=0x8c094e2cu;r[4]=4294967290u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c094e2e; /* bsr 0x8c094dd0 */
L8c094e2e: *stop=0x8c094e2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e6cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c094e30; /* mov.l 0x8c094e6c,r1 */
L8c094e30: *stop=0x8c094e30u;r[3]=4294967290u;goto L8c094e32; /* mov #-0x6,r3 */
L8c094e32: *stop=0x8c094e32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c094e34; /* mov.l 0x8c094e68,r2 */
L8c094e34: *stop=0x8c094e34u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c094e36; /* mov.l r3,@r2 */
L8c094e36: *stop=0x8c094e36u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c094e38; /* mov.l r3,@r1 */
L8c094e38: *stop=0x8c094e3au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,0x8c09536au,m,f,sr,r,pr); /* bra 0x8c09536a */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c09306c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c09306c;
L8c09306c: *stop=0x8c09306cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c09306e; /* mov.l r14,@-r15 */
L8c09306e: *stop=0x8c09306eu;r[4]=0u;goto L8c093070; /* mov #0x0,r4 */
L8c093070: *stop=0x8c093070u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c093072; /* mov.l r13,@-r15 */
L8c093072: *stop=0x8c093072u;r[13]=32u;goto L8c093074; /* mov #0x20,r13 */
L8c093074: *stop=0x8c093074u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093114u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c093076; /* mov.l 0x8c093114,r3 */
L8c093076: *stop=0x8c093076u;r[14]=r[4];goto L8c093078; /* mov r4,r14 */
L8c093078: *stop=0x8c093078u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09307a; /* sts.l PR,@-r15 */
L8c09307a: *stop=0x8c09307au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c09307c; /* mov.l r4,@r3 */
L8c09307c: target=0x8c093094u;*pr=0x8c093080u;*stop=0x8c09307eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093080; /* bsr 0x8c093094 */
L8c093080: *stop=0x8c093080u;r[14]+=1u;goto L8c093082; /* add #0x1,r14 */
L8c093082: target=0x8c093094u;*pr=0x8c093086u;*stop=0x8c093084u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c093086; /* bsr 0x8c093094 */
L8c093086: *stop=0x8c093086u;r[14]+=1u;goto L8c093088; /* add #0x1,r14 */
L8c093088: *stop=0x8c093088u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[13]);goto L8c09308a; /* cmp/ge r13,r14 */
L8c09308a: if(!(*sr&1u))goto L8c09307c;goto L8c09308c; /* bf 0x8c09307c */
L8c09308c: *stop=0x8c09308cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c09308e; /* lds.l @r15+,PR */
L8c09308e: *stop=0x8c09308eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c093090; /* mov.l @r15+,r13 */
L8c093090: *stop=0x8c093090u;if(*pr!=entry_pr)return -3;*stop=0x8c093092u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c093094(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c093094;
L8c093094: *stop=0x8c093094u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c093096; /* mov.l r14,@-r15 */
L8c093096: *stop=0x8c093096u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c093098; /* mov.l r13,@-r15 */
L8c093098: *stop=0x8c093098u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09309a; /* sts.l PR,@-r15 */
L8c09309a: *stop=0x8c09309au;r[15]+=4294967292u;goto L8c09309c; /* add #-0x4,r15 */
L8c09309c: *stop=0x8c09309cu;r[14]=r[4];goto L8c09309e; /* mov r4,r14 */
L8c09309e: *stop=0x8c09309eu;r[4]=15u;goto L8c0930a0; /* mov #0xf,r4 */
L8c0930a0: *stop=0x8c0930a0u;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]);goto L8c0930a2; /* cmp/gt r4,r14 */
L8c0930a2: take=!(*sr&1u);*stop=0x8c0930a4u;r[13]=0u;if(take)goto L8c0930a8;goto L8c0930a6; /* bf/s 0x8c0930a8 */
L8c0930a6: *stop=0x8c0930a6u;r[13]=1u;goto L8c0930a8; /* mov #0x1,r13 */
L8c0930a8: *stop=0x8c0930a8u;r[2]=0u;goto L8c0930aa; /* mov #0x0,r2 */
L8c0930aa: *stop=0x8c0930aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c093118u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0930ac; /* mov.l 0x8c093118,r5 */
L8c0930ac: *stop=0x8c0930acu;r[3]=121u;goto L8c0930ae; /* mov #0x79,r3 */
L8c0930ae: *stop=0x8c0930aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0930b0; /* mov.l r2,@-r15 */
L8c0930b0: *stop=0x8c0930b0u;r[1]=4294967216u;goto L8c0930b2; /* mov #-0x50,r1 */
L8c0930b2: *stop=0x8c0930b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0930b4; /* mov.l r3,@-r15 */
L8c0930b4: *stop=0x8c0930b4u;r[14]&=r[4];goto L8c0930b6; /* and r4,r14 */
L8c0930b6: *stop=0x8c0930b6u;r[4]=r[13];goto L8c0930b8; /* mov r13,r4 */
L8c0930b8: *stop=0x8c0930b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09311cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0930ba; /* mov.l 0x8c09311c,r3 */
L8c0930ba: *stop=0x8c0930bau;r[1]=r[1]&255u;goto L8c0930bc; /* extu.b r1,r1 */
L8c0930bc: *stop=0x8c0930bcu;r[4]<<=2;goto L8c0930be; /* shll2 r4 */
L8c0930be: *stop=0x8c0930beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c093120u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0930c0; /* mov.l 0x8c093120,r2 */
L8c0930c0: *stop=0x8c0930c0u;r[14]|=r[1];goto L8c0930c2; /* or r1,r14 */
L8c0930c2: *stop=0x8c0930c2u;r[4]+=r[3];goto L8c0930c4; /* add r3,r4 */
L8c0930c4: *stop=0x8c0930c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0930c6; /* mov.l r14,@-r15 */
L8c0930c6: *stop=0x8c0930c6u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;goto L8c0930c8; /* mov.l r4,@(0xc,r15) */
L8c0930c8: target=r[2];*pr=0x8c0930ccu;*stop=0x8c0930cau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0930cc; /* jsr @r2 */
L8c0930cc: *stop=0x8c0930ccu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0930ce; /* mov.l @(0xc,r15),r4 */
L8c0930ce: *stop=0x8c0930ceu;r[6]=0u;goto L8c0930d0; /* mov #0x0,r6 */
L8c0930d0: *stop=0x8c0930d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093118u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0930d2; /* mov.l 0x8c093118,r5 */
L8c0930d2: *stop=0x8c0930d2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c0930d4; /* mov.l @r4,r4 */
L8c0930d4: *stop=0x8c0930d4u;r[15]+=16u;goto L8c0930d6; /* add #0x10,r15 */
L8c0930d6: *stop=0x8c0930d6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0930d8; /* lds.l @r15+,PR */
L8c0930d8: *stop=0x8c0930d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c093124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0930da; /* mov.l 0x8c093124,r3 */
L8c0930da: *stop=0x8c0930dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0930dc; /* mov.l @r15+,r13 */
L8c0930dc: target=r[3];*stop=0x8c0930deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c643ac0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c643ac0;
L8c643ac0: *stop=0x8c643ac0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643af4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643ac2; /* mov.l 0x8c643af4,r2 */
L8c643ac2: *stop=0x8c643ac2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643ac4; /* mov.l r14,@-r15 */
L8c643ac4: *stop=0x8c643ac4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643ac6; /* sts.l PR,@-r15 */
L8c643ac6: *stop=0x8c643ac6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643ac8; /* mov.l @r2,r3 */
L8c643ac8: *stop=0x8c643ac8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c643aca; /* tst r3,r3 */
L8c643aca: take=!(*sr&1u);*stop=0x8c643accu;r[14]=0u;if(take)goto L8c643ad4;goto L8c643ace; /* bf/s 0x8c643ad4 */
L8c643ace: *stop=0x8c643aceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643af8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643ad0; /* mov.l 0x8c643af8,r14 */
L8c643ad0: *stop=0x8c643ad2u;goto L8c643ae0; /* bra 0x8c643ae0 */
L8c643ad4: *stop=0x8c643ad4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c643ad6; /* tst r4,r4 */
L8c643ad6: if((*sr&1u))goto L8c643ae0;goto L8c643ad8; /* bt 0x8c643ae0 */
L8c643ad8: *stop=0x8c643ad8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643afcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643ada; /* mov.l 0x8c643afc,r3 */
L8c643ada: target=r[3];*pr=0x8c643adeu;*stop=0x8c643adcu;r[5]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643ade; /* jsr @r3 */
L8c643ade: *stop=0x8c643adeu;r[14]=r[0];goto L8c643ae0; /* mov r0,r14 */
L8c643ae0: *stop=0x8c643ae0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643ae2; /* tst r14,r14 */
L8c643ae2: if((*sr&1u))goto L8c643aec;goto L8c643ae4; /* bt 0x8c643aec */
L8c643ae4: *stop=0x8c643ae4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643b04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643ae6; /* mov.l 0x8c643b04,r3 */
L8c643ae6: *stop=0x8c643ae6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643b00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643ae8; /* mov.l 0x8c643b00,r4 */
L8c643ae8: target=r[3];*pr=0x8c643aecu;*stop=0x8c643aeau;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643aec; /* jsr @r3 */
L8c643aec: *stop=0x8c643aecu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643aee; /* lds.l @r15+,PR */
L8c643aee: *stop=0x8c643aeeu;r[0]=r[14];goto L8c643af0; /* mov r14,r0 */
L8c643af0: *stop=0x8c643af0u;if(*pr!=entry_pr)return -3;*stop=0x8c643af2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c094dd0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c094dd0;
L8c094dd0: *stop=0x8c094dd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c094dd2; /* sts.l PR,@-r15 */
L8c094dd2: *stop=0x8c094dd2u;r[15]+=4294967288u;goto L8c094dd4; /* add #-0x8,r15 */
L8c094dd4: *stop=0x8c094dd4u;r[0]=r[4];goto L8c094dd6; /* mov r4,r0 */
L8c094dd6: *stop=0x8c094dd6u;r[0]<<=2;goto L8c094dd8; /* shll2 r0 */
L8c094dd8: *stop=0x8c094dd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c094dda; /* mov.l 0x8c094e60,r3 */
L8c094dda: *stop=0x8c094ddau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c094ddc; /* shll r0 */
L8c094ddc: *stop=0x8c094ddcu;r[5]=0u;goto L8c094dde; /* mov #0x0,r5 */
L8c094dde: *stop=0x8c094ddeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c094de0; /* mov.l r4,@r15 */
L8c094de0: *stop=0x8c094de0u;if(!valid_access(r[15]+4u,1u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],1);if(io_status)return io_status;goto L8c094de2; /* mov.b r0,@(0x4,r15) */
L8c094de2: *stop=0x8c094de2u;r[6]=r[0];goto L8c094de4; /* mov r0,r6 */
L8c094de4: target=r[3];*pr=0x8c094de8u;*stop=0x8c094de6u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c094de8; /* jsr @r3 */
L8c094de8: *stop=0x8c094de8u;if(!valid_access(r[15]+4u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c094dea; /* mov.b @(0x4,r15),r0 */
L8c094dea: *stop=0x8c094deau;r[4]=1u;goto L8c094dec; /* mov #0x1,r4 */
L8c094dec: *stop=0x8c094decu;r[5]=0u;goto L8c094dee; /* mov #0x0,r5 */
L8c094dee: *stop=0x8c094deeu;r[6]=r[0];goto L8c094df0; /* mov r0,r6 */
L8c094df0: *stop=0x8c094df0u;r[15]+=8u;goto L8c094df2; /* add #0x8,r15 */
L8c094df2: *stop=0x8c094df2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c094df4; /* mov.l 0x8c094e60,r3 */
L8c094df4: target=r[3];*stop=0x8c094df6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c09536a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09536a;
L8c09536a: *stop=0x8c09536au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09536c; /* sts.l PR,@-r15 */
L8c09536c: target=0x8c094df8u;*pr=0x8c095370u;*stop=0x8c09536eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c095370; /* bsr 0x8c094df8 */
L8c095370: *stop=0x8c095370u;r[1]=2u;goto L8c095372; /* mov #0x2,r1 */
L8c095372: *stop=0x8c095372u;{uint32_t loaded=rd(m,bus,&io_status,0x8c095464u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c095374; /* mov.l 0x8c095464,r4 */
L8c095374: *stop=0x8c095374u;r[1]<<=8;goto L8c095376; /* shll8 r1 */
L8c095376: *stop=0x8c095376u;r[1]+=48u;goto L8c095378; /* add #0x30,r1 */
L8c095378: *stop=0x8c095378u;r[6]=r[4];goto L8c09537a; /* mov r4,r6 */
L8c09537a: *stop=0x8c09537au;r[1]+=r[4];goto L8c09537c; /* add r4,r1 */
L8c09537c: *stop=0x8c09537cu;*sr=(*sr&~1u)|(r[6]>=r[1]);goto L8c09537e; /* cmp/hs r1,r6 */
L8c09537e: *stop=0x8c09537eu;r[0]=4u;goto L8c095380; /* mov #0x4,r0 */
L8c095380: take=(*sr&1u);*stop=0x8c095382u;r[4]=0u;if(take)goto L8c09539c;goto L8c095384; /* bt/s 0x8c09539c */
L8c095384: *stop=0x8c095384u;r[5]=r[6];goto L8c095386; /* mov r6,r5 */
L8c095386: *stop=0x8c095386u;r[7]=r[4];goto L8c095388; /* mov r4,r7 */
L8c095388: *stop=0x8c095388u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c09538a; /* mov.l r4,@r5 */
L8c09538a: *stop=0x8c09538au;r[7]+=2u;goto L8c09538c; /* add #0x2,r7 */
L8c09538c: *stop=0x8c09538cu;r[5]+=4u;goto L8c09538e; /* add #0x4,r5 */
L8c09538e: *stop=0x8c09538eu;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[0]);goto L8c095390; /* cmp/ge r0,r7 */
L8c095390: *stop=0x8c095390u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c095392; /* mov.l r4,@r5 */
L8c095392: take=!(*sr&1u);*stop=0x8c095394u;r[5]+=4u;if(take)goto L8c095388;goto L8c095396; /* bf/s 0x8c095388 */
L8c095396: *stop=0x8c095396u;r[6]+=16u;goto L8c095398; /* add #0x10,r6 */
L8c095398: *stop=0x8c095398u;*sr=(*sr&~1u)|(r[6]>=r[1]);goto L8c09539a; /* cmp/hs r1,r6 */
L8c09539a: if(!(*sr&1u))goto L8c095384;goto L8c09539c; /* bf 0x8c095384 */
L8c09539c: *stop=0x8c09539cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09546cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c09539e; /* mov.l 0x8c09546c,r7 */
L8c09539e: *stop=0x8c09539eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c095468u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0953a0; /* mov.l 0x8c095468,r6 */
L8c0953a0: *stop=0x8c0953a0u;r[1]=r[7];goto L8c0953a2; /* mov r7,r1 */
L8c0953a2: *stop=0x8c0953a2u;r[1]+=8u;goto L8c0953a4; /* add #0x8,r1 */
L8c0953a4: *stop=0x8c0953a4u;r[5]=r[7];goto L8c0953a6; /* mov r7,r5 */
L8c0953a6: *stop=0x8c0953a6u;*sr=(*sr&~1u)|(r[5]>=r[1]);goto L8c0953a8; /* cmp/hs r1,r5 */
L8c0953a8: take=(*sr&1u);*stop=0x8c0953aau;r[7]=15u;if(take)goto L8c0953c0;goto L8c0953ac; /* bt/s 0x8c0953c0 */
L8c0953ac: *stop=0x8c0953acu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[7],1);if(io_status)return io_status;goto L8c0953ae; /* mov.b r7,@r5 */
L8c0953ae: *stop=0x8c0953aeu;r[5]+=1u;goto L8c0953b0; /* add #0x1,r5 */
L8c0953b0: *stop=0x8c0953b0u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[4],1);if(io_status)return io_status;goto L8c0953b2; /* mov.b r4,@r6 */
L8c0953b2: *stop=0x8c0953b2u;r[6]+=1u;goto L8c0953b4; /* add #0x1,r6 */
L8c0953b4: *stop=0x8c0953b4u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[7],1);if(io_status)return io_status;goto L8c0953b6; /* mov.b r7,@r5 */
L8c0953b6: *stop=0x8c0953b6u;r[5]+=1u;goto L8c0953b8; /* add #0x1,r5 */
L8c0953b8: *stop=0x8c0953b8u;*sr=(*sr&~1u)|(r[5]>=r[1]);goto L8c0953ba; /* cmp/hs r1,r5 */
L8c0953ba: *stop=0x8c0953bau;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[4],1);if(io_status)return io_status;goto L8c0953bc; /* mov.b r4,@r6 */
L8c0953bc: take=!(*sr&1u);*stop=0x8c0953beu;r[6]+=1u;if(take)goto L8c0953ac;goto L8c0953c0; /* bf/s 0x8c0953ac */
L8c0953c0: *stop=0x8c0953c0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0953c2; /* lds.l @r15+,PR */
L8c0953c2: *stop=0x8c0953c2u;if(*pr!=entry_pr)return -3;*stop=0x8c0953c4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c643f6c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c643f6c;
L8c643f6c: *stop=0x8c643f6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643f6e; /* mov.l r14,@-r15 */
L8c643f6e: *stop=0x8c643f6eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643f70; /* sts.l PR,@-r15 */
L8c643f70: *stop=0x8c643f70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643f72; /* mov.l 0x8c643f8c,r3 */
L8c643f72: target=r[3];*pr=0x8c643f76u;*stop=0x8c643f74u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643f76; /* jsr @r3 */
L8c643f76: *stop=0x8c643f76u;r[14]=r[0];goto L8c643f78; /* mov r0,r14 */
L8c643f78: *stop=0x8c643f78u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643f7a; /* tst r14,r14 */
L8c643f7a: if((*sr&1u))goto L8c643f84;goto L8c643f7c; /* bt 0x8c643f84 */
L8c643f7c: *stop=0x8c643f7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643f7e; /* mov.l 0x8c643f94,r3 */
L8c643f7e: *stop=0x8c643f7eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f90u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643f80; /* mov.l 0x8c643f90,r4 */
L8c643f80: target=r[3];*pr=0x8c643f84u;*stop=0x8c643f82u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643f84; /* jsr @r3 */
L8c643f84: *stop=0x8c643f84u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643f86; /* lds.l @r15+,PR */
L8c643f86: *stop=0x8c643f86u;r[0]=r[14];goto L8c643f88; /* mov r14,r0 */
L8c643f88: *stop=0x8c643f88u;if(*pr!=entry_pr)return -3;*stop=0x8c643f8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c5ff2cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff2cc;
L8c5ff2cc: *stop=0x8c5ff2ccu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ff2ce; /* tst r4,r4 */
L8c5ff2ce: if(!(*sr&1u))goto L8c5ff2d4;goto L8c5ff2d0; /* bf 0x8c5ff2d4 */
L8c5ff2d0: *stop=0x8c5ff2d0u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff2d2u;r[0]=4294967295u;return 0; /* rts */
L8c5ff2d4: *stop=0x8c5ff2d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ff2d6; /* mov.l 0x8c5ff4b4,r2 */
L8c5ff2d6: *stop=0x8c5ff2d6u;r[0]=0u;goto L8c5ff2d8; /* mov #0x0,r0 */
L8c5ff2d8: *stop=0x8c5ff2d8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c5ff2da; /* mov.l r4,@r2 */
L8c5ff2da: *stop=0x8c5ff2dau;if(*pr!=entry_pr)return -3;*stop=0x8c5ff2dcu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c5ff2de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff2de;
L8c5ff2de: *stop=0x8c5ff2deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ff2e0; /* mov.l 0x8c5ff4b4,r2 */
L8c5ff2e0: *stop=0x8c5ff2e0u;r[3]=0u;goto L8c5ff2e2; /* mov #0x0,r3 */
L8c5ff2e2: *stop=0x8c5ff2e2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c5ff2e4; /* mov.l r3,@r2 */
L8c5ff2e4: *stop=0x8c5ff2e4u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff2e6u;r[0]=r[3];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c5ff31e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5ff31e;
L8c5ff31e: *stop=0x8c5ff31eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5ff320; /* mov.l r14,@-r15 */
L8c5ff320: *stop=0x8c5ff320u;r[14]=r[4];goto L8c5ff322; /* mov r4,r14 */
L8c5ff322: *stop=0x8c5ff322u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ff324; /* sts.l PR,@-r15 */
L8c5ff324: *stop=0x8c5ff324u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c5ff326; /* tst r14,r14 */
L8c5ff326: if((*sr&1u))goto L8c5ff35c;goto L8c5ff328; /* bt 0x8c5ff35c */
L8c5ff328: *stop=0x8c5ff328u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ff32a; /* mov.l 0x8c5ff4b4,r2 */
L8c5ff32a: *stop=0x8c5ff32au;r[3]=0u;goto L8c5ff32c; /* mov #0x0,r3 */
L8c5ff32c: *stop=0x8c5ff32cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c5ff32e; /* mov.l r3,@r14 */
L8c5ff32e: *stop=0x8c5ff32eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c5ff330; /* mov.l @r2,r5 */
L8c5ff330: *stop=0x8c5ff330u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c5ff332; /* tst r5,r5 */
L8c5ff332: if((*sr&1u))goto L8c5ff35c;goto L8c5ff334; /* bt 0x8c5ff35c */
L8c5ff334: *stop=0x8c5ff334u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ff336; /* mov.l 0x8c5ff4bc,r2 */
L8c5ff336: target=r[2];*pr=0x8c5ff33au;*stop=0x8c5ff338u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ff33a; /* jsr @r2 */
L8c5ff33a: *stop=0x8c5ff33au;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c5ff33c; /* cmp/eq #-0x1,r0 */
L8c5ff33c: take=(*sr&1u);*stop=0x8c5ff33eu;r[4]=r[0];if(take)goto L8c5ff35c;goto L8c5ff340; /* bt/s 0x8c5ff35c */
L8c5ff340: *stop=0x8c5ff340u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ff342; /* mov.l 0x8c5ff4b8,r3 */
L8c5ff342: *stop=0x8c5ff342u;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[5]=loaded;}goto L8c5ff344; /* mov.b @r3,r5 */
L8c5ff344: *stop=0x8c5ff344u;r[4]=(uint32_t)(int32_t)(int8_t)r[5];goto L8c5ff346; /* exts.b r5,r4 */
L8c5ff346: *stop=0x8c5ff346u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ff348; /* tst r4,r4 */
L8c5ff348: if(!(*sr&1u))goto L8c5ff350;goto L8c5ff34a; /* bf 0x8c5ff350 */
L8c5ff34a: *stop=0x8c5ff34au;r[1]=0u;goto L8c5ff34c; /* mov #0x0,r1 */
L8c5ff34c: *stop=0x8c5ff34eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c5ff364; /* bra 0x8c5ff364 */
L8c5ff350: *stop=0x8c5ff350u;r[0]=r[4];goto L8c5ff352; /* mov r4,r0 */
L8c5ff352: *stop=0x8c5ff352u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5ff354; /* cmp/eq #0x1,r0 */
L8c5ff354: if(!(*sr&1u))goto L8c5ff35c;goto L8c5ff356; /* bf 0x8c5ff35c */
L8c5ff356: *stop=0x8c5ff356u;r[2]=1u;goto L8c5ff358; /* mov #0x1,r2 */
L8c5ff358: *stop=0x8c5ff35au;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c5ff364; /* bra 0x8c5ff364 */
L8c5ff35c: *stop=0x8c5ff35cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff35e; /* lds.l @r15+,PR */
L8c5ff35e: *stop=0x8c5ff35eu;r[0]=4294967295u;goto L8c5ff360; /* mov #-0x1,r0 */
L8c5ff360: *stop=0x8c5ff360u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff362u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c5ff364: *stop=0x8c5ff364u;r[0]=0u;goto L8c5ff366; /* mov #0x0,r0 */
L8c5ff366: *stop=0x8c5ff366u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff368; /* lds.l @r15+,PR */
L8c5ff368: *stop=0x8c5ff368u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff36au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c094df8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c094df8;
L8c094df8: *stop=0x8c094df8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c094dfa; /* sts.l PR,@-r15 */
L8c094dfa: *stop=0x8c094dfau;r[15]+=4294967288u;goto L8c094dfc; /* add #-0x8,r15 */
L8c094dfc: *stop=0x8c094dfcu;r[0]=r[4];goto L8c094dfe; /* mov r4,r0 */
L8c094dfe: *stop=0x8c094dfeu;r[0]<<=2;goto L8c094e00; /* shll2 r0 */
L8c094e00: *stop=0x8c094e00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c094e02; /* mov.l 0x8c094e60,r3 */
L8c094e02: *stop=0x8c094e02u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c094e04; /* shll r0 */
L8c094e04: *stop=0x8c094e04u;r[5]=0u;goto L8c094e06; /* mov #0x0,r5 */
L8c094e06: *stop=0x8c094e06u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c094e08; /* mov.l r4,@r15 */
L8c094e08: *stop=0x8c094e08u;if(!valid_access(r[15]+4u,1u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],1);if(io_status)return io_status;goto L8c094e0a; /* mov.b r0,@(0x4,r15) */
L8c094e0a: *stop=0x8c094e0au;r[6]=r[0];goto L8c094e0c; /* mov r0,r6 */
L8c094e0c: target=r[3];*pr=0x8c094e10u;*stop=0x8c094e0eu;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c094e10; /* jsr @r3 */
L8c094e10: *stop=0x8c094e10u;if(!valid_access(r[15]+4u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c094e12; /* mov.b @(0x4,r15),r0 */
L8c094e12: *stop=0x8c094e12u;r[4]=3u;goto L8c094e14; /* mov #0x3,r4 */
L8c094e14: *stop=0x8c094e14u;r[5]=0u;goto L8c094e16; /* mov #0x0,r5 */
L8c094e16: *stop=0x8c094e16u;r[6]=r[0];goto L8c094e18; /* mov r0,r6 */
L8c094e18: *stop=0x8c094e18u;r[15]+=8u;goto L8c094e1a; /* add #0x8,r15 */
L8c094e1a: *stop=0x8c094e1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c094e60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c094e1c; /* mov.l 0x8c094e60,r3 */
L8c094e1c: target=r[3];*stop=0x8c094e1eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c65aa30(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65aa30;
L8c65aa30: *stop=0x8c65aa30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65aa32; /* mov.l r14,@-r15 */
L8c65aa32: *stop=0x8c65aa32u;r[0]=r[5];goto L8c65aa34; /* mov r5,r0 */
L8c65aa34: *stop=0x8c65aa34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65aa36; /* mov.l r13,@-r15 */
L8c65aa36: *stop=0x8c65aa36u;r[13]=0u;goto L8c65aa38; /* mov #0x0,r13 */
L8c65aa38: *stop=0x8c65aa38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65aa3a; /* sts.l PR,@-r15 */
L8c65aa3a: *stop=0x8c65aa3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aae0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65aa3c; /* mov.l 0x8c65aae0,r3 */
L8c65aa3c: *stop=0x8c65aa3cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65aa3e; /* cmp/eq #0x1,r0 */
L8c65aa3e: *stop=0x8c65aa3eu;r[14]=r[13];goto L8c65aa40; /* mov r13,r14 */
L8c65aa40: *stop=0x8c65aa40u;r[6]=8u;goto L8c65aa42; /* mov #0x8,r6 */
L8c65aa42: take=(*sr&1u);*stop=0x8c65aa44u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[7]=loaded;}if(take)goto L8c65aa52;goto L8c65aa46; /* bt/s 0x8c65aa52 */
L8c65aa46: *stop=0x8c65aa46u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c65aa48; /* cmp/eq #0x2,r0 */
L8c65aa48: if((*sr&1u))goto L8c65aa70;goto L8c65aa4a; /* bt 0x8c65aa70 */
L8c65aa4a: *stop=0x8c65aa4au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c65aa4c; /* cmp/eq #0x3,r0 */
L8c65aa4c: if((*sr&1u))goto L8c65aa8e;goto L8c65aa4e; /* bt 0x8c65aa8e */
L8c65aa4e: *stop=0x8c65aa50u;goto L8c65aaae; /* bra 0x8c65aaae */
L8c65aa52: *stop=0x8c65aa52u;r[5]=r[7];goto L8c65aa54; /* mov r7,r5 */
L8c65aa54: *stop=0x8c65aa54u;r[7]=r[13];goto L8c65aa56; /* mov r13,r7 */
L8c65aa56: *stop=0x8c65aa56u;r[5]+=8u;goto L8c65aa58; /* add #0x8,r5 */
L8c65aa58: *stop=0x8c65aa58u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65aa5a; /* mov.l @(0x4,r5),r0 */
L8c65aa5a: *stop=0x8c65aa5au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65aa5c; /* cmp/eq #0x1,r0 */
L8c65aa5c: if(!(*sr&1u))goto L8c65aa66;goto L8c65aa5e; /* bf 0x8c65aa66 */
L8c65aa5e: *stop=0x8c65aa5eu;r[7]+=1u;goto L8c65aa60; /* add #0x1,r7 */
L8c65aa60: *stop=0x8c65aa60u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]);goto L8c65aa62; /* cmp/ge r6,r7 */
L8c65aa62: take=!(*sr&1u);*stop=0x8c65aa64u;r[5]+=8u;if(take)goto L8c65aa58;goto L8c65aa66; /* bf/s 0x8c65aa58 */
L8c65aa66: *stop=0x8c65aa66u;r[0]=r[7];goto L8c65aa68; /* mov r7,r0 */
L8c65aa68: *stop=0x8c65aa68u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c65aa6a; /* cmp/eq #0x8,r0 */
L8c65aa6a: if((*sr&1u))goto L8c65aaaa;goto L8c65aa6c; /* bt 0x8c65aaaa */
L8c65aa6c: *stop=0x8c65aa6eu;goto L8c65aab4; /* bra 0x8c65aab4 */
L8c65aa70: *stop=0x8c65aa70u;r[5]=r[7];goto L8c65aa72; /* mov r7,r5 */
L8c65aa72: *stop=0x8c65aa72u;r[7]=r[13];goto L8c65aa74; /* mov r13,r7 */
L8c65aa74: *stop=0x8c65aa74u;r[5]+=72u;goto L8c65aa76; /* add #0x48,r5 */
L8c65aa76: *stop=0x8c65aa76u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65aa78; /* mov.l @(0x4,r5),r0 */
L8c65aa78: *stop=0x8c65aa78u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65aa7a; /* cmp/eq #0x1,r0 */
L8c65aa7a: if(!(*sr&1u))goto L8c65aa84;goto L8c65aa7c; /* bf 0x8c65aa84 */
L8c65aa7c: *stop=0x8c65aa7cu;r[7]+=1u;goto L8c65aa7e; /* add #0x1,r7 */
L8c65aa7e: *stop=0x8c65aa7eu;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]);goto L8c65aa80; /* cmp/ge r6,r7 */
L8c65aa80: take=!(*sr&1u);*stop=0x8c65aa82u;r[5]+=8u;if(take)goto L8c65aa76;goto L8c65aa84; /* bf/s 0x8c65aa76 */
L8c65aa84: *stop=0x8c65aa84u;r[0]=r[7];goto L8c65aa86; /* mov r7,r0 */
L8c65aa86: *stop=0x8c65aa86u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c65aa88; /* cmp/eq #0x8,r0 */
L8c65aa88: if(!(*sr&1u))goto L8c65aab4;goto L8c65aa8a; /* bf 0x8c65aab4 */
L8c65aa8a: *stop=0x8c65aa8cu;goto L8c65aaaa; /* bra 0x8c65aaaa */
L8c65aa8e: *stop=0x8c65aa8eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65aadeu,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65aa90; /* mov.w 0x8c65aade,r5 */
L8c65aa90: *stop=0x8c65aa90u;r[6]=r[13];goto L8c65aa92; /* mov r13,r6 */
L8c65aa92: *stop=0x8c65aa92u;r[5]+=r[7];goto L8c65aa94; /* add r7,r5 */
L8c65aa94: *stop=0x8c65aa94u;r[7]=4u;goto L8c65aa96; /* mov #0x4,r7 */
L8c65aa96: *stop=0x8c65aa96u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65aa98; /* mov.l @(0x4,r5),r0 */
L8c65aa98: *stop=0x8c65aa98u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65aa9a; /* cmp/eq #0x1,r0 */
L8c65aa9a: if(!(*sr&1u))goto L8c65aaa4;goto L8c65aa9c; /* bf 0x8c65aaa4 */
L8c65aa9c: *stop=0x8c65aa9cu;r[6]+=1u;goto L8c65aa9e; /* add #0x1,r6 */
L8c65aa9e: *stop=0x8c65aa9eu;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c65aaa0; /* cmp/ge r7,r6 */
L8c65aaa0: take=!(*sr&1u);*stop=0x8c65aaa2u;r[5]+=8u;if(take)goto L8c65aa96;goto L8c65aaa4; /* bf/s 0x8c65aa96 */
L8c65aaa4: *stop=0x8c65aaa4u;r[0]=r[6];goto L8c65aaa6; /* mov r6,r0 */
L8c65aaa6: *stop=0x8c65aaa6u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c65aaa8; /* cmp/eq #0x4,r0 */
L8c65aaa8: if(!(*sr&1u))goto L8c65aab4;goto L8c65aaaa; /* bf 0x8c65aab4 */
L8c65aaaa: *stop=0x8c65aaacu;r[5]=r[13];goto L8c65aab4; /* bra 0x8c65aab4 */
L8c65aaae: *stop=0x8c65aaaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aae4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65aab0; /* mov.l 0x8c65aae4,r14 */
L8c65aab0: *stop=0x8c65aab2u;goto L8c65aac8; /* bra 0x8c65aac8 */
L8c65aab4: *stop=0x8c65aab4u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c65aab6; /* tst r5,r5 */
L8c65aab6: if((*sr&1u))goto L8c65aac4;goto L8c65aab8; /* bt 0x8c65aac4 */
L8c65aab8: *stop=0x8c65aab8u;r[3]=1u;goto L8c65aaba; /* mov #0x1,r3 */
L8c65aaba: *stop=0x8c65aabau;r[2]=r[5];goto L8c65aabc; /* mov r5,r2 */
L8c65aabc: *stop=0x8c65aabcu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c65aabe; /* mov.l r5,@r4 */
L8c65aabe: *stop=0x8c65aabeu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c65aac0; /* mov.l r3,@(0x4,r2) */
L8c65aac0: *stop=0x8c65aac2u;goto L8c65aac8; /* bra 0x8c65aac8 */
L8c65aac4: *stop=0x8c65aac4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aae8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65aac6; /* mov.l 0x8c65aae8,r14 */
L8c65aac6: *stop=0x8c65aac6u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c65aac8; /* mov.l r13,@r4 */
L8c65aac8: *stop=0x8c65aac8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65aaca; /* tst r14,r14 */
L8c65aaca: if((*sr&1u))goto L8c65aad4;goto L8c65aacc; /* bt 0x8c65aad4 */
L8c65aacc: *stop=0x8c65aaccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aaf0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65aace; /* mov.l 0x8c65aaf0,r3 */
L8c65aace: *stop=0x8c65aaceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aaecu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65aad0; /* mov.l 0x8c65aaec,r4 */
L8c65aad0: target=r[3];*pr=0x8c65aad4u;*stop=0x8c65aad2u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65aad4; /* jsr @r3 */
L8c65aad4: *stop=0x8c65aad4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65aad6; /* lds.l @r15+,PR */
L8c65aad6: *stop=0x8c65aad6u;r[0]=r[14];goto L8c65aad8; /* mov r14,r0 */
L8c65aad8: *stop=0x8c65aad8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65aada; /* mov.l @r15+,r13 */
L8c65aada: *stop=0x8c65aadau;if(*pr!=entry_pr)return -3;*stop=0x8c65aadcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c65acac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65acac;
L8c65acac: *stop=0x8c65acacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65acb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65acae; /* mov.l 0x8c65acb8,r3 */
L8c65acae: *stop=0x8c65acaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65acbcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65acb0; /* mov.l 0x8c65acbc,r2 */
L8c65acb0: *stop=0x8c65acb0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c65acb2; /* mov.l r4,@r3 */
L8c65acb2: *stop=0x8c65acb2u;if(*pr!=entry_pr)return -3;*stop=0x8c65acb4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c65acc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65acc0;
L8c65acc0: *stop=0x8c65acc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65acc2; /* mov.l r14,@-r15 */
L8c65acc2: *stop=0x8c65acc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65acc4; /* sts.l PR,@-r15 */
L8c65acc4: *stop=0x8c65acc4u;r[15]+=4294967288u;goto L8c65acc6; /* add #-0x8,r15 */
L8c65acc6: *stop=0x8c65acc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65acc8; /* mov.l 0x8c65ad30,r3 */
L8c65acc8: *stop=0x8c65acc8u;r[4]=4294967264u;goto L8c65acca; /* mov #-0x20,r4 */
L8c65acca: *stop=0x8c65accau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad2cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65accc; /* mov.l 0x8c65ad2c,r14 */
L8c65accc: *stop=0x8c65acccu;r[3]&=r[4];goto L8c65acce; /* and r4,r3 */
L8c65acce: *stop=0x8c65acceu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ad26u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65acd0; /* mov.w 0x8c65ad26,r2 */
L8c65acd0: *stop=0x8c65acd0u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c65acd2; /* mov.l r3,@r14 */
L8c65acd2: *stop=0x8c65acd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65acd4; /* mov.l 0x8c65ad34,r3 */
L8c65acd4: *stop=0x8c65acd4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c65acd6; /* mov.l r2,@(0x4,r15) */
L8c65acd6: *stop=0x8c65acd6u;r[4]&=r[3];goto L8c65acd8; /* and r3,r4 */
L8c65acd8: *stop=0x8c65acd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65acda; /* mov.l 0x8c65ad3c,r2 */
L8c65acda: *stop=0x8c65acdau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c65acdc; /* mov.l r4,@r15 */
L8c65acdc: *stop=0x8c65acdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65acde; /* mov.l 0x8c65ad38,r4 */
L8c65acde: target=r[2];*pr=0x8c65ace2u;*stop=0x8c65ace0u;r[5]=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ace2; /* jsr @r2 */
L8c65ace2: *stop=0x8c65ace2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ace4; /* mov.l 0x8c65ad44,r3 */
L8c65ace4: *stop=0x8c65ace4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad40u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ace6; /* mov.l 0x8c65ad40,r4 */
L8c65ace6: target=r[3];*pr=0x8c65aceau;*stop=0x8c65ace8u;r[5]=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65acea; /* jsr @r3 */
L8c65acea: *stop=0x8c65aceau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65acec; /* mov.l 0x8c65ad48,r3 */
L8c65acec: target=r[3];*pr=0x8c65acf0u;*stop=0x8c65aceeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65acf0; /* jsr @r3 */
L8c65acf0: *stop=0x8c65acf0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65acf2; /* mov.l 0x8c65ad50,r3 */
L8c65acf2: *stop=0x8c65acf2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad4cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65acf4; /* mov.l 0x8c65ad4c,r4 */
L8c65acf4: target=r[3];*pr=0x8c65acf8u;*stop=0x8c65acf6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65acf8; /* jsr @r3 */
L8c65acf8: *stop=0x8c65acf8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65acfa; /* mov.l 0x8c65ad54,r2 */
L8c65acfa: target=r[2];*pr=0x8c65acfeu;*stop=0x8c65acfcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65acfe; /* jsr @r2 */
L8c65acfe: *stop=0x8c65acfeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ad28u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ad00; /* mov.w 0x8c65ad28,r0 */
L8c65ad00: *stop=0x8c65ad00u;r[2]=0u;goto L8c65ad02; /* mov #0x0,r2 */
L8c65ad02: *stop=0x8c65ad02u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ad04; /* mov.l @r14,r3 */
L8c65ad04: *stop=0x8c65ad04u;r[5]=r[2];goto L8c65ad06; /* mov r2,r5 */
L8c65ad06: *stop=0x8c65ad06u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65ad08; /* mov.l r2,@(r0,r3) */
L8c65ad08: *stop=0x8c65ad08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ad0a; /* mov.l 0x8c65ad58,r2 */
L8c65ad0a: *stop=0x8c65ad0au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ad2au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65ad0c; /* mov.w 0x8c65ad2a,r3 */
L8c65ad0c: *stop=0x8c65ad0cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ad0e; /* mov.l @r14,r4 */
L8c65ad0e: target=r[2];*pr=0x8c65ad12u;*stop=0x8c65ad10u;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ad12; /* jsr @r2 */
L8c65ad12: *stop=0x8c65ad12u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ad2au,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ad14; /* mov.w 0x8c65ad2a,r0 */
L8c65ad14: *stop=0x8c65ad14u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ad16; /* mov.l @r14,r3 */
L8c65ad16: *stop=0x8c65ad16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ad5cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ad18; /* mov.l 0x8c65ad5c,r1 */
L8c65ad18: *stop=0x8c65ad18u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ad1a; /* mov.l @(r0,r3),r2 */
L8c65ad1a: *stop=0x8c65ad1au;r[0]=0u;goto L8c65ad1c; /* mov #0x0,r0 */
L8c65ad1c: *stop=0x8c65ad1cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c65ad1e; /* mov.l r2,@r1 */
L8c65ad1e: *stop=0x8c65ad1eu;r[15]+=8u;goto L8c65ad20; /* add #0x8,r15 */
L8c65ad20: *stop=0x8c65ad20u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65ad22; /* lds.l @r15+,PR */
L8c65ad22: *stop=0x8c65ad22u;if(*pr!=entry_pr)return -3;*stop=0x8c65ad24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c6434e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6434e0;
L8c6434e0: *stop=0x8c6434e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6434e2; /* mov.l r14,@-r15 */
L8c6434e2: *stop=0x8c6434e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6434e4; /* mov.l r13,@-r15 */
L8c6434e4: *stop=0x8c6434e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6434e6; /* mov.l r12,@-r15 */
L8c6434e6: *stop=0x8c6434e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6434e8; /* mov.l r11,@-r15 */
L8c6434e8: *stop=0x8c6434e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6434ea; /* mov.l r10,@-r15 */
L8c6434ea: *stop=0x8c6434eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6434ec; /* mov.l r9,@-r15 */
L8c6434ec: *stop=0x8c6434ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6434ee; /* mov.l r8,@-r15 */
L8c6434ee: *stop=0x8c6434eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6434f0; /* sts.l PR,@-r15 */
L8c6434f0: *stop=0x8c6434f0u;r[15]+=4294967292u;goto L8c6434f2; /* add #-0x4,r15 */
L8c6434f2: *stop=0x8c6434f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64358cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6434f4; /* mov.l 0x8c64358c,r3 */
L8c6434f4: *stop=0x8c6434f4u;r[9]=0u;goto L8c6434f6; /* mov #0x0,r9 */
L8c6434f6: *stop=0x8c6434f6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6434f8; /* mov.l r4,@r15 */
L8c6434f8: *stop=0x8c6434f8u;r[11]=r[9];goto L8c6434fa; /* mov r9,r11 */
L8c6434fa: *stop=0x8c6434fau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6434fc; /* mov.l @r3,r4 */
L8c6434fc: *stop=0x8c6434fcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c643588u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6434fe; /* mov.w 0x8c643588,r0 */
L8c6434fe: *stop=0x8c6434feu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c643500; /* tst r4,r4 */
L8c643500: take=!(*sr&1u);*stop=0x8c643502u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[14]=loaded;}if(take)goto L8c64350a;goto L8c643504; /* bf/s 0x8c64350a */
L8c643504: *stop=0x8c643504u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643590u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c643506; /* mov.l 0x8c643590,r11 */
L8c643506: *stop=0x8c643508u;goto L8c643566; /* bra 0x8c643566 */
L8c64350a: *stop=0x8c64350au;{uint32_t loaded=rd(m,bus,&io_status,0x8c643598u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64350c; /* mov.l 0x8c643598,r2 */
L8c64350c: *stop=0x8c64350cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643594u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64350e; /* mov.l 0x8c643594,r12 */
L8c64350e: target=r[12];*pr=0x8c643512u;*stop=0x8c643510u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643512; /* jsr @r12 */
L8c643512: *stop=0x8c643512u;r[15]+=4u;goto L8c643514; /* add #0x4,r15 */
L8c643514: *stop=0x8c643514u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64359cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c643516; /* mov.l 0x8c64359c,r8 */
L8c643516: *stop=0x8c643516u;r[10]=64u;goto L8c643518; /* mov #0x40,r10 */
L8c643518: *stop=0x8c643518u;r[13]=r[9];goto L8c64351a; /* mov r9,r13 */
L8c64351a: *stop=0x8c64351au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64351c; /* mov.l r13,@-r15 */
L8c64351c: target=r[12];*pr=0x8c643520u;*stop=0x8c64351eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643520; /* jsr @r12 */
L8c643520: *stop=0x8c643520u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c643522; /* mov.l @r14,r0 */
L8c643522: *stop=0x8c643522u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c643524; /* cmp/eq #0x1,r0 */
L8c643524: take=!(*sr&1u);*stop=0x8c643526u;r[15]+=8u;if(take)goto L8c643542;goto L8c643528; /* bf/s 0x8c643542 */
L8c643528: *stop=0x8c643528u;r[13]+=1u;goto L8c64352a; /* add #0x1,r13 */
L8c64352a: *stop=0x8c64352au;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c64352c; /* cmp/ge r10,r13 */
L8c64352c: take=!(*sr&1u);*stop=0x8c64352eu;r[14]+=32u;if(take)goto L8c64351a;goto L8c643530; /* bf/s 0x8c64351a */
L8c643530: *stop=0x8c643530u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643532; /* mov.l 0x8c6435a0,r2 */
L8c643532: target=r[12];*pr=0x8c643536u;*stop=0x8c643534u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643536; /* jsr @r12 */
L8c643536: *stop=0x8c643536u;r[15]+=4u;goto L8c643538; /* add #0x4,r15 */
L8c643538: *stop=0x8c643538u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64353a; /* mov.l @r15,r3 */
L8c64353a: *stop=0x8c64353au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435a4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64353c; /* mov.l 0x8c6435a4,r11 */
L8c64353c: *stop=0x8c64353cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[9],4);if(io_status)return io_status;goto L8c64353e; /* mov.l r9,@r3 */
L8c64353e: *stop=0x8c643540u;goto L8c643566; /* bra 0x8c643566 */
L8c643542: *stop=0x8c643542u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643544; /* mov.l 0x8c6435a8,r2 */
L8c643544: *stop=0x8c643544u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643546; /* mov.l r14,@-r15 */
L8c643546: target=r[12];*pr=0x8c64354au;*stop=0x8c643548u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64354a; /* jsr @r12 */
L8c64354a: *stop=0x8c64354au;r[15]+=8u;goto L8c64354c; /* add #0x8,r15 */
L8c64354c: *stop=0x8c64354cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64354e; /* mov.l 0x8c6435b0,r3 */
L8c64354e: *stop=0x8c64354eu;r[1]=r[14];goto L8c643550; /* mov r14,r1 */
L8c643550: *stop=0x8c643550u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643552; /* mov.l 0x8c6435ac,r2 */
L8c643552: target=r[3];*pr=0x8c643556u;*stop=0x8c643554u;r[0]=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643556; /* jsr @r3 */
L8c643556: *stop=0x8c643556u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c643558; /* mov.l @r15,r1 */
L8c643558: *stop=0x8c643558u;{uint32_t loaded=sx(m,bus,&io_status,0x8c64358au,2);if(io_status)return io_status;r[0]=loaded;}goto L8c64355a; /* mov.w 0x8c64358a,r0 */
L8c64355a: *stop=0x8c64355au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c64355c; /* mov.l r14,@r1 */
L8c64355c: *stop=0x8c64355cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64358cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64355e; /* mov.l 0x8c64358c,r1 */
L8c64355e: *stop=0x8c64355eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643560; /* mov.l @r1,r3 */
L8c643560: *stop=0x8c643560u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c643562; /* mov.l @(r0,r3),r2 */
L8c643562: *stop=0x8c643562u;r[2]+=1u;goto L8c643564; /* add #0x1,r2 */
L8c643564: *stop=0x8c643564u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c643566; /* mov.l r2,@(r0,r3) */
L8c643566: *stop=0x8c643566u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c643568; /* tst r11,r11 */
L8c643568: if((*sr&1u))goto L8c643572;goto L8c64356a; /* bt 0x8c643572 */
L8c64356a: *stop=0x8c64356au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64356c; /* mov.l 0x8c6435b8,r2 */
L8c64356c: *stop=0x8c64356cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6435b4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64356e; /* mov.l 0x8c6435b4,r4 */
L8c64356e: target=r[2];*pr=0x8c643572u;*stop=0x8c643570u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643572; /* jsr @r2 */
L8c643572: *stop=0x8c643572u;r[0]=r[11];goto L8c643574; /* mov r11,r0 */
L8c643574: *stop=0x8c643574u;r[15]+=4u;goto L8c643576; /* add #0x4,r15 */
L8c643576: *stop=0x8c643576u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643578; /* lds.l @r15+,PR */
L8c643578: *stop=0x8c643578u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64357a; /* mov.l @r15+,r8 */
L8c64357a: *stop=0x8c64357au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64357c; /* mov.l @r15+,r9 */
L8c64357c: *stop=0x8c64357cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64357e; /* mov.l @r15+,r10 */
L8c64357e: *stop=0x8c64357eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c643580; /* mov.l @r15+,r11 */
L8c643580: *stop=0x8c643580u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c643582; /* mov.l @r15+,r12 */
L8c643582: *stop=0x8c643582u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c643584; /* mov.l @r15+,r13 */
L8c643584: *stop=0x8c643584u;if(*pr!=entry_pr)return -3;*stop=0x8c643586u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c643624(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c643624;
L8c643624: *stop=0x8c643624u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643626; /* mov.l r14,@-r15 */
L8c643626: *stop=0x8c643626u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c643628; /* mov.l r13,@-r15 */
L8c643628: *stop=0x8c643628u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64362a; /* sts.l PR,@-r15 */
L8c64362a: *stop=0x8c64362au;r[15]+=4294967280u;goto L8c64362c; /* add #-0x10,r15 */
L8c64362c: *stop=0x8c64362cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64362e; /* mov.l 0x8c6436a4,r2 */
L8c64362e: *stop=0x8c64362eu;r[14]=r[4];goto L8c643630; /* mov r4,r14 */
L8c643630: *stop=0x8c643630u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c643632; /* mov.l r5,@(0xc,r15) */
L8c643632: *stop=0x8c643632u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c643634; /* mov.l r6,@(0x4,r15) */
L8c643634: *stop=0x8c643634u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[7],4);if(io_status)return io_status;goto L8c643636; /* mov.l r7,@(0x8,r15) */
L8c643636: *stop=0x8c643636u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643638; /* mov.l @r2,r3 */
L8c643638: *stop=0x8c643638u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64363a; /* tst r3,r3 */
L8c64363a: take=!(*sr&1u);*stop=0x8c64363cu;r[13]=0u;if(take)goto L8c643644;goto L8c64363e; /* bf/s 0x8c643644 */
L8c64363e: *stop=0x8c64363eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436a8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c643640; /* mov.l 0x8c6436a8,r13 */
L8c643640: *stop=0x8c643642u;goto L8c64368c; /* bra 0x8c64368c */
L8c643644: *stop=0x8c643644u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643646; /* tst r14,r14 */
L8c643646: if(!(*sr&1u))goto L8c64364e;goto L8c643648; /* bf 0x8c64364e */
L8c643648: *stop=0x8c643648u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436acu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64364a; /* mov.l 0x8c6436ac,r13 */
L8c64364a: *stop=0x8c64364cu;goto L8c64368c; /* bra 0x8c64368c */
L8c64364e: *stop=0x8c64364eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643650; /* mov.l 0x8c6436b0,r3 */
L8c643650: *stop=0x8c643650u;r[5]=r[15];goto L8c643652; /* mov r15,r5 */
L8c643652: target=r[3];*pr=0x8c643656u;*stop=0x8c643654u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643656; /* jsr @r3 */
L8c643656: *stop=0x8c643656u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c643658; /* mov.l @r15,r0 */
L8c643658: *stop=0x8c643658u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64365a; /* cmp/eq #0x2,r0 */
L8c64365a: if(!(*sr&1u))goto L8c643662;goto L8c64365c; /* bf 0x8c643662 */
L8c64365c: *stop=0x8c64365cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436b4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64365e; /* mov.l 0x8c6436b4,r13 */
L8c64365e: *stop=0x8c643660u;goto L8c64368c; /* bra 0x8c64368c */
L8c643662: *stop=0x8c643662u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643664; /* mov.l 0x8c6436b8,r3 */
L8c643664: *stop=0x8c643664u;r[5]=0u;goto L8c643666; /* mov #0x0,r5 */
L8c643666: target=r[3];*pr=0x8c64366au;*stop=0x8c643668u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64366a; /* jsr @r3 */
L8c64366a: *stop=0x8c64366au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64366c; /* mov.l 0x8c6436bc,r3 */
L8c64366c: *stop=0x8c64366cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64366e; /* mov.l @(0xc,r15),r5 */
L8c64366e: target=r[3];*pr=0x8c643672u;*stop=0x8c643670u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643672; /* jsr @r3 */
L8c643672: *stop=0x8c643672u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643674; /* mov.l 0x8c6436c0,r3 */
L8c643674: *stop=0x8c643674u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c643676; /* mov.l @(0x4,r15),r5 */
L8c643676: target=r[3];*pr=0x8c64367au;*stop=0x8c643678u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64367a; /* jsr @r3 */
L8c64367a: *stop=0x8c64367au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64367c; /* mov.l 0x8c6436c4,r3 */
L8c64367c: *stop=0x8c64367cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64367e; /* mov.l @(0x8,r15),r5 */
L8c64367e: *stop=0x8c64367eu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c643680; /* mov.l @(0x1c,r15),r6 */
L8c643680: target=r[3];*pr=0x8c643684u;*stop=0x8c643682u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643684; /* jsr @r3 */
L8c643684: *stop=0x8c643684u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643686; /* mov.l 0x8c6436b8,r2 */
L8c643686: *stop=0x8c643686u;r[5]=0u;goto L8c643688; /* mov #0x0,r5 */
L8c643688: target=r[2];*pr=0x8c64368cu;*stop=0x8c64368au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64368c; /* jsr @r2 */
L8c64368c: *stop=0x8c64368cu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c64368e; /* tst r13,r13 */
L8c64368e: if((*sr&1u))goto L8c643698;goto L8c643690; /* bt 0x8c643698 */
L8c643690: *stop=0x8c643690u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643692; /* mov.l 0x8c6436cc,r2 */
L8c643692: *stop=0x8c643692u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436c8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643694; /* mov.l 0x8c6436c8,r4 */
L8c643694: target=r[2];*pr=0x8c643698u;*stop=0x8c643696u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643698; /* jsr @r2 */
L8c643698: *stop=0x8c643698u;r[0]=r[13];goto L8c64369a; /* mov r13,r0 */
L8c64369a: *stop=0x8c64369au;r[15]+=16u;goto L8c64369c; /* add #0x10,r15 */
L8c64369c: *stop=0x8c64369cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64369e; /* lds.l @r15+,PR */
L8c64369e: *stop=0x8c64369eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6436a0; /* mov.l @r15+,r13 */
L8c6436a0: *stop=0x8c6436a0u;if(*pr!=entry_pr)return -3;*stop=0x8c6436a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c6435bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6435bc;
L8c6435bc: *stop=0x8c6435bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643610u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6435be; /* mov.l 0x8c643610,r2 */
L8c6435be: *stop=0x8c6435beu;r[6]=0u;goto L8c6435c0; /* mov #0x0,r6 */
L8c6435c0: *stop=0x8c6435c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6435c2; /* mov.l r14,@-r15 */
L8c6435c2: *stop=0x8c6435c2u;r[14]=r[6];goto L8c6435c4; /* mov r6,r14 */
L8c6435c4: *stop=0x8c6435c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6435c6; /* sts.l PR,@-r15 */
L8c6435c6: *stop=0x8c6435c6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6435c8; /* mov.l @r2,r3 */
L8c6435c8: *stop=0x8c6435c8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6435ca; /* tst r3,r3 */
L8c6435ca: take=!(*sr&1u);*stop=0x8c6435ccu;r[5]=r[4];if(take)goto L8c6435d4;goto L8c6435ce; /* bf/s 0x8c6435d4 */
L8c6435ce: *stop=0x8c6435ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643614u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6435d0; /* mov.l 0x8c643614,r14 */
L8c6435d0: *stop=0x8c6435d2u;goto L8c6435f8; /* bra 0x8c6435f8 */
L8c6435d4: *stop=0x8c6435d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643618u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6435d6; /* mov.l 0x8c643618,r7 */
L8c6435d6: *stop=0x8c6435d6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6435d8; /* tst r4,r4 */
L8c6435d8: if(!(*sr&1u))goto L8c6435de;goto L8c6435da; /* bf 0x8c6435de */
L8c6435da: *stop=0x8c6435dcu;r[14]=r[7];goto L8c6435f8; /* bra 0x8c6435f8 */
L8c6435de: *stop=0x8c6435deu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6435e0; /* mov.l @r5,r0 */
L8c6435e0: *stop=0x8c6435e0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6435e2; /* cmp/eq #0x1,r0 */
L8c6435e2: if(!(*sr&1u))goto L8c6435f6;goto L8c6435e4; /* bf 0x8c6435f6 */
L8c6435e4: *stop=0x8c6435e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6435e6; /* mov.l 0x8c643610,r3 */
L8c6435e6: *stop=0x8c6435e6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c64360cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6435e8; /* mov.w 0x8c64360c,r0 */
L8c6435e8: *stop=0x8c6435e8u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[6],4);if(io_status)return io_status;goto L8c6435ea; /* mov.l r6,@r5 */
L8c6435ea: *stop=0x8c6435eau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6435ec; /* mov.l @r3,r2 */
L8c6435ec: *stop=0x8c6435ecu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6435ee; /* mov.l @(r0,r2),r1 */
L8c6435ee: *stop=0x8c6435eeu;r[1]+=4294967295u;goto L8c6435f0; /* add #-0x1,r1 */
L8c6435f0: *stop=0x8c6435f0u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6435f2; /* mov.l r1,@(r0,r2) */
L8c6435f2: *stop=0x8c6435f4u;goto L8c6435f8; /* bra 0x8c6435f8 */
L8c6435f6: *stop=0x8c6435f6u;r[14]=r[7];goto L8c6435f8; /* mov r7,r14 */
L8c6435f8: *stop=0x8c6435f8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6435fa; /* tst r14,r14 */
L8c6435fa: if((*sr&1u))goto L8c643604;goto L8c6435fc; /* bt 0x8c643604 */
L8c6435fc: *stop=0x8c6435fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643620u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6435fe; /* mov.l 0x8c643620,r2 */
L8c6435fe: *stop=0x8c6435feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64361cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643600; /* mov.l 0x8c64361c,r4 */
L8c643600: target=r[2];*pr=0x8c643604u;*stop=0x8c643602u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643604; /* jsr @r2 */
L8c643604: *stop=0x8c643604u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643606; /* lds.l @r15+,PR */
L8c643606: *stop=0x8c643606u;r[0]=r[14];goto L8c643608; /* mov r14,r0 */
L8c643608: *stop=0x8c643608u;if(*pr!=entry_pr)return -3;*stop=0x8c64360au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_init_records_8c643e6c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c643e6c;
L8c643e6c: *stop=0x8c643e6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643e6e; /* mov.l r14,@-r15 */
L8c643e6e: *stop=0x8c643e6eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c643e70; /* mov.l r13,@-r15 */
L8c643e70: *stop=0x8c643e70u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c643e72; /* mov.l r12,@-r15 */
L8c643e72: *stop=0x8c643e72u;r[12]=r[4];goto L8c643e74; /* mov r4,r12 */
L8c643e74: *stop=0x8c643e74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643e76; /* sts.l PR,@-r15 */
L8c643e76: *stop=0x8c643e76u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c643e78; /* tst r12,r12 */
L8c643e78: take=!(*sr&1u);*stop=0x8c643e7au;r[14]=0u;if(take)goto L8c643e82;goto L8c643e7c; /* bf/s 0x8c643e82 */
L8c643e7c: *stop=0x8c643e7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f24u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643e7e; /* mov.l 0x8c643f24,r14 */
L8c643e7e: *stop=0x8c643e80u;goto L8c643efc; /* bra 0x8c643efc */
L8c643e82: *stop=0x8c643e82u;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c643e84; /* mov.l @(0x8,r12),r13 */
L8c643e84: *stop=0x8c643e84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643e86; /* mov.l 0x8c643f28,r2 */
L8c643e86: *stop=0x8c643e86u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643e88; /* mov.l @r13,r3 */
L8c643e88: *stop=0x8c643e88u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c643e8a; /* cmp/eq r2,r3 */
L8c643e8a: if((*sr&1u))goto L8c643e92;goto L8c643e8c; /* bt 0x8c643e92 */
L8c643e8c: *stop=0x8c643e8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f2cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643e8e; /* mov.l 0x8c643f2c,r14 */
L8c643e8e: *stop=0x8c643e90u;goto L8c643efc; /* bra 0x8c643efc */
L8c643e92: *stop=0x8c643e92u;if(!valid_access(r[13]+4u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+0x4u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643e94; /* mov.b @(0x4,r13),r0 */
L8c643e94: *stop=0x8c643e94u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c643e96; /* cmp/eq #0x1,r0 */
L8c643e96: if(!(*sr&1u))goto L8c643ea0;goto L8c643e98; /* bf 0x8c643ea0 */
L8c643e98: *stop=0x8c643e98u;if(!valid_access(r[13]+5u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+0x5u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643e9a; /* mov.b @(0x5,r13),r0 */
L8c643e9a: *stop=0x8c643e9au;r[3]=1u;goto L8c643e9c; /* mov #0x1,r3 */
L8c643e9c: *stop=0x8c643e9cu;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c643e9e; /* cmp/gt r3,r0 */
L8c643e9e: if(!(*sr&1u))goto L8c643ea6;goto L8c643ea0; /* bf 0x8c643ea6 */
L8c643ea0: *stop=0x8c643ea0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f30u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643ea2; /* mov.l 0x8c643f30,r14 */
L8c643ea2: *stop=0x8c643ea4u;goto L8c643efc; /* bra 0x8c643efc */
L8c643ea6: *stop=0x8c643ea6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643ea8; /* mov.l 0x8c643f34,r2 */
L8c643ea8: target=r[2];*pr=0x8c643eacu;*stop=0x8c643eaau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643eac; /* jsr @r2 */
L8c643eac: *stop=0x8c643eacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643eae; /* mov.l 0x8c643f38,r3 */
L8c643eae: target=r[3];*pr=0x8c643eb2u;*stop=0x8c643eb0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643eb2; /* jsr @r3 */
L8c643eb2: *stop=0x8c643eb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643eb4; /* mov.l 0x8c643f40,r2 */
L8c643eb4: *stop=0x8c643eb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f3cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643eb6; /* mov.l 0x8c643f3c,r4 */
L8c643eb6: target=r[2];*pr=0x8c643ebau;*stop=0x8c643eb8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643eba; /* jsr @r2 */
L8c643eba: *stop=0x8c643ebau;r[5]=r[13];goto L8c643ebc; /* mov r13,r5 */
L8c643ebc: *stop=0x8c643ebcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643ebe; /* mov.l 0x8c643f3c,r2 */
L8c643ebe: *stop=0x8c643ebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f44u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643ec0; /* mov.l 0x8c643f44,r1 */
L8c643ec0: *stop=0x8c643ec0u;r[3]=0u;goto L8c643ec2; /* mov #0x0,r3 */
L8c643ec2: *stop=0x8c643ec2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c643ec4; /* mov.l r3,@-r15 */
L8c643ec4: *stop=0x8c643ec4u;r[7]=4294967295u;goto L8c643ec6; /* mov #-0x1,r7 */
L8c643ec6: *stop=0x8c643ec6u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c643ec8; /* mov.l @(0x8,r13),r6 */
L8c643ec8: *stop=0x8c643ec8u;r[5]+=32u;goto L8c643eca; /* add #0x20,r5 */
L8c643eca: target=r[1];*pr=0x8c643eceu;*stop=0x8c643eccu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643ece; /* jsr @r1 */
L8c643ece: *stop=0x8c643eceu;r[15]+=4u;goto L8c643ed0; /* add #0x4,r15 */
L8c643ed0: *stop=0x8c643ed0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643ed2; /* mov.l 0x8c643f48,r2 */
L8c643ed2: *stop=0x8c643ed2u;r[5]=0u;goto L8c643ed4; /* mov #0x0,r5 */
L8c643ed4: *stop=0x8c643ed4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643ed6; /* mov.l 0x8c643f3c,r3 */
L8c643ed6: target=r[2];*pr=0x8c643edau;*stop=0x8c643ed8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643eda; /* jsr @r2 */
L8c643eda: *stop=0x8c643edau;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f4cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643edc; /* mov.l 0x8c643f4c,r1 */
L8c643edc: target=r[1];*pr=0x8c643ee0u;*stop=0x8c643edeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643ee0; /* jsr @r1 */
L8c643ee0: *stop=0x8c643ee0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f50u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643ee2; /* mov.l 0x8c643f50,r4 */
L8c643ee2: *stop=0x8c643ee4u;goto L8c643ee8; /* bra 0x8c643ee8 */
L8c643ee6: *stop=0x8c643ee6u;r[4]+=4294967295u;goto L8c643ee8; /* add #-0x1,r4 */
L8c643ee8: *stop=0x8c643ee8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c643eea; /* tst r4,r4 */
L8c643eea: if(!(*sr&1u))goto L8c643ee6;goto L8c643eec; /* bf 0x8c643ee6 */
L8c643eec: *stop=0x8c643eecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643eee; /* mov.l 0x8c643f58,r1 */
L8c643eee: *stop=0x8c643eeeu;r[3]=1u;goto L8c643ef0; /* mov #0x1,r3 */
L8c643ef0: *stop=0x8c643ef0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643ef2; /* mov.l 0x8c643f54,r2 */
L8c643ef2: target=r[1];*pr=0x8c643ef6u;*stop=0x8c643ef4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643ef6; /* jsr @r1 */
L8c643ef6: *stop=0x8c643ef6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643ef8; /* mov.l 0x8c643f5c,r3 */
L8c643ef8: target=r[3];*pr=0x8c643efcu;*stop=0x8c643efau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643efc; /* jsr @r3 */
L8c643efc: *stop=0x8c643efcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643efe; /* mov.l 0x8c643f3c,r3 */
L8c643efe: *stop=0x8c643efeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c643f00; /* mov.l @r3,r2 */
L8c643f00: *stop=0x8c643f00u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c643f02; /* tst r2,r2 */
L8c643f02: if((*sr&1u))goto L8c643f0a;goto L8c643f04; /* bt 0x8c643f0a */
L8c643f04: *stop=0x8c643f04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f60u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643f06; /* mov.l 0x8c643f60,r2 */
L8c643f06: target=r[2];*pr=0x8c643f0au;*stop=0x8c643f08u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643f0a; /* jsr @r2 */
L8c643f0a: *stop=0x8c643f0au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643f0c; /* tst r14,r14 */
L8c643f0c: if((*sr&1u))goto L8c643f16;goto L8c643f0e; /* bt 0x8c643f16 */
L8c643f0e: *stop=0x8c643f0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643f10; /* mov.l 0x8c643f68,r3 */
L8c643f10: *stop=0x8c643f10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643f64u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643f12; /* mov.l 0x8c643f64,r4 */
L8c643f12: target=r[3];*pr=0x8c643f16u;*stop=0x8c643f14u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643f16; /* jsr @r3 */
L8c643f16: *stop=0x8c643f16u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643f18; /* lds.l @r15+,PR */
L8c643f18: *stop=0x8c643f18u;r[0]=r[14];goto L8c643f1a; /* mov r14,r0 */
L8c643f1a: *stop=0x8c643f1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c643f1c; /* mov.l @r15+,r12 */
L8c643f1c: *stop=0x8c643f1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c643f1e; /* mov.l @r15+,r13 */
L8c643f1e: *stop=0x8c643f1eu;if(*pr!=entry_pr)return -3;*stop=0x8c643f20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
