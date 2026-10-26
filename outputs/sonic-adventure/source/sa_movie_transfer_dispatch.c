/* Original registered-event and ASIC interrupt dispatch; explicit machine/platform state.
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
SA_STAGE_EXPORT int sa_movie_transfer_dispatch_8c645afa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645afa;
L8c645afa: *stop=0x8c645afau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645afc; /* mov.l r14,@-r15 */
L8c645afc: *stop=0x8c645afcu;r[3]=4294967292u;goto L8c645afe; /* mov #-0x4,r3 */
L8c645afe: *stop=0x8c645afeu;r[14]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c645b00; /* exts.w r4,r14 */
L8c645b00: *stop=0x8c645b00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c645b02; /* mov.l 0x8c645b60,r0 */
L8c645b02: *stop=0x8c645b02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645b04; /* sts.l PR,@-r15 */
L8c645b04: *stop=0x8c645b04u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[14]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[14]=n?(uint32_t)((int32_t)r[14]>>n):((r[14]&0x80000000u)?0xffffffffu:0u);}}goto L8c645b06; /* shad r3,r14 */
L8c645b06: *stop=0x8c645b06u;r[14]<<=2;goto L8c645b08; /* shll2 r14 */
L8c645b08: *stop=0x8c645b0au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b14; /* bra 0x8c645b14 */
L8c645b0c: *stop=0x8c645b0cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645b0e; /* mov.l @r14,r3 */
L8c645b0e: target=r[3];*pr=0x8c645b12u;*stop=0x8c645b10u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645b12; /* jsr @r3 */
L8c645b12: *stop=0x8c645b12u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b14; /* mov.l @(0x4,r14),r14 */
L8c645b14: *stop=0x8c645b14u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c645b16; /* tst r14,r14 */
L8c645b16: if(!(*sr&1u))goto L8c645b0c;goto L8c645b18; /* bf 0x8c645b0c */
L8c645b18: *stop=0x8c645b18u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645b1a; /* lds.l @r15+,PR */
L8c645b1a: *stop=0x8c645b1au;if(*pr!=entry_pr)return -3;*stop=0x8c645b1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_transfer_dispatch_8c645b1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645b1e;
L8c645b1e: *stop=0x8c645b1eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645b20; /* stc SR,r0 */
L8c645b20: *stop=0x8c645b20u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645b48u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645b22; /* mov.w 0x8c645b48,r3 */
L8c645b22: *stop=0x8c645b22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645b24; /* mov.l r14,@-r15 */
L8c645b24: *stop=0x8c645b24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645b26; /* sts.l PR,@-r15 */
L8c645b26: *stop=0x8c645b26u;r[0]&=r[3];goto L8c645b28; /* and r3,r0 */
L8c645b28: *stop=0x8c645b28u;r[0]|=0x60u;goto L8c645b2a; /* or #0x60,r0 */
L8c645b2a: *stop=0x8c645b2au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645b2c; /* ldc r0,SR */
L8c645b2c: *stop=0x8c645b2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b64u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b2e; /* mov.l 0x8c645b64,r14 */
L8c645b2e: *stop=0x8c645b30u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b3a; /* bra 0x8c645b3a */
L8c645b32: *stop=0x8c645b32u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645b34; /* mov.l @r14,r3 */
L8c645b34: target=r[3];*pr=0x8c645b38u;*stop=0x8c645b36u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645b38; /* jsr @r3 */
L8c645b38: *stop=0x8c645b38u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b3a; /* mov.l @(0x4,r14),r14 */
L8c645b3a: *stop=0x8c645b3au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c645b3c; /* tst r14,r14 */
L8c645b3c: if(!(*sr&1u))goto L8c645b32;goto L8c645b3e; /* bf 0x8c645b32 */
L8c645b3e: *stop=0x8c645b3eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645b40; /* lds.l @r15+,PR */
L8c645b40: *stop=0x8c645b40u;if(*pr!=entry_pr)return -3;*stop=0x8c645b42u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_transfer_dispatch_8c645b68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645b68;
L8c645b68: *stop=0x8c645b68u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645b6a; /* stc SR,r0 */
L8c645b6a: *stop=0x8c645b6au;{uint32_t loaded=sx(m,bus,&io_status,0x8c645c38u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645b6c; /* mov.w 0x8c645c38,r3 */
L8c645b6c: *stop=0x8c645b6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645b6e; /* mov.l r14,@-r15 */
L8c645b6e: *stop=0x8c645b6eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645b70; /* sts.l PR,@-r15 */
L8c645b70: *stop=0x8c645b70u;r[0]&=r[3];goto L8c645b72; /* and r3,r0 */
L8c645b72: *stop=0x8c645b72u;r[0]|=0x60u;goto L8c645b74; /* or #0x60,r0 */
L8c645b74: *stop=0x8c645b74u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645b76; /* ldc r0,SR */
L8c645b76: *stop=0x8c645b76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645c40u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b78; /* mov.l 0x8c645c40,r14 */
L8c645b78: *stop=0x8c645b7au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b84; /* bra 0x8c645b84 */
L8c645b7c: *stop=0x8c645b7cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645b7e; /* mov.l @r14,r3 */
L8c645b7e: target=r[3];*pr=0x8c645b82u;*stop=0x8c645b80u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645b82; /* jsr @r3 */
L8c645b82: *stop=0x8c645b82u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c645b84; /* mov.l @(0x4,r14),r14 */
L8c645b84: *stop=0x8c645b84u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c645b86; /* tst r14,r14 */
L8c645b86: if(!(*sr&1u))goto L8c645b7c;goto L8c645b88; /* bf 0x8c645b7c */
L8c645b88: *stop=0x8c645b88u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645b8a; /* lds.l @r15+,PR */
L8c645b8a: *stop=0x8c645b8au;if(*pr!=entry_pr)return -3;*stop=0x8c645b8cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_transfer_dispatch_8c645b8e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c645b8e;
L8c645b8e: *stop=0x8c645b8eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645b90; /* mov.l r14,@-r15 */
L8c645b90: *stop=0x8c645b90u;r[3]=4294967292u;goto L8c645b92; /* mov #-0x4,r3 */
L8c645b92: *stop=0x8c645b92u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645b94; /* mov.l r13,@-r15 */
L8c645b94: *stop=0x8c645b94u;r[0]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c645b96; /* exts.w r4,r0 */
L8c645b96: *stop=0x8c645b96u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645b98; /* mov.l r12,@-r15 */
L8c645b98: *stop=0x8c645b98u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645b9a; /* shad r3,r0 */
L8c645b9a: *stop=0x8c645b9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c645b9c; /* mov.l r11,@-r15 */
L8c645b9c: *stop=0x8c645b9cu;*sr=(*sr&~1u)|(r[0]==0x00000032u);goto L8c645b9e; /* cmp/eq #0x32,r0 */
L8c645b9e: *stop=0x8c645b9eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c645ba0; /* mov.l r10,@-r15 */
L8c645ba0: *stop=0x8c645ba0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645ba2; /* sts.l PR,@-r15 */
L8c645ba2: if((*sr&1u))goto L8c645c2a;goto L8c645ba4; /* bt 0x8c645c2a */
L8c645ba4: *stop=0x8c645ba4u;*sr=(*sr&~1u)|(r[0]==0x00000036u);goto L8c645ba6; /* cmp/eq #0x36,r0 */
L8c645ba6: if((*sr&1u))goto L8c645bb0;goto L8c645ba8; /* bt 0x8c645bb0 */
L8c645ba8: *stop=0x8c645ba8u;*sr=(*sr&~1u)|(r[0]==0x0000003au);goto L8c645baa; /* cmp/eq #0x3a,r0 */
L8c645baa: if((*sr&1u))goto L8c645c2a;goto L8c645bac; /* bt 0x8c645c2a */
L8c645bac: *stop=0x8c645baeu;goto L8c645c2a; /* bra 0x8c645c2a */
L8c645bb0: *stop=0x8c645bb0u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645bb2; /* stc SR,r0 */
L8c645bb2: *stop=0x8c645bb2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645c38u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c645bb4; /* mov.w 0x8c645c38,r1 */
L8c645bb4: *stop=0x8c645bb4u;r[0]&=r[1];goto L8c645bb6; /* and r1,r0 */
L8c645bb6: *stop=0x8c645bb6u;r[0]|=0x40u;goto L8c645bb8; /* or #0x40,r0 */
L8c645bb8: *stop=0x8c645bb8u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645bba; /* ldc r0,SR */
L8c645bba: *stop=0x8c645bbau;{uint32_t loaded=rd(m,bus,&io_status,0x8c645c48u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c645bbc; /* mov.l 0x8c645c48,r0 */
L8c645bbc: *stop=0x8c645bbcu;r[4]=0u;goto L8c645bbe; /* mov #0x0,r4 */
L8c645bbe: *stop=0x8c645bbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645c44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645bc0; /* mov.l 0x8c645c44,r2 */
L8c645bc0: *stop=0x8c645bc0u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[12]=loaded;}goto L8c645bc2; /* mov.l @r0,r12 */
L8c645bc2: *stop=0x8c645bc2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645bc4; /* mov.l @r2,r3 */
L8c645bc4: *stop=0x8c645bc4u;r[12]&=r[3];goto L8c645bc6; /* and r3,r12 */
L8c645bc6: *stop=0x8c645bc6u;r[12]>>=8;goto L8c645bc8; /* shlr8 r12 */
L8c645bc8: *stop=0x8c645bc8u;r[12]>>=2;goto L8c645bca; /* shlr2 r12 */
L8c645bca: *stop=0x8c645bcau;r[12]>>=2;goto L8c645bcc; /* shlr2 r12 */
L8c645bcc: *stop=0x8c645bccu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c645bce; /* tst r12,r12 */
L8c645bce: take=(*sr&1u);*stop=0x8c645bd0u;r[14]=1u;if(take)goto L8c645bfc;goto L8c645bd2; /* bt/s 0x8c645bfc */
L8c645bd2: *stop=0x8c645bd2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645c3cu,2);if(io_status)return io_status;r[11]=loaded;}goto L8c645bd4; /* mov.w 0x8c645c3c,r11 */
L8c645bd4: *stop=0x8c645bd4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645c3au,2);if(io_status)return io_status;r[10]=loaded;}goto L8c645bd6; /* mov.w 0x8c645c3a,r10 */
L8c645bd6: *stop=0x8c645bd8u;r[13]=r[4];goto L8c645bf0; /* bra 0x8c645bf0 */
L8c645bda: *stop=0x8c645bdau;r[3]=r[14];goto L8c645bdc; /* mov r14,r3 */
L8c645bdc: *stop=0x8c645bdcu;{uint32_t shift=r[13];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c645bde; /* shad r13,r3 */
L8c645bde: *stop=0x8c645bdeu;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c645be0; /* tst r12,r3 */
L8c645be0: if((*sr&1u))goto L8c645bee;goto L8c645be2; /* bt 0x8c645bee */
L8c645be2: *stop=0x8c645be2u;r[4]=r[13];goto L8c645be4; /* mov r13,r4 */
L8c645be4: *stop=0x8c645be4u;r[4]<<=2;goto L8c645be6; /* shll2 r4 */
L8c645be6: *stop=0x8c645be6u;r[4]<<=2;goto L8c645be8; /* shll2 r4 */
L8c645be8: *stop=0x8c645be8u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c645bea; /* shll r4 */
L8c645bea: target=0x8c645afau;*pr=0x8c645beeu;*stop=0x8c645becu;r[4]+=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645bee; /* bsr 0x8c645afa */
L8c645bee: *stop=0x8c645beeu;r[13]+=1u;goto L8c645bf0; /* add #0x1,r13 */
L8c645bf0: *stop=0x8c645bf0u;r[2]=r[14];goto L8c645bf2; /* mov r14,r2 */
L8c645bf2: *stop=0x8c645bf2u;{uint32_t shift=r[13];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c645bf4; /* shad r13,r2 */
L8c645bf4: *stop=0x8c645bf4u;*sr=(*sr&~1u)|(r[11]==r[2]);goto L8c645bf6; /* cmp/eq r11,r2 */
L8c645bf6: if(!(*sr&1u))goto L8c645bda;goto L8c645bf8; /* bf 0x8c645bda */
L8c645bf8: *stop=0x8c645bfau;goto L8c645c2a; /* bra 0x8c645c2a */
L8c645bfc: *stop=0x8c645bfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645c50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645bfe; /* mov.l 0x8c645c50,r2 */
L8c645bfe: *stop=0x8c645bfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645c4cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c645c00; /* mov.l 0x8c645c4c,r1 */
L8c645c00: *stop=0x8c645c00u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[11]=loaded;}goto L8c645c02; /* mov.l @r2,r11 */
L8c645c02: *stop=0x8c645c02u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645c04; /* mov.l @r1,r3 */
L8c645c04: *stop=0x8c645c04u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645c3eu,2);if(io_status)return io_status;r[12]=loaded;}goto L8c645c06; /* mov.w 0x8c645c3e,r12 */
L8c645c06: *stop=0x8c645c06u;r[11]&=r[3];goto L8c645c08; /* and r3,r11 */
L8c645c08: *stop=0x8c645c0au;r[13]=r[4];goto L8c645c22; /* bra 0x8c645c22 */
L8c645c0c: *stop=0x8c645c0cu;r[2]=r[14];goto L8c645c0e; /* mov r14,r2 */
L8c645c0e: *stop=0x8c645c0eu;{uint32_t shift=r[13];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c645c10; /* shad r13,r2 */
L8c645c10: *stop=0x8c645c10u;*sr=(*sr&~1u)|!(r[11]&r[2]);goto L8c645c12; /* tst r11,r2 */
L8c645c12: if((*sr&1u))goto L8c645c20;goto L8c645c14; /* bt 0x8c645c20 */
L8c645c14: *stop=0x8c645c14u;r[4]=r[13];goto L8c645c16; /* mov r13,r4 */
L8c645c16: *stop=0x8c645c16u;r[4]<<=2;goto L8c645c18; /* shll2 r4 */
L8c645c18: *stop=0x8c645c18u;r[4]<<=2;goto L8c645c1a; /* shll2 r4 */
L8c645c1a: *stop=0x8c645c1au;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c645c1c; /* shll r4 */
L8c645c1c: target=0x8c645afau;*pr=0x8c645c20u;*stop=0x8c645c1eu;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645c20; /* bsr 0x8c645afa */
L8c645c20: *stop=0x8c645c20u;r[13]+=1u;goto L8c645c22; /* add #0x1,r13 */
L8c645c22: *stop=0x8c645c22u;r[0]=r[14];goto L8c645c24; /* mov r14,r0 */
L8c645c24: *stop=0x8c645c24u;{uint32_t shift=r[13];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645c26; /* shad r13,r0 */
L8c645c26: *stop=0x8c645c26u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c645c28; /* cmp/eq #0x10,r0 */
L8c645c28: if(!(*sr&1u))goto L8c645c0c;goto L8c645c2a; /* bf 0x8c645c0c */
L8c645c2a: *stop=0x8c645c2au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645c2c; /* lds.l @r15+,PR */
L8c645c2c: *stop=0x8c645c2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c645c2e; /* mov.l @r15+,r10 */
L8c645c2e: *stop=0x8c645c2eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c645c30; /* mov.l @r15+,r11 */
L8c645c30: *stop=0x8c645c30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c645c32; /* mov.l @r15+,r12 */
L8c645c32: *stop=0x8c645c32u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645c34; /* mov.l @r15+,r13 */
L8c645c34: *stop=0x8c645c34u;if(*pr!=entry_pr)return -3;*stop=0x8c645c36u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
