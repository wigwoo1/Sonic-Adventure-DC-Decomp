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
/* Static C recovery of USA Rev A event-indexed registered interrupt callback traversal. */
SA_STAGE_EXPORT int sa_interrupt_event_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c645afa;
L8c645afa: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c645afc;
L8c645afc: r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645afe;
L8c645afe: r[14]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r14 */
 goto L8c645b00;
L8c645b00: r[0]=rd(m,0x8c645b60u,4); /* mov.l 0x8c645b60,r0 */
 goto L8c645b02;
L8c645b02: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c645b04;
L8c645b04: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[14]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[14]=n?(uint32_t)((int32_t)r[14]>>n):((r[14]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r14 */
 goto L8c645b06;
L8c645b06: r[14]<<=2; /* shll2 r14 */
 goto L8c645b08;
L8c645b08: r[14]=rd(m,r[14]+r[0],4);goto L8c645b14; /* bra 0x8c645b14 */
 goto L8c645b0c;
L8c645b0c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c645b0e;
L8c645b0e: target=r[3];*pr=0x8c645b12u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c645b12;
L8c645b12: r[14]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r14 */
 goto L8c645b14;
L8c645b14: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c645b16;
L8c645b16: if(!(*sr&1u))goto L8c645b0c; /* bf 0x8c645b0c */
 goto L8c645b18;
L8c645b18: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645b1a;
L8c645b1a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
