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
/* Static C recovery of retail Tikal helper 8c6691d0..8c669240.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_descriptor_byte_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6691d0;
L8c6691d0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6691d2;
L8c6691d2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6691d4;
L8c6691d4: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c6691d6;
L8c6691d6: r[3]=rd(m,0x8c669244u,4); /* mov.l 0x8c669244,r3 */
 goto L8c6691d8;
L8c6691d8: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c6691da;
L8c6691da: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c6691dc;
L8c6691dc: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c6691de;
L8c6691de: take=!(*sr&1u);r[14]=0u;if(take)goto L8c6691e8;goto L8c6691e2; /* bf/s 0x8c6691e8 */
 goto L8c6691e2;
L8c6691e2: r[14]=rd(m,0x8c669248u,4); /* mov.l 0x8c669248,r14 */
 goto L8c6691e4;
L8c6691e4: goto L8c66922a; /* bra 0x8c66922a */
 goto L8c6691e8;
L8c6691e8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6691ea;
L8c6691ea: if(!(*sr&1u))goto L8c6691f2; /* bf 0x8c6691f2 */
 goto L8c6691ec;
L8c6691ec: r[14]=rd(m,0x8c66924cu,4); /* mov.l 0x8c66924c,r14 */
 goto L8c6691ee;
L8c6691ee: goto L8c66922a; /* bra 0x8c66922a */
 goto L8c6691f2;
L8c6691f2: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c6691f4;
L8c6691f4: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6691f6;
L8c6691f6: if(!(*sr&1u))goto L8c6691fe; /* bf 0x8c6691fe */
 goto L8c6691f8;
L8c6691f8: r[14]=rd(m,0x8c669250u,4); /* mov.l 0x8c669250,r14 */
 goto L8c6691fa;
L8c6691fa: goto L8c66922a; /* bra 0x8c66922a */
 goto L8c6691fe;
L8c6691fe: r[0]=sx(m,0x8c669240u,2); /* mov.w 0x8c669240,r0 */
 goto L8c669200;
L8c669200: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c669202;
L8c669202: r[3]=sx(m,0x8c669242u,2); /* mov.w 0x8c669242,r3 */
 goto L8c669204;
L8c669204: r[5]<<=2; /* shll2 r5 */
 goto L8c669206;
L8c669206: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c669208;
L8c669208: r[5]<<=2; /* shll2 r5 */
 goto L8c66920a;
L8c66920a: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66920c;
L8c66920c: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66920e;
L8c66920e: r[3]+=r[2]; /* add r2,r3 */
 goto L8c669210;
L8c669210: r[2]=rd(m,0x8c669254u,4); /* mov.l 0x8c669254,r2 */
 goto L8c669212;
L8c669212: r[0]<<=2; /* shll2 r0 */
 goto L8c669214;
L8c669214: r[0]<<=2; /* shll2 r0 */
 goto L8c669216;
L8c669216: r[0]<<=2; /* shll2 r0 */
 goto L8c669218;
L8c669218: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66921a;
L8c66921a: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66921c;
L8c66921c: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66921e;
L8c66921e: target=r[2];*pr=0x8c669222u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669222;
L8c669222: r[2]=sx(m,r[15],1); /* mov.b @r15,r2 */
 goto L8c669224;
L8c669224: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c669226;
L8c669226: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669228;
L8c669228: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66922a;
L8c66922a: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66922c;
L8c66922c: if((*sr&1u))goto L8c669236; /* bt 0x8c669236 */
 goto L8c66922e;
L8c66922e: r[2]=rd(m,0x8c66925cu,4); /* mov.l 0x8c66925c,r2 */
 goto L8c669230;
L8c669230: r[4]=rd(m,0x8c669258u,4); /* mov.l 0x8c669258,r4 */
 goto L8c669232;
L8c669232: target=r[2];*pr=0x8c669236u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669236;
L8c669236: r[0]=r[14]; /* mov r14,r0 */
 goto L8c669238;
L8c669238: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66923a;
L8c66923a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66923c;
L8c66923c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
