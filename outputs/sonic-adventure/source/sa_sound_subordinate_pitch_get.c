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
/* Retail pitch cache getter65ded4..65df12. */
SA_STAGE_EXPORT int sa_sound_subordinate_pitch_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ded4;
L8c65ded4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ded6;
L8c65ded6: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65ded8;
L8c65ded8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65deda;
L8c65deda: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c65dedc;
L8c65dedc: take=!(*sr&1u);wr(m,r[15],r[6],4);if(take)goto L8c65defa;goto L8c65dee0; /* bf/s 0x8c65defa */
 goto L8c65dee0;
L8c65dee0: r[2]=rd(m,0x8c65dfa0u,4); /* mov.l 0x8c65dfa0,r2 */
 goto L8c65dee2;
L8c65dee2: r[4]=rd(m,0x8c65df9cu,4); /* mov.l 0x8c65df9c,r4 */
 goto L8c65dee4;
L8c65dee4: target=r[2];*pr=0x8c65dee8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65dee8;
L8c65dee8: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65deea;
L8c65deea: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c65deec;
L8c65deec: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65deee;
L8c65deee: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65def0;
L8c65def0: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65def2;
L8c65def2: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65def4;
L8c65def4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65def6;
L8c65def6: return 0; /* rts */
L8c65defa: r[0]=sx(m,0x8c65df94u,2); /* mov.w 0x8c65df94,r0 */
 goto L8c65defc;
L8c65defc: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65defe;
L8c65defe: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c65df00;
L8c65df00: r[0]=sx(m,0x8c65df96u,2); /* mov.w 0x8c65df96,r0 */
 goto L8c65df02;
L8c65df02: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65df04;
L8c65df04: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c65df06;
L8c65df06: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65df08;
L8c65df08: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65df0a;
L8c65df0a: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65df0c;
L8c65df0c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65df0e;
L8c65df0e: return 0; /* rts */
}
