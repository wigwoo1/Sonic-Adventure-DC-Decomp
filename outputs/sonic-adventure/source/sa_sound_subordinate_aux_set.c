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
/* Retail auxiliary sound setter65df2a..65df5e; field names provisional. */
SA_STAGE_EXPORT int sa_sound_subordinate_aux_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65df2a;
L8c65df2a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65df2c;
L8c65df2c: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65df2e;
L8c65df2e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65df30;
L8c65df30: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65df32;
L8c65df32: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c65df34;
L8c65df34: take=!(*sr&1u);wr(m,r[15],r[6],4);if(take)goto L8c65df42;goto L8c65df38; /* bf/s 0x8c65df42 */
 goto L8c65df38;
L8c65df38: r[4]=rd(m,0x8c65df9cu,4); /* mov.l 0x8c65df9c,r4 */
 goto L8c65df3a;
L8c65df3a: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65df3c;
L8c65df3c: r[2]=rd(m,0x8c65dfa0u,4); /* mov.l 0x8c65dfa0,r2 */
 goto L8c65df3e;
L8c65df3e: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c65df42: r[0]=sx(m,0x8c65df98u,2); /* mov.w 0x8c65df98,r0 */
 goto L8c65df44;
L8c65df44: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65df46;
L8c65df46: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c65df48;
L8c65df48: r[0]=sx(m,0x8c65df9au,2); /* mov.w 0x8c65df9a,r0 */
 goto L8c65df4a;
L8c65df4a: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65df4c;
L8c65df4c: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c65df4e;
L8c65df4e: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65df50;
L8c65df50: r[4]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r4 */
 goto L8c65df52;
L8c65df52: r[6]=rd(m,r[15],4); /* mov.l @r15,r6 */
 goto L8c65df54;
L8c65df54: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65df56;
L8c65df56: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65df58;
L8c65df58: r[3]=rd(m,0x8c65dfb0u,4); /* mov.l 0x8c65dfb0,r3 */
 goto L8c65df5a;
L8c65df5a: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
