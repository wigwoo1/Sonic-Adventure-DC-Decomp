/* Original active-input scheduler, record cleanup and context restore.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; no callback result is substituted.
 * Context and interrupt entry remain explicit platform state.
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
SA_STAGE_EXPORT int sa_movie_active_input_8c668e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];(void)fr;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668e60;
L8c668e60: *stop=0x8c668e60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668e62; /* mov.l r14,@-r15 */
L8c668e62: *stop=0x8c668e62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c668e64; /* mov.l r13,@-r15 */
L8c668e64: *stop=0x8c668e64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c668e66; /* mov.l r12,@-r15 */
L8c668e66: *stop=0x8c668e66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c668e68; /* mov.l r11,@-r15 */
L8c668e68: *stop=0x8c668e68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c668e6a; /* mov.l r10,@-r15 */
L8c668e6a: *stop=0x8c668e6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c668e6c; /* mov.l r9,@-r15 */
L8c668e6c: *stop=0x8c668e6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668e6e; /* sts.l PR,@-r15 */
L8c668e6e: *stop=0x8c668e6eu;r[15]+=4294967292u;goto L8c668e70; /* add #-0x4,r15 */
L8c668e70: *stop=0x8c668e70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668ea4u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c668e72; /* mov.l 0x8c668ea4,r9 */
L8c668e72: *stop=0x8c668e72u;r[13]=r[4];goto L8c668e74; /* mov r4,r13 */
L8c668e74: *stop=0x8c668e74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668eb4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668e76; /* mov.l 0x8c668eb4,r2 */
L8c668e76: *stop=0x8c668e76u;r[4]=r[13];goto L8c668e78; /* mov r13,r4 */
L8c668e78: *stop=0x8c668e78u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668e7a; /* mov.l @r9,r3 */
L8c668e7a: *stop=0x8c668e7au;r[12]=0u;goto L8c668e7c; /* mov #0x0,r12 */
L8c668e7c: *stop=0x8c668e7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668eb8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c668e7e; /* mov.l 0x8c668eb8,r1 */
L8c668e7e: *stop=0x8c668e7eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c668e80; /* mov.l r3,@r2 */
L8c668e80: *stop=0x8c668e80u;if(*sr&0x8000u)return -4;*stop=0x8c668e80u;r[3]=f->fpscr;goto L8c668e82; /* sts FPSCR,r3 */
L8c668e82: *stop=0x8c668e82u;if(*sr&0x8000u)return -4;*stop=0x8c668e82u;f->fpscr=r[1];fr=f->bank[(f->fpscr>>21)&1u];goto L8c668e84; /* lds r1,fpscr */
L8c668e84: *stop=0x8c668e84u;r[14]=sx(m,bus,&io_status,0x8c668e9au,2);if(io_status)return io_status;goto L8c668e86; /* mov.w 0x8c668e9a,r14 */
L8c668e86: *stop=0x8c668e86u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[13],4);if(io_status)return io_status;goto L8c668e88; /* mov.l r13,@r9 */
L8c668e88: *stop=0x8c668e88u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c668e8a; /* mov.l r3,@r15 */
L8c668e8a: *stop=0x8c668e8au;r[14]+=r[13];goto L8c668e8c; /* add r13,r14 */
L8c668e8c: *stop=0x8c668e8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668eacu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668e8e; /* mov.l 0x8c668eac,r3 */
L8c668e8e: target=r[3];*pr=0x8c668e92u;*stop=0x8c668e90u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668e92; /* jsr @r3 */
L8c668e92: *stop=0x8c668e92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668eb0u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c668e94; /* mov.l 0x8c668eb0,r10 */
L8c668e94: *stop=0x8c668e96u;r[11]=3u;goto L8c668f02; /* bra 0x8c668f02 */
L8c668ebc: *stop=0x8c668ebcu;r[0]=sx(m,bus,&io_status,0x8c668f2eu,2);if(io_status)return io_status;goto L8c668ebe; /* mov.w 0x8c668f2e,r0 */
L8c668ebe: *stop=0x8c668ebeu;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[14],4);if(io_status)return io_status;goto L8c668ec0; /* mov.l r14,@(r0,r13) */
L8c668ec0: *stop=0x8c668ec0u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c668ec2; /* mov.l @(0xc,r14),r0 */
L8c668ec2: *stop=0x8c668ec2u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c668ec4; /* cmp/eq #0x0,r0 */
L8c668ec4: if((*sr&1u))goto L8c668efc;goto L8c668ec6; /* bt 0x8c668efc */
L8c668ec6: *stop=0x8c668ec6u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c668ec8; /* cmp/eq #0x3,r0 */
L8c668ec8: if((*sr&1u))goto L8c668ed6;goto L8c668eca; /* bt 0x8c668ed6 */
L8c668eca: *stop=0x8c668ecau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c668ecc; /* cmp/eq #0x1,r0 */
L8c668ecc: if((*sr&1u))goto L8c668ee0;goto L8c668ece; /* bt 0x8c668ee0 */
L8c668ece: *stop=0x8c668eceu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c668ed0; /* cmp/eq #0x2,r0 */
L8c668ed0: if((*sr&1u))goto L8c668ef6;goto L8c668ed2; /* bt 0x8c668ef6 */
L8c668ed2: *stop=0x8c668ed4u;goto L8c668efc; /* bra 0x8c668efc */
L8c668ed6: *stop=0x8c668ed6u;r[5]=r[14];goto L8c668ed8; /* mov r14,r5 */
L8c668ed8: target=0x8c668ccau;*pr=0x8c668edcu;*stop=0x8c668edau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668edc; /* bsr 0x8c668cca */
L8c668edc: *stop=0x8c668edeu;goto L8c668efc; /* bra 0x8c668efc */
L8c668ee0: *stop=0x8c668ee0u;r[0]=sx(m,bus,&io_status,0x8c668f30u,2);if(io_status)return io_status;goto L8c668ee2; /* mov.w 0x8c668f30,r0 */
L8c668ee2: *stop=0x8c668ee2u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668ee4; /* mov.l @(0x14,r14),r3 */
L8c668ee4: *stop=0x8c668ee4u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668ee6; /* mov.l @(r0,r13),r2 */
L8c668ee6: *stop=0x8c668ee6u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c668ee8; /* cmp/hi r2,r3 */
L8c668ee8: if((*sr&1u))goto L8c668efc;goto L8c668eea; /* bt 0x8c668efc */
L8c668eea: *stop=0x8c668eeau;r[4]=r[14];goto L8c668eec; /* mov r14,r4 */
L8c668eec: *stop=0x8c668eecu;r[5]=1u;goto L8c668eee; /* mov #0x1,r5 */
L8c668eee: target=r[10];*pr=0x8c668ef2u;*stop=0x8c668ef0u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status==3 && *pr==0x8c668e92u)goto L8c668e92;if(status)return status;goto L8c668ef2; /* jsr @r10 */
L8c668ef2: *stop=0x8c668ef4u;goto L8c668efc; /* bra 0x8c668efc */
L8c668ef6: *stop=0x8c668ef6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668f38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668ef8; /* mov.l 0x8c668f38,r3 */
L8c668ef8: *stop=0x8c668ef8u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[11],4);if(io_status)return io_status;goto L8c668efa; /* mov.l r11,@(0xc,r14) */
L8c668efa: *stop=0x8c668efau;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c668efc; /* mov.l r3,@(0x20,r14) */
L8c668efc: *stop=0x8c668efcu;r[3]=sx(m,bus,&io_status,0x8c668f32u,2);if(io_status)return io_status;goto L8c668efe; /* mov.w 0x8c668f32,r3 */
L8c668efe: *stop=0x8c668efeu;r[12]+=1u;goto L8c668f00; /* add #0x1,r12 */
L8c668f00: *stop=0x8c668f00u;r[14]+=r[3];goto L8c668f02; /* add r3,r14 */
L8c668f02: *stop=0x8c668f02u;r[0]=sx(m,bus,&io_status,0x8c668f34u,2);if(io_status)return io_status;goto L8c668f04; /* mov.w 0x8c668f34,r0 */
L8c668f04: *stop=0x8c668f04u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f06; /* mov.l @(r0,r13),r2 */
L8c668f06: *stop=0x8c668f06u;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]);goto L8c668f08; /* cmp/ge r2,r12 */
L8c668f08: if(!(*sr&1u))goto L8c668ebc;goto L8c668f0a; /* bf 0x8c668ebc */
L8c668f0a: *stop=0x8c668f0au;r[0]=sx(m,bus,&io_status,0x8c668f30u,2);if(io_status)return io_status;goto L8c668f0c; /* mov.w 0x8c668f30,r0 */
L8c668f0c: *stop=0x8c668f0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668f3cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c668f0e; /* mov.l 0x8c668f3c,r1 */
L8c668f0e: *stop=0x8c668f0eu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f10; /* mov.l @(r0,r13),r2 */
L8c668f10: *stop=0x8c668f10u;r[2]+=1u;goto L8c668f12; /* add #0x1,r2 */
L8c668f12: *stop=0x8c668f12u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c668f14; /* mov.l r2,@(r0,r13) */
L8c668f14: *stop=0x8c668f14u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668f16; /* mov.l @r1,r3 */
L8c668f16: *stop=0x8c668f16u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;goto L8c668f18; /* mov.l r3,@r9 */
L8c668f18: *stop=0x8c668f18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f1a; /* mov.l @r15,r2 */
L8c668f1a: *stop=0x8c668f1au;if(*sr&0x8000u)return -4;*stop=0x8c668f1au;f->fpscr=r[2];fr=f->bank[(f->fpscr>>21)&1u];goto L8c668f1c; /* lds r2,fpscr */
L8c668f1c: *stop=0x8c668f1cu;r[15]+=4u;goto L8c668f1e; /* add #0x4,r15 */
L8c668f1e: *stop=0x8c668f1eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668f20; /* lds.l @r15+,PR */
L8c668f20: *stop=0x8c668f20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c668f22; /* mov.l @r15+,r9 */
L8c668f22: *stop=0x8c668f22u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c668f24; /* mov.l @r15+,r10 */
L8c668f24: *stop=0x8c668f24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c668f26; /* mov.l @r15+,r11 */
L8c668f26: *stop=0x8c668f26u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c668f28; /* mov.l @r15+,r12 */
L8c668f28: *stop=0x8c668f28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c668f2a; /* mov.l @r15+,r13 */
L8c668f2a: *stop=0x8c668f2au;if(*pr!=entry_pr)return -3;*stop=0x8c668f2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_active_input_8c668cca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668cca;
L8c668cca: *stop=0x8c668ccau;r[0]=sx(m,bus,&io_status,0x8c668d70u,2);if(io_status)return io_status;goto L8c668ccc; /* mov.w 0x8c668d70,r0 */
L8c668ccc: *stop=0x8c668cccu;r[2]=0u;goto L8c668cce; /* mov #0x0,r2 */
L8c668cce: *stop=0x8c668cceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668cd0; /* mov.l r14,@-r15 */
L8c668cd0: *stop=0x8c668cd0u;r[14]=r[5];goto L8c668cd2; /* mov r5,r14 */
L8c668cd2: *stop=0x8c668cd2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668cd4; /* mov.l @(r0,r4),r3 */
L8c668cd4: *stop=0x8c668cd4u;r[3]+=4294967295u;goto L8c668cd6; /* add #-0x1,r3 */
L8c668cd6: *stop=0x8c668cd6u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c668cd8; /* mov.l r3,@(r0,r4) */
L8c668cd8: *stop=0x8c668cd8u;r[0]=sx(m,bus,&io_status,0x8c668d6au,2);if(io_status)return io_status;goto L8c668cda; /* mov.w 0x8c668d6a,r0 */
L8c668cda: *stop=0x8c668cdau;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c668cdc; /* mov.l r2,@(0xc,r14) */
L8c668cdc: *stop=0x8c668cdcu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668cde; /* mov.l @(r0,r14),r3 */
L8c668cde: *stop=0x8c668cdeu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c668ce0; /* tst r3,r3 */
L8c668ce0: if((*sr&1u))goto L8c668ce8;goto L8c668ce2; /* bt 0x8c668ce8 */
L8c668ce2: *stop=0x8c668ce2u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668ce4; /* mov.l @(r0,r14),r3 */
L8c668ce4: target=r[3];*stop=0x8c668ce6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c668ce8: *stop=0x8c668ce8u;if(*pr!=entry_pr)return -3;*stop=0x8c668ceau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_active_input_8c1108e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];(void)fr;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c1108e0;
L8c1108e0: *stop=0x8c1108e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c110950u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c1108e2; /* mov.l 0x8c110950,r0 */
L8c1108e2: *stop=0x8c1108e2u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c1108e4; /* mov.l r5,@r0 */
L8c1108e4: *stop=0x8c1108e4u;r[4]+=80u;goto L8c1108e6; /* add #0x50,r4 */
L8c1108e6: *stop=0x8c1108e6u;if(*sr&0x8000u)return -4;*stop=0x8c1108e6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108e8; /* fmov.s @r4+,fr0 */
L8c1108e8: *stop=0x8c1108e8u;if(*sr&0x8000u)return -4;*stop=0x8c1108e8u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108ea; /* fmov.s @r4+,fr1 */
L8c1108ea: *stop=0x8c1108eau;if(*sr&0x8000u)return -4;*stop=0x8c1108eau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108ec; /* fmov.s @r4+,fr2 */
L8c1108ec: *stop=0x8c1108ecu;if(*sr&0x8000u)return -4;*stop=0x8c1108ecu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108ee; /* fmov.s @r4+,fr3 */
L8c1108ee: *stop=0x8c1108eeu;if(*sr&0x8000u)return -4;*stop=0x8c1108eeu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108f0; /* fmov.s @r4+,fr4 */
L8c1108f0: *stop=0x8c1108f0u;if(*sr&0x8000u)return -4;*stop=0x8c1108f0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108f2; /* fmov.s @r4+,fr5 */
L8c1108f2: *stop=0x8c1108f2u;if(*sr&0x8000u)return -4;*stop=0x8c1108f2u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108f4; /* fmov.s @r4+,fr6 */
L8c1108f4: *stop=0x8c1108f4u;if(*sr&0x8000u)return -4;*stop=0x8c1108f4u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108f6; /* fmov.s @r4+,fr7 */
L8c1108f6: *stop=0x8c1108f6u;if(*sr&0x8000u)return -4;*stop=0x8c1108f6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108f8; /* fmov.s @r4+,fr8 */
L8c1108f8: *stop=0x8c1108f8u;if(*sr&0x8000u)return -4;*stop=0x8c1108f8u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108fa; /* fmov.s @r4+,fr9 */
L8c1108fa: *stop=0x8c1108fau;if(*sr&0x8000u)return -4;*stop=0x8c1108fau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108fc; /* fmov.s @r4+,fr10 */
L8c1108fc: *stop=0x8c1108fcu;if(*sr&0x8000u)return -4;*stop=0x8c1108fcu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c1108fe; /* fmov.s @r4+,fr11 */
L8c1108fe: *stop=0x8c1108feu;if(*sr&0x8000u)return -4;*stop=0x8c1108feu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c110900; /* fmov.s @r4+,fr12 */
L8c110900: *stop=0x8c110900u;if(*sr&0x8000u)return -4;*stop=0x8c110900u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c110902; /* fmov.s @r4+,fr13 */
L8c110902: *stop=0x8c110902u;if(*sr&0x8000u)return -4;*stop=0x8c110902u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c110904; /* fmov.s @r4+,fr14 */
L8c110904: *stop=0x8c110904u;if(*sr&0x8000u)return -4;*stop=0x8c110904u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c110906; /* fmov.s @r4,fr15 */
L8c110906: *stop=0x8c110906u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c110908; /* mov.l @(0x4,r4),r0 */
L8c110908: *stop=0x8c110908u;if(*sr&0x8000u)return -4;*stop=0x8c110908u;*fpul=r[0];goto L8c11090a; /* lds r0,FPUL */
L8c11090a: *stop=0x8c11090au;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c11090c; /* mov.l @(0x8,r4),r0 */
L8c11090c: *stop=0x8c11090cu;if(*sr&0x8000u)return -4;*stop=0x8c11090cu;f->fpscr=r[0];fr=f->bank[(f->fpscr>>21)&1u];goto L8c11090e; /* lds r0,fpscr */
L8c11090e: *stop=0x8c11090eu;r[4]+=4294967208u;goto L8c110910; /* add #-0x58,r4 */
L8c110910: *stop=0x8c110910u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c110912; /* mov.l @(0x18,r4),r0 */
L8c110912: *stop=0x8c110912u;*gbr=r[0];goto L8c110914; /* ldc r0,GBR */
L8c110914: *stop=0x8c110914u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c110916; /* mov.l @(0x14,r4),r0 */
L8c110916: *stop=0x8c110916u;*macl=r[0];goto L8c110918; /* lds r0,MACL */
L8c110918: *stop=0x8c110918u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c11091a; /* mov.l @(0x10,r4),r0 */
L8c11091a: *stop=0x8c11091au;integer->mach=r[0];goto L8c11091c; /* lds r0,MACH */
L8c11091c: *stop=0x8c11091cu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c11091e; /* mov.l @(0x8,r4),r1 */
L8c11091e: *stop=0x8c11091eu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c110920; /* mov.l @(0x4,r4),r2 */
L8c110920: *stop=0x8c110920u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c110922; /* mov.l @r4,r3 */
L8c110922: *stop=0x8c110922u;r[4]+=4294967244u;goto L8c110924; /* add #-0x34,r4 */
L8c110924: *stop=0x8c110924u;if(!valid_access(r[4]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x2cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c110926; /* mov.l @(0x2c,r4),r5 */
L8c110926: *stop=0x8c110926u;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c110928; /* mov.l @(0x28,r4),r6 */
L8c110928: *stop=0x8c110928u;if(!valid_access(r[4]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x24u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c11092a; /* mov.l @(0x24,r4),r7 */
L8c11092a: *stop=0x8c11092au;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c11092c; /* mov.l @(0x20,r4),r8 */
L8c11092c: *stop=0x8c11092cu;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c11092e; /* mov.l @(0x1c,r4),r9 */
L8c11092e: *stop=0x8c11092eu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c110930; /* mov.l @(0x18,r4),r10 */
L8c110930: *stop=0x8c110930u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c110932; /* mov.l @(0x14,r4),r11 */
L8c110932: *stop=0x8c110932u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c110934; /* mov.l @(0x10,r4),r12 */
L8c110934: *stop=0x8c110934u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c110936; /* mov.l @(0xc,r4),r13 */
L8c110936: *stop=0x8c110936u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c110938; /* mov.l @(0x4,r4),r14 */
L8c110938: *stop=0x8c110938u;r[0]=r[4];goto L8c11093a; /* mov r4,r0 */
L8c11093a: *stop=0x8c11093au;if(!valid_access(r[0]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x30u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c11093c; /* mov.l @(0x30,r0),r4 */
L8c11093c: *stop=0x8c11093cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[15]=loaded;}goto L8c11093e; /* mov.l @r0,r15 */
L8c11093e: *stop=0x8c11093eu;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c110940; /* mov.l @(0x8,r0),r0 */
L8c110940: *stop=0x8c110940u;*pr=r[0];goto L8c110942; /* lds r0,PR */
L8c110942: *stop=0x8c110942u;{uint32_t loaded=rd(m,bus,&io_status,0x8c110950u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c110944; /* mov.l 0x8c110950,r0 */
L8c110944: *stop=0x8c110944u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c110946; /* mov.l @r0,r0 */
L8c110946: *stop=0x8c110946u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c110948; /* cmp/eq #0x0,r0 */
L8c110948: if(!(*sr&1u))goto L8c11094c;goto L8c11094a; /* bf 0x8c11094c */
L8c11094a: *stop=0x8c11094au;r[0]=1u;goto L8c11094c; /* mov #0x1,r0 */
L8c11094c: *stop=0x8c11094cu;*stop=0x8c11094eu;if(*pr==entry_pr)return 0;if(*pr==0x8c668e92u)return 3;if(!call)return -2;return call(ctx,*pr,m,f,sr,r,pr); /* rts */
}
