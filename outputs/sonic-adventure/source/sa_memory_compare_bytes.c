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
/* Static C recovery of retail Tikal helper 8c10eb1c..8c10eb50.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_memory_compare_bytes(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10eb1c;
L8c10eb1c: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c10eb1e;
L8c10eb1e: take=!(*sr&1u);r[7]=r[5];if(take)goto L8c10eb26;goto L8c10eb22; /* bf/s 0x8c10eb26 */
 goto L8c10eb22;
L8c10eb22: r[0]=0u;return 0; /* rts */
L8c10eb26: r[5]=0u; /* mov #0x0,r5 */
 goto L8c10eb28;
L8c10eb28: r[2]=r[5]; /* mov r5,r2 */
 goto L8c10eb2a;
L8c10eb2a: *sr=(*sr&~1u)|(r[2]>=r[6]); /* cmp/hs r6,r2 */
 goto L8c10eb2c;
L8c10eb2c: if((*sr&1u))goto L8c10eb3c; /* bt 0x8c10eb3c */
 goto L8c10eb2e;
L8c10eb2e: r[2]=sx(m,r[7],1);r[7]+=1; /* mov.b @r7+,r2 */
 goto L8c10eb30;
L8c10eb30: r[3]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r3 */
 goto L8c10eb32;
L8c10eb32: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c10eb34;
L8c10eb34: if(!(*sr&1u))goto L8c10eb3c; /* bf 0x8c10eb3c */
 goto L8c10eb36;
L8c10eb36: r[5]+=1u; /* add #0x1,r5 */
 goto L8c10eb38;
L8c10eb38: *sr=(*sr&~1u)|(r[5]>=r[6]); /* cmp/hs r6,r5 */
 goto L8c10eb3a;
L8c10eb3a: if(!(*sr&1u))goto L8c10eb2e; /* bf 0x8c10eb2e */
 goto L8c10eb3c;
L8c10eb3c: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10eb3e;
L8c10eb3e: r[7]+=4294967295u; /* add #-0x1,r7 */
 goto L8c10eb40;
L8c10eb40: r[0]+=4294967295u; /* add #-0x1,r0 */
 goto L8c10eb42;
L8c10eb42: r[3]=sx(m,r[7],1); /* mov.b @r7,r3 */
 goto L8c10eb44;
L8c10eb44: r[0]=sx(m,r[0],1); /* mov.b @r0,r0 */
 goto L8c10eb46;
L8c10eb46: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c10eb48;
L8c10eb48: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10eb4a;
L8c10eb4a: r[0]-=r[3]; /* sub r3,r0 */
 goto L8c10eb4c;
L8c10eb4c: return 0; /* rts */
}
