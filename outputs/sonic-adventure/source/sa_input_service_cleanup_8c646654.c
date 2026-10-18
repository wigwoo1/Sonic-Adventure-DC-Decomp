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
/* Static C recovery of retail Tikal helper 8c646654..8c6467f0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_service_cleanup_8c646654(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c646654;
L8c646654: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c646656;
L8c646656: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c646658;
L8c646658: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c64665a;
L8c64665a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c64665c;
L8c64665c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c64665e;
L8c64665e: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c646660;
L8c646660: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c646662;
L8c646662: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c646664;
L8c646664: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c646666;
L8c646666: r[2]=rd(m,0x8c646738u,4); /* mov.l 0x8c646738,r2 */
 goto L8c646668;
L8c646668: r[10]=1u; /* mov #0x1,r10 */
 goto L8c64666a;
L8c64666a: r[3]=rd(m,0x8c646734u,4); /* mov.l 0x8c646734,r3 */
 goto L8c64666c;
L8c64666c: r[13]=0u; /* mov #0x0,r13 */
 goto L8c64666e;
L8c64666e: r[8]=rd(m,0x8c64672cu,4); /* mov.l 0x8c64672c,r8 */
 goto L8c646670;
L8c646670: r[9]=rd(m,0x8c646730u,4); /* mov.l 0x8c646730,r9 */
 goto L8c646672;
L8c646672: target=r[2];*pr=0x8c646676u;wr(m,r[3],r[10],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c646676;
L8c646676: *sr=(*sr&~1u)|(r[0]==0xfffffffeu); /* cmp/eq #-0x2,r0 */
 goto L8c646678;
L8c646678: take=!(*sr&1u);r[12]=r[0];if(take)goto L8c646684;goto L8c64667c; /* bf/s 0x8c646684 */
 goto L8c64667c;
L8c64667c: r[0]=sx(m,0x8c646726u,2); /* mov.w 0x8c646726,r0 */
 goto L8c64667e;
L8c64667e: r[2]=rd(m,r[8],4); /* mov.l @r8,r2 */
 goto L8c646680;
L8c646680: wr(m,r[2]+r[0],r[10],4);goto L8c6467da; /* bra 0x8c6467da */
 goto L8c646684;
L8c646684: *sr=(*sr&~1u)|((int32_t)r[12]>=0); /* cmp/pz r12 */
 goto L8c646686;
L8c646686: if((*sr&1u))goto L8c64668c; /* bt 0x8c64668c */
 goto L8c646688;
L8c646688: goto L8c6467c4; /* bra 0x8c6467c4 */
 goto L8c64668c;
L8c64668c: r[3]=rd(m,0x8c64673cu,4); /* mov.l 0x8c64673c,r3 */
 goto L8c64668e;
L8c64668e: target=r[3];*pr=0x8c646692u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c646692;
L8c646692: r[2]=rd(m,0x8c646740u,4); /* mov.l 0x8c646740,r2 */
 goto L8c646694;
L8c646694: r[3]=r[9]; /* mov r9,r3 */
 goto L8c646696;
L8c646696: r[3]+=48u; /* add #0x30,r3 */
 goto L8c646698;
L8c646698: r[0]=rd(m,0x8c646744u,4); /* mov.l 0x8c646744,r0 */
 goto L8c64669a;
L8c64669a: r[4]=rd(m,r[2],4); /* mov.l @r2,r4 */
 goto L8c64669c;
L8c64669c: r[5]=sx(m,0x8c646728u,2); /* mov.w 0x8c646728,r5 */
 goto L8c64669e;
L8c64669e: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c6466a0;
L8c6466a0: r[3]+=r[4]; /* add r4,r3 */
 goto L8c6466a2;
L8c6466a2: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6466a4;
L8c6466a4: r[1]=rd(m,r[0],4); /* mov.l @r0,r1 */
 goto L8c6466a6;
L8c6466a6: r[2]=r[9]; /* mov r9,r2 */
 goto L8c6466a8;
L8c6466a8: r[3]=rd(m,0x8c646748u,4); /* mov.l 0x8c646748,r3 */
 goto L8c6466aa;
L8c6466aa: r[2]+=52u; /* add #0x34,r2 */
 goto L8c6466ac;
L8c6466ac: r[1]&=r[5]; /* and r5,r1 */
 goto L8c6466ae;
L8c6466ae: r[2]+=r[4]; /* add r4,r2 */
 goto L8c6466b0;
L8c6466b0: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c6466b2;
L8c6466b2: r[2]=sx(m,r[2],2); /* mov.w @r2,r2 */
 goto L8c6466b4;
L8c6466b4: r[0]&=r[5]; /* and r5,r0 */
 goto L8c6466b6;
L8c6466b6: *macl=(r[0]&65535u)*(r[2]&65535u); /* mulu.w r0,r2 */
 goto L8c6466b8;
L8c6466b8: r[0]=*macl; /* sts MACL,r0 */
 goto L8c6466ba;
L8c6466ba: r[1]+=r[0]; /* add r0,r1 */
 goto L8c6466bc;
L8c6466bc: r[0]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r0 */
 goto L8c6466be;
L8c6466be: *pr=0x8c6466c2u;wr(m,r[0],r[1],2);status=call(ctx,0x8c6461aeu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6461ae */
 goto L8c6466c2;
L8c6466c2: r[3]=rd(m,0x8c64674cu,4); /* mov.l 0x8c64674c,r3 */
 goto L8c6466c4;
L8c6466c4: target=r[3];*pr=0x8c6466c8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6466c8;
L8c6466c8: r[2]=rd(m,0x8c646750u,4); /* mov.l 0x8c646750,r2 */
 goto L8c6466ca;
L8c6466ca: target=r[2];*pr=0x8c6466ceu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6466ce;
L8c6466ce: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6466d0;
L8c6466d0: if((*sr&1u))goto L8c646720; /* bt 0x8c646720 */
 goto L8c6466d2;
L8c6466d2: r[2]=rd(m,0x8c646754u,4); /* mov.l 0x8c646754,r2 */
 goto L8c6466d4;
L8c6466d4: r[14]=rd(m,r[2],4); /* mov.l @r2,r14 */
 goto L8c6466d6;
L8c6466d6: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6466d8;
L8c6466d8: if((*sr&1u))goto L8c6466e2; /* bt 0x8c6466e2 */
 goto L8c6466da;
L8c6466da: target=r[14];*pr=0x8c6466deu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c6466de;
L8c6466de: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6466e0;
L8c6466e0: if((*sr&1u))goto L8c6466ea; /* bt 0x8c6466ea */
 goto L8c6466e2;
L8c6466e2: *pr=0x8c6466e6u;status=call(ctx,0x8c64658au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c64658a */
 goto L8c6466e6;
L8c6466e6: r[1]=rd(m,0x8c646758u,4); /* mov.l 0x8c646758,r1 */
 goto L8c6466e8;
L8c6466e8: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c6466ea;
L8c6466ea: r[0]=sx(m,0x8c64672au,2); /* mov.w 0x8c64672a,r0 */
 goto L8c6466ec;
L8c6466ec: r[3]=rd(m,r[8],4); /* mov.l @r8,r3 */
 goto L8c6466ee;
L8c6466ee: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c6466f0;
L8c6466f0: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6466f2;
L8c6466f2: if((*sr&1u))goto L8c646720; /* bt 0x8c646720 */
 goto L8c6466f4;
L8c6466f4: r[1]=rd(m,0x8c646758u,4); /* mov.l 0x8c646758,r1 */
 goto L8c6466f6;
L8c6466f6: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c6466f8;
L8c6466f8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6466fa;
L8c6466fa: if((*sr&1u))goto L8c646720; /* bt 0x8c646720 */
 goto L8c6466fc;
L8c6466fc: r[4]=rd(m,0x8c64675cu,4); /* mov.l 0x8c64675c,r4 */
 goto L8c6466fe;
L8c6466fe: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c646700;
L8c646700: r[2]=rd(m,0x8c646760u,4); /* mov.l 0x8c646760,r2 */
 goto L8c646702;
L8c646702: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c646704;
L8c646704: r[3]=sx(m,0x8c64672au,2); /* mov.w 0x8c64672a,r3 */
 goto L8c646706;
L8c646706: r[0]=rd(m,r[8],4); /* mov.l @r8,r0 */
 goto L8c646708;
L8c646708: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c64670a;
L8c64670a: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c64670c;
L8c64670c: r[3]=rd(m,0x8c646764u,4); /* mov.l 0x8c646764,r3 */
 goto L8c64670e;
L8c64670e: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c646710;
L8c646710: r[0]|=0x8u; /* or #0x8,r0 */
 goto L8c646712;
L8c646712: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c646714;
L8c646714: r[4]=rd(m,0x8c64676cu,4); /* mov.l 0x8c64676c,r4 */
 goto L8c646716;
L8c646716: r[1]=rd(m,0x8c646768u,4); /* mov.l 0x8c646768,r1 */
 goto L8c646718;
L8c646718: wr(m,r[1],r[10],4); /* mov.l r10,@r1 */
 goto L8c64671a;
L8c64671a: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c64671c;
L8c64671c: r[3]+=1u; /* add #0x1,r3 */
 goto L8c64671e;
L8c64671e: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c646720;
L8c646720: r[14]=rd(m,0x8c646770u,4); /* mov.l 0x8c646770,r14 */
 goto L8c646722;
L8c646722: r[11]=r[13];goto L8c6467a8; /* bra 0x8c6467a8 */
 goto L8c646774;
L8c646774: r[3]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r3 */
 goto L8c646776;
L8c646776: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c646778;
L8c646778: if((*sr&1u))goto L8c6467a4; /* bt 0x8c6467a4 */
 goto L8c64677a;
L8c64677a: r[5]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r5 */
 goto L8c64677c;
L8c64677c: *pr=0x8c646780u;r[4]=rd(m,r[14],4);status=call(ctx,0x8c646884u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c646884 */
 goto L8c646780;
L8c646780: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c646782;
L8c646782: if((*sr&1u))goto L8c6467a4; /* bt 0x8c6467a4 */
 goto L8c646784;
L8c646784: r[3]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r3 */
 goto L8c646786;
L8c646786: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c646788;
L8c646788: wr(m,r[14]+0x8u,r[13],4); /* mov.l r13,@(0x8,r14) */
 goto L8c64678a;
L8c64678a: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c64678c;
L8c64678c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c64678e;
L8c64678e: *pr=0x8c646792u;r[4]=rd(m,r[14],4);status=call(ctx,0x8c646868u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c646868 */
 goto L8c646792;
L8c646792: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c646794;
L8c646794: *pr=0x8c646798u;r[4]=rd(m,r[14],4);status=call(ctx,0x8c646856u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c646856 */
 goto L8c646798;
L8c646798: r[7]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r7 */
 goto L8c64679a;
L8c64679a: r[6]=r[0]; /* mov r0,r6 */
 goto L8c64679c;
L8c64679c: r[5]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r5 */
 goto L8c64679e;
L8c64679e: r[1]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r1 */
 goto L8c6467a0;
L8c6467a0: target=r[1];*pr=0x8c6467a4u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6467a4;
L8c6467a4: r[14]+=12u; /* add #0xc,r14 */
 goto L8c6467a6;
L8c6467a6: r[11]+=1u; /* add #0x1,r11 */
 goto L8c6467a8;
L8c6467a8: r[3]=48u; /* mov #0x30,r3 */
 goto L8c6467aa;
L8c6467aa: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]); /* cmp/ge r3,r11 */
 goto L8c6467ac;
L8c6467ac: if(!(*sr&1u))goto L8c646774; /* bf 0x8c646774 */
 goto L8c6467ae;
L8c6467ae: r[3]=rd(m,0x8c646894u,4); /* mov.l 0x8c646894,r3 */
 goto L8c6467b0;
L8c6467b0: r[0]=sx(m,0x8c64688au,2); /* mov.w 0x8c64688a,r0 */
 goto L8c6467b2;
L8c6467b2: r[1]=rd(m,r[8],4); /* mov.l @r8,r1 */
 goto L8c6467b4;
L8c6467b4: wr(m,r[1]+r[0],r[13],4); /* mov.l r13,@(r0,r1) */
 goto L8c6467b6;
L8c6467b6: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c6467b8;
L8c6467b8: r[2]^=r[10]; /* xor r10,r2 */
 goto L8c6467ba;
L8c6467ba: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c6467bc;
L8c6467bc: r[3]=rd(m,r[9]+0x4u,4); /* mov.l @(0x4,r9),r3 */
 goto L8c6467be;
L8c6467be: r[3]+=1u; /* add #0x1,r3 */
 goto L8c6467c0;
L8c6467c0: wr(m,r[9]+0x4u,r[3],4);goto L8c6467d6; /* bra 0x8c6467d6 */
 goto L8c6467c4;
L8c6467c4: r[1]=rd(m,0x8c646898u,4); /* mov.l 0x8c646898,r1 */
 goto L8c6467c6;
L8c6467c6: r[14]=rd(m,r[1],4); /* mov.l @r1,r14 */
 goto L8c6467c8;
L8c6467c8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6467ca;
L8c6467ca: if((*sr&1u))goto L8c6467d6; /* bt 0x8c6467d6 */
 goto L8c6467cc;
L8c6467cc: r[6]=rd(m,r[9]+0x8u,4); /* mov.l @(0x8,r9),r6 */
 goto L8c6467ce;
L8c6467ce: r[5]=4294967288u; /* mov #-0x8,r5 */
 goto L8c6467d0;
L8c6467d0: r[7]=r[12]; /* mov r12,r7 */
 goto L8c6467d2;
L8c6467d2: target=r[14];*pr=0x8c6467d6u;r[4]=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c6467d6;
L8c6467d6: r[2]=rd(m,0x8c64689cu,4); /* mov.l 0x8c64689c,r2 */
 goto L8c6467d8;
L8c6467d8: wr(m,r[2],r[13],4); /* mov.l r13,@r2 */
 goto L8c6467da;
L8c6467da: r[0]=r[12]; /* mov r12,r0 */
 goto L8c6467dc;
L8c6467dc: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6467de;
L8c6467de: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6467e0;
L8c6467e0: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c6467e2;
L8c6467e2: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6467e4;
L8c6467e4: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6467e6;
L8c6467e6: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6467e8;
L8c6467e8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6467ea;
L8c6467ea: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6467ec;
L8c6467ec: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
