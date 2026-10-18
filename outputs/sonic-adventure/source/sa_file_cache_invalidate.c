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
/* Static C recovery of retail Tikal helper 8c5fc0d8..8c5fc336.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_cache_invalidate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fc0d8;
L8c5fc0d8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c5fc0da;
L8c5fc0da: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c5fc0dc;
L8c5fc0dc: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c5fc0de;
L8c5fc0de: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c5fc0e0;
L8c5fc0e0: r[13]=r[5]; /* mov r5,r13 */
 goto L8c5fc0e2;
L8c5fc0e2: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c5fc0e4;
L8c5fc0e4: take=!(*sr&1u);r[12]=r[4];if(take)goto L8c5fc0ee;goto L8c5fc0e8; /* bf/s 0x8c5fc0ee */
 goto L8c5fc0e8;
L8c5fc0e8: r[3]=rd(m,0x8c5fc118u,4); /* mov.l 0x8c5fc118,r3 */
 goto L8c5fc0ea;
L8c5fc0ea: target=r[3];switch(target&0x1fffffffu){case 0xc5fc32cu:goto L8c5fc32c;default:return -2;} /* jmp @r3 */
L8c5fc0ee: r[2]=rd(m,0x8c5fc120u,4); /* mov.l 0x8c5fc120,r2 */
 goto L8c5fc0f0;
L8c5fc0f0: r[14]=rd(m,0x8c5fc11cu,4); /* mov.l 0x8c5fc11c,r14 */
 goto L8c5fc0f2;
L8c5fc0f2: *sr=(*sr&~1u)|(r[13]>=r[2]); /* cmp/hs r2,r13 */
 goto L8c5fc0f4;
L8c5fc0f4: take=!(*sr&1u);r[14]&=r[12];if(take)goto L8c5fc0fe;goto L8c5fc0f8; /* bf/s 0x8c5fc0fe */
 goto L8c5fc0f8;
L8c5fc0f8: r[3]=rd(m,0x8c5fc124u,4); /* mov.l 0x8c5fc124,r3 */
 goto L8c5fc0fa;
L8c5fc0fa: target=r[3];switch(target&0x1fffffffu){case 0xc5fc13eu:goto L8c5fc13e;default:return -2;} /* jmp @r3 */
L8c5fc0fe: r[5]=31u; /* mov #0x1f,r5 */
 goto L8c5fc100;
L8c5fc100: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc102;
L8c5fc102: r[5]&=r[12]; /* and r12,r5 */
 goto L8c5fc104;
L8c5fc104: r[5]+=r[13]; /* add r13,r5 */
 goto L8c5fc106;
L8c5fc106: goto L8c5fc128; /* bra 0x8c5fc128 */
 goto L8c5fc128;
L8c5fc128: r[5]+=31u; /* add #0x1f,r5 */
 goto L8c5fc12a;
L8c5fc12a: r[5]>>=2; /* shlr2 r5 */
 goto L8c5fc12c;
L8c5fc12c: r[5]>>=2; /* shlr2 r5 */
 goto L8c5fc12e;
L8c5fc12e: *sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1; /* shlr r5 */
 goto L8c5fc130;
L8c5fc130: status=sa_sh4_cache_invalidate(ctx,r[4]);if(status)return status; /* ocbi @r4 */
 goto L8c5fc132;
L8c5fc132: r[5]--;*sr=(*sr&~1u)|(!r[5]); /* dt r5 */
 goto L8c5fc134;
L8c5fc134: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c5fc130;goto L8c5fc138; /* bf/s 0x8c5fc130 */
 goto L8c5fc138;
L8c5fc138: r[2]=rd(m,0x8c5fc158u,4); /* mov.l 0x8c5fc158,r2 */
 goto L8c5fc13a;
L8c5fc13a: target=r[2];switch(target&0x1fffffffu){case 0xc5fc32cu:goto L8c5fc32c;default:return -2;} /* jmp @r2 */
L8c5fc13e: r[0]=*sr; /* stc SR,r0 */
 goto L8c5fc140;
L8c5fc140: r[3]=sx(m,0x8c5fc156u,2); /* mov.w 0x8c5fc156,r3 */
 goto L8c5fc142;
L8c5fc142: r[0]>>=2; /* shlr2 r0 */
 goto L8c5fc144;
L8c5fc144: r[0]>>=2; /* shlr2 r0 */
 goto L8c5fc146;
L8c5fc146: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5fc148;
L8c5fc148: wr(m,r[15],r[0],2); /* mov.w r0,@r15 */
 goto L8c5fc14a;
L8c5fc14a: r[0]=*sr; /* stc SR,r0 */
 goto L8c5fc14c;
L8c5fc14c: r[0]&=r[3]; /* and r3,r0 */
 goto L8c5fc14e;
L8c5fc14e: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c5fc150;
L8c5fc150: *sr=r[0]; /* ldc r0,SR */
 goto L8c5fc152;
L8c5fc152: goto L8c5fc15c; /* bra 0x8c5fc15c */
 goto L8c5fc15c;
L8c5fc15c: r[0]=0x8c5fc16cu; /* mova 0x8c5fc16c,r0 */
 goto L8c5fc15e;
L8c5fc15e: r[1]=rd(m,0x8c5fc168u,4); /* mov.l 0x8c5fc168,r1 */
 goto L8c5fc160;
L8c5fc160: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc162;
L8c5fc162: target=r[1];switch(target&0x1fffffffu){case 0xc5fc16cu:goto L8c5fc16c;default:return -2;} /* jmp @r1 */
L8c5fc16c: r[2]=rd(m,0x8c5fc19cu,4); /* mov.l 0x8c5fc19c,r2 */
 goto L8c5fc16e;
L8c5fc16e: r[12]+=r[13]; /* add r13,r12 */
 goto L8c5fc170;
L8c5fc170: r[1]=4294967289u; /* mov #-0x7,r1 */
 goto L8c5fc172;
L8c5fc172: r[3]=rd(m,0x8c5fc198u,4); /* mov.l 0x8c5fc198,r3 */
 goto L8c5fc174;
L8c5fc174: r[4]=rd(m,r[2],4); /* mov.l @r2,r4 */
 goto L8c5fc176;
L8c5fc176: r[5]=1u; /* mov #0x1,r5 */
 goto L8c5fc178;
L8c5fc178: r[12]+=4294967295u; /* add #-0x1,r12 */
 goto L8c5fc17a;
L8c5fc17a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c5fc17c;
L8c5fc17c: r[13]>>=2; /* shlr2 r13 */
 goto L8c5fc17e;
L8c5fc17e: r[4]=(r[1]&0x80000000u)?((r[1]&31u)?r[4]>>((0u-r[1])&31u):0u):(r[4]<<(r[1]&31u)); /* shld r1,r4 */
 goto L8c5fc180;
L8c5fc180: r[13]>>=2; /* shlr2 r13 */
 goto L8c5fc182;
L8c5fc182: r[4]&=r[5]; /* and r5,r4 */
 goto L8c5fc184;
L8c5fc184: *sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1; /* shlr r13 */
 goto L8c5fc186;
L8c5fc186: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c5fc188;
L8c5fc188: r[12]&=r[3]; /* and r3,r12 */
 goto L8c5fc18a;
L8c5fc18a: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc18c;
L8c5fc18c: r[13]&=r[5]; /* and r5,r13 */
 goto L8c5fc18e;
L8c5fc18e: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc190;
L8c5fc190: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc192;
L8c5fc192: goto L8c5fc1a0; /* bra 0x8c5fc1a0 */
 goto L8c5fc1a0;
L8c5fc1a0: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc1a2;
L8c5fc1a2: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc1a4;
L8c5fc1a4: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc1a6;
L8c5fc1a6: r[0]=rd(m,0x8c5fc1c0u,4); /* mov.l 0x8c5fc1c0,r0 */
 goto L8c5fc1a8;
L8c5fc1a8: r[8]=rd(m,0x8c5fc1c4u,4); /* mov.l 0x8c5fc1c4,r8 */
 goto L8c5fc1aa;
L8c5fc1aa: r[9]=rd(m,0x8c5fc1c8u,4); /* mov.l 0x8c5fc1c8,r9 */
 goto L8c5fc1ac;
L8c5fc1ac: r[10]=rd(m,0x8c5fc1ccu,4); /* mov.l 0x8c5fc1cc,r10 */
 goto L8c5fc1ae;
L8c5fc1ae: r[2]=rd(m,0x8c5fc1d0u,4); /* mov.l 0x8c5fc1d0,r2 */
 goto L8c5fc1b0;
L8c5fc1b0: r[3]=rd(m,0x8c5fc1d4u,4); /* mov.l 0x8c5fc1d4,r3 */
 goto L8c5fc1b2;
L8c5fc1b2: r[7]=rd(m,0x8c5fc1d8u,4); /* mov.l 0x8c5fc1d8,r7 */
 goto L8c5fc1b4;
L8c5fc1b4: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc1b6;
L8c5fc1b6: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc1b8;
L8c5fc1b8: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc1ba;
L8c5fc1ba: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc1bc;
L8c5fc1bc: goto L8c5fc1fc; /* bra 0x8c5fc1fc */
 goto L8c5fc1dc;
L8c5fc1dc: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc1de;
L8c5fc1de: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc1e0;
L8c5fc1e0: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc1e2;
L8c5fc1e2: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc1e4;
L8c5fc1e4: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc1e6;
L8c5fc1e6: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc1e8;
L8c5fc1e8: if((*sr&1u))goto L8c5fc1f2; /* bt 0x8c5fc1f2 */
 goto L8c5fc1ea;
L8c5fc1ea: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc1ec;
L8c5fc1ec: if((*sr&1u))goto L8c5fc1f2; /* bt 0x8c5fc1f2 */
 goto L8c5fc1ee;
L8c5fc1ee: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc1f0;
L8c5fc1f0: status=sa_sh4_cache_invalidate(ctx,r[1]);if(status)return status; /* ocbi @r1 */
 goto L8c5fc1f2;
L8c5fc1f2: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc1f4;
L8c5fc1f4: if((*sr&1u))goto L8c5fc1fa; /* bt 0x8c5fc1fa */
 goto L8c5fc1f6;
L8c5fc1f6: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc1f8;
L8c5fc1f8: if((*sr&1u))goto L8c5fc200; /* bt 0x8c5fc200 */
 goto L8c5fc1fa;
L8c5fc1fa: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc1fc;
L8c5fc1fc: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc1fe;
L8c5fc1fe: if((*sr&1u))goto L8c5fc1dc; /* bt 0x8c5fc1dc */
 goto L8c5fc200;
L8c5fc200: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc202;
L8c5fc202: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc204;
L8c5fc204: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc206;
L8c5fc206:  /* nop */
 goto L8c5fc208;
L8c5fc208: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c5fc20a;
L8c5fc20a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c5fc20c;
L8c5fc20c: if(!(*sr&1u))goto L8c5fc214; /* bf 0x8c5fc214 */
 goto L8c5fc20e;
L8c5fc20e: r[3]=rd(m,0x8c5fc220u,4); /* mov.l 0x8c5fc220,r3 */
 goto L8c5fc210;
L8c5fc210: target=r[3];switch(target&0x1fffffffu){case 0xc5fc29eu:goto L8c5fc29e;default:return -2;} /* jmp @r3 */
L8c5fc214: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc216;
L8c5fc216: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc218;
L8c5fc218: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc21a;
L8c5fc21a: goto L8c5fc224; /* bra 0x8c5fc224 */
 goto L8c5fc224;
L8c5fc224: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5fc226;
L8c5fc226: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc228;
L8c5fc228: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc22a;
L8c5fc22a: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc22c;
L8c5fc22c: r[0]=rd(m,0x8c5fc248u,4); /* mov.l 0x8c5fc248,r0 */
 goto L8c5fc22e;
L8c5fc22e: r[8]=rd(m,0x8c5fc24cu,4); /* mov.l 0x8c5fc24c,r8 */
 goto L8c5fc230;
L8c5fc230: r[9]=rd(m,0x8c5fc250u,4); /* mov.l 0x8c5fc250,r9 */
 goto L8c5fc232;
L8c5fc232: r[10]=rd(m,0x8c5fc254u,4); /* mov.l 0x8c5fc254,r10 */
 goto L8c5fc234;
L8c5fc234: r[2]=rd(m,0x8c5fc258u,4); /* mov.l 0x8c5fc258,r2 */
 goto L8c5fc236;
L8c5fc236: r[3]=rd(m,0x8c5fc25cu,4); /* mov.l 0x8c5fc25c,r3 */
 goto L8c5fc238;
L8c5fc238: r[7]=rd(m,0x8c5fc260u,4); /* mov.l 0x8c5fc260,r7 */
 goto L8c5fc23a;
L8c5fc23a: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc23c;
L8c5fc23c: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc23e;
L8c5fc23e: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc240;
L8c5fc240: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc242;
L8c5fc242: goto L8c5fc28c; /* bra 0x8c5fc28c */
 goto L8c5fc264;
L8c5fc264: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc266;
L8c5fc266: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc268;
L8c5fc268: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc26a;
L8c5fc26a: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc26c;
L8c5fc26c: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc26e;
L8c5fc26e: r[11]=r[0]; /* mov r0,r11 */
 goto L8c5fc270;
L8c5fc270: r[11]&=r[10]; /* and r10,r11 */
 goto L8c5fc272;
L8c5fc272: r[1]|=r[11]; /* or r11,r1 */
 goto L8c5fc274;
L8c5fc274: r[1]|=r[12]; /* or r12,r1 */
 goto L8c5fc276;
L8c5fc276: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc278;
L8c5fc278: if((*sr&1u))goto L8c5fc282; /* bt 0x8c5fc282 */
 goto L8c5fc27a;
L8c5fc27a: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc27c;
L8c5fc27c: if((*sr&1u))goto L8c5fc282; /* bt 0x8c5fc282 */
 goto L8c5fc27e;
L8c5fc27e: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc280;
L8c5fc280: status=sa_sh4_cache_invalidate(ctx,r[1]);if(status)return status; /* ocbi @r1 */
 goto L8c5fc282;
L8c5fc282: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc284;
L8c5fc284: if((*sr&1u))goto L8c5fc28a; /* bt 0x8c5fc28a */
 goto L8c5fc286;
L8c5fc286: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc288;
L8c5fc288: if((*sr&1u))goto L8c5fc290; /* bt 0x8c5fc290 */
 goto L8c5fc28a;
L8c5fc28a: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc28c;
L8c5fc28c: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc28e;
L8c5fc28e: if((*sr&1u))goto L8c5fc264; /* bt 0x8c5fc264 */
 goto L8c5fc290;
L8c5fc290: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc292;
L8c5fc292: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc294;
L8c5fc294: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc296;
L8c5fc296: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5fc298;
L8c5fc298: r[2]=rd(m,0x8c5fc2a8u,4); /* mov.l 0x8c5fc2a8,r2 */
 goto L8c5fc29a;
L8c5fc29a: target=r[2];switch(target&0x1fffffffu){case 0xc5fc318u:goto L8c5fc318;default:return -2;} /* jmp @r2 */
L8c5fc29e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc2a0;
L8c5fc2a0: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc2a2;
L8c5fc2a2: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc2a4;
L8c5fc2a4: goto L8c5fc2ac; /* bra 0x8c5fc2ac */
 goto L8c5fc2ac;
L8c5fc2ac: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5fc2ae;
L8c5fc2ae: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc2b0;
L8c5fc2b0: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc2b2;
L8c5fc2b2: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc2b4;
L8c5fc2b4: r[0]=rd(m,0x8c5fc2d0u,4); /* mov.l 0x8c5fc2d0,r0 */
 goto L8c5fc2b6;
L8c5fc2b6: r[8]=rd(m,0x8c5fc2d4u,4); /* mov.l 0x8c5fc2d4,r8 */
 goto L8c5fc2b8;
L8c5fc2b8: r[9]=rd(m,0x8c5fc2d8u,4); /* mov.l 0x8c5fc2d8,r9 */
 goto L8c5fc2ba;
L8c5fc2ba: r[10]=rd(m,0x8c5fc2dcu,4); /* mov.l 0x8c5fc2dc,r10 */
 goto L8c5fc2bc;
L8c5fc2bc: r[2]=rd(m,0x8c5fc2e0u,4); /* mov.l 0x8c5fc2e0,r2 */
 goto L8c5fc2be;
L8c5fc2be: r[3]=rd(m,0x8c5fc2e4u,4); /* mov.l 0x8c5fc2e4,r3 */
 goto L8c5fc2c0;
L8c5fc2c0: r[7]=rd(m,0x8c5fc2e8u,4); /* mov.l 0x8c5fc2e8,r7 */
 goto L8c5fc2c2;
L8c5fc2c2: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc2c4;
L8c5fc2c4: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc2c6;
L8c5fc2c6: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc2c8;
L8c5fc2c8: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc2ca;
L8c5fc2ca: goto L8c5fc30c; /* bra 0x8c5fc30c */
 goto L8c5fc2ec;
L8c5fc2ec: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc2ee;
L8c5fc2ee: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc2f0;
L8c5fc2f0: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc2f2;
L8c5fc2f2: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc2f4;
L8c5fc2f4: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc2f6;
L8c5fc2f6: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc2f8;
L8c5fc2f8: if((*sr&1u))goto L8c5fc302; /* bt 0x8c5fc302 */
 goto L8c5fc2fa;
L8c5fc2fa: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc2fc;
L8c5fc2fc: if((*sr&1u))goto L8c5fc302; /* bt 0x8c5fc302 */
 goto L8c5fc2fe;
L8c5fc2fe: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc300;
L8c5fc300: status=sa_sh4_cache_invalidate(ctx,r[1]);if(status)return status; /* ocbi @r1 */
 goto L8c5fc302;
L8c5fc302: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc304;
L8c5fc304: if((*sr&1u))goto L8c5fc30a; /* bt 0x8c5fc30a */
 goto L8c5fc306;
L8c5fc306: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc308;
L8c5fc308: if((*sr&1u))goto L8c5fc310; /* bt 0x8c5fc310 */
 goto L8c5fc30a;
L8c5fc30a: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc30c;
L8c5fc30c: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc30e;
L8c5fc30e: if((*sr&1u))goto L8c5fc2ec; /* bt 0x8c5fc2ec */
 goto L8c5fc310;
L8c5fc310: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc312;
L8c5fc312: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc314;
L8c5fc314: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc316;
L8c5fc316: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5fc318;
L8c5fc318: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c5fc31a;
L8c5fc31a: r[3]=*sr; /* stc SR,r3 */
 goto L8c5fc31c;
L8c5fc31c: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c5fc31e;
L8c5fc31e: r[2]=sx(m,0x8c5fc368u,2); /* mov.w 0x8c5fc368,r2 */
 goto L8c5fc320;
L8c5fc320: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5fc322;
L8c5fc322: r[0]<<=2; /* shll2 r0 */
 goto L8c5fc324;
L8c5fc324: r[3]&=r[2]; /* and r2,r3 */
 goto L8c5fc326;
L8c5fc326: r[0]<<=2; /* shll2 r0 */
 goto L8c5fc328;
L8c5fc328: r[0]|=r[3]; /* or r3,r0 */
 goto L8c5fc32a;
L8c5fc32a: *sr=r[0]; /* ldc r0,SR */
 goto L8c5fc32c;
L8c5fc32c: r[15]+=8u; /* add #0x8,r15 */
 goto L8c5fc32e;
L8c5fc32e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c5fc330;
L8c5fc330: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c5fc332;
L8c5fc332: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
