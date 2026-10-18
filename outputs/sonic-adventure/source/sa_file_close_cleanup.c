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
/* Static C recovery of retail File close cleanup helper 8c6013d2..8c601500.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_close_cleanup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6013d2;
L8c6013d2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6013d4;
L8c6013d4: r[14]=r[4]; /* mov r4,r14 */
 goto L8c6013d6;
L8c6013d6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6013d8;
L8c6013d8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6013da;
L8c6013da: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6013dc;
L8c6013dc: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6013de;
L8c6013de: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6013e0;
L8c6013e0: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6013e2;
L8c6013e2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6013e4;
L8c6013e4: take=(*sr&1u);r[13]=0u;if(take)goto L8c6013f0;goto L8c6013e8; /* bt/s 0x8c6013f0 */
 goto L8c6013e8;
L8c6013e8: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c6013ea;
L8c6013ea: r[3]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r3 */
 goto L8c6013ec;
L8c6013ec: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6013ee;
L8c6013ee: if(!(*sr&1u))goto L8c6013f4; /* bf 0x8c6013f4 */
 goto L8c6013f0;
L8c6013f0: r[0]=4294967286u;goto L8c6014f0; /* bra 0x8c6014f0 */
 goto L8c6013f4;
L8c6013f4: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6013f6;
L8c6013f6: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c6013f8;
L8c6013f8: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c6013fa;
L8c6013fa: take=(*sr&1u);r[4]=r[0];if(take)goto L8c601404;goto L8c6013fe; /* bt/s 0x8c601404 */
 goto L8c6013fe;
L8c6013fe: r[0]=r[4]; /* mov r4,r0 */
 goto L8c601400;
L8c601400: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c601402;
L8c601402: if(!(*sr&1u))goto L8c60142c; /* bf 0x8c60142c */
 goto L8c601404;
L8c601404: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c601406;
L8c601406: r[3]=rd(m,r[2]+0x1cu,4); /* mov.l @(0x1c,r2),r3 */
 goto L8c601408;
L8c601408: *sr=(*sr&~1u)|(r[14]==r[3]); /* cmp/eq r14,r3 */
 goto L8c60140a;
L8c60140a: if((*sr&1u))goto L8c601412; /* bt 0x8c601412 */
 goto L8c60140c;
L8c60140c: r[0]=4294967283u;goto L8c6014f0; /* bra 0x8c6014f0 */
 goto L8c601412;
L8c601412: r[12]=rd(m,0x8c60151cu,4); /* mov.l 0x8c60151c,r12 */
 goto L8c601414;
L8c601414: target=r[12];*pr=0x8c601418u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c601418;
L8c601418: r[10]=r[0];goto L8c601456; /* bra 0x8c601456 */
 goto L8c60141c;
L8c60141c: r[0]=80u; /* mov #0x50,r0 */
 goto L8c60141e;
L8c60141e: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c601420;
L8c601420: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c601422;
L8c601422: take=(*sr&1u);r[4]=r[0];if(take)goto L8c601434;goto L8c601426; /* bt/s 0x8c601434 */
 goto L8c601426;
L8c601426: r[0]=r[4]; /* mov r4,r0 */
 goto L8c601428;
L8c601428: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c60142a;
L8c60142a: if((*sr&1u))goto L8c601434; /* bt 0x8c601434 */
 goto L8c60142c;
L8c60142c: r[0]=80u; /* mov #0x50,r0 */
 goto L8c60142e;
L8c60142e: wr(m,r[14]+r[0],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c601430;
L8c601430: r[0]=0u;goto L8c6014f0; /* bra 0x8c6014f0 */
 goto L8c601434;
L8c601434: target=r[12];*pr=0x8c601438u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c601438;
L8c601438: r[5]=r[0]; /* mov r0,r5 */
 goto L8c60143a;
L8c60143a: target=r[11];*pr=0x8c60143eu;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c60143e;
L8c60143e: r[5]=rd(m,0x8c601520u,4); /* mov.l 0x8c601520,r5 */
 goto L8c601440;
L8c601440: r[4]=r[0]; /* mov r0,r4 */
 goto L8c601442;
L8c601442: *sr=(*sr&~1u)|(r[4]>r[5]); /* cmp/hi r5,r4 */
 goto L8c601444;
L8c601444: if(!(*sr&1u))goto L8c60144a; /* bf 0x8c60144a */
 goto L8c601446;
L8c601446: r[0]=4294967275u;goto L8c6014f0; /* bra 0x8c6014f0 */
 goto L8c60144a;
L8c60144a: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c60144c;
L8c60144c: r[0]=72u; /* mov #0x48,r0 */
 goto L8c60144e;
L8c60144e: r[3]=rd(m,r[2]+0x3cu,4); /* mov.l @(0x3c,r2),r3 */
 goto L8c601450;
L8c601450: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c601452;
L8c601452: target=r[1];*pr=0x8c601456u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c601456;
L8c601456: r[11]=rd(m,0x8c601524u,4); /* mov.l 0x8c601524,r11 */
 goto L8c601458;
L8c601458: r[0]=64u; /* mov #0x40,r0 */
 goto L8c60145a;
L8c60145a: r[0]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r0 */
 goto L8c60145c;
L8c60145c: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c60145e;
L8c60145e: if(!(*sr&1u))goto L8c60141c; /* bf 0x8c60141c */
 goto L8c601460;
L8c601460: target=r[12];*pr=0x8c601464u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c601464;
L8c601464: r[10]=sx(m,0x8c601518u,2); /* mov.w 0x8c601518,r10 */
 goto L8c601466;
L8c601466: r[9]=r[0]; /* mov r0,r9 */
 goto L8c601468;
L8c601468: target=r[12];*pr=0x8c60146cu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c60146c;
L8c60146c: r[5]=r[0]; /* mov r0,r5 */
 goto L8c60146e;
L8c60146e: target=r[11];*pr=0x8c601472u;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c601472;
L8c601472: r[4]=r[0]; /* mov r0,r4 */
 goto L8c601474;
L8c601474: *sr=(*sr&~1u)|(r[4]>=r[10]); /* cmp/hs r10,r4 */
 goto L8c601476;
L8c601476: if(!(*sr&1u))goto L8c601468; /* bf 0x8c601468 */
 goto L8c601478;
L8c601478: r[0]=*sr; /* stc SR,r0 */
 goto L8c60147a;
L8c60147a: r[3]=sx(m,0x8c60151au,2); /* mov.w 0x8c60151a,r3 */
 goto L8c60147c;
L8c60147c: r[0]>>=2; /* shlr2 r0 */
 goto L8c60147e;
L8c60147e: r[0]>>=2; /* shlr2 r0 */
 goto L8c601480;
L8c601480: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c601482;
L8c601482: r[12]=r[0]; /* mov r0,r12 */
 goto L8c601484;
L8c601484: r[0]=*sr; /* stc SR,r0 */
 goto L8c601486;
L8c601486: r[0]&=r[3]; /* and r3,r0 */
 goto L8c601488;
L8c601488: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c60148a;
L8c60148a: *sr=r[0]; /* ldc r0,SR */
 goto L8c60148c;
L8c60148c: r[0]=80u; /* mov #0x50,r0 */
 goto L8c60148e;
L8c60148e: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c601490;
L8c601490: r[5]=4294967283u; /* mov #-0xd,r5 */
 goto L8c601492;
L8c601492: r[11]=3u; /* mov #0x3,r11 */
 goto L8c601494;
L8c601494: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c601496;
L8c601496: take=(*sr&1u);r[4]=r[0];if(take)goto L8c6014a8;goto L8c60149a; /* bt/s 0x8c6014a8 */
 goto L8c60149a;
L8c60149a: r[0]=r[4]; /* mov r4,r0 */
 goto L8c60149c;
L8c60149c: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c60149e;
L8c60149e: if((*sr&1u))goto L8c6014a8; /* bt 0x8c6014a8 */
 goto L8c6014a0;
L8c6014a0: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6014a2;
L8c6014a2: wr(m,r[14]+r[0],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c6014a4;
L8c6014a4: r[4]=r[13];goto L8c6014dc; /* bra 0x8c6014dc */
 goto L8c6014a8;
L8c6014a8: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c6014aa;
L8c6014aa: r[3]=rd(m,r[2]+0x1cu,4); /* mov.l @(0x1c,r2),r3 */
 goto L8c6014ac;
L8c6014ac: *sr=(*sr&~1u)|(r[14]==r[3]); /* cmp/eq r14,r3 */
 goto L8c6014ae;
L8c6014ae: if((*sr&1u))goto L8c6014b4; /* bt 0x8c6014b4 */
 goto L8c6014b0;
L8c6014b0: r[4]=r[5];goto L8c6014dc; /* bra 0x8c6014dc */
 goto L8c6014b4;
L8c6014b4: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c6014b6;
L8c6014b6: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6014b8;
L8c6014b8: r[1]=rd(m,r[3]+0x14u,4); /* mov.l @(0x14,r3),r1 */
 goto L8c6014ba;
L8c6014ba: target=r[1];*pr=0x8c6014beu;r[4]=rd(m,r[14]+0x3cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6014be;
L8c6014be: r[0]=72u; /* mov #0x48,r0 */
 goto L8c6014c0;
L8c6014c0: r[2]=sx(m,0x8c60151au,2); /* mov.w 0x8c60151a,r2 */
 goto L8c6014c2;
L8c6014c2: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c6014c4;
L8c6014c4: r[0]=r[12]; /* mov r12,r0 */
 goto L8c6014c6;
L8c6014c6: r[3]=*sr; /* stc SR,r3 */
 goto L8c6014c8;
L8c6014c8: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c6014ca;
L8c6014ca: r[0]<<=2; /* shll2 r0 */
 goto L8c6014cc;
L8c6014cc: r[3]&=r[2]; /* and r2,r3 */
 goto L8c6014ce;
L8c6014ce: r[0]<<=2; /* shll2 r0 */
 goto L8c6014d0;
L8c6014d0: r[0]|=r[3]; /* or r3,r0 */
 goto L8c6014d2;
L8c6014d2: *sr=r[0]; /* ldc r0,SR */
 goto L8c6014d4;
L8c6014d4: *pr=0x8c6014d8u;r[4]=r[14];status=call(ctx,0x8c600cf0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c600cf0 */
 goto L8c6014d8;
L8c6014d8: r[4]=r[0];goto L8c6014ee; /* bra 0x8c6014ee */
 goto L8c6014dc;
L8c6014dc: r[3]=*sr; /* stc SR,r3 */
 goto L8c6014de;
L8c6014de: r[0]=r[12]; /* mov r12,r0 */
 goto L8c6014e0;
L8c6014e0: r[2]=sx(m,0x8c60151au,2); /* mov.w 0x8c60151a,r2 */
 goto L8c6014e2;
L8c6014e2: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c6014e4;
L8c6014e4: r[0]<<=2; /* shll2 r0 */
 goto L8c6014e6;
L8c6014e6: r[3]&=r[2]; /* and r2,r3 */
 goto L8c6014e8;
L8c6014e8: r[0]<<=2; /* shll2 r0 */
 goto L8c6014ea;
L8c6014ea: r[0]|=r[3]; /* or r3,r0 */
 goto L8c6014ec;
L8c6014ec: *sr=r[0]; /* ldc r0,SR */
 goto L8c6014ee;
L8c6014ee: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6014f0;
L8c6014f0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6014f2;
L8c6014f2: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6014f4;
L8c6014f4: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6014f6;
L8c6014f6: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6014f8;
L8c6014f8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6014fa;
L8c6014fa: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6014fc;
L8c6014fc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
