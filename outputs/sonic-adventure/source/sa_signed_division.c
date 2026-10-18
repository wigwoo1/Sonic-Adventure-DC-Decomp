#include "fpu_cleanup_transfer.h"
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
/* Retail signed32 quotient helper 8c10ca5c..8c10cb06.
 * DIV0S/ROTCL/DIV1/SUBC/ADDC instruction behavior preserved.
 * Zero divisor returns zero and records error0x44e. No external calls. */
SA_STAGE_EXPORT int sa_signed_division(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ca5c;
L8c10ca5c: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c10ca5e;
L8c10ca5e: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c10ca60;
L8c10ca60: if((*sr&1u))goto L8c10cafa; /* bt 0x8c10cafa */
 goto L8c10ca62;
L8c10ca62: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10ca64;
L8c10ca64: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10ca66;
L8c10ca66: *sr=(*sr&~0x301u)|((r[1]>>31)<<8)|((r[2]>>31)<<9)|((r[1]^r[2])>>31); /* div0s r2,r1 */
 goto L8c10ca68;
L8c10ca68: {uint64_t lhs=r[3],rhs=(uint64_t)r[3]+(*sr&1u);r[3]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);} /* subc r3,r3 */
 goto L8c10ca6a;
L8c10ca6a: {uint64_t lhs=r[1],rhs=(uint64_t)r[2]+(*sr&1u);r[1]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);} /* subc r2,r1 */
 goto L8c10ca6c;
L8c10ca6c: *sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31); /* div0s r0,r3 */
 goto L8c10ca6e;
L8c10ca6e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca70;
L8c10ca70: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca72;
L8c10ca72: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca74;
L8c10ca74: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca76;
L8c10ca76: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca78;
L8c10ca78: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca7a;
L8c10ca7a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca7c;
L8c10ca7c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca7e;
L8c10ca7e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca80;
L8c10ca80: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca82;
L8c10ca82: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca84;
L8c10ca84: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca86;
L8c10ca86: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca88;
L8c10ca88: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca8a;
L8c10ca8a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca8c;
L8c10ca8c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca8e;
L8c10ca8e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca90;
L8c10ca90: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca92;
L8c10ca92: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca94;
L8c10ca94: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca96;
L8c10ca96: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca98;
L8c10ca98: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca9a;
L8c10ca9a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca9c;
L8c10ca9c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca9e;
L8c10ca9e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caa0;
L8c10caa0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caa2;
L8c10caa2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caa4;
L8c10caa4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caa6;
L8c10caa6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caa8;
L8c10caa8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caaa;
L8c10caaa: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caac;
L8c10caac: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caae;
L8c10caae: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cab0;
L8c10cab0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cab2;
L8c10cab2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cab4;
L8c10cab4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cab6;
L8c10cab6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cab8;
L8c10cab8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caba;
L8c10caba: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cabc;
L8c10cabc: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cabe;
L8c10cabe: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cac0;
L8c10cac0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cac2;
L8c10cac2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cac4;
L8c10cac4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cac6;
L8c10cac6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cac8;
L8c10cac8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caca;
L8c10caca: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cacc;
L8c10cacc: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cace;
L8c10cace: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cad0;
L8c10cad0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cad2;
L8c10cad2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cad4;
L8c10cad4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cad6;
L8c10cad6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cad8;
L8c10cad8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cada;
L8c10cada: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cadc;
L8c10cadc: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cade;
L8c10cade: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cae0;
L8c10cae0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cae2;
L8c10cae2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cae4;
L8c10cae4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cae6;
L8c10cae6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cae8;
L8c10cae8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caea;
L8c10caea: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caec;
L8c10caec: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caee;
L8c10caee: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caf0;
L8c10caf0: {uint64_t sum=(uint64_t)r[1]+r[2]+(*sr&1u);r[1]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r2,r1 */
 goto L8c10caf2;
L8c10caf2: r[0]=r[1]; /* mov r1,r0 */
 goto L8c10caf4;
L8c10caf4: r[3]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r3 */
 goto L8c10caf6;
L8c10caf6: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10cafa: r[1]=rd(m,0x8c10cb08u,4); /* mov.l 0x8c10cb08,r1 */
 goto L8c10cafc;
L8c10cafc: r[2]=rd(m,0x8c10cb0cu,4); /* mov.l 0x8c10cb0c,r2 */
 goto L8c10cafe;
L8c10cafe: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10cb00;
L8c10cb00: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c10cb02;
L8c10cb02: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
