#include "sa_draw_motion_stop.h"
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
L8c10ca5c: sa_motion_publish_stop(ctx,0x8c10ca5cu); *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c10ca5e;
L8c10ca5e: sa_motion_publish_stop(ctx,0x8c10ca5eu); r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c10ca60;
L8c10ca60: sa_motion_publish_stop(ctx,0x8c10ca60u); if((*sr&1u))goto L8c10cafa; /* bt 0x8c10cafa */
 goto L8c10ca62;
L8c10ca62: sa_motion_publish_stop(ctx,0x8c10ca62u); r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10ca64;
L8c10ca64: sa_motion_publish_stop(ctx,0x8c10ca64u); r[2]=0u; /* mov #0x0,r2 */
 goto L8c10ca66;
L8c10ca66: sa_motion_publish_stop(ctx,0x8c10ca66u); *sr=(*sr&~0x301u)|((r[1]>>31)<<8)|((r[2]>>31)<<9)|((r[1]^r[2])>>31); /* div0s r2,r1 */
 goto L8c10ca68;
L8c10ca68: sa_motion_publish_stop(ctx,0x8c10ca68u); {uint64_t lhs=r[3],rhs=(uint64_t)r[3]+(*sr&1u);r[3]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);} /* subc r3,r3 */
 goto L8c10ca6a;
L8c10ca6a: sa_motion_publish_stop(ctx,0x8c10ca6au); {uint64_t lhs=r[1],rhs=(uint64_t)r[2]+(*sr&1u);r[1]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);} /* subc r2,r1 */
 goto L8c10ca6c;
L8c10ca6c: sa_motion_publish_stop(ctx,0x8c10ca6cu); *sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31); /* div0s r0,r3 */
 goto L8c10ca6e;
L8c10ca6e: sa_motion_publish_stop(ctx,0x8c10ca6eu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca70;
L8c10ca70: sa_motion_publish_stop(ctx,0x8c10ca70u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca72;
L8c10ca72: sa_motion_publish_stop(ctx,0x8c10ca72u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca74;
L8c10ca74: sa_motion_publish_stop(ctx,0x8c10ca74u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca76;
L8c10ca76: sa_motion_publish_stop(ctx,0x8c10ca76u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca78;
L8c10ca78: sa_motion_publish_stop(ctx,0x8c10ca78u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca7a;
L8c10ca7a: sa_motion_publish_stop(ctx,0x8c10ca7au); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca7c;
L8c10ca7c: sa_motion_publish_stop(ctx,0x8c10ca7cu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca7e;
L8c10ca7e: sa_motion_publish_stop(ctx,0x8c10ca7eu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca80;
L8c10ca80: sa_motion_publish_stop(ctx,0x8c10ca80u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca82;
L8c10ca82: sa_motion_publish_stop(ctx,0x8c10ca82u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca84;
L8c10ca84: sa_motion_publish_stop(ctx,0x8c10ca84u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca86;
L8c10ca86: sa_motion_publish_stop(ctx,0x8c10ca86u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca88;
L8c10ca88: sa_motion_publish_stop(ctx,0x8c10ca88u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca8a;
L8c10ca8a: sa_motion_publish_stop(ctx,0x8c10ca8au); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca8c;
L8c10ca8c: sa_motion_publish_stop(ctx,0x8c10ca8cu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca8e;
L8c10ca8e: sa_motion_publish_stop(ctx,0x8c10ca8eu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca90;
L8c10ca90: sa_motion_publish_stop(ctx,0x8c10ca90u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca92;
L8c10ca92: sa_motion_publish_stop(ctx,0x8c10ca92u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca94;
L8c10ca94: sa_motion_publish_stop(ctx,0x8c10ca94u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca96;
L8c10ca96: sa_motion_publish_stop(ctx,0x8c10ca96u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca98;
L8c10ca98: sa_motion_publish_stop(ctx,0x8c10ca98u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca9a;
L8c10ca9a: sa_motion_publish_stop(ctx,0x8c10ca9au); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10ca9c;
L8c10ca9c: sa_motion_publish_stop(ctx,0x8c10ca9cu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10ca9e;
L8c10ca9e: sa_motion_publish_stop(ctx,0x8c10ca9eu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caa0;
L8c10caa0: sa_motion_publish_stop(ctx,0x8c10caa0u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caa2;
L8c10caa2: sa_motion_publish_stop(ctx,0x8c10caa2u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caa4;
L8c10caa4: sa_motion_publish_stop(ctx,0x8c10caa4u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caa6;
L8c10caa6: sa_motion_publish_stop(ctx,0x8c10caa6u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caa8;
L8c10caa8: sa_motion_publish_stop(ctx,0x8c10caa8u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caaa;
L8c10caaa: sa_motion_publish_stop(ctx,0x8c10caaau); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caac;
L8c10caac: sa_motion_publish_stop(ctx,0x8c10caacu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caae;
L8c10caae: sa_motion_publish_stop(ctx,0x8c10caaeu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cab0;
L8c10cab0: sa_motion_publish_stop(ctx,0x8c10cab0u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cab2;
L8c10cab2: sa_motion_publish_stop(ctx,0x8c10cab2u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cab4;
L8c10cab4: sa_motion_publish_stop(ctx,0x8c10cab4u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cab6;
L8c10cab6: sa_motion_publish_stop(ctx,0x8c10cab6u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cab8;
L8c10cab8: sa_motion_publish_stop(ctx,0x8c10cab8u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caba;
L8c10caba: sa_motion_publish_stop(ctx,0x8c10cabau); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cabc;
L8c10cabc: sa_motion_publish_stop(ctx,0x8c10cabcu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cabe;
L8c10cabe: sa_motion_publish_stop(ctx,0x8c10cabeu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cac0;
L8c10cac0: sa_motion_publish_stop(ctx,0x8c10cac0u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cac2;
L8c10cac2: sa_motion_publish_stop(ctx,0x8c10cac2u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cac4;
L8c10cac4: sa_motion_publish_stop(ctx,0x8c10cac4u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cac6;
L8c10cac6: sa_motion_publish_stop(ctx,0x8c10cac6u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cac8;
L8c10cac8: sa_motion_publish_stop(ctx,0x8c10cac8u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caca;
L8c10caca: sa_motion_publish_stop(ctx,0x8c10cacau); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cacc;
L8c10cacc: sa_motion_publish_stop(ctx,0x8c10caccu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cace;
L8c10cace: sa_motion_publish_stop(ctx,0x8c10caceu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cad0;
L8c10cad0: sa_motion_publish_stop(ctx,0x8c10cad0u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cad2;
L8c10cad2: sa_motion_publish_stop(ctx,0x8c10cad2u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cad4;
L8c10cad4: sa_motion_publish_stop(ctx,0x8c10cad4u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cad6;
L8c10cad6: sa_motion_publish_stop(ctx,0x8c10cad6u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cad8;
L8c10cad8: sa_motion_publish_stop(ctx,0x8c10cad8u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cada;
L8c10cada: sa_motion_publish_stop(ctx,0x8c10cadau); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cadc;
L8c10cadc: sa_motion_publish_stop(ctx,0x8c10cadcu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cade;
L8c10cade: sa_motion_publish_stop(ctx,0x8c10cadeu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cae0;
L8c10cae0: sa_motion_publish_stop(ctx,0x8c10cae0u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cae2;
L8c10cae2: sa_motion_publish_stop(ctx,0x8c10cae2u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cae4;
L8c10cae4: sa_motion_publish_stop(ctx,0x8c10cae4u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cae6;
L8c10cae6: sa_motion_publish_stop(ctx,0x8c10cae6u); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cae8;
L8c10cae8: sa_motion_publish_stop(ctx,0x8c10cae8u); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caea;
L8c10caea: sa_motion_publish_stop(ctx,0x8c10caeau); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caec;
L8c10caec: sa_motion_publish_stop(ctx,0x8c10caecu); div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10caee;
L8c10caee: sa_motion_publish_stop(ctx,0x8c10caeeu); {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10caf0;
L8c10caf0: sa_motion_publish_stop(ctx,0x8c10caf0u); {uint64_t sum=(uint64_t)r[1]+r[2]+(*sr&1u);r[1]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r2,r1 */
 goto L8c10caf2;
L8c10caf2: sa_motion_publish_stop(ctx,0x8c10caf2u); r[0]=r[1]; /* mov r1,r0 */
 goto L8c10caf4;
L8c10caf4: sa_motion_publish_stop(ctx,0x8c10caf4u); r[3]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r3 */
 goto L8c10caf6;
L8c10caf6: sa_motion_publish_stop(ctx,0x8c10caf8u); r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10cafa: sa_motion_publish_stop(ctx,0x8c10cafau); r[1]=rd(m,0x8c10cb08u,4); /* mov.l 0x8c10cb08,r1 */
 goto L8c10cafc;
L8c10cafc: sa_motion_publish_stop(ctx,0x8c10cafcu); r[2]=rd(m,0x8c10cb0cu,4); /* mov.l 0x8c10cb0c,r2 */
 goto L8c10cafe;
L8c10cafe: sa_motion_publish_stop(ctx,0x8c10cafeu); r[0]=0u; /* mov #0x0,r0 */
 goto L8c10cb00;
L8c10cb00: sa_motion_publish_stop(ctx,0x8c10cb00u); wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c10cb02;
L8c10cb02: sa_motion_publish_stop(ctx,0x8c10cb04u); r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
