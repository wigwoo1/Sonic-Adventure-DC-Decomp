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
/* Retail sound slot setup/allocation. Typed slot exhaustion requires separate verification. */
SA_STAGE_EXPORT int sa_sound_object_table_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093c74;
L8c093c74: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093c76;
L8c093c76: r[4]=rd(m,0x8c093cc0u,4); /* mov.l 0x8c093cc0,r4 */
 goto L8c093c78;
L8c093c78: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093c7a;
L8c093c7a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c093c7c;
L8c093c7c: r[12]=r[4]; /* mov r4,r12 */
 goto L8c093c7e;
L8c093c7e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093c80;
L8c093c80: r[13]=rd(m,0x8c093cd0u,4); /* mov.l 0x8c093cd0,r13 */
 goto L8c093c82;
L8c093c82: r[12]+=32u; /* add #0x20,r12 */
 goto L8c093c84;
L8c093c84: r[14]=r[4]; /* mov r4,r14 */
 goto L8c093c86;
L8c093c86: *sr=(*sr&~1u)|(r[14]>=r[12]); /* cmp/hs r12,r14 */
 goto L8c093c88;
L8c093c88: if((*sr&1u))goto L8c093c9a; /* bt 0x8c093c9a */
 goto L8c093c8a;
L8c093c8a: target=r[13];*pr=0x8c093c8eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c093c8e;
L8c093c8e: r[14]+=4u; /* add #0x4,r14 */
 goto L8c093c90;
L8c093c90: target=r[13];*pr=0x8c093c94u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c093c94;
L8c093c94: r[14]+=4u; /* add #0x4,r14 */
 goto L8c093c96;
L8c093c96: *sr=(*sr&~1u)|(r[14]>=r[12]); /* cmp/hs r12,r14 */
 goto L8c093c98;
L8c093c98: if(!(*sr&1u))goto L8c093c8a; /* bf 0x8c093c8a */
 goto L8c093c9a;
L8c093c9a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093c9c;
L8c093c9c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c093c9e;
L8c093c9e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c093ca0;
L8c093ca0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
