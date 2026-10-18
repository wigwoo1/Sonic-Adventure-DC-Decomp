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
/* Static C recovery of retail Tikal helper 8c5fc336..8c5fc58a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_cache_purge(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fc336;
L8c5fc336: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c5fc338;
L8c5fc338: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c5fc33a;
L8c5fc33a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c5fc33c;
L8c5fc33c: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c5fc33e;
L8c5fc33e: r[13]=r[5]; /* mov r5,r13 */
 goto L8c5fc340;
L8c5fc340: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c5fc342;
L8c5fc342: take=!(*sr&1u);r[12]=r[4];if(take)goto L8c5fc34c;goto L8c5fc346; /* bf/s 0x8c5fc34c */
 goto L8c5fc346;
L8c5fc346: r[3]=rd(m,0x8c5fc36cu,4); /* mov.l 0x8c5fc36c,r3 */
 goto L8c5fc348;
L8c5fc348: target=r[3];switch(target&0x1fffffffu){case 0xc5fc580u:goto L8c5fc580;default:return -2;} /* jmp @r3 */
L8c5fc34c: r[2]=rd(m,0x8c5fc374u,4); /* mov.l 0x8c5fc374,r2 */
 goto L8c5fc34e;
L8c5fc34e: r[14]=rd(m,0x8c5fc370u,4); /* mov.l 0x8c5fc370,r14 */
 goto L8c5fc350;
L8c5fc350: *sr=(*sr&~1u)|(r[13]>=r[2]); /* cmp/hs r2,r13 */
 goto L8c5fc352;
L8c5fc352: take=!(*sr&1u);r[14]&=r[12];if(take)goto L8c5fc35c;goto L8c5fc356; /* bf/s 0x8c5fc35c */
 goto L8c5fc356;
L8c5fc356: r[3]=rd(m,0x8c5fc378u,4); /* mov.l 0x8c5fc378,r3 */
 goto L8c5fc358;
L8c5fc358: target=r[3];switch(target&0x1fffffffu){case 0xc5fc392u:goto L8c5fc392;default:return -2;} /* jmp @r3 */
L8c5fc35c: r[5]=31u; /* mov #0x1f,r5 */
 goto L8c5fc35e;
L8c5fc35e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc360;
L8c5fc360: r[5]&=r[12]; /* and r12,r5 */
 goto L8c5fc362;
L8c5fc362: r[5]+=r[13]; /* add r13,r5 */
 goto L8c5fc364;
L8c5fc364: goto L8c5fc37c; /* bra 0x8c5fc37c */
 goto L8c5fc37c;
L8c5fc37c: r[5]+=31u; /* add #0x1f,r5 */
 goto L8c5fc37e;
L8c5fc37e: r[5]>>=2; /* shlr2 r5 */
 goto L8c5fc380;
L8c5fc380: r[5]>>=2; /* shlr2 r5 */
 goto L8c5fc382;
L8c5fc382: *sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1; /* shlr r5 */
 goto L8c5fc384;
L8c5fc384: status=sa_sh4_cache_purge(ctx,r[4]);if(status)return status; /* ocbp @r4 */
 goto L8c5fc386;
L8c5fc386: r[5]--;*sr=(*sr&~1u)|(!r[5]); /* dt r5 */
 goto L8c5fc388;
L8c5fc388: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c5fc384;goto L8c5fc38c; /* bf/s 0x8c5fc384 */
 goto L8c5fc38c;
L8c5fc38c: r[2]=rd(m,0x8c5fc3acu,4); /* mov.l 0x8c5fc3ac,r2 */
 goto L8c5fc38e;
L8c5fc38e: target=r[2];switch(target&0x1fffffffu){case 0xc5fc580u:goto L8c5fc580;default:return -2;} /* jmp @r2 */
L8c5fc392: r[0]=*sr; /* stc SR,r0 */
 goto L8c5fc394;
L8c5fc394: r[3]=sx(m,0x8c5fc3aau,2); /* mov.w 0x8c5fc3aa,r3 */
 goto L8c5fc396;
L8c5fc396: r[0]>>=2; /* shlr2 r0 */
 goto L8c5fc398;
L8c5fc398: r[0]>>=2; /* shlr2 r0 */
 goto L8c5fc39a;
L8c5fc39a: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5fc39c;
L8c5fc39c: wr(m,r[15],r[0],2); /* mov.w r0,@r15 */
 goto L8c5fc39e;
L8c5fc39e: r[0]=*sr; /* stc SR,r0 */
 goto L8c5fc3a0;
L8c5fc3a0: r[0]&=r[3]; /* and r3,r0 */
 goto L8c5fc3a2;
L8c5fc3a2: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c5fc3a4;
L8c5fc3a4: *sr=r[0]; /* ldc r0,SR */
 goto L8c5fc3a6;
L8c5fc3a6: goto L8c5fc3b0; /* bra 0x8c5fc3b0 */
 goto L8c5fc3b0;
L8c5fc3b0: r[0]=0x8c5fc3c0u; /* mova 0x8c5fc3c0,r0 */
 goto L8c5fc3b2;
L8c5fc3b2: r[1]=rd(m,0x8c5fc3bcu,4); /* mov.l 0x8c5fc3bc,r1 */
 goto L8c5fc3b4;
L8c5fc3b4: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc3b6;
L8c5fc3b6: target=r[1];switch(target&0x1fffffffu){case 0xc5fc3c0u:goto L8c5fc3c0;default:return -2;} /* jmp @r1 */
L8c5fc3c0: r[2]=rd(m,0x8c5fc3f0u,4); /* mov.l 0x8c5fc3f0,r2 */
 goto L8c5fc3c2;
L8c5fc3c2: r[12]+=r[13]; /* add r13,r12 */
 goto L8c5fc3c4;
L8c5fc3c4: r[1]=4294967289u; /* mov #-0x7,r1 */
 goto L8c5fc3c6;
L8c5fc3c6: r[3]=rd(m,0x8c5fc3ecu,4); /* mov.l 0x8c5fc3ec,r3 */
 goto L8c5fc3c8;
L8c5fc3c8: r[4]=rd(m,r[2],4); /* mov.l @r2,r4 */
 goto L8c5fc3ca;
L8c5fc3ca: r[5]=1u; /* mov #0x1,r5 */
 goto L8c5fc3cc;
L8c5fc3cc: r[12]+=4294967295u; /* add #-0x1,r12 */
 goto L8c5fc3ce;
L8c5fc3ce: r[13]=r[4]; /* mov r4,r13 */
 goto L8c5fc3d0;
L8c5fc3d0: r[13]>>=2; /* shlr2 r13 */
 goto L8c5fc3d2;
L8c5fc3d2: r[4]=(r[1]&0x80000000u)?((r[1]&31u)?r[4]>>((0u-r[1])&31u):0u):(r[4]<<(r[1]&31u)); /* shld r1,r4 */
 goto L8c5fc3d4;
L8c5fc3d4: r[13]>>=2; /* shlr2 r13 */
 goto L8c5fc3d6;
L8c5fc3d6: r[4]&=r[5]; /* and r5,r4 */
 goto L8c5fc3d8;
L8c5fc3d8: *sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1; /* shlr r13 */
 goto L8c5fc3da;
L8c5fc3da: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c5fc3dc;
L8c5fc3dc: r[12]&=r[3]; /* and r3,r12 */
 goto L8c5fc3de;
L8c5fc3de: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc3e0;
L8c5fc3e0: r[13]&=r[5]; /* and r5,r13 */
 goto L8c5fc3e2;
L8c5fc3e2: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc3e4;
L8c5fc3e4: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc3e6;
L8c5fc3e6: goto L8c5fc3f4; /* bra 0x8c5fc3f4 */
 goto L8c5fc3f4;
L8c5fc3f4: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc3f6;
L8c5fc3f6: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc3f8;
L8c5fc3f8: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc3fa;
L8c5fc3fa: r[0]=rd(m,0x8c5fc414u,4); /* mov.l 0x8c5fc414,r0 */
 goto L8c5fc3fc;
L8c5fc3fc: r[8]=rd(m,0x8c5fc418u,4); /* mov.l 0x8c5fc418,r8 */
 goto L8c5fc3fe;
L8c5fc3fe: r[9]=rd(m,0x8c5fc41cu,4); /* mov.l 0x8c5fc41c,r9 */
 goto L8c5fc400;
L8c5fc400: r[10]=rd(m,0x8c5fc420u,4); /* mov.l 0x8c5fc420,r10 */
 goto L8c5fc402;
L8c5fc402: r[2]=rd(m,0x8c5fc424u,4); /* mov.l 0x8c5fc424,r2 */
 goto L8c5fc404;
L8c5fc404: r[3]=rd(m,0x8c5fc428u,4); /* mov.l 0x8c5fc428,r3 */
 goto L8c5fc406;
L8c5fc406: r[7]=rd(m,0x8c5fc42cu,4); /* mov.l 0x8c5fc42c,r7 */
 goto L8c5fc408;
L8c5fc408: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc40a;
L8c5fc40a: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc40c;
L8c5fc40c: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc40e;
L8c5fc40e: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc410;
L8c5fc410: goto L8c5fc450; /* bra 0x8c5fc450 */
 goto L8c5fc430;
L8c5fc430: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc432;
L8c5fc432: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc434;
L8c5fc434: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc436;
L8c5fc436: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc438;
L8c5fc438: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc43a;
L8c5fc43a: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc43c;
L8c5fc43c: if((*sr&1u))goto L8c5fc446; /* bt 0x8c5fc446 */
 goto L8c5fc43e;
L8c5fc43e: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc440;
L8c5fc440: if((*sr&1u))goto L8c5fc446; /* bt 0x8c5fc446 */
 goto L8c5fc442;
L8c5fc442: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc444;
L8c5fc444: status=sa_sh4_cache_purge(ctx,r[1]);if(status)return status; /* ocbp @r1 */
 goto L8c5fc446;
L8c5fc446: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc448;
L8c5fc448: if((*sr&1u))goto L8c5fc44e; /* bt 0x8c5fc44e */
 goto L8c5fc44a;
L8c5fc44a: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc44c;
L8c5fc44c: if((*sr&1u))goto L8c5fc454; /* bt 0x8c5fc454 */
 goto L8c5fc44e;
L8c5fc44e: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc450;
L8c5fc450: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc452;
L8c5fc452: if((*sr&1u))goto L8c5fc430; /* bt 0x8c5fc430 */
 goto L8c5fc454;
L8c5fc454: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc456;
L8c5fc456: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc458;
L8c5fc458: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc45a;
L8c5fc45a:  /* nop */
 goto L8c5fc45c;
L8c5fc45c: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c5fc45e;
L8c5fc45e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c5fc460;
L8c5fc460: if(!(*sr&1u))goto L8c5fc468; /* bf 0x8c5fc468 */
 goto L8c5fc462;
L8c5fc462: r[3]=rd(m,0x8c5fc474u,4); /* mov.l 0x8c5fc474,r3 */
 goto L8c5fc464;
L8c5fc464: target=r[3];switch(target&0x1fffffffu){case 0xc5fc4f2u:goto L8c5fc4f2;default:return -2;} /* jmp @r3 */
L8c5fc468: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc46a;
L8c5fc46a: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc46c;
L8c5fc46c: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc46e;
L8c5fc46e: goto L8c5fc478; /* bra 0x8c5fc478 */
 goto L8c5fc478;
L8c5fc478: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5fc47a;
L8c5fc47a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc47c;
L8c5fc47c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc47e;
L8c5fc47e: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc480;
L8c5fc480: r[0]=rd(m,0x8c5fc49cu,4); /* mov.l 0x8c5fc49c,r0 */
 goto L8c5fc482;
L8c5fc482: r[8]=rd(m,0x8c5fc4a0u,4); /* mov.l 0x8c5fc4a0,r8 */
 goto L8c5fc484;
L8c5fc484: r[9]=rd(m,0x8c5fc4a4u,4); /* mov.l 0x8c5fc4a4,r9 */
 goto L8c5fc486;
L8c5fc486: r[10]=rd(m,0x8c5fc4a8u,4); /* mov.l 0x8c5fc4a8,r10 */
 goto L8c5fc488;
L8c5fc488: r[2]=rd(m,0x8c5fc4acu,4); /* mov.l 0x8c5fc4ac,r2 */
 goto L8c5fc48a;
L8c5fc48a: r[3]=rd(m,0x8c5fc4b0u,4); /* mov.l 0x8c5fc4b0,r3 */
 goto L8c5fc48c;
L8c5fc48c: r[7]=rd(m,0x8c5fc4b4u,4); /* mov.l 0x8c5fc4b4,r7 */
 goto L8c5fc48e;
L8c5fc48e: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc490;
L8c5fc490: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc492;
L8c5fc492: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc494;
L8c5fc494: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc496;
L8c5fc496: goto L8c5fc4e0; /* bra 0x8c5fc4e0 */
 goto L8c5fc4b8;
L8c5fc4b8: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc4ba;
L8c5fc4ba: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc4bc;
L8c5fc4bc: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc4be;
L8c5fc4be: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc4c0;
L8c5fc4c0: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc4c2;
L8c5fc4c2: r[11]=r[0]; /* mov r0,r11 */
 goto L8c5fc4c4;
L8c5fc4c4: r[11]&=r[10]; /* and r10,r11 */
 goto L8c5fc4c6;
L8c5fc4c6: r[1]|=r[11]; /* or r11,r1 */
 goto L8c5fc4c8;
L8c5fc4c8: r[1]|=r[12]; /* or r12,r1 */
 goto L8c5fc4ca;
L8c5fc4ca: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc4cc;
L8c5fc4cc: if((*sr&1u))goto L8c5fc4d6; /* bt 0x8c5fc4d6 */
 goto L8c5fc4ce;
L8c5fc4ce: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc4d0;
L8c5fc4d0: if((*sr&1u))goto L8c5fc4d6; /* bt 0x8c5fc4d6 */
 goto L8c5fc4d2;
L8c5fc4d2: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc4d4;
L8c5fc4d4: status=sa_sh4_cache_purge(ctx,r[1]);if(status)return status; /* ocbp @r1 */
 goto L8c5fc4d6;
L8c5fc4d6: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc4d8;
L8c5fc4d8: if((*sr&1u))goto L8c5fc4de; /* bt 0x8c5fc4de */
 goto L8c5fc4da;
L8c5fc4da: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc4dc;
L8c5fc4dc: if((*sr&1u))goto L8c5fc4e4; /* bt 0x8c5fc4e4 */
 goto L8c5fc4de;
L8c5fc4de: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc4e0;
L8c5fc4e0: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc4e2;
L8c5fc4e2: if((*sr&1u))goto L8c5fc4b8; /* bt 0x8c5fc4b8 */
 goto L8c5fc4e4;
L8c5fc4e4: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc4e6;
L8c5fc4e6: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc4e8;
L8c5fc4e8: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc4ea;
L8c5fc4ea: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5fc4ec;
L8c5fc4ec: r[2]=rd(m,0x8c5fc4fcu,4); /* mov.l 0x8c5fc4fc,r2 */
 goto L8c5fc4ee;
L8c5fc4ee: target=r[2];switch(target&0x1fffffffu){case 0xc5fc56cu:goto L8c5fc56c;default:return -2;} /* jmp @r2 */
L8c5fc4f2: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc4f4;
L8c5fc4f4: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc4f6;
L8c5fc4f6: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc4f8;
L8c5fc4f8: goto L8c5fc500; /* bra 0x8c5fc500 */
 goto L8c5fc500;
L8c5fc500: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5fc502;
L8c5fc502: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc504;
L8c5fc504: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc506;
L8c5fc506: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc508;
L8c5fc508: r[0]=rd(m,0x8c5fc524u,4); /* mov.l 0x8c5fc524,r0 */
 goto L8c5fc50a;
L8c5fc50a: r[8]=rd(m,0x8c5fc528u,4); /* mov.l 0x8c5fc528,r8 */
 goto L8c5fc50c;
L8c5fc50c: r[9]=rd(m,0x8c5fc52cu,4); /* mov.l 0x8c5fc52c,r9 */
 goto L8c5fc50e;
L8c5fc50e: r[10]=rd(m,0x8c5fc530u,4); /* mov.l 0x8c5fc530,r10 */
 goto L8c5fc510;
L8c5fc510: r[2]=rd(m,0x8c5fc534u,4); /* mov.l 0x8c5fc534,r2 */
 goto L8c5fc512;
L8c5fc512: r[3]=rd(m,0x8c5fc538u,4); /* mov.l 0x8c5fc538,r3 */
 goto L8c5fc514;
L8c5fc514: r[7]=rd(m,0x8c5fc53cu,4); /* mov.l 0x8c5fc53c,r7 */
 goto L8c5fc516;
L8c5fc516: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc518;
L8c5fc518: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc51a;
L8c5fc51a: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc51c;
L8c5fc51c: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc51e;
L8c5fc51e: goto L8c5fc560; /* bra 0x8c5fc560 */
 goto L8c5fc540;
L8c5fc540: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc542;
L8c5fc542: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc544;
L8c5fc544: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc546;
L8c5fc546: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc548;
L8c5fc548: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc54a;
L8c5fc54a: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc54c;
L8c5fc54c: if((*sr&1u))goto L8c5fc556; /* bt 0x8c5fc556 */
 goto L8c5fc54e;
L8c5fc54e: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc550;
L8c5fc550: if((*sr&1u))goto L8c5fc556; /* bt 0x8c5fc556 */
 goto L8c5fc552;
L8c5fc552: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc554;
L8c5fc554: status=sa_sh4_cache_purge(ctx,r[1]);if(status)return status; /* ocbp @r1 */
 goto L8c5fc556;
L8c5fc556: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc558;
L8c5fc558: if((*sr&1u))goto L8c5fc55e; /* bt 0x8c5fc55e */
 goto L8c5fc55a;
L8c5fc55a: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc55c;
L8c5fc55c: if((*sr&1u))goto L8c5fc564; /* bt 0x8c5fc564 */
 goto L8c5fc55e;
L8c5fc55e: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc560;
L8c5fc560: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc562;
L8c5fc562: if((*sr&1u))goto L8c5fc540; /* bt 0x8c5fc540 */
 goto L8c5fc564;
L8c5fc564: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc566;
L8c5fc566: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc568;
L8c5fc568: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc56a;
L8c5fc56a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5fc56c;
L8c5fc56c: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c5fc56e;
L8c5fc56e: r[3]=*sr; /* stc SR,r3 */
 goto L8c5fc570;
L8c5fc570: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c5fc572;
L8c5fc572: r[2]=sx(m,0x8c5fc5bcu,2); /* mov.w 0x8c5fc5bc,r2 */
 goto L8c5fc574;
L8c5fc574: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5fc576;
L8c5fc576: r[0]<<=2; /* shll2 r0 */
 goto L8c5fc578;
L8c5fc578: r[3]&=r[2]; /* and r2,r3 */
 goto L8c5fc57a;
L8c5fc57a: r[0]<<=2; /* shll2 r0 */
 goto L8c5fc57c;
L8c5fc57c: r[0]|=r[3]; /* or r3,r0 */
 goto L8c5fc57e;
L8c5fc57e: *sr=r[0]; /* ldc r0,SR */
 goto L8c5fc580;
L8c5fc580: r[15]+=8u; /* add #0x8,r15 */
 goto L8c5fc582;
L8c5fc582: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c5fc584;
L8c5fc584: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c5fc586;
L8c5fc586: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
