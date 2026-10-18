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
/* Retail result/error wrapper10f90c..10f946.
 * Calls10fcbc; preserves result bits; infinity1100,NaN1101 atglobal7ac108.
 * PR/SZ0 verification scope; lower calculation remains external. */
SA_STAGE_EXPORT int sa_math_result_error_wrapper(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f90c;
L8c10f90c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10f90e;
L8c10f90e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c10f910;
L8c10f910: r[3]=rd(m,0x8c10f94cu,4); /* mov.l 0x8c10f94c,r3 */
 goto L8c10f912;
L8c10f912: target=r[3];*pr=0x8c10f916u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10f916;
L8c10f916: r[3]=r[15]; /* mov r15,r3 */
 goto L8c10f918;
L8c10f918: r[4]=rd(m,0x8c10f950u,4); /* mov.l 0x8c10f950,r4 */
 goto L8c10f91a;
L8c10f91a: wr(m,r[15],fr[0],4); /* fmov.s fr0,@r15 */
 goto L8c10f91c;
L8c10f91c: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c10f91e;
L8c10f91e: r[2]&=r[4]; /* and r4,r2 */
 goto L8c10f920;
L8c10f920: *sr=(*sr&~1u)|(r[4]==r[2]); /* cmp/eq r4,r2 */
 goto L8c10f922;
L8c10f922: if(!(*sr&1u))goto L8c10f93c; /* bf 0x8c10f93c */
 goto L8c10f924;
L8c10f924: r[2]=r[15]; /* mov r15,r2 */
 goto L8c10f926;
L8c10f926: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c10f928;
L8c10f928: r[3]=rd(m,0x8c10f954u,4); /* mov.l 0x8c10f954,r3 */
 goto L8c10f92a;
L8c10f92a: *sr=(*sr&~1u)|!(r[3]&r[1]); /* tst r3,r1 */
 goto L8c10f92c;
L8c10f92c: if((*sr&1u))goto L8c10f936; /* bt 0x8c10f936 */
 goto L8c10f92e;
L8c10f92e: r[2]=rd(m,0x8c10f958u,4); /* mov.l 0x8c10f958,r2 */
 goto L8c10f930;
L8c10f930: r[0]=sx(m,0x8c10f946u,2); /* mov.w 0x8c10f946,r0 */
 goto L8c10f932;
L8c10f932: wr(m,r[2],r[0],4);goto L8c10f93c; /* bra 0x8c10f93c */
 goto L8c10f936;
L8c10f936: r[1]=rd(m,0x8c10f958u,4); /* mov.l 0x8c10f958,r1 */
 goto L8c10f938;
L8c10f938: r[2]=sx(m,0x8c10f948u,2); /* mov.w 0x8c10f948,r2 */
 goto L8c10f93a;
L8c10f93a: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c10f93c;
L8c10f93c: fr[0]=rd(m,r[15],4); /* fmov.s @r15,fr0 */
 goto L8c10f93e;
L8c10f93e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10f940;
L8c10f940: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10f942;
L8c10f942: return 0; /* rts */
}
