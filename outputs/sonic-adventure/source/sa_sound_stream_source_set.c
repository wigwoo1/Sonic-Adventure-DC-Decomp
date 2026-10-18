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
/* Retail stream source setter66771c..66776e, explicit SH4 ABI.
 * Verification scope: sound_stream_source_set_verification.json. */
SA_STAGE_EXPORT int sa_sound_stream_source_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66771c;
L8c66771c: r[0]=sx(m,0x8c6677c0u,2); /* mov.w 0x8c6677c0,r0 */
 goto L8c66771e;
L8c66771e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667720;
L8c667720: r[3]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r3 */
 goto L8c667722;
L8c667722: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c667724;
L8c667724: if(!(*sr&1u))goto L8c667736; /* bf 0x8c667736 */
 goto L8c667726;
L8c667726: r[0]=80u; /* mov #0x50,r0 */
 goto L8c667728;
L8c667728: r[3]=rd(m,0x8c6677ccu,4); /* mov.l 0x8c6677cc,r3 */
 goto L8c66772a;
L8c66772a: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c66772c;
L8c66772c: r[0]=sx(m,r[4]+0xfu,1); /* mov.b @(0xf,r4),r0 */
 goto L8c66772e;
L8c66772e: target=r[3];*pr=0x8c667732u;r[1]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667732;
L8c667732: goto L8c667756; /* bra 0x8c667756 */
 goto L8c667736;
L8c667736: r[0]=80u; /* mov #0x50,r0 */
 goto L8c667738;
L8c667738: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c66773a;
L8c66773a: r[0]=sx(m,r[4]+0xdu,1); /* mov.b @(0xd,r4),r0 */
 goto L8c66773c;
L8c66773c: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c66773e;
L8c66773e: if((*sr&1u))goto L8c667742; /* bt 0x8c667742 */
 goto L8c667740;
L8c667740: r[0]+=7u; /* add #0x7,r0 */
 goto L8c667742;
L8c667742: *sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u); /* shar r0 */
 goto L8c667744;
L8c667744: r[2]=rd(m,0x8c6677ccu,4); /* mov.l 0x8c6677cc,r2 */
 goto L8c667746;
L8c667746: *sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u); /* shar r0 */
 goto L8c667748;
L8c667748: *sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u); /* shar r0 */
 goto L8c66774a;
L8c66774a: r[3]=r[0]; /* mov r0,r3 */
 goto L8c66774c;
L8c66774c: r[0]=sx(m,r[4]+0xeu,1); /* mov.b @(0xe,r4),r0 */
 goto L8c66774e;
L8c66774e: *macl=r[0]*r[3]; /* mul.l r0,r3 */
 goto L8c667750;
L8c667750: r[0]=*macl; /* sts MACL,r0 */
 goto L8c667752;
L8c667752: target=r[2];*pr=0x8c667756u;r[1]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667756;
L8c667756: r[1]=84u; /* mov #0x54,r1 */
 goto L8c667758;
L8c667758: r[1]+=r[4]; /* add r4,r1 */
 goto L8c66775a;
L8c66775a: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c66775c;
L8c66775c: r[5]=0u; /* mov #0x0,r5 */
 goto L8c66775e;
L8c66775e: r[0]=sx(m,0x8c6677c4u,2); /* mov.w 0x8c6677c4,r0 */
 goto L8c667760;
L8c667760: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c667762;
L8c667762: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c667764;
L8c667764: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c667766;
L8c667766: r[0]+=4u; /* add #0x4,r0 */
 goto L8c667768;
L8c667768: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66776a;
L8c66776a: wr(m,r[4]+r[0],r[5],4);return 0; /* rts */
}
