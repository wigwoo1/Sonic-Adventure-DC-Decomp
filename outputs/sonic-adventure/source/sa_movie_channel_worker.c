/* Original channel selectors, command worker and yield thunk; semantics inferred.
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
SA_STAGE_EXPORT int sa_movie_channel_worker_8c64790e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64790e;
L8c64790e: *stop=0x8c647910u;r[4]=0u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c647912(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647912;
L8c647912: *stop=0x8c647914u;r[4]=1u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c647916(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647916;
L8c647916: *stop=0x8c647918u;r[4]=2u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c64791a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64791a;
L8c64791a: *stop=0x8c64791cu;r[4]=3u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c64791e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64791e;
L8c64791e: *stop=0x8c647920u;r[4]=4u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c647922(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647922;
L8c647922: *stop=0x8c647924u;r[4]=5u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c647926(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647926;
L8c647926: *stop=0x8c647928u;r[4]=6u;if(!call)return -2;return call(ctx,0x8c647982u,m,f,sr,r,pr); /* bra 0x8c647982 */
}

static int channel_worker_run(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c647982u)goto L8c647982;if(resume_pc==0x8c647b74u)goto L8c647b74;goto L8c647980;
L8c647980: *stop=0x8c647980u;r[4]=7u;goto L8c647982; /* mov #0x7,r4 */
L8c647982: *stop=0x8c647982u;r[14]=r[4];goto L8c647984; /* mov r4,r14 */
L8c647984: *stop=0x8c647984u;r[14]<<=2;goto L8c647986; /* shll2 r14 */
L8c647986: *stop=0x8c647986u;r[14]<<=2;goto L8c647988; /* shll2 r14 */
L8c647988: *stop=0x8c647988u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6479bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64798a; /* mov.l 0x8c6479bc,r3 */
L8c64798a: *stop=0x8c64798au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6479c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64798c; /* mov.l 0x8c6479c4,r2 */
L8c64798c: *stop=0x8c64798cu;r[9]=r[4];goto L8c64798e; /* mov r4,r9 */
L8c64798e: *stop=0x8c64798eu;r[14]<<=2;goto L8c647990; /* shll2 r14 */
L8c647990: *stop=0x8c647990u;r[10]=sx(m,bus,&io_status,0x8c6479b8u,2);if(io_status)return io_status;goto L8c647992; /* mov.w 0x8c6479b8,r10 */
L8c647992: *stop=0x8c647992u;r[9]<<=2;goto L8c647994; /* shll2 r9 */
L8c647994: *stop=0x8c647994u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6479c0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c647996; /* mov.l 0x8c6479c0,r8 */
L8c647996: *stop=0x8c647996u;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c647998; /* shll r14 */
L8c647998: *stop=0x8c647998u;r[11]=r[4];goto L8c64799a; /* mov r4,r11 */
L8c64799a: *stop=0x8c64799au;r[9]+=r[2];goto L8c64799c; /* add r2,r9 */
L8c64799c: *stop=0x8c64799cu;r[14]+=r[3];goto L8c64799e; /* add r3,r14 */
L8c64799e: *stop=0x8c64799eu;r[13]=0u;goto L8c6479a0; /* mov #0x0,r13 */
L8c6479a0: *stop=0x8c6479a0u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6479a2; /* mov.l @(0x8,r14),r0 */
L8c6479a2: *stop=0x8c6479a2u;r[1]=21u;goto L8c6479a4; /* mov #0x15,r1 */
L8c6479a4: *stop=0x8c6479a4u;*sr=(*sr&~1u)|(r[0]>=r[1]);goto L8c6479a6; /* cmp/hs r1,r0 */
L8c6479a6: if(!(*sr&1u))goto L8c6479ac;goto L8c6479a8; /* bf 0x8c6479ac */
L8c6479a8: *stop=0x8c6479aau;goto L8c647b6e; /* bra 0x8c647b6e */
L8c6479ac: *stop=0x8c6479acu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c6479ae; /* shll r0 */
L8c6479ae: *stop=0x8c6479aeu;r[1]=r[0];goto L8c6479b0; /* mov r0,r1 */
L8c6479b0: *stop=0x8c6479b0u;r[0]=0x8c6479c8u;goto L8c6479b2; /* mova 0x8c6479c8,r0 */
L8c6479b2: *stop=0x8c6479b2u;if(!valid_access(r[0]+r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[1],2);if(io_status)return io_status;goto L8c6479b4; /* mov.w @(r0,r1),r0 */
L8c6479b4: target=0x8c6479b8u+r[0];*stop=0x8c6479b6u;switch(target){case 0x8c6479f2u:goto L8c6479f2;case 0x8c6479feu:goto L8c6479fe;case 0x8c647a0cu:goto L8c647a0c;case 0x8c647a28u:goto L8c647a28;case 0x8c647a3au:goto L8c647a3a;case 0x8c647a4eu:goto L8c647a4e;case 0x8c647a62u:goto L8c647a62;case 0x8c647a74u:goto L8c647a74;case 0x8c647a8au:goto L8c647a8a;case 0x8c647aa0u:goto L8c647aa0;case 0x8c647ab0u:goto L8c647ab0;case 0x8c647ac0u:goto L8c647ac0;case 0x8c647ad0u:goto L8c647ad0;case 0x8c647ae6u:goto L8c647ae6;case 0x8c647b06u:goto L8c647b06;case 0x8c647b26u:goto L8c647b26;case 0x8c647b40u:goto L8c647b40;case 0x8c647b52u:goto L8c647b52;case 0x8c647b6eu:goto L8c647b6e;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* braf r0 */
L8c6479f2: *stop=0x8c6479f2u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6479f4; /* mov.l @(0x14,r14),r3 */
L8c6479f4: *stop=0x8c6479f4u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6479f6; /* mov.l r3,@(0x10,r14) */
L8c6479f6: target=r[8];*pr=0x8c6479fau;*stop=0x8c6479f8u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6479fa; /* jsr @r8 */
L8c6479fa: *stop=0x8c6479fcu;goto L8c647b6a; /* bra 0x8c647b6a */
L8c6479fe: *stop=0x8c6479feu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a00; /* mov.l @(0x14,r14),r3 */
L8c647a00: *stop=0x8c647a00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a02; /* mov.l 0x8c647c00,r2 */
L8c647a02: *stop=0x8c647a02u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647a04; /* mov.l r3,@(0x10,r14) */
L8c647a04: target=r[2];*pr=0x8c647a08u;*stop=0x8c647a06u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a08; /* jsr @r2 */
L8c647a08: *stop=0x8c647a0au;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a0c: *stop=0x8c647a0cu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a0e; /* mov.l @(0x14,r14),r2 */
L8c647a0e: *stop=0x8c647a0eu;r[5]=r[14];goto L8c647a10; /* mov r14,r5 */
L8c647a10: *stop=0x8c647a10u;r[7]=r[14];goto L8c647a12; /* mov r14,r7 */
L8c647a12: *stop=0x8c647a12u;r[5]+=76u;goto L8c647a14; /* add #0x4c,r5 */
L8c647a14: *stop=0x8c647a14u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647a16; /* mov.l r2,@(0x10,r14) */
L8c647a16: *stop=0x8c647a16u;r[7]+=108u;goto L8c647a18; /* add #0x6c,r7 */
L8c647a18: *stop=0x8c647a18u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a1a; /* mov.l @(0x24,r14),r3 */
L8c647a1a: *stop=0x8c647a1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a1c; /* mov.l 0x8c647c04,r2 */
L8c647a1c: *stop=0x8c647a1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c647a1e; /* mov.l r3,@-r15 */
L8c647a1e: *stop=0x8c647a1eu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647a20; /* mov.l @(0x1c,r14),r6 */
L8c647a20: target=r[2];*pr=0x8c647a24u;*stop=0x8c647a22u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a24; /* jsr @r2 */
L8c647a24: *stop=0x8c647a26u;r[15]+=4u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a28: *stop=0x8c647a28u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a2a; /* mov.l @(0x14,r14),r2 */
L8c647a2a: *stop=0x8c647a2au;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a2c; /* mov.l @(0x28,r2),r3 */
L8c647a2c: *stop=0x8c647a2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c08u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a2e; /* mov.l 0x8c647c08,r2 */
L8c647a2e: *stop=0x8c647a2eu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647a30; /* mov.l r3,@(0x10,r14) */
L8c647a30: *stop=0x8c647a30u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647a32; /* mov.l @(0x18,r14),r5 */
L8c647a32: target=r[2];*pr=0x8c647a36u;*stop=0x8c647a34u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a36; /* jsr @r2 */
L8c647a36: *stop=0x8c647a38u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a3a: *stop=0x8c647a3au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a3c; /* mov.l @(0x14,r14),r2 */
L8c647a3c: *stop=0x8c647a3cu;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a3e; /* mov.l @(0x28,r2),r3 */
L8c647a3e: *stop=0x8c647a3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c0cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a40; /* mov.l 0x8c647c0c,r2 */
L8c647a40: *stop=0x8c647a40u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647a42; /* mov.l r3,@(0x10,r14) */
L8c647a42: *stop=0x8c647a42u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647a44; /* mov.l @(0x18,r14),r5 */
L8c647a44: *stop=0x8c647a44u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647a46; /* mov.l @(0x1c,r14),r6 */
L8c647a46: target=r[2];*pr=0x8c647a4au;*stop=0x8c647a48u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a4a; /* jsr @r2 */
L8c647a4a: *stop=0x8c647a4cu;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a4e: *stop=0x8c647a4eu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a50; /* mov.l @(0x14,r14),r2 */
L8c647a50: *stop=0x8c647a50u;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a52; /* mov.l @(0x28,r2),r3 */
L8c647a52: *stop=0x8c647a52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a54; /* mov.l 0x8c647c10,r2 */
L8c647a54: *stop=0x8c647a54u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647a56; /* mov.l r3,@(0x10,r14) */
L8c647a56: *stop=0x8c647a56u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647a58; /* mov.l @(0x18,r14),r5 */
L8c647a58: *stop=0x8c647a58u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647a5a; /* mov.l @(0x1c,r14),r6 */
L8c647a5a: target=r[2];*pr=0x8c647a5eu;*stop=0x8c647a5cu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a5e; /* jsr @r2 */
L8c647a5e: *stop=0x8c647a60u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a62: *stop=0x8c647a62u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a64; /* mov.l @(0x14,r14),r2 */
L8c647a64: *stop=0x8c647a64u;r[5]=r[14];goto L8c647a66; /* mov r14,r5 */
L8c647a66: *stop=0x8c647a66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a68; /* mov.l 0x8c647c14,r3 */
L8c647a68: *stop=0x8c647a68u;r[5]+=44u;goto L8c647a6a; /* add #0x2c,r5 */
L8c647a6a: *stop=0x8c647a6au;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647a6c; /* mov.l r2,@(0x10,r14) */
L8c647a6c: target=r[3];*pr=0x8c647a70u;*stop=0x8c647a6eu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a70; /* jsr @r3 */
L8c647a70: *stop=0x8c647a72u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a74: *stop=0x8c647a74u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a76; /* mov.l @(0x14,r14),r3 */
L8c647a76: *stop=0x8c647a76u;r[5]=r[14];goto L8c647a78; /* mov r14,r5 */
L8c647a78: *stop=0x8c647a78u;r[6]=r[14];goto L8c647a7a; /* mov r14,r6 */
L8c647a7a: *stop=0x8c647a7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a7c; /* mov.l 0x8c647c18,r2 */
L8c647a7c: *stop=0x8c647a7cu;r[5]+=44u;goto L8c647a7e; /* add #0x2c,r5 */
L8c647a7e: *stop=0x8c647a7eu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647a80; /* mov.l r3,@(0x10,r14) */
L8c647a80: *stop=0x8c647a80u;r[6]+=60u;goto L8c647a82; /* add #0x3c,r6 */
L8c647a82: target=r[2];*pr=0x8c647a86u;*stop=0x8c647a84u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a86; /* jsr @r2 */
L8c647a86: *stop=0x8c647a88u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647a8a: *stop=0x8c647a8au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647a8c; /* mov.l @(0x14,r14),r2 */
L8c647a8c: *stop=0x8c647a8cu;r[5]=r[14];goto L8c647a8e; /* mov r14,r5 */
L8c647a8e: *stop=0x8c647a8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647a90; /* mov.l 0x8c647c1c,r3 */
L8c647a90: *stop=0x8c647a90u;r[5]+=44u;goto L8c647a92; /* add #0x2c,r5 */
L8c647a92: *stop=0x8c647a92u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647a94; /* mov.l r2,@(0x10,r14) */
L8c647a94: *stop=0x8c647a94u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647a96; /* mov.l @(0x1c,r14),r6 */
L8c647a96: *stop=0x8c647a96u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647a98; /* mov.l @(0x20,r14),r7 */
L8c647a98: target=r[3];*pr=0x8c647a9cu;*stop=0x8c647a9au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647a9c; /* jsr @r3 */
L8c647a9c: *stop=0x8c647a9eu;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647aa0: *stop=0x8c647aa0u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647aa2; /* mov.l @(0x14,r14),r3 */
L8c647aa2: *stop=0x8c647aa2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647aa4; /* mov.l 0x8c647c20,r2 */
L8c647aa4: *stop=0x8c647aa4u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647aa6; /* mov.l r3,@(0x10,r14) */
L8c647aa6: *stop=0x8c647aa6u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647aa8; /* mov.l @(0x18,r14),r5 */
L8c647aa8: target=r[2];*pr=0x8c647aacu;*stop=0x8c647aaau;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647aac; /* jsr @r2 */
L8c647aac: *stop=0x8c647aaeu;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647ab0: *stop=0x8c647ab0u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ab2; /* mov.l @(0x14,r14),r2 */
L8c647ab2: *stop=0x8c647ab2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ab4; /* mov.l 0x8c647c24,r3 */
L8c647ab4: *stop=0x8c647ab4u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647ab6; /* mov.l r2,@(0x10,r14) */
L8c647ab6: *stop=0x8c647ab6u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647ab8; /* mov.l @(0x18,r14),r5 */
L8c647ab8: target=r[3];*pr=0x8c647abcu;*stop=0x8c647abau;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647abc; /* jsr @r3 */
L8c647abc: *stop=0x8c647abeu;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647ac0: *stop=0x8c647ac0u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ac2; /* mov.l @(0x14,r14),r3 */
L8c647ac2: *stop=0x8c647ac2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ac4; /* mov.l 0x8c647c28,r2 */
L8c647ac4: *stop=0x8c647ac4u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647ac6; /* mov.l r3,@(0x10,r14) */
L8c647ac6: *stop=0x8c647ac6u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647ac8; /* mov.l @(0x18,r14),r5 */
L8c647ac8: target=r[2];*pr=0x8c647accu;*stop=0x8c647acau;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647acc; /* jsr @r2 */
L8c647acc: *stop=0x8c647aceu;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647ad0: *stop=0x8c647ad0u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ad2; /* mov.l @(0x14,r14),r2 */
L8c647ad2: *stop=0x8c647ad2u;r[5]=r[14];goto L8c647ad4; /* mov r14,r5 */
L8c647ad4: *stop=0x8c647ad4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ad6; /* mov.l 0x8c647c2c,r3 */
L8c647ad6: *stop=0x8c647ad6u;r[5]+=44u;goto L8c647ad8; /* add #0x2c,r5 */
L8c647ad8: *stop=0x8c647ad8u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647ada; /* mov.l r2,@(0x10,r14) */
L8c647ada: *stop=0x8c647adau;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647adc; /* mov.l @(0x1c,r14),r6 */
L8c647adc: *stop=0x8c647adcu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647ade; /* mov.l @(0x20,r14),r7 */
L8c647ade: target=r[3];*pr=0x8c647ae2u;*stop=0x8c647ae0u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647ae2; /* jsr @r3 */
L8c647ae2: *stop=0x8c647ae4u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647ae6: *stop=0x8c647ae6u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ae8; /* mov.l @(0x14,r14),r3 */
L8c647ae8: *stop=0x8c647ae8u;r[5]=r[14];goto L8c647aea; /* mov r14,r5 */
L8c647aea: *stop=0x8c647aeau;r[5]+=44u;goto L8c647aec; /* add #0x2c,r5 */
L8c647aec: *stop=0x8c647aecu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647aee; /* mov.l r3,@(0x10,r14) */
L8c647aee: *stop=0x8c647aeeu;r[3]=r[14];goto L8c647af0; /* mov r14,r3 */
L8c647af0: *stop=0x8c647af0u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647af2; /* mov.l @(0x28,r14),r2 */
L8c647af2: *stop=0x8c647af2u;r[3]+=108u;goto L8c647af4; /* add #0x6c,r3 */
L8c647af4: *stop=0x8c647af4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c647af6; /* mov.l r2,@-r15 */
L8c647af6: *stop=0x8c647af6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647af8; /* mov.l 0x8c647c30,r2 */
L8c647af8: *stop=0x8c647af8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c647afa; /* mov.l r3,@-r15 */
L8c647afa: *stop=0x8c647afau;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647afc; /* mov.l @(0x1c,r14),r6 */
L8c647afc: *stop=0x8c647afcu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647afe; /* mov.l @(0x20,r14),r7 */
L8c647afe: target=r[2];*pr=0x8c647b02u;*stop=0x8c647b00u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b02; /* jsr @r2 */
L8c647b02: *stop=0x8c647b04u;r[15]+=8u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647b06: *stop=0x8c647b06u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647b08; /* mov.l @(0x14,r14),r2 */
L8c647b08: *stop=0x8c647b08u;r[5]=r[14];goto L8c647b0a; /* mov r14,r5 */
L8c647b0a: *stop=0x8c647b0au;r[5]+=44u;goto L8c647b0c; /* add #0x2c,r5 */
L8c647b0c: *stop=0x8c647b0cu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647b0e; /* mov.l r2,@(0x10,r14) */
L8c647b0e: *stop=0x8c647b0eu;r[2]=r[14];goto L8c647b10; /* mov r14,r2 */
L8c647b10: *stop=0x8c647b10u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b12; /* mov.l @(0x28,r14),r3 */
L8c647b12: *stop=0x8c647b12u;r[2]+=108u;goto L8c647b14; /* add #0x6c,r2 */
L8c647b14: *stop=0x8c647b14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c647b16; /* mov.l r3,@-r15 */
L8c647b16: *stop=0x8c647b16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b18; /* mov.l 0x8c647c34,r3 */
L8c647b18: *stop=0x8c647b18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c647b1a; /* mov.l r2,@-r15 */
L8c647b1a: *stop=0x8c647b1au;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647b1c; /* mov.l @(0x1c,r14),r6 */
L8c647b1c: *stop=0x8c647b1cu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647b1e; /* mov.l @(0x20,r14),r7 */
L8c647b1e: target=r[3];*pr=0x8c647b22u;*stop=0x8c647b20u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b22; /* jsr @r3 */
L8c647b22: *stop=0x8c647b24u;r[15]+=8u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647b26: *stop=0x8c647b26u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b28; /* mov.l @(0x14,r14),r3 */
L8c647b28: *stop=0x8c647b28u;r[5]=r[14];goto L8c647b2a; /* mov r14,r5 */
L8c647b2a: *stop=0x8c647b2au;r[5]+=44u;goto L8c647b2c; /* add #0x2c,r5 */
L8c647b2c: *stop=0x8c647b2cu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647b2e; /* mov.l r3,@(0x10,r14) */
L8c647b2e: *stop=0x8c647b2eu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647b30; /* mov.l @(0x24,r14),r2 */
L8c647b30: *stop=0x8c647b30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b32; /* mov.l 0x8c647c38,r3 */
L8c647b32: *stop=0x8c647b32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c647b34; /* mov.l r2,@-r15 */
L8c647b34: *stop=0x8c647b34u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647b36; /* mov.l @(0x1c,r14),r6 */
L8c647b36: *stop=0x8c647b36u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647b38; /* mov.l @(0x20,r14),r7 */
L8c647b38: target=r[3];*pr=0x8c647b3cu;*stop=0x8c647b3au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b3c; /* jsr @r3 */
L8c647b3c: *stop=0x8c647b3eu;r[15]+=4u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647b40: *stop=0x8c647b40u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b42; /* mov.l @(0x14,r14),r3 */
L8c647b42: *stop=0x8c647b42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647b44; /* mov.l 0x8c647c3c,r2 */
L8c647b44: *stop=0x8c647b44u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c647b46; /* mov.l r3,@(0x10,r14) */
L8c647b46: *stop=0x8c647b46u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647b48; /* mov.l @(0x18,r14),r5 */
L8c647b48: *stop=0x8c647b48u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647b4a; /* mov.l @(0x1c,r14),r6 */
L8c647b4a: target=r[2];*pr=0x8c647b4eu;*stop=0x8c647b4cu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b4e; /* jsr @r2 */
L8c647b4e: *stop=0x8c647b50u;goto L8c647b6a; /* bra 0x8c647b6a */
L8c647b52: *stop=0x8c647b52u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647b54; /* mov.l @(0x14,r14),r2 */
L8c647b54: *stop=0x8c647b54u;r[5]=r[14];goto L8c647b56; /* mov r14,r5 */
L8c647b56: *stop=0x8c647b56u;r[5]+=44u;goto L8c647b58; /* add #0x2c,r5 */
L8c647b58: *stop=0x8c647b58u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647b5a; /* mov.l r2,@(0x10,r14) */
L8c647b5a: *stop=0x8c647b5au;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b5c; /* mov.l @(0x24,r14),r3 */
L8c647b5c: *stop=0x8c647b5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647b5e; /* mov.l 0x8c647c40,r2 */
L8c647b5e: *stop=0x8c647b5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c647b60; /* mov.l r3,@-r15 */
L8c647b60: *stop=0x8c647b60u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647b62; /* mov.l @(0x1c,r14),r6 */
L8c647b62: *stop=0x8c647b62u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647b64; /* mov.l @(0x20,r14),r7 */
L8c647b64: target=r[2];*pr=0x8c647b68u;*stop=0x8c647b66u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b68; /* jsr @r2 */
L8c647b68: *stop=0x8c647b68u;r[15]+=4u;goto L8c647b6a; /* add #0x4,r15 */
L8c647b6a: *stop=0x8c647b6cu;r[12]=r[0];goto L8c647b78; /* bra 0x8c647b78 */
L8c647b6e: *stop=0x8c647b6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647b70; /* mov.l 0x8c647c44,r2 */
L8c647b70: target=r[2];*pr=0x8c647b74u;*stop=0x8c647b72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b74; /* jsr @r2 */
L8c647b74: *stop=0x8c647b76u;goto L8c6479a0; /* bra 0x8c6479a0 */
L8c647b78: *stop=0x8c647b78u;*sr=(*sr&~1u)|(r[10]==r[12]);goto L8c647b7a; /* cmp/eq r10,r12 */
L8c647b7a: if(!(*sr&1u))goto L8c647b82;goto L8c647b7c; /* bf 0x8c647b82 */
L8c647b7c: *stop=0x8c647b7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647b7e; /* mov.l 0x8c647c48,r3 */
L8c647b7e: target=r[3];*pr=0x8c647b82u;*stop=0x8c647b80u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647b82; /* jsr @r3 */
L8c647b82: *stop=0x8c647b82u;r[0]=124u;goto L8c647b84; /* mov #0x7c,r0 */
L8c647b84: *stop=0x8c647b84u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[12],4);if(io_status)return io_status;goto L8c647b86; /* mov.l r12,@r9 */
L8c647b86: *stop=0x8c647b86u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c647b88; /* mov.l @(r0,r14),r7 */
L8c647b88: *stop=0x8c647b88u;r[0]=116u;goto L8c647b8a; /* mov #0x74,r0 */
L8c647b8a: *stop=0x8c647b8au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647b8c; /* mov.l @(0x8,r14),r5 */
L8c647b8c: *stop=0x8c647b8cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[13],4);if(io_status)return io_status;goto L8c647b8e; /* mov.l r13,@r14 */
L8c647b8e: *stop=0x8c647b8eu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[13],4);if(io_status)return io_status;goto L8c647b90; /* mov.l r13,@(0x4,r14) */
L8c647b90: *stop=0x8c647b90u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[13],4);if(io_status)return io_status;goto L8c647b92; /* mov.l r13,@(0x8,r14) */
L8c647b92: *stop=0x8c647b92u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[13],4);if(io_status)return io_status;goto L8c647b94; /* mov.l r13,@(0xc,r14) */
L8c647b94: *stop=0x8c647b94u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[13],4);if(io_status)return io_status;goto L8c647b96; /* mov.l r13,@(0x10,r14) */
L8c647b96: *stop=0x8c647b96u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;goto L8c647b98; /* mov.l r13,@(0x14,r14) */
L8c647b98: *stop=0x8c647b98u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[13],4);if(io_status)return io_status;goto L8c647b9a; /* mov.l r13,@(0x18,r14) */
L8c647b9a: *stop=0x8c647b9au;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c647b9c; /* mov.l r13,@(0x1c,r14) */
L8c647b9c: *stop=0x8c647b9cu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[13],4);if(io_status)return io_status;goto L8c647b9e; /* mov.l r13,@(0x20,r14) */
L8c647b9e: *stop=0x8c647b9eu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[13],4);if(io_status)return io_status;goto L8c647ba0; /* mov.l r13,@(0x24,r14) */
L8c647ba0: *stop=0x8c647ba0u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[13],4);if(io_status)return io_status;goto L8c647ba2; /* mov.l r13,@(0x28,r14) */
L8c647ba2: *stop=0x8c647ba2u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ba4; /* mov.l @(r0,r14),r3 */
L8c647ba4: *stop=0x8c647ba4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c647ba6; /* tst r3,r3 */
L8c647ba6: if(!(*sr&1u))goto L8c647bac;goto L8c647ba8; /* bf 0x8c647bac */
L8c647ba8: *stop=0x8c647baau;goto L8c6479a0; /* bra 0x8c6479a0 */
L8c647bac: *stop=0x8c647bacu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c647bae; /* mov.l @(r0,r14),r3 */
L8c647bae: *stop=0x8c647baeu;r[6]=r[12];goto L8c647bb0; /* mov r12,r6 */
L8c647bb0: target=r[3];*pr=0x8c647bb4u;*stop=0x8c647bb2u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647bb4; /* jsr @r3 */
L8c647bb4: *stop=0x8c647bb6u;goto L8c6479a0; /* bra 0x8c6479a0 */
}

SA_STAGE_EXPORT int sa_movie_channel_worker_8c647980(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return channel_worker_run(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_worker_resume_8c647982(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return channel_worker_run(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c647982u);}
SA_STAGE_EXPORT int sa_movie_channel_worker_resume_8c647b74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return channel_worker_run(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c647b74u);}
SA_STAGE_EXPORT int sa_movie_channel_worker_8c66086a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66086a;
L8c66086a: *stop=0x8c66086au;{uint32_t loaded=rd(m,bus,&io_status,0x8c660890u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66086c; /* mov.l 0x8c660890,r2 */
L8c66086c: target=r[2];*stop=0x8c66086eu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}
