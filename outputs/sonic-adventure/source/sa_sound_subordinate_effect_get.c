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
/* Retail sound-object query 8c65de7a..8c65ded4. */
SA_STAGE_EXPORT int sa_sound_subordinate_effect_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65de7a;
L8c65de7a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65de7c;
L8c65de7c: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65de7e;
L8c65de7e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65de80;
L8c65de80: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c65de82;
L8c65de82: take=!(*sr&1u);wr(m,r[15],r[6],4);if(take)goto L8c65debc;goto L8c65de86; /* bf/s 0x8c65debc */
 goto L8c65de86;
L8c65de86: r[2]=rd(m,0x8c65deb0u,4); /* mov.l 0x8c65deb0,r2 */
 goto L8c65de88;
L8c65de88: r[4]=rd(m,0x8c65deacu,4); /* mov.l 0x8c65deac,r4 */
 goto L8c65de8a;
L8c65de8a: target=r[2];*pr=0x8c65de8eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65de8e;
L8c65de8e: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65de90;
L8c65de90: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c65de92;
L8c65de92: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65de94;
L8c65de94: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65de96;
L8c65de96: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65de98;
L8c65de98: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65de9a;
L8c65de9a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65de9c;
L8c65de9c: return 0; /* rts */
L8c65debc: r[0]=120u; /* mov #0x78,r0 */
 goto L8c65debe;
L8c65debe: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65dec0;
L8c65dec0: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c65dec2;
L8c65dec2: r[0]=124u; /* mov #0x7c,r0 */
 goto L8c65dec4;
L8c65dec4: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65dec6;
L8c65dec6: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c65dec8;
L8c65dec8: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65deca;
L8c65deca: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65decc;
L8c65decc: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65dece;
L8c65dece: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ded0;
L8c65ded0: return 0; /* rts */
}
