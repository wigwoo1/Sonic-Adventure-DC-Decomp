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
/* Static C recovery of retail Tikal helper 8c66b504..8c66b578.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_pitch_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b504;
L8c66b504: r[2]=rd(m,0x8c66b57cu,4); /* mov.l 0x8c66b57c,r2 */
 goto L8c66b506;
L8c66b506: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b508;
L8c66b508: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b50a;
L8c66b50a: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66b50c;
L8c66b50c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66b50e;
L8c66b50e: if(!(*sr&1u))goto L8c66b516; /* bf 0x8c66b516 */
 goto L8c66b510;
L8c66b510: r[14]=rd(m,0x8c66b580u,4); /* mov.l 0x8c66b580,r14 */
 goto L8c66b512;
L8c66b512: goto L8c66b564; /* bra 0x8c66b564 */
 goto L8c66b516;
L8c66b516: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b518;
L8c66b518: if(!(*sr&1u))goto L8c66b520; /* bf 0x8c66b520 */
 goto L8c66b51a;
L8c66b51a: r[14]=rd(m,0x8c66b584u,4); /* mov.l 0x8c66b584,r14 */
 goto L8c66b51c;
L8c66b51c: goto L8c66b564; /* bra 0x8c66b564 */
 goto L8c66b520;
L8c66b520: r[3]=sx(m,0x8c66b578u,2); /* mov.w 0x8c66b578,r3 */
 goto L8c66b522;
L8c66b522: r[14]=(uint32_t)(int32_t)(int16_t)r[5]; /* exts.w r5,r14 */
 goto L8c66b524;
L8c66b524: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]); /* cmp/ge r3,r14 */
 goto L8c66b526;
L8c66b526: if(!(*sr&1u))goto L8c66b52e; /* bf 0x8c66b52e */
 goto L8c66b528;
L8c66b528: r[1]=sx(m,0x8c66b57au,2); /* mov.w 0x8c66b57a,r1 */
 goto L8c66b52a;
L8c66b52a: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[1]); /* cmp/gt r1,r14 */
 goto L8c66b52c;
L8c66b52c: if(!(*sr&1u))goto L8c66b534; /* bf 0x8c66b534 */
 goto L8c66b52e;
L8c66b52e: r[14]=rd(m,0x8c66b588u,4); /* mov.l 0x8c66b588,r14 */
 goto L8c66b530;
L8c66b530: goto L8c66b564; /* bra 0x8c66b564 */
 goto L8c66b534;
L8c66b534: r[3]=rd(m,0x8c66b590u,4); /* mov.l 0x8c66b590,r3 */
 goto L8c66b536;
L8c66b536: r[2]=40u; /* mov #0x28,r2 */
 goto L8c66b538;
L8c66b538: r[1]=rd(m,0x8c66b594u,4); /* mov.l 0x8c66b594,r1 */
 goto L8c66b53a;
L8c66b53a: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66b53c;
L8c66b53c: r[3]+=7u; /* add #0x7,r3 */
 goto L8c66b53e;
L8c66b53e: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b540;
L8c66b540: r[5]=rd(m,0x8c66b58cu,4); /* mov.l 0x8c66b58c,r5 */
 goto L8c66b542;
L8c66b542: r[2]=rd(m,0x8c66b598u,4); /* mov.l 0x8c66b598,r2 */
 goto L8c66b544;
L8c66b544: r[5]+=r[14]; /* add r14,r5 */
 goto L8c66b546;
L8c66b546: wr(m,r[1],r[0],1); /* mov.b r0,@r1 */
 goto L8c66b548;
L8c66b548: r[1]=4294967288u; /* mov #-0x8,r1 */
 goto L8c66b54a;
L8c66b54a: wr(m,r[2],r[5],1); /* mov.b r5,@r2 */
 goto L8c66b54c;
L8c66b54c: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r5 */
 goto L8c66b54e;
L8c66b54e: r[0]=rd(m,0x8c66b59cu,4); /* mov.l 0x8c66b59c,r0 */
 goto L8c66b550;
L8c66b550: wr(m,r[3],r[5],1); /* mov.b r5,@r3 */
 goto L8c66b552;
L8c66b552: r[3]=r[1]; /* mov r1,r3 */
 goto L8c66b554;
L8c66b554: r[4]=rd(m,0x8c66b590u,4); /* mov.l 0x8c66b590,r4 */
 goto L8c66b556;
L8c66b556: r[2]+=3u; /* add #0x3,r2 */
 goto L8c66b558;
L8c66b558: wr(m,r[0],r[6],1); /* mov.b r6,@r0 */
 goto L8c66b55a;
L8c66b55a: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[6]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[6]=n?(uint32_t)((int32_t)r[6]>>n):((r[6]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r6 */
 goto L8c66b55c;
L8c66b55c: r[1]=rd(m,0x8c66b5a0u,4); /* mov.l 0x8c66b5a0,r1 */
 goto L8c66b55e;
L8c66b55e: target=r[1];*pr=0x8c66b562u;wr(m,r[2],r[6],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c66b562;
L8c66b562: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b564;
L8c66b564: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b566;
L8c66b566: if((*sr&1u))goto L8c66b570; /* bt 0x8c66b570 */
 goto L8c66b568;
L8c66b568: r[2]=rd(m,0x8c66b5a8u,4); /* mov.l 0x8c66b5a8,r2 */
 goto L8c66b56a;
L8c66b56a: r[4]=rd(m,0x8c66b5a4u,4); /* mov.l 0x8c66b5a4,r4 */
 goto L8c66b56c;
L8c66b56c: target=r[2];*pr=0x8c66b570u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66b570;
L8c66b570: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b572;
L8c66b572: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b574;
L8c66b574: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
