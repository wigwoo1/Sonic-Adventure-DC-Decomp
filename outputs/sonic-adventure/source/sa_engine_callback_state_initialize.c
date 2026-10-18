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
/* Static C recovery of retail callback-state initializer 8c651520..8c651570.
 * Callback registration653640 remains an external service;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_callback_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c651520;
L8c651520: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c651522;
L8c651522: r[3]=rd(m,0x8c651644u,4); /* mov.l 0x8c651644,r3 */
 goto L8c651524;
L8c651524: r[4]=rd(m,0x8c651640u,4); /* mov.l 0x8c651640,r4 */
 goto L8c651526;
L8c651526: target=r[3];*pr=0x8c65152au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65152a;
L8c65152a: r[2]=rd(m,0x8c651648u,4); /* mov.l 0x8c651648,r2 */
 goto L8c65152c;
L8c65152c: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65152e;
L8c65152e: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c651530;
L8c651530: r[3]=rd(m,0x8c65164cu,4); /* mov.l 0x8c65164c,r3 */
 goto L8c651532;
L8c651532: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c651534;
L8c651534: r[1]=rd(m,0x8c651650u,4); /* mov.l 0x8c651650,r1 */
 goto L8c651536;
L8c651536: wr(m,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c651538;
L8c651538: r[2]=rd(m,0x8c651654u,4); /* mov.l 0x8c651654,r2 */
 goto L8c65153a;
L8c65153a: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c65153c;
L8c65153c: r[3]=rd(m,0x8c65165cu,4); /* mov.l 0x8c65165c,r3 */
 goto L8c65153e;
L8c65153e: r[6]=r[4]; /* mov r4,r6 */
 goto L8c651540;
L8c651540: r[1]=rd(m,0x8c651660u,4); /* mov.l 0x8c651660,r1 */
 goto L8c651542;
L8c651542: r[7]=20u; /* mov #0x14,r7 */
 goto L8c651544;
L8c651544: r[5]=rd(m,0x8c651658u,4); /* mov.l 0x8c651658,r5 */
 goto L8c651546;
L8c651546: wr(m,r[5],r[4],4); /* mov.l r4,@r5 */
 goto L8c651548;
L8c651548: wr(m,r[5]+0x4u,r[4],4); /* mov.l r4,@(0x4,r5) */
 goto L8c65154a;
L8c65154a: wr(m,r[5]+0x8u,r[4],4); /* mov.l r4,@(0x8,r5) */
 goto L8c65154c;
L8c65154c: wr(m,r[5]+0xcu,r[4],4); /* mov.l r4,@(0xc,r5) */
 goto L8c65154e;
L8c65154e: wr(m,r[5]+0x10u,r[4],4); /* mov.l r4,@(0x10,r5) */
 goto L8c651550;
L8c651550: wr(m,r[5]+0x14u,r[4],4); /* mov.l r4,@(0x14,r5) */
 goto L8c651552;
L8c651552: wr(m,r[5]+0x18u,r[4],4); /* mov.l r4,@(0x18,r5) */
 goto L8c651554;
L8c651554: wr(m,r[5]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r5) */
 goto L8c651556;
L8c651556: wr(m,r[5]+0x20u,r[4],4); /* mov.l r4,@(0x20,r5) */
 goto L8c651558;
L8c651558: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65155a;
L8c65155a: wr(m,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c65155c;
L8c65155c: r[3]=r[5]; /* mov r5,r3 */
 goto L8c65155e;
L8c65155e: r[3]+=44u; /* add #0x2c,r3 */
 goto L8c651560;
L8c651560: r[3]+=r[6]; /* add r6,r3 */
 goto L8c651562;
L8c651562: r[6]+=4u; /* add #0x4,r6 */
 goto L8c651564;
L8c651564: *sr=(*sr&~1u)|(r[6]>=r[7]); /* cmp/hs r7,r6 */
 goto L8c651566;
L8c651566: take=!(*sr&1u);wr(m,r[3],r[4],4);if(take)goto L8c65155c;goto L8c65156a; /* bf/s 0x8c65155c */
 goto L8c65156a;
L8c65156a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65156c;
L8c65156c: return 0; /* rts */
}
