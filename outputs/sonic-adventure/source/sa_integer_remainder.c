#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c10cbb8..8c10cc76.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_integer_remainder(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cbb8;
L8c10cbb8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c10cbba;
L8c10cbba: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c10cbbc;
L8c10cbbc: if((*sr&1u))goto L8c10cc6a; /* bt 0x8c10cc6a */
 goto L8c10cbbe;
L8c10cbbe: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10cbc0;
L8c10cbc0: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10cbc2;
L8c10cbc2: r[15]-=4;wr(m,r[15],r[4],4); /* mov.l r4,@-r15 */
 goto L8c10cbc4;
L8c10cbc4: *sr=(*sr&~0x301u)|((r[1]>>31)<<8)|((r[2]>>31)<<9)|((r[1]^r[2])>>31); /* div0s r2,r1 */
 goto L8c10cbc6;
L8c10cbc6: r[4]=*sr&1u; /* movt r4 */
 goto L8c10cbc8;
L8c10cbc8: {uint64_t lhs=r[3],rhs=(uint64_t)r[3]+(*sr&1u);r[3]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);} /* subc r3,r3 */
 goto L8c10cbca;
L8c10cbca: {uint64_t lhs=r[1],rhs=(uint64_t)r[2]+(*sr&1u);r[1]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);} /* subc r2,r1 */
 goto L8c10cbcc;
L8c10cbcc: *sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31); /* div0s r0,r3 */
 goto L8c10cbce;
L8c10cbce: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbd0;
L8c10cbd0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbd2;
L8c10cbd2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbd4;
L8c10cbd4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbd6;
L8c10cbd6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbd8;
L8c10cbd8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbda;
L8c10cbda: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbdc;
L8c10cbdc: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbde;
L8c10cbde: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbe0;
L8c10cbe0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbe2;
L8c10cbe2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbe4;
L8c10cbe4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbe6;
L8c10cbe6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbe8;
L8c10cbe8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbea;
L8c10cbea: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbec;
L8c10cbec: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbee;
L8c10cbee: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbf0;
L8c10cbf0: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbf2;
L8c10cbf2: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbf4;
L8c10cbf4: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbf6;
L8c10cbf6: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbf8;
L8c10cbf8: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbfa;
L8c10cbfa: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cbfc;
L8c10cbfc: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cbfe;
L8c10cbfe: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc00;
L8c10cc00: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc02;
L8c10cc02: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc04;
L8c10cc04: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc06;
L8c10cc06: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc08;
L8c10cc08: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc0a;
L8c10cc0a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc0c;
L8c10cc0c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc0e;
L8c10cc0e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc10;
L8c10cc10: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc12;
L8c10cc12: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc14;
L8c10cc14: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc16;
L8c10cc16: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc18;
L8c10cc18: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc1a;
L8c10cc1a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc1c;
L8c10cc1c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc1e;
L8c10cc1e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc20;
L8c10cc20: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc22;
L8c10cc22: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc24;
L8c10cc24: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc26;
L8c10cc26: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc28;
L8c10cc28: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc2a;
L8c10cc2a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc2c;
L8c10cc2c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc2e;
L8c10cc2e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc30;
L8c10cc30: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc32;
L8c10cc32: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc34;
L8c10cc34: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc36;
L8c10cc36: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc38;
L8c10cc38: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc3a;
L8c10cc3a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc3c;
L8c10cc3c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc3e;
L8c10cc3e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc40;
L8c10cc40: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc42;
L8c10cc42: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc44;
L8c10cc44: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc46;
L8c10cc46: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc48;
L8c10cc48: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc4a;
L8c10cc4a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;} /* rotcl r1 */
 goto L8c10cc4c;
L8c10cc4c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc4e;
L8c10cc4e: *sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[2]>>31)<<9)|((r[3]^r[2])>>31); /* div0s r2,r3 */
 goto L8c10cc50;
L8c10cc50: r[2]=*sr&1u; /* movt r2 */
 goto L8c10cc52;
L8c10cc52: r[2]^=r[4]; /* xor r4,r2 */
 goto L8c10cc54;
L8c10cc54: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(incoming<<31);} /* rotcr r2 */
 goto L8c10cc56;
L8c10cc56: if(!(*sr&1u))goto L8c10cc5e; /* bf 0x8c10cc5e */
 goto L8c10cc58;
L8c10cc58: *sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31); /* div0s r0,r3 */
 goto L8c10cc5a;
L8c10cc5a: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c10cc5c;
L8c10cc5c: div_step(r[0],&r[3],sr); /* div1 r0,r3 */
 goto L8c10cc5e;
L8c10cc5e: r[3]+=r[4]; /* add r4,r3 */
 goto L8c10cc60;
L8c10cc60: r[0]=r[3]; /* mov r3,r0 */
 goto L8c10cc62;
L8c10cc62: r[4]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r4 */
 goto L8c10cc64;
L8c10cc64: r[3]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r3 */
 goto L8c10cc66;
L8c10cc66: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10cc6a: r[1]=rd(m,0x8c10cc78u,4); /* mov.l 0x8c10cc78,r1 */
 goto L8c10cc6c;
L8c10cc6c: r[2]=rd(m,0x8c10cc7cu,4); /* mov.l 0x8c10cc7c,r2 */
 goto L8c10cc6e;
L8c10cc6e: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10cc70;
L8c10cc70: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c10cc72;
L8c10cc72: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
