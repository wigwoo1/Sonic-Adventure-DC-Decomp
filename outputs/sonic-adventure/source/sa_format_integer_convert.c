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
/* Static C recovery of retail Tikal helper 8c10e304..8c10e50e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_format_integer_convert(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e304;
L8c10e304: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10e306;
L8c10e306: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10e308;
L8c10e308: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10e30a;
L8c10e30a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10e30c;
L8c10e30c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c10e30e;
L8c10e30e: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c10e310;
L8c10e310: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c10e312;
L8c10e312: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10e314;
L8c10e314: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c10e316;
L8c10e316: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e318;
L8c10e318: *sr=(*sr&~1u)|(r[0]==0x00000064u); /* cmp/eq #0x64,r0 */
 goto L8c10e31a;
L8c10e31a: r[10]=r[5]; /* mov r5,r10 */
 goto L8c10e31c;
L8c10e31c: r[9]=0u; /* mov #0x0,r9 */
 goto L8c10e31e;
L8c10e31e: take=(*sr&1u);wr(m,r[10],r[9],1);if(take)goto L8c10e33a;goto L8c10e322; /* bt/s 0x8c10e33a */
 goto L8c10e322;
L8c10e322: *sr=(*sr&~1u)|(r[0]==0x00000069u); /* cmp/eq #0x69,r0 */
 goto L8c10e324;
L8c10e324: if((*sr&1u))goto L8c10e33a; /* bt 0x8c10e33a */
 goto L8c10e326;
L8c10e326: *sr=(*sr&~1u)|(r[0]==0x00000075u); /* cmp/eq #0x75,r0 */
 goto L8c10e328;
L8c10e328: if((*sr&1u))goto L8c10e33a; /* bt 0x8c10e33a */
 goto L8c10e32a;
L8c10e32a: *sr=(*sr&~1u)|(r[0]==0x0000006fu); /* cmp/eq #0x6f,r0 */
 goto L8c10e32c;
L8c10e32c: if((*sr&1u))goto L8c10e33e; /* bt 0x8c10e33e */
 goto L8c10e32e;
L8c10e32e: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10e330;
L8c10e330: if((*sr&1u))goto L8c10e342; /* bt 0x8c10e342 */
 goto L8c10e332;
L8c10e332: *sr=(*sr&~1u)|(r[0]==0x00000058u); /* cmp/eq #0x58,r0 */
 goto L8c10e334;
L8c10e334: if((*sr&1u))goto L8c10e342; /* bt 0x8c10e342 */
 goto L8c10e336;
L8c10e336: goto L8c10e344; /* bra 0x8c10e344 */
 goto L8c10e33a;
L8c10e33a: r[8]=10u;goto L8c10e344; /* bra 0x8c10e344 */
 goto L8c10e33e;
L8c10e33e: r[8]=8u;goto L8c10e344; /* bra 0x8c10e344 */
 goto L8c10e342;
L8c10e342: r[8]=16u; /* mov #0x10,r8 */
 goto L8c10e344;
L8c10e344: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e346;
L8c10e346: *sr=(*sr&~1u)|(r[0]==0x00000064u); /* cmp/eq #0x64,r0 */
 goto L8c10e348;
L8c10e348: if((*sr&1u))goto L8c10e350; /* bt 0x8c10e350 */
 goto L8c10e34a;
L8c10e34a: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e34c;
L8c10e34c: *sr=(*sr&~1u)|(r[0]==0x00000069u); /* cmp/eq #0x69,r0 */
 goto L8c10e34e;
L8c10e34e: if(!(*sr&1u))goto L8c10e358; /* bf 0x8c10e358 */
 goto L8c10e350;
L8c10e350: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c10e352;
L8c10e352: if((*sr&1u))goto L8c10e358; /* bt 0x8c10e358 */
 goto L8c10e354;
L8c10e354: r[12]=0u-r[4];goto L8c10e35a; /* bra 0x8c10e35a */
 goto L8c10e358;
L8c10e358: r[12]=r[4]; /* mov r4,r12 */
 goto L8c10e35a;
L8c10e35a: r[14]=r[15]; /* mov r15,r14 */
 goto L8c10e35c;
L8c10e35c: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c10e35e;
L8c10e35e: r[13]=r[9]; /* mov r9,r13 */
 goto L8c10e360;
L8c10e360: r[11]=r[9]; /* mov r9,r11 */
 goto L8c10e362;
L8c10e362: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c10e374;goto L8c10e366; /* bf/s 0x8c10e374 */
 goto L8c10e366;
L8c10e366: r[3]=rd(m,r[7]+0x8u,4); /* mov.l @(0x8,r7),r3 */
 goto L8c10e368;
L8c10e368: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c10e36a;
L8c10e36a: if((*sr&1u))goto L8c10e374; /* bt 0x8c10e374 */
 goto L8c10e36c;
L8c10e36c: r[3]=48u; /* mov #0x30,r3 */
 goto L8c10e36e;
L8c10e36e: wr(m,r[14],r[3],1); /* mov.b r3,@r14 */
 goto L8c10e370;
L8c10e370: r[13]=1u;goto L8c10e3ce; /* bra 0x8c10e3ce */
 goto L8c10e374;
L8c10e374: r[3]=0u; /* mov #0x0,r3 */
 goto L8c10e376;
L8c10e376: *sr=(*sr&~1u)|(r[12]>r[3]); /* cmp/hi r3,r12 */
 goto L8c10e378;
L8c10e378: if(!(*sr&1u))goto L8c10e3ce; /* bf 0x8c10e3ce */
 goto L8c10e37a;
L8c10e37a: r[1]=r[13]; /* mov r13,r1 */
 goto L8c10e37c;
L8c10e37c: r[1]+=r[14]; /* add r14,r1 */
 goto L8c10e37e;
L8c10e37e: r[3]=rd(m,0x8c10e498u,4); /* mov.l 0x8c10e498,r3 */
 goto L8c10e380;
L8c10e380: r[2]=r[1]; /* mov r1,r2 */
 goto L8c10e382;
L8c10e382: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c10e384;
L8c10e384: r[1]=r[12]; /* mov r12,r1 */
 goto L8c10e386;
L8c10e386: target=r[3];*pr=0x8c10e38au;r[0]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10e38a;
L8c10e38a: wr(m,r[2],r[0],1); /* mov.b r0,@r2 */
 goto L8c10e38c;
L8c10e38c: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10e38e;
L8c10e38e: r[2]=sx(m,r[3],1); /* mov.b @r3,r2 */
 goto L8c10e390;
L8c10e390: r[3]=9u; /* mov #0x9,r3 */
 goto L8c10e392;
L8c10e392: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]); /* cmp/gt r3,r2 */
 goto L8c10e394;
L8c10e394: if((*sr&1u))goto L8c10e3a2; /* bt 0x8c10e3a2 */
 goto L8c10e396;
L8c10e396: r[0]=r[13]; /* mov r13,r0 */
 goto L8c10e398;
L8c10e398: r[2]=sx(m,r[14]+r[0],1); /* mov.b @(r0,r14),r2 */
 goto L8c10e39a;
L8c10e39a: r[2]+=48u; /* add #0x30,r2 */
 goto L8c10e39c;
L8c10e39c: wr(m,r[14]+r[0],r[2],1); /* mov.b r2,@(r0,r14) */
 goto L8c10e39e;
L8c10e39e: goto L8c10e3bc; /* bra 0x8c10e3bc */
 goto L8c10e3a2;
L8c10e3a2: r[1]=r[13]; /* mov r13,r1 */
 goto L8c10e3a4;
L8c10e3a4: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e3a6;
L8c10e3a6: r[1]+=r[14]; /* add r14,r1 */
 goto L8c10e3a8;
L8c10e3a8: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10e3aa;
L8c10e3aa: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c10e3ac;
L8c10e3ac: if(!(*sr&1u))goto L8c10e3b6; /* bf 0x8c10e3b6 */
 goto L8c10e3ae;
L8c10e3ae: r[0]=sx(m,r[1],1); /* mov.b @r1,r0 */
 goto L8c10e3b0;
L8c10e3b0: r[0]+=87u; /* add #0x57,r0 */
 goto L8c10e3b2;
L8c10e3b2: wr(m,r[1],r[0],1);goto L8c10e3bc; /* bra 0x8c10e3bc */
 goto L8c10e3b6;
L8c10e3b6: r[2]=sx(m,r[1],1); /* mov.b @r1,r2 */
 goto L8c10e3b8;
L8c10e3b8: r[2]+=55u; /* add #0x37,r2 */
 goto L8c10e3ba;
L8c10e3ba: wr(m,r[1],r[2],1); /* mov.b r2,@r1 */
 goto L8c10e3bc;
L8c10e3bc: r[3]=rd(m,0x8c10e49cu,4); /* mov.l 0x8c10e49c,r3 */
 goto L8c10e3be;
L8c10e3be: r[1]=r[12]; /* mov r12,r1 */
 goto L8c10e3c0;
L8c10e3c0: target=r[3];*pr=0x8c10e3c4u;r[0]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10e3c4;
L8c10e3c4: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10e3c6;
L8c10e3c6: r[12]=r[0]; /* mov r0,r12 */
 goto L8c10e3c8;
L8c10e3c8: *sr=(*sr&~1u)|(r[12]>r[2]); /* cmp/hi r2,r12 */
 goto L8c10e3ca;
L8c10e3ca: take=(*sr&1u);r[13]+=1u;if(take)goto L8c10e37a;goto L8c10e3ce; /* bt/s 0x8c10e37a */
 goto L8c10e3ce;
L8c10e3ce: r[0]=sx(m,r[7],1); /* mov.b @r7,r0 */
 goto L8c10e3d0;
L8c10e3d0: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto L8c10e3d2;
L8c10e3d2: if((*sr&1u))goto L8c10e404; /* bt 0x8c10e404 */
 goto L8c10e3d4;
L8c10e3d4: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e3d6;
L8c10e3d6: *sr=(*sr&~1u)|(r[0]==0x0000006fu); /* cmp/eq #0x6f,r0 */
 goto L8c10e3d8;
L8c10e3d8: if((*sr&1u))goto L8c10e3e6; /* bt 0x8c10e3e6 */
 goto L8c10e3da;
L8c10e3da: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10e3dc;
L8c10e3dc: if((*sr&1u))goto L8c10e3f4; /* bt 0x8c10e3f4 */
 goto L8c10e3de;
L8c10e3de: *sr=(*sr&~1u)|(r[0]==0x00000058u); /* cmp/eq #0x58,r0 */
 goto L8c10e3e0;
L8c10e3e0: if((*sr&1u))goto L8c10e3f4; /* bt 0x8c10e3f4 */
 goto L8c10e3e2;
L8c10e3e2: goto L8c10e404; /* bra 0x8c10e404 */
 goto L8c10e3e6;
L8c10e3e6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c10e3e8;
L8c10e3e8: if((*sr&1u))goto L8c10e404; /* bt 0x8c10e404 */
 goto L8c10e3ea;
L8c10e3ea: r[0]=r[13]; /* mov r13,r0 */
 goto L8c10e3ec;
L8c10e3ec: r[3]=48u; /* mov #0x30,r3 */
 goto L8c10e3ee;
L8c10e3ee: r[13]+=1u; /* add #0x1,r13 */
 goto L8c10e3f0;
L8c10e3f0: wr(m,r[14]+r[0],r[3],1);goto L8c10e404; /* bra 0x8c10e404 */
 goto L8c10e3f4;
L8c10e3f4: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c10e3f6;
L8c10e3f6: if((*sr&1u))goto L8c10e404; /* bt 0x8c10e404 */
 goto L8c10e3f8;
L8c10e3f8: r[3]=48u; /* mov #0x30,r3 */
 goto L8c10e3fa;
L8c10e3fa: wr(m,r[5],r[3],1); /* mov.b r3,@r5 */
 goto L8c10e3fc;
L8c10e3fc: r[5]+=1u; /* add #0x1,r5 */
 goto L8c10e3fe;
L8c10e3fe: wr(m,r[5],r[6],1); /* mov.b r6,@r5 */
 goto L8c10e400;
L8c10e400: r[11]=2u; /* mov #0x2,r11 */
 goto L8c10e402;
L8c10e402: r[5]+=1u; /* add #0x1,r5 */
 goto L8c10e404;
L8c10e404: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e406;
L8c10e406: *sr=(*sr&~1u)|(r[0]==0x00000064u); /* cmp/eq #0x64,r0 */
 goto L8c10e408;
L8c10e408: wr(m,r[15],r[13],4); /* mov.l r13,@r15 */
 goto L8c10e40a;
L8c10e40a: take=(*sr&1u);r[11]+=r[13];if(take)goto L8c10e414;goto L8c10e40e; /* bt/s 0x8c10e414 */
 goto L8c10e40e;
L8c10e40e: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e410;
L8c10e410: *sr=(*sr&~1u)|(r[0]==0x00000069u); /* cmp/eq #0x69,r0 */
 goto L8c10e412;
L8c10e412: if(!(*sr&1u))goto L8c10e446; /* bf 0x8c10e446 */
 goto L8c10e414;
L8c10e414: r[3]=rd(m,r[7]+0x8u,4); /* mov.l @(0x8,r7),r3 */
 goto L8c10e416;
L8c10e416: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c10e418;
L8c10e418: if(!(*sr&1u))goto L8c10e41e; /* bf 0x8c10e41e */
 goto L8c10e41a;
L8c10e41a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c10e41c;
L8c10e41c: if((*sr&1u))goto L8c10e446; /* bt 0x8c10e446 */
 goto L8c10e41e;
L8c10e41e: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c10e420;
L8c10e420: if(!(*sr&1u))goto L8c10e43e; /* bf 0x8c10e43e */
 goto L8c10e422;
L8c10e422: r[0]=sx(m,r[7],1); /* mov.b @r7,r0 */
 goto L8c10e424;
L8c10e424: *sr=(*sr&~1u)|!(r[0]&0x8u); /* tst #0x8,r0 */
 goto L8c10e426;
L8c10e426: if((*sr&1u))goto L8c10e430; /* bt 0x8c10e430 */
 goto L8c10e428;
L8c10e428: r[3]=43u; /* mov #0x2b,r3 */
 goto L8c10e42a;
L8c10e42a: r[11]+=1u; /* add #0x1,r11 */
 goto L8c10e42c;
L8c10e42c: wr(m,r[5],r[3],1);goto L8c10e444; /* bra 0x8c10e444 */
 goto L8c10e430;
L8c10e430: *sr=(*sr&~1u)|!(r[0]&0x10u); /* tst #0x10,r0 */
 goto L8c10e432;
L8c10e432: if((*sr&1u))goto L8c10e446; /* bt 0x8c10e446 */
 goto L8c10e434;
L8c10e434: r[3]=32u; /* mov #0x20,r3 */
 goto L8c10e436;
L8c10e436: wr(m,r[5],r[3],1); /* mov.b r3,@r5 */
 goto L8c10e438;
L8c10e438: r[11]+=1u; /* add #0x1,r11 */
 goto L8c10e43a;
L8c10e43a: r[5]+=1u;goto L8c10e446; /* bra 0x8c10e446 */
 goto L8c10e43e;
L8c10e43e: r[2]=45u; /* mov #0x2d,r2 */
 goto L8c10e440;
L8c10e440: wr(m,r[5],r[2],1); /* mov.b r2,@r5 */
 goto L8c10e442;
L8c10e442: r[11]+=1u; /* add #0x1,r11 */
 goto L8c10e444;
L8c10e444: r[5]+=1u; /* add #0x1,r5 */
 goto L8c10e446;
L8c10e446: r[0]=sx(m,r[10],1); /* mov.b @r10,r0 */
 goto L8c10e448;
L8c10e448: r[4]=r[10]; /* mov r10,r4 */
 goto L8c10e44a;
L8c10e44a: r[4]+=1u; /* add #0x1,r4 */
 goto L8c10e44c;
L8c10e44c: *sr=(*sr&~1u)|(r[0]==0x0000002bu); /* cmp/eq #0x2b,r0 */
 goto L8c10e44e;
L8c10e44e: take=(*sr&1u);r[13]=r[0];if(take)goto L8c10e45e;goto L8c10e452; /* bt/s 0x8c10e45e */
 goto L8c10e452;
L8c10e452: r[0]=r[13]; /* mov r13,r0 */
 goto L8c10e454;
L8c10e454: *sr=(*sr&~1u)|(r[0]==0x0000002du); /* cmp/eq #0x2d,r0 */
 goto L8c10e456;
L8c10e456: if((*sr&1u))goto L8c10e45e; /* bt 0x8c10e45e */
 goto L8c10e458;
L8c10e458: r[0]=r[13]; /* mov r13,r0 */
 goto L8c10e45a;
L8c10e45a: *sr=(*sr&~1u)|(r[0]==0x00000020u); /* cmp/eq #0x20,r0 */
 goto L8c10e45c;
L8c10e45c: if(!(*sr&1u))goto L8c10e462; /* bf 0x8c10e462 */
 goto L8c10e45e;
L8c10e45e: wr(m,r[7]+0x14u,r[4],4);goto L8c10e488; /* bra 0x8c10e488 */
 goto L8c10e462;
L8c10e462: r[0]=sx(m,r[7],1); /* mov.b @r7,r0 */
 goto L8c10e464;
L8c10e464: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto L8c10e466;
L8c10e466: if((*sr&1u))goto L8c10e486; /* bt 0x8c10e486 */
 goto L8c10e468;
L8c10e468: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e46a;
L8c10e46a: *sr=(*sr&~1u)|(r[0]==0x00000058u); /* cmp/eq #0x58,r0 */
 goto L8c10e46c;
L8c10e46c: if((*sr&1u))goto L8c10e47a; /* bt 0x8c10e47a */
 goto L8c10e46e;
L8c10e46e: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10e470;
L8c10e470: if((*sr&1u))goto L8c10e47a; /* bt 0x8c10e47a */
 goto L8c10e472;
L8c10e472: *sr=(*sr&~1u)|(r[0]==0x0000006fu); /* cmp/eq #0x6f,r0 */
 goto L8c10e474;
L8c10e474: if((*sr&1u))goto L8c10e482; /* bt 0x8c10e482 */
 goto L8c10e476;
L8c10e476: goto L8c10e488; /* bra 0x8c10e488 */
 goto L8c10e47a;
L8c10e47a: r[2]=r[10]; /* mov r10,r2 */
 goto L8c10e47c;
L8c10e47c: r[2]+=2u; /* add #0x2,r2 */
 goto L8c10e47e;
L8c10e47e: wr(m,r[7]+0x14u,r[2],4);goto L8c10e488; /* bra 0x8c10e488 */
 goto L8c10e482;
L8c10e482: wr(m,r[7]+0x14u,r[4],4);goto L8c10e488; /* bra 0x8c10e488 */
 goto L8c10e486;
L8c10e486: wr(m,r[7]+0x14u,r[10],4); /* mov.l r10,@(0x14,r7) */
 goto L8c10e488;
L8c10e488: r[3]=rd(m,r[7]+0x8u,4); /* mov.l @(0x8,r7),r3 */
 goto L8c10e48a;
L8c10e48a: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[11]); /* cmp/gt r11,r3 */
 goto L8c10e48c;
L8c10e48c: if(!(*sr&1u))goto L8c10e4a0; /* bf 0x8c10e4a0 */
 goto L8c10e48e;
L8c10e48e: r[6]=rd(m,r[7]+0x8u,4); /* mov.l @(0x8,r7),r6 */
 goto L8c10e490;
L8c10e490: r[4]=r[6]; /* mov r6,r4 */
 goto L8c10e492;
L8c10e492: r[6]-=r[11]; /* sub r11,r6 */
 goto L8c10e494;
L8c10e494: wr(m,r[7]+0x20u,r[6],4);goto L8c10e4a4; /* bra 0x8c10e4a4 */
 goto L8c10e4a0;
L8c10e4a0: wr(m,r[7]+0x20u,r[9],4); /* mov.l r9,@(0x20,r7) */
 goto L8c10e4a2;
L8c10e4a2: r[4]=r[11]; /* mov r11,r4 */
 goto L8c10e4a4;
L8c10e4a4: r[0]=sx(m,r[10],1); /* mov.b @r10,r0 */
 goto L8c10e4a6;
L8c10e4a6: *sr=(*sr&~1u)|(r[0]==0x0000002bu); /* cmp/eq #0x2b,r0 */
 goto L8c10e4a8;
L8c10e4a8: take=(*sr&1u);r[6]=r[0];if(take)goto L8c10e4b8;goto L8c10e4ac; /* bt/s 0x8c10e4b8 */
 goto L8c10e4ac;
L8c10e4ac: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e4ae;
L8c10e4ae: *sr=(*sr&~1u)|(r[0]==0x0000002du); /* cmp/eq #0x2d,r0 */
 goto L8c10e4b0;
L8c10e4b0: if((*sr&1u))goto L8c10e4b8; /* bt 0x8c10e4b8 */
 goto L8c10e4b2;
L8c10e4b2: r[0]=r[6]; /* mov r6,r0 */
 goto L8c10e4b4;
L8c10e4b4: *sr=(*sr&~1u)|(r[0]==0x00000020u); /* cmp/eq #0x20,r0 */
 goto L8c10e4b6;
L8c10e4b6: if(!(*sr&1u))goto L8c10e4d2; /* bf 0x8c10e4d2 */
 goto L8c10e4b8;
L8c10e4b8: r[3]=rd(m,r[7]+0x4u,4); /* mov.l @(0x4,r7),r3 */
 goto L8c10e4ba;
L8c10e4ba: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]); /* cmp/gt r4,r3 */
 goto L8c10e4bc;
L8c10e4bc: if(!(*sr&1u))goto L8c10e4e0; /* bf 0x8c10e4e0 */
 goto L8c10e4be;
L8c10e4be: r[0]=sx(m,r[7]+0x1u,1); /* mov.b @(0x1,r7),r0 */
 goto L8c10e4c0;
L8c10e4c0: *sr=(*sr&~1u)|(r[0]==0x00000030u); /* cmp/eq #0x30,r0 */
 goto L8c10e4c2;
L8c10e4c2: if(!(*sr&1u))goto L8c10e4d2; /* bf 0x8c10e4d2 */
 goto L8c10e4c4;
L8c10e4c4: r[2]=rd(m,r[7]+0x4u,4); /* mov.l @(0x4,r7),r2 */
 goto L8c10e4c6;
L8c10e4c6: r[3]=rd(m,r[7]+0x20u,4); /* mov.l @(0x20,r7),r3 */
 goto L8c10e4c8;
L8c10e4c8: r[2]-=r[4]; /* sub r4,r2 */
 goto L8c10e4ca;
L8c10e4ca: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10e4cc;
L8c10e4cc: wr(m,r[7]+0x20u,r[3],4); /* mov.l r3,@(0x20,r7) */
 goto L8c10e4ce;
L8c10e4ce: wr(m,r[7]+0x28u,r[9],4);goto L8c10e4e2; /* bra 0x8c10e4e2 */
 goto L8c10e4d2;
L8c10e4d2: r[3]=rd(m,r[7]+0x4u,4); /* mov.l @(0x4,r7),r3 */
 goto L8c10e4d4;
L8c10e4d4: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]); /* cmp/gt r4,r3 */
 goto L8c10e4d6;
L8c10e4d6: if(!(*sr&1u))goto L8c10e4e0; /* bf 0x8c10e4e0 */
 goto L8c10e4d8;
L8c10e4d8: r[1]=rd(m,r[7]+0x4u,4); /* mov.l @(0x4,r7),r1 */
 goto L8c10e4da;
L8c10e4da: r[1]-=r[4]; /* sub r4,r1 */
 goto L8c10e4dc;
L8c10e4dc: wr(m,r[7]+0x28u,r[1],4);goto L8c10e4e2; /* bra 0x8c10e4e2 */
 goto L8c10e4e0;
L8c10e4e0: wr(m,r[7]+0x28u,r[9],4); /* mov.l r9,@(0x28,r7) */
 goto L8c10e4e2;
L8c10e4e2: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c10e4e4;
L8c10e4e4: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c10e4e6;
L8c10e4e6: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c10e4e8;
L8c10e4e8: if(!(*sr&1u))goto L8c10e4f8; /* bf 0x8c10e4f8 */
 goto L8c10e4ea;
L8c10e4ea: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10e4ec;
L8c10e4ec: r[3]=sx(m,r[14]+r[0],1); /* mov.b @(r0,r14),r3 */
 goto L8c10e4ee;
L8c10e4ee: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c10e4f0;
L8c10e4f0: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c10e4f2;
L8c10e4f2: wr(m,r[5],r[3],1); /* mov.b r3,@r5 */
 goto L8c10e4f4;
L8c10e4f4: take=(*sr&1u);r[5]+=1u;if(take)goto L8c10e4ea;goto L8c10e4f8; /* bt/s 0x8c10e4ea */
 goto L8c10e4f8;
L8c10e4f8: wr(m,r[5],r[9],1); /* mov.b r9,@r5 */
 goto L8c10e4fa;
L8c10e4fa: r[15]+=16u; /* add #0x10,r15 */
 goto L8c10e4fc;
L8c10e4fc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10e4fe;
L8c10e4fe: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c10e500;
L8c10e500: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c10e502;
L8c10e502: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c10e504;
L8c10e504: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10e506;
L8c10e506: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10e508;
L8c10e508: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10e50a;
L8c10e50a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
