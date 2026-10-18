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
/* Retail eight-entry sound slot pool helper 8c66abc2..8c66abea.
 * Entries contain occupancy word and identifiers15..8. */
SA_STAGE_EXPORT int sa_sound_small_slot_allocate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66abc2;
L8c66abc2: r[7]=rd(m,0x8c66abf4u,4); /* mov.l 0x8c66abf4,r7 */
 goto L8c66abc4;
L8c66abc4: r[6]=0u; /* mov #0x0,r6 */
 goto L8c66abc6;
L8c66abc6: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66abc8;
L8c66abc8: r[4]=r[7]; /* mov r7,r4 */
 goto L8c66abca;
L8c66abca: r[7]+=64u; /* add #0x40,r7 */
 goto L8c66abcc;
L8c66abcc: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c66abce;
L8c66abce: take=(*sr&1u);r[0]=1u;if(take)goto L8c66abe6;goto L8c66abd2; /* bt/s 0x8c66abe6 */
 goto L8c66abd2;
L8c66abd2: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c66abd4;
L8c66abd4: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66abd6;
L8c66abd6: if(!(*sr&1u))goto L8c66abde; /* bf 0x8c66abde */
 goto L8c66abd8;
L8c66abd8: r[6]=r[5]; /* mov r5,r6 */
 goto L8c66abda;
L8c66abda: wr(m,r[6],r[0],4);goto L8c66abe6; /* bra 0x8c66abe6 */
 goto L8c66abde;
L8c66abde: r[4]+=8u; /* add #0x8,r4 */
 goto L8c66abe0;
L8c66abe0: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c66abe2;
L8c66abe2: take=!(*sr&1u);r[5]+=8u;if(take)goto L8c66abd2;goto L8c66abe6; /* bf/s 0x8c66abd2 */
 goto L8c66abe6;
L8c66abe6: r[0]=r[6];return 0; /* rts */
}
