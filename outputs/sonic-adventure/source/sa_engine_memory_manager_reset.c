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
/* Static C recovery of retail Tikal helper 8c663080..8c663120.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_memory_manager_reset(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c663080;
L8c663080: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c663082;
L8c663082: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c663084;
L8c663084: r[13]=rd(m,0x8c663120u,4); /* mov.l 0x8c663120,r13 */
 goto L8c663086;
L8c663086: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c663088;
L8c663088: r[4]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r4 */
 goto L8c66308a;
L8c66308a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66308c;
L8c66308c: if((*sr&1u))goto L8c66309a; /* bt 0x8c66309a */
 goto L8c66308e;
L8c66308e: r[2]=rd(m,0x8c663124u,4); /* mov.l 0x8c663124,r2 */
 goto L8c663090;
L8c663090: target=r[2];*pr=0x8c663094u;r[14]=rd(m,r[4]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c663094;
L8c663094: r[4]=r[14]; /* mov r14,r4 */
 goto L8c663096;
L8c663096: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c663098;
L8c663098: if(!(*sr&1u))goto L8c66308e; /* bf 0x8c66308e */
 goto L8c66309a;
L8c66309a: r[4]=rd(m,r[13],4); /* mov.l @r13,r4 */
 goto L8c66309c;
L8c66309c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66309e;
L8c66309e: if((*sr&1u))goto L8c6630ac; /* bt 0x8c6630ac */
 goto L8c6630a0;
L8c6630a0: r[3]=rd(m,0x8c663124u,4); /* mov.l 0x8c663124,r3 */
 goto L8c6630a2;
L8c6630a2: target=r[3];*pr=0x8c6630a6u;r[14]=rd(m,r[4]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6630a6;
L8c6630a6: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6630a8;
L8c6630a8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6630aa;
L8c6630aa: if(!(*sr&1u))goto L8c6630a0; /* bf 0x8c6630a0 */
 goto L8c6630ac;
L8c6630ac: r[4]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r4 */
 goto L8c6630ae;
L8c6630ae: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6630b0;
L8c6630b0: if((*sr&1u))goto L8c6630be; /* bt 0x8c6630be */
 goto L8c6630b2;
L8c6630b2: r[2]=rd(m,0x8c663124u,4); /* mov.l 0x8c663124,r2 */
 goto L8c6630b4;
L8c6630b4: target=r[2];*pr=0x8c6630b8u;r[14]=rd(m,r[4]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6630b8;
L8c6630b8: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6630ba;
L8c6630ba: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6630bc;
L8c6630bc: if(!(*sr&1u))goto L8c6630b2; /* bf 0x8c6630b2 */
 goto L8c6630be;
L8c6630be: r[4]=rd(m,r[13]+0x14u,4); /* mov.l @(0x14,r13),r4 */
 goto L8c6630c0;
L8c6630c0: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6630c2;
L8c6630c2: if((*sr&1u))goto L8c6630d0; /* bt 0x8c6630d0 */
 goto L8c6630c4;
L8c6630c4: r[3]=rd(m,0x8c663124u,4); /* mov.l 0x8c663124,r3 */
 goto L8c6630c6;
L8c6630c6: target=r[3];*pr=0x8c6630cau;r[14]=rd(m,r[4]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6630ca;
L8c6630ca: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6630cc;
L8c6630cc: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6630ce;
L8c6630ce: if(!(*sr&1u))goto L8c6630c4; /* bf 0x8c6630c4 */
 goto L8c6630d0;
L8c6630d0: r[5]=rd(m,0x8c66312cu,4); /* mov.l 0x8c66312c,r5 */
 goto L8c6630d2;
L8c6630d2: r[14]=0u; /* mov #0x0,r14 */
 goto L8c6630d4;
L8c6630d4: r[6]=rd(m,0x8c663128u,4); /* mov.l 0x8c663128,r6 */
 goto L8c6630d6;
L8c6630d6: r[5]+=r[6]; /* add r6,r5 */
 goto L8c6630d8;
L8c6630d8: r[4]=r[6]; /* mov r6,r4 */
 goto L8c6630da;
L8c6630da: wr(m,r[4],r[14],2); /* mov.w r14,@r4 */
 goto L8c6630dc;
L8c6630dc: r[4]+=24u; /* add #0x18,r4 */
 goto L8c6630de;
L8c6630de: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c6630e0;
L8c6630e0: if(!(*sr&1u))goto L8c6630da; /* bf 0x8c6630da */
 goto L8c6630e2;
L8c6630e2: r[2]=0u; /* mov #0x0,r2 */
 goto L8c6630e4;
L8c6630e4: r[4]=r[13]; /* mov r13,r4 */
 goto L8c6630e6;
L8c6630e6: wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c6630e8;
L8c6630e8: r[3]=r[2]; /* mov r2,r3 */
 goto L8c6630ea;
L8c6630ea: wr(m,r[4]+0x4u,r[14],4); /* mov.l r14,@(0x4,r4) */
 goto L8c6630ec;
L8c6630ec: wr(m,r[4]+0x8u,r[14],4); /* mov.l r14,@(0x8,r4) */
 goto L8c6630ee;
L8c6630ee: wr(m,r[4]+0xcu,r[14],4); /* mov.l r14,@(0xc,r4) */
 goto L8c6630f0;
L8c6630f0: wr(m,r[4]+0x10u,r[2],4); /* mov.l r2,@(0x10,r4) */
 goto L8c6630f2;
L8c6630f2: r[4]+=20u; /* add #0x14,r4 */
 goto L8c6630f4;
L8c6630f4: wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c6630f6;
L8c6630f6: wr(m,r[4]+0x4u,r[14],4); /* mov.l r14,@(0x4,r4) */
 goto L8c6630f8;
L8c6630f8: wr(m,r[4]+0x8u,r[14],4); /* mov.l r14,@(0x8,r4) */
 goto L8c6630fa;
L8c6630fa: wr(m,r[4]+0xcu,r[14],4); /* mov.l r14,@(0xc,r4) */
 goto L8c6630fc;
L8c6630fc: wr(m,r[4]+0x10u,r[2],4); /* mov.l r2,@(0x10,r4) */
 goto L8c6630fe;
L8c6630fe: r[2]=rd(m,0x8c663130u,4); /* mov.l 0x8c663130,r2 */
 goto L8c663100;
L8c663100: r[4]=rd(m,r[2],4); /* mov.l @r2,r4 */
 goto L8c663102;
L8c663102: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c663104;
L8c663104: if((*sr&1u))goto L8c663112; /* bt 0x8c663112 */
 goto L8c663106;
L8c663106: r[2]=rd(m,0x8c663134u,4); /* mov.l 0x8c663134,r2 */
 goto L8c663108;
L8c663108: target=r[2];*pr=0x8c66310cu;r[13]=rd(m,r[4]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66310c;
L8c66310c: r[4]=r[13]; /* mov r13,r4 */
 goto L8c66310e;
L8c66310e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c663110;
L8c663110: if(!(*sr&1u))goto L8c663106; /* bf 0x8c663106 */
 goto L8c663112;
L8c663112: r[2]=rd(m,0x8c663130u,4); /* mov.l 0x8c663130,r2 */
 goto L8c663114;
L8c663114: r[0]=0u; /* mov #0x0,r0 */
 goto L8c663116;
L8c663116: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c663118;
L8c663118: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66311a;
L8c66311a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66311c;
L8c66311c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
