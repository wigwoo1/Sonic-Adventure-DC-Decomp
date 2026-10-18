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
/* Retail audio signature detector 8c669e38..8c669e6a.
 * Native four-byte comparator connected through caller dispatch. */
SA_STAGE_EXPORT int sa_sound_snd_header_detect(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669e38;
L8c669e38: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669e3a;
L8c669e3a: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c669e3c;
L8c669e3c: r[3]=rd(m,0x8c669edcu,4); /* mov.l 0x8c669edc,r3 */
 goto L8c669e3e;
L8c669e3e: r[5]=rd(m,0x8c669ed8u,4); /* mov.l 0x8c669ed8,r5 */
 goto L8c669e40;
L8c669e40: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c669e42;
L8c669e42: target=r[3];*pr=0x8c669e46u;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669e46;
L8c669e46: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c669e48;
L8c669e48: if((*sr&1u))goto L8c669e58; /* bt 0x8c669e58 */
 goto L8c669e4a;
L8c669e4a: r[3]=rd(m,0x8c669edcu,4); /* mov.l 0x8c669edc,r3 */
 goto L8c669e4c;
L8c669e4c: r[6]=4u; /* mov #0x4,r6 */
 goto L8c669e4e;
L8c669e4e: r[5]=rd(m,0x8c669ee0u,4); /* mov.l 0x8c669ee0,r5 */
 goto L8c669e50;
L8c669e50: target=r[3];*pr=0x8c669e54u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669e54;
L8c669e54: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c669e56;
L8c669e56: if(!(*sr&1u))goto L8c669e60; /* bf 0x8c669e60 */
 goto L8c669e58;
L8c669e58: r[15]+=4u; /* add #0x4,r15 */
 goto L8c669e5a;
L8c669e5a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669e5c;
L8c669e5c: r[0]=1u;return 0; /* rts */
L8c669e60: r[0]=0u; /* mov #0x0,r0 */
 goto L8c669e62;
L8c669e62: r[15]+=4u; /* add #0x4,r15 */
 goto L8c669e64;
L8c669e64: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669e66;
L8c669e66: return 0; /* rts */
}
