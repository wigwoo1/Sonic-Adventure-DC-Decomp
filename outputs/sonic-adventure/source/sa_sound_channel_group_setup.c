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
/* Static C recovery of retail Tikal helper 8c669284..8c6693e2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_channel_group_setup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669284;
L8c669284: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669286;
L8c669286: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c669288;
L8c669288: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66928a;
L8c66928a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66928c;
L8c66928c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66928e;
L8c66928e: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c669290;
L8c669290: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669292;
L8c669292: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c669294;
L8c669294: r[2]=rd(m,0x8c669378u,4); /* mov.l 0x8c669378,r2 */
 goto L8c669296;
L8c669296: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c669298;
L8c669298: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66929a;
L8c66929a: take=!(*sr&1u);r[9]=r[4];if(take)goto L8c6692a4;goto L8c66929e; /* bf/s 0x8c6692a4 */
 goto L8c66929e;
L8c66929e: r[12]=rd(m,0x8c66937cu,4); /* mov.l 0x8c66937c,r12 */
 goto L8c6692a0;
L8c6692a0: goto L8c6693c2; /* bra 0x8c6693c2 */
 goto L8c6692a4;
L8c6692a4: r[0]=32u; /* mov #0x20,r0 */
 goto L8c6692a6;
L8c6692a6: r[4]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r4 */
 goto L8c6692a8;
L8c6692a8: r[10]=1u; /* mov #0x1,r10 */
 goto L8c6692aa;
L8c6692aa: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[10]); /* cmp/ge r10,r4 */
 goto L8c6692ac;
L8c6692ac: if(!(*sr&1u))goto L8c66930a; /* bf 0x8c66930a */
 goto L8c6692ae;
L8c6692ae: r[2]=2u; /* mov #0x2,r2 */
 goto L8c6692b0;
L8c6692b0: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c6692b2;
L8c6692b2: if((*sr&1u))goto L8c66930a; /* bt 0x8c66930a */
 goto L8c6692b4;
L8c6692b4: r[4]=r[15]; /* mov r15,r4 */
 goto L8c6692b6;
L8c6692b6: r[4]+=32u; /* add #0x20,r4 */
 goto L8c6692b8;
L8c6692b8: r[4]+=1u; /* add #0x1,r4 */
 goto L8c6692ba;
L8c6692ba: r[5]=3u; /* mov #0x3,r5 */
 goto L8c6692bc;
L8c6692bc: *sr=(*sr&~1u)|!(r[5]&r[4]); /* tst r5,r4 */
 goto L8c6692be;
L8c6692be: take=(*sr&1u);r[14]=0u;if(take)goto L8c6692ca;goto L8c6692c2; /* bt/s 0x8c6692ca */
 goto L8c6692c2;
L8c6692c2: r[4]=r[15]; /* mov r15,r4 */
 goto L8c6692c4;
L8c6692c4: r[4]+=32u; /* add #0x20,r4 */
 goto L8c6692c6;
L8c6692c6: r[4]+=4u;goto L8c6692d0; /* bra 0x8c6692d0 */
 goto L8c6692ca;
L8c6692ca: r[4]=r[15]; /* mov r15,r4 */
 goto L8c6692cc;
L8c6692cc: r[4]+=32u; /* add #0x20,r4 */
 goto L8c6692ce;
L8c6692ce: r[4]+=1u; /* add #0x1,r4 */
 goto L8c6692d0;
L8c6692d0: r[6]=r[14];goto L8c669312; /* bra 0x8c669312 */
 goto L8c6692d4;
L8c6692d4: r[7]=r[4]; /* mov r4,r7 */
 goto L8c6692d6;
L8c6692d6: r[7]+=4u; /* add #0x4,r7 */
 goto L8c6692d8;
L8c6692d8: r[3]=r[7]; /* mov r7,r3 */
 goto L8c6692da;
L8c6692da: *sr=(*sr&~1u)|!(r[5]&r[3]); /* tst r5,r3 */
 goto L8c6692dc;
L8c6692dc: r[13]=r[6]; /* mov r6,r13 */
 goto L8c6692de;
L8c6692de: take=(*sr&1u);r[13]+=r[11];if(take)goto L8c6692e6;goto L8c6692e2; /* bt/s 0x8c6692e6 */
 goto L8c6692e2;
L8c6692e2: r[4]+=8u;goto L8c6692e8; /* bra 0x8c6692e8 */
 goto L8c6692e6;
L8c6692e6: r[4]=r[7]; /* mov r7,r4 */
 goto L8c6692e8;
L8c6692e8: r[12]=r[4]; /* mov r4,r12 */
 goto L8c6692ea;
L8c6692ea: r[12]+=4294967292u; /* add #-0x4,r12 */
 goto L8c6692ec;
L8c6692ec: r[3]=r[12]; /* mov r12,r3 */
 goto L8c6692ee;
L8c6692ee: *sr=(*sr&~1u)|!(r[5]&r[3]); /* tst r5,r3 */
 goto L8c6692f0;
L8c6692f0: if((*sr&1u))goto L8c6692f8; /* bt 0x8c6692f8 */
 goto L8c6692f2;
L8c6692f2: r[2]=r[4]; /* mov r4,r2 */
 goto L8c6692f4;
L8c6692f4: r[2]+=4294967288u;goto L8c6692fa; /* bra 0x8c6692fa */
 goto L8c6692f8;
L8c6692f8: r[2]=r[12]; /* mov r12,r2 */
 goto L8c6692fa;
L8c6692fa: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c6692fc;
L8c6692fc: wr(m,r[13],r[1],1); /* mov.b r1,@r13 */
 goto L8c6692fe;
L8c6692fe: r[7]=sx(m,r[13],1); /* mov.b @r13,r7 */
 goto L8c669300;
L8c669300: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c669302;
L8c669302: if(!(*sr&1u))goto L8c66930a; /* bf 0x8c66930a */
 goto L8c669304;
L8c669304: r[2]=15u; /* mov #0xf,r2 */
 goto L8c669306;
L8c669306: *sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[2]); /* cmp/gt r2,r7 */
 goto L8c669308;
L8c669308: if(!(*sr&1u))goto L8c669310; /* bf 0x8c669310 */
 goto L8c66930a;
L8c66930a: r[4]=rd(m,0x8c669380u,4); /* mov.l 0x8c669380,r4 */
 goto L8c66930c;
L8c66930c: r[12]=r[4];goto L8c6693c2; /* bra 0x8c6693c2 */
 goto L8c669310;
L8c669310: r[6]+=1u; /* add #0x1,r6 */
 goto L8c669312;
L8c669312: r[0]=32u; /* mov #0x20,r0 */
 goto L8c669314;
L8c669314: r[13]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r13 */
 goto L8c669316;
L8c669316: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[13]); /* cmp/ge r13,r6 */
 goto L8c669318;
L8c669318: take=!(*sr&1u);r[11]=r[15];if(take)goto L8c6692d4;goto L8c66931c; /* bf/s 0x8c6692d4 */
 goto L8c66931c;
L8c66931c: r[5]=r[11]; /* mov r11,r5 */
 goto L8c66931e;
L8c66931e: r[4]=r[13]; /* mov r13,r4 */
 goto L8c669320;
L8c669320: r[5]+=2u; /* add #0x2,r5 */
 goto L8c669322;
L8c669322: r[4]+=r[11]; /* add r11,r4 */
 goto L8c669324;
L8c669324: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c669326;
L8c669326: if((*sr&1u))goto L8c669332; /* bt 0x8c669332 */
 goto L8c669328;
L8c669328: r[6]=4294967295u; /* mov #-0x1,r6 */
 goto L8c66932a;
L8c66932a: wr(m,r[4],r[6],1); /* mov.b r6,@r4 */
 goto L8c66932c;
L8c66932c: r[4]+=1u; /* add #0x1,r4 */
 goto L8c66932e;
L8c66932e: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c669330;
L8c669330: if(!(*sr&1u))goto L8c66932a; /* bf 0x8c66932a */
 goto L8c669332;
L8c669332: r[2]=rd(m,0x8c669384u,4); /* mov.l 0x8c669384,r2 */
 goto L8c669334;
L8c669334: r[5]=3u; /* mov #0x3,r5 */
 goto L8c669336;
L8c669336: target=r[2];*pr=0x8c66933au;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66933a;
L8c66933a: r[12]=r[0]; /* mov r0,r12 */
 goto L8c66933c;
L8c66933c: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c66933e;
L8c66933e: if(!(*sr&1u))goto L8c6693c2; /* bf 0x8c6693c2 */
 goto L8c669340;
L8c669340: r[4]=rd(m,r[9],4); /* mov.l @r9,r4 */
 goto L8c669342;
L8c669342: r[2]=44u; /* mov #0x2c,r2 */
 goto L8c669344;
L8c669344: r[3]=rd(m,0x8c669378u,4); /* mov.l 0x8c669378,r3 */
 goto L8c669346;
L8c669346: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c669348;
L8c669348: r[1]=sx(m,0x8c669376u,2); /* mov.w 0x8c669376,r1 */
 goto L8c66934a;
L8c66934a: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66934c;
L8c66934c: r[4]=(uint32_t)(int32_t)(int8_t)r[4]; /* exts.b r4,r4 */
 goto L8c66934e;
L8c66934e: *macl=(uint32_t)((int32_t)(int16_t)r[2]*(int32_t)(int16_t)r[4]); /* muls.w r2,r4 */
 goto L8c669350;
L8c669350: r[13]=rd(m,r[3],4); /* mov.l @r3,r13 */
 goto L8c669352;
L8c669352: r[0]=32u; /* mov #0x20,r0 */
 goto L8c669354;
L8c669354: r[3]=rd(m,0x8c669388u,4); /* mov.l 0x8c669388,r3 */
 goto L8c669356;
L8c669356: r[13]+=r[1]; /* add r1,r13 */
 goto L8c669358;
L8c669358: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66935a;
L8c66935a: r[13]+=r[4]; /* add r4,r13 */
 goto L8c66935c;
L8c66935c: wr(m,r[13],r[10],4); /* mov.l r10,@r13 */
 goto L8c66935e;
L8c66935e: r[0]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r0 */
 goto L8c669360;
L8c669360: wr(m,r[13]+0x4u,r[0],1); /* mov.b r0,@(0x4,r13) */
 goto L8c669362;
L8c669362: r[0]=32u; /* mov #0x20,r0 */
 goto L8c669364;
L8c669364: r[5]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r5 */
 goto L8c669366;
L8c669366: target=r[3];*pr=0x8c66936au;r[4]=rd(m,r[9],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66936a;
L8c66936a: r[2]=rd(m,0x8c66938cu,4); /* mov.l 0x8c66938c,r2 */
 goto L8c66936c;
L8c66936c: r[5]=0u; /* mov #0x0,r5 */
 goto L8c66936e;
L8c66936e: target=r[2];*pr=0x8c669372u;r[4]=rd(m,r[9],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669372;
L8c669372: r[4]=r[14];goto L8c6693ba; /* bra 0x8c6693ba */
 goto L8c669390;
L8c669390: r[0]=r[4]; /* mov r4,r0 */
 goto L8c669392;
L8c669392: r[3]=r[13]; /* mov r13,r3 */
 goto L8c669394;
L8c669394: r[2]=sx(m,r[11]+r[0],1); /* mov.b @(r0,r11),r2 */
 goto L8c669396;
L8c669396: r[3]+=7u; /* add #0x7,r3 */
 goto L8c669398;
L8c669398: r[3]+=r[4]; /* add r4,r3 */
 goto L8c66939a;
L8c66939a: r[5]=r[0]; /* mov r0,r5 */
 goto L8c66939c;
L8c66939c: wr(m,r[3],r[2],1); /* mov.b r2,@r3 */
 goto L8c66939e;
L8c66939e: r[3]=r[13]; /* mov r13,r3 */
 goto L8c6693a0;
L8c6693a0: r[3]+=12u; /* add #0xc,r3 */
 goto L8c6693a2;
L8c6693a2: r[2]=r[13]; /* mov r13,r2 */
 goto L8c6693a4;
L8c6693a4: r[5]<<=2; /* shll2 r5 */
 goto L8c6693a6;
L8c6693a6: r[3]+=r[5]; /* add r5,r3 */
 goto L8c6693a8;
L8c6693a8: wr(m,r[3],r[10],4); /* mov.l r10,@r3 */
 goto L8c6693aa;
L8c6693aa: r[3]=r[13]; /* mov r13,r3 */
 goto L8c6693ac;
L8c6693ac: r[2]+=20u; /* add #0x14,r2 */
 goto L8c6693ae;
L8c6693ae: r[3]+=36u; /* add #0x24,r3 */
 goto L8c6693b0;
L8c6693b0: r[2]+=r[5]; /* add r5,r2 */
 goto L8c6693b2;
L8c6693b2: r[5]+=r[3]; /* add r3,r5 */
 goto L8c6693b4;
L8c6693b4: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c6693b6;
L8c6693b6: r[4]+=1u; /* add #0x1,r4 */
 goto L8c6693b8;
L8c6693b8: wr(m,r[5],r[14],4); /* mov.l r14,@r5 */
 goto L8c6693ba;
L8c6693ba: r[0]=32u; /* mov #0x20,r0 */
 goto L8c6693bc;
L8c6693bc: r[2]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r2 */
 goto L8c6693be;
L8c6693be: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c6693c0;
L8c6693c0: if(!(*sr&1u))goto L8c669390; /* bf 0x8c669390 */
 goto L8c6693c2;
L8c6693c2: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c6693c4;
L8c6693c4: if((*sr&1u))goto L8c6693ce; /* bt 0x8c6693ce */
 goto L8c6693c6;
L8c6693c6: r[3]=rd(m,0x8c6693e8u,4); /* mov.l 0x8c6693e8,r3 */
 goto L8c6693c8;
L8c6693c8: r[4]=rd(m,0x8c6693e4u,4); /* mov.l 0x8c6693e4,r4 */
 goto L8c6693ca;
L8c6693ca: target=r[3];*pr=0x8c6693ceu;r[5]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6693ce;
L8c6693ce: r[0]=r[12]; /* mov r12,r0 */
 goto L8c6693d0;
L8c6693d0: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6693d2;
L8c6693d2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6693d4;
L8c6693d4: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6693d6;
L8c6693d6: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6693d8;
L8c6693d8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6693da;
L8c6693da: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6693dc;
L8c6693dc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6693de;
L8c6693de: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
