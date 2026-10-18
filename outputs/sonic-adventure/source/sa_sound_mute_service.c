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
/* Retail final mute service 8c107bf2..8c107c22; two optional handles.
 * Lower volume service remains external. */
SA_STAGE_EXPORT int sa_sound_mute_service(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c107bf2;
L8c107bf2: r[3]=rd(m,0x8c107c78u,4); /* mov.l 0x8c107c78,r3 */
 goto L8c107bf4;
L8c107bf4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c107bf6;
L8c107bf6: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c107bf8;
L8c107bf8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c107bfa;
L8c107bfa: if((*sr&1u))goto L8c107c08; /* bt 0x8c107c08 */
 goto L8c107bfc;
L8c107bfc: r[5]=4294967292u; /* mov #-0x4,r5 */
 goto L8c107bfe;
L8c107bfe: r[1]=rd(m,0x8c107c7cu,4); /* mov.l 0x8c107c7c,r1 */
 goto L8c107c00;
L8c107c00: r[5]<<=8; /* shll8 r5 */
 goto L8c107c02;
L8c107c02: r[5]+=64u; /* add #0x40,r5 */
 goto L8c107c04;
L8c107c04: target=r[1];*pr=0x8c107c08u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c107c08;
L8c107c08: r[3]=rd(m,0x8c107c68u,4); /* mov.l 0x8c107c68,r3 */
 goto L8c107c0a;
L8c107c0a: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c107c0c;
L8c107c0c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c107c0e;
L8c107c0e: if((*sr&1u))goto L8c107c1c; /* bt 0x8c107c1c */
 goto L8c107c10;
L8c107c10: r[5]=4294967292u; /* mov #-0x4,r5 */
 goto L8c107c12;
L8c107c12: r[1]=rd(m,0x8c107c7cu,4); /* mov.l 0x8c107c7c,r1 */
 goto L8c107c14;
L8c107c14: r[5]<<=8; /* shll8 r5 */
 goto L8c107c16;
L8c107c16: r[5]+=64u; /* add #0x40,r5 */
 goto L8c107c18;
L8c107c18: target=r[1];*pr=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r1 */
L8c107c1c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c107c1e;
L8c107c1e: return 0; /* rts */
}
