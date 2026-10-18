#include "input_io.h"
#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(io,m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(const struct sa_input_io *io,uint8_t *m,uint32_t a,uint32_t v,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of the USA Rev A main executable transfer service. External calls use the supplied dispatcher. */
SA_STAGE_EXPORT int sa_engine_transfer_submit_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10faa4;
L8c10faa4: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10faa6;
L8c10faa6: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10faa8;
L8c10faa8: r[15]-=4;wr(io,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10faaa;
L8c10faaa: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10faac;
L8c10faac: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c10faae;
L8c10faae: r[14]=r[4]; /* mov r4,r14 */
 goto L8c10fab0;
L8c10fab0: r[3]=rd(io,m,0x8c10fb34u,4); /* mov.l 0x8c10fb34,r3 */
 goto L8c10fab2;
L8c10fab2: r[12]=0u; /* mov #0x0,r12 */
 goto L8c10fab4;
L8c10fab4: wr(io,m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c10fab6;
L8c10fab6: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10fab8;
L8c10fab8: r[13]=rd(io,m,r[3],4); /* mov.l @r3,r13 */
 goto L8c10faba;
L8c10faba: r[2]<<=2; /* shll2 r2 */
 goto L8c10fabc;
L8c10fabc: r[2]<<=2; /* shll2 r2 */
 goto L8c10fabe;
L8c10fabe: r[13]=rd(io,m,r[13],4); /* mov.l @r13,r13 */
 goto L8c10fac0;
L8c10fac0: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c10fac2;
L8c10fac2: r[13]+=r[2]; /* add r2,r13 */
 goto L8c10fac4;
L8c10fac4: r[2]=rd(io,m,0x8c10fb38u,4); /* mov.l 0x8c10fb38,r2 */
 goto L8c10fac6;
L8c10fac6: target=r[2];*pr=0x8c10facau;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10faca;
L8c10faca: r[0]=rd(io,m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10facc;
L8c10facc: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10face;
L8c10face: if(!(*sr&1u))goto L8c10fad6; /* bf 0x8c10fad6 */
 goto L8c10fad0;
L8c10fad0: r[12]=rd(io,m,0x8c10fb3cu,4); /* mov.l 0x8c10fb3c,r12 */
 goto L8c10fad2;
L8c10fad2: goto L8c10fb24; /* bra 0x8c10fb24 */
 goto L8c10fad6;
L8c10fad6: r[5]=rd(io,m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r5 */
 goto L8c10fad8;
L8c10fad8: r[3]=rd(io,m,0x8c10fb40u,4); /* mov.l 0x8c10fb40,r3 */
 goto L8c10fada;
L8c10fada: r[2]=rd(io,m,0x8c10fb44u,4); /* mov.l 0x8c10fb44,r2 */
 goto L8c10fadc;
L8c10fadc: r[5]&=r[3]; /* and r3,r5 */
 goto L8c10fade;
L8c10fade: target=r[2];*pr=0x8c10fae2u;r[4]=rd(io,m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10fae2;
L8c10fae2: r[3]=0u; /* mov #0x0,r3 */
 goto L8c10fae4;
L8c10fae4: wr(io,m,r[13]+0x14u,r[3],4); /* mov.l r3,@(0x14,r13) */
 goto L8c10fae6;
L8c10fae6: r[2]=rd(io,m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r2 */
 goto L8c10fae8;
L8c10fae8: wr(io,m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c10faea;
L8c10faea: r[3]=rd(io,m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r3 */
 goto L8c10faec;
L8c10faec: wr(io,m,r[13]+0x4u,r[3],4); /* mov.l r3,@(0x4,r13) */
 goto L8c10faee;
L8c10faee: r[2]=rd(io,m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c10faf0;
L8c10faf0: wr(io,m,r[13]+0x8u,r[2],4); /* mov.l r2,@(0x8,r13) */
 goto L8c10faf2;
L8c10faf2: r[3]=rd(io,m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c10faf4;
L8c10faf4: wr(io,m,r[13]+0xcu,r[3],4); /* mov.l r3,@(0xc,r13) */
 goto L8c10faf6;
L8c10faf6: r[1]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c10faf8;
L8c10faf8: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c10fafa;
L8c10fafa: if(!(*sr&1u))goto L8c10fb00; /* bf 0x8c10fb00 */
 goto L8c10fafc;
L8c10fafc: r[2]=0u;goto L8c10fb02; /* bra 0x8c10fb02 */
 goto L8c10fb00;
L8c10fb00: r[2]=2u; /* mov #0x2,r2 */
 goto L8c10fb02;
L8c10fb02: r[1]=rd(io,m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r1 */
 goto L8c10fb04;
L8c10fb04: r[4]=1u; /* mov #0x1,r4 */
 goto L8c10fb06;
L8c10fb06: r[3]=rd(io,m,0x8c10fb34u,4); /* mov.l 0x8c10fb34,r3 */
 goto L8c10fb08;
L8c10fb08: r[2]|=r[1]; /* or r1,r2 */
 goto L8c10fb0a;
L8c10fb0a: r[1]=sx(io,m,0x8c10fb32u,2); /* mov.w 0x8c10fb32,r1 */
 goto L8c10fb0c;
L8c10fb0c: wr(io,m,r[13]+0x10u,r[2],4); /* mov.l r2,@(0x10,r13) */
 goto L8c10fb0e;
L8c10fb0e: wr(io,m,r[13]+0x14u,r[4],4); /* mov.l r4,@(0x14,r13) */
 goto L8c10fb10;
L8c10fb10: wr(io,m,r[14]+0x2cu,r[4],4); /* mov.l r4,@(0x2c,r14) */
 goto L8c10fb12;
L8c10fb12: r[0]=rd(io,m,r[3],4); /* mov.l @r3,r0 */
 goto L8c10fb14;
L8c10fb14: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10fb16;
L8c10fb16: r[1]+=r[0]; /* add r0,r1 */
 goto L8c10fb18;
L8c10fb18: r[2]<<=2; /* shll2 r2 */
 goto L8c10fb1a;
L8c10fb1a: r[2]+=r[1]; /* add r1,r2 */
 goto L8c10fb1c;
L8c10fb1c: r[0]=rd(io,m,r[2],4); /* mov.l @r2,r0 */
 goto L8c10fb1e;
L8c10fb1e: r[0]+=1u; /* add #0x1,r0 */
 goto L8c10fb20;
L8c10fb20: wr(io,m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c10fb22;
L8c10fb22: wr(io,m,r[13]+0x18u,r[4],4); /* mov.l r4,@(0x18,r13) */
 goto L8c10fb24;
L8c10fb24: r[0]=r[12]; /* mov r12,r0 */
 goto L8c10fb26;
L8c10fb26: r[15]+=8u; /* add #0x8,r15 */
 goto L8c10fb28;
L8c10fb28: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10fb2a;
L8c10fb2a: r[12]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10fb2c;
L8c10fb2c: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10fb2e;
L8c10fb2e: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
