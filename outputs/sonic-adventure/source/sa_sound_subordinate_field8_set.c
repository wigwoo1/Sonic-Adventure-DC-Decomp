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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Retail sound subordinate field setter 8c65dc1c..8c65dc54. Field meanings await caller recovery. */
SA_STAGE_EXPORT int sa_sound_subordinate_field8_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65dc1c;
L8c65dc1c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65dc1e;
L8c65dc1e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65dc20;
L8c65dc20: if(!(*sr&1u))goto L8c65dc2a; /* bf 0x8c65dc2a */
 goto L8c65dc22;
L8c65dc22: r[2]=rd(m,0x8c65dc88u,4); /* mov.l 0x8c65dc88,r2 */
 goto L8c65dc24;
L8c65dc24: r[4]=rd(m,0x8c65dc84u,4); /* mov.l 0x8c65dc84,r4 */
 goto L8c65dc26;
L8c65dc26: target=r[2];*pr=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c65dc2a: r[2]=rd(m,r[4]+0x30u,4); /* mov.l @(0x30,r4),r2 */
 goto L8c65dc2c;
L8c65dc2c: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c65dc2e;
L8c65dc2e: *macl=r[3]*r[2]; /* mul.l r3,r2 */
 goto L8c65dc30;
L8c65dc30: r[3]=*macl; /* sts MACL,r3 */
 goto L8c65dc32;
L8c65dc32: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c65dc34;
L8c65dc34: if((*sr&1u))goto L8c65dc38; /* bt 0x8c65dc38 */
 goto L8c65dc36;
L8c65dc36: r[3]+=7u; /* add #0x7,r3 */
 goto L8c65dc38;
L8c65dc38: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c65dc3a;
L8c65dc3a: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c65dc3c;
L8c65dc3c: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c65dc3e;
L8c65dc3e: wr(m,r[4]+0x30u,r[3],4); /* mov.l r3,@(0x30,r4) */
 goto L8c65dc40;
L8c65dc40: r[3]=rd(m,0x8c65dc94u,4); /* mov.l 0x8c65dc94,r3 */
 goto L8c65dc42;
L8c65dc42: wr(m,r[4]+0x8u,r[5],4); /* mov.l r5,@(0x8,r4) */
 goto L8c65dc44;
L8c65dc44: r[1]=rd(m,r[4]+0x30u,4); /* mov.l @(0x30,r4),r1 */
 goto L8c65dc46;
L8c65dc46: target=r[3];*pr=0x8c65dc4au;r[0]=r[5];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dc4a;
L8c65dc4a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65dc4c;
L8c65dc4c: r[0]<<=2; /* shll2 r0 */
 goto L8c65dc4e;
L8c65dc4e: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c65dc50;
L8c65dc50: wr(m,r[4]+0x30u,r[0],4);return 0; /* rts */
}
