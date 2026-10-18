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
/* Retail source capacity query 8c65e462..8c65e49a. Invalid modes optionally invoke error callback. */
SA_STAGE_EXPORT int sa_sound_source_capacity(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e462;
L8c65e462: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e464;
L8c65e464: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e466;
L8c65e466: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65e468;
L8c65e468:  /* nop */
 goto L8c65e46a;
L8c65e46a: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e46c;
L8c65e46c: take=!(*sr&1u);r[14]=r[4];if(take)goto L8c65e478;goto L8c65e470; /* bf/s 0x8c65e478 */
 goto L8c65e470;
L8c65e470: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e472;
L8c65e472: r[0]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r0 */
 goto L8c65e474;
L8c65e474: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65e478: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c65e47a;
L8c65e47a: if(!(*sr&1u))goto L8c65e484; /* bf 0x8c65e484 */
 goto L8c65e47c;
L8c65e47c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e47e;
L8c65e47e: r[0]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r0 */
 goto L8c65e480;
L8c65e480: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65e484: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e486;
L8c65e486: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e488;
L8c65e488: if((*sr&1u))goto L8c65e492; /* bt 0x8c65e492 */
 goto L8c65e48a;
L8c65e48a: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e48c;
L8c65e48c: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e48e;
L8c65e48e: target=r[3];*pr=0x8c65e492u;r[4]=rd(m,r[14]+0x2cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e492;
L8c65e492: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65e494;
L8c65e494: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e496;
L8c65e496: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
