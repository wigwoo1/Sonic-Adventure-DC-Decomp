#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
static void div_step(uint32_t divisor,uint32_t *value,uint32_t *sr){
 unsigned oldq=(*sr>>8)&1u,flagm=(*sr>>9)&1u,q=*value>>31;uint32_t before=(*value<<1)|(*sr&1u),result;unsigned overflow;
 if(oldq==flagm){result=before-divisor;overflow=result>before;}else{result=before+divisor;overflow=result<before;}
 q=flagm?(q?overflow:!overflow):(q?!overflow:overflow);*value=result;*sr=(*sr&~0x101u)|(q<<8)|(q==flagm);
}
/* Retail unsigned division helper; exact SH-4 register and SR effects. */
SA_STAGE_EXPORT int sa_unsigned_quotient(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cb10;
L8c10cb10: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c10cb12;
L8c10cb12: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c10cb14;
L8c10cb14: if((*sr&1u))goto L8c10cba2; /* bt 0x8c10cba2 */
 goto L8c10cb16;
L8c10cb16: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10cb18;
L8c10cb18: *sr&=~0x301u; /* div0u */
 goto L8c10cb1a;
L8c10cb1a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb1c;
L8c10cb1c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb1e;
L8c10cb1e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb20;
L8c10cb20: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb22;
L8c10cb22: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb24;
L8c10cb24: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb26;
L8c10cb26: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb28;
L8c10cb28: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb2a;
L8c10cb2a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb2c;
L8c10cb2c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb2e;
L8c10cb2e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb30;
L8c10cb30: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb32;
L8c10cb32: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb34;
L8c10cb34: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb36;
L8c10cb36: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb38;
L8c10cb38: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb3a;
L8c10cb3a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb3c;
L8c10cb3c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb3e;
L8c10cb3e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb40;
L8c10cb40: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb42;
L8c10cb42: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb44;
L8c10cb44: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb46;
L8c10cb46: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb48;
L8c10cb48: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb4a;
L8c10cb4a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb4c;
L8c10cb4c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb4e;
L8c10cb4e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb50;
L8c10cb50: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb52;
L8c10cb52: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb54;
L8c10cb54: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb56;
L8c10cb56: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb58;
L8c10cb58: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb5a;
L8c10cb5a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb5c;
L8c10cb5c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb5e;
L8c10cb5e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb60;
L8c10cb60: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb62;
L8c10cb62: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb64;
L8c10cb64: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb66;
L8c10cb66: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb68;
L8c10cb68: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb6a;
L8c10cb6a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb6c;
L8c10cb6c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb6e;
L8c10cb6e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb70;
L8c10cb70: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb72;
L8c10cb72: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb74;
L8c10cb74: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb76;
L8c10cb76: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb78;
L8c10cb78: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb7a;
L8c10cb7a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb7c;
L8c10cb7c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb7e;
L8c10cb7e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb80;
L8c10cb80: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb82;
L8c10cb82: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb84;
L8c10cb84: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb86;
L8c10cb86: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb88;
L8c10cb88: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb8a;
L8c10cb8a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb8c;
L8c10cb8c: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb8e;
L8c10cb8e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb90;
L8c10cb90: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb92;
L8c10cb92: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb94;
L8c10cb94: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb96;
L8c10cb96: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb98;
L8c10cb98: div_step(r[0],&r[2],sr); /* div1 r0,r2 */
 goto L8c10cb9a;
L8c10cb9a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cb9c;
L8c10cb9c: r[0]=r[1]; /* mov r1,r0 */
 goto L8c10cb9e;
L8c10cb9e: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10cba2: r[2]=rd(m,0x8c10cbb0u,4); /* mov.l 0x8c10cbb0,r2 */
 goto L8c10cba4;
L8c10cba4: r[1]=rd(m,0x8c10cbb4u,4); /* mov.l 0x8c10cbb4,r1 */
 goto L8c10cba6;
L8c10cba6: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10cba8;
L8c10cba8: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c10cbaa;
L8c10cbaa: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
