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
/* Static C recovery of retail sound resource lookup 8c65a900..8c65aa22.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_resource_lookup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a900;
L8c65a900: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a902;
L8c65a902: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65a904;
L8c65a904: r[3]=rd(m,0x8c65a9b0u,4); /* mov.l 0x8c65a9b0,r3 */
 goto L8c65a906;
L8c65a906: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65a908;
L8c65a908: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65a90a;
L8c65a90a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65a90c;
L8c65a90c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65a90e;
L8c65a90e: r[9]=0u; /* mov #0x0,r9 */
 goto L8c65a910;
L8c65a910: r[13]=rd(m,r[3],4); /* mov.l @r3,r13 */
 goto L8c65a912;
L8c65a912: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c65a914;
L8c65a914: take=!(*sr&1u);r[12]=r[9];if(take)goto L8c65a91e;goto L8c65a918; /* bf/s 0x8c65a91e */
 goto L8c65a918;
L8c65a918: r[12]=rd(m,0x8c65a9b4u,4); /* mov.l 0x8c65a9b4,r12 */
 goto L8c65a91a;
L8c65a91a: goto L8c65aa12; /* bra 0x8c65aa12 */
 goto L8c65a91e;
L8c65a91e: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c65a920;
L8c65a920: r[0]=*sr&1u; /* movt r0 */
 goto L8c65a922;
L8c65a922: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c65a924;
L8c65a924: r[1]=*sr&1u; /* movt r1 */
 goto L8c65a926;
L8c65a926: r[0]|=r[1]; /* or r1,r0 */
 goto L8c65a928;
L8c65a928: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65a92a;
L8c65a92a: if((*sr&1u))goto L8c65a932; /* bt 0x8c65a932 */
 goto L8c65a92c;
L8c65a92c: r[12]=rd(m,0x8c65a9b8u,4); /* mov.l 0x8c65a9b8,r12 */
 goto L8c65a92e;
L8c65a92e: goto L8c65aa12; /* bra 0x8c65aa12 */
 goto L8c65a932;
L8c65a932: r[14]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r14 */
 goto L8c65a934;
L8c65a934: *sr=(*sr&~1u)|((int32_t)r[14]>=0); /* cmp/pz r14 */
 goto L8c65a936;
L8c65a936: if(!(*sr&1u))goto L8c65a9e6; /* bf 0x8c65a9e6 */
 goto L8c65a938;
L8c65a938: r[1]=rd(m,0x8c65a9bcu,4); /* mov.l 0x8c65a9bc,r1 */
 goto L8c65a93a;
L8c65a93a: r[11]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r11 */
 goto L8c65a93c;
L8c65a93c: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65a93e;
L8c65a93e: r[11]<<=2; /* shll2 r11 */
 goto L8c65a940;
L8c65a940: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c65a942;
L8c65a942: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a944;
L8c65a944: take=(*sr&1u);r[10]=16u;if(take)goto L8c65a970;goto L8c65a948; /* bt/s 0x8c65a970 */
 goto L8c65a948;
L8c65a948: r[1]=rd(m,0x8c65a9c0u,4); /* mov.l 0x8c65a9c0,r1 */
 goto L8c65a94a;
L8c65a94a: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a94c;
L8c65a94c: if((*sr&1u))goto L8c65a97a; /* bt 0x8c65a97a */
 goto L8c65a94e;
L8c65a94e: r[1]=rd(m,0x8c65a9c4u,4); /* mov.l 0x8c65a9c4,r1 */
 goto L8c65a950;
L8c65a950: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a952;
L8c65a952: if((*sr&1u))goto L8c65a984; /* bt 0x8c65a984 */
 goto L8c65a954;
L8c65a954: r[1]=rd(m,0x8c65a9c8u,4); /* mov.l 0x8c65a9c8,r1 */
 goto L8c65a956;
L8c65a956: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a958;
L8c65a958: if((*sr&1u))goto L8c65a98e; /* bt 0x8c65a98e */
 goto L8c65a95a;
L8c65a95a: r[1]=rd(m,0x8c65a9ccu,4); /* mov.l 0x8c65a9cc,r1 */
 goto L8c65a95c;
L8c65a95c: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a95e;
L8c65a95e: if((*sr&1u))goto L8c65a99a; /* bt 0x8c65a99a */
 goto L8c65a960;
L8c65a960: r[1]=rd(m,0x8c65a9d0u,4); /* mov.l 0x8c65a9d0,r1 */
 goto L8c65a962;
L8c65a962: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a964;
L8c65a964: if((*sr&1u))goto L8c65a9d8; /* bt 0x8c65a9d8 */
 goto L8c65a966;
L8c65a966: r[1]=rd(m,0x8c65a9d4u,4); /* mov.l 0x8c65a9d4,r1 */
 goto L8c65a968;
L8c65a968: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a96a;
L8c65a96a: if((*sr&1u))goto L8c65a9e2; /* bt 0x8c65a9e2 */
 goto L8c65a96c;
L8c65a96c: goto L8c65a9f2; /* bra 0x8c65a9f2 */
 goto L8c65a970;
L8c65a970: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c65a972;
L8c65a972: if((*sr&1u))goto L8c65a9e6; /* bt 0x8c65a9e6 */
 goto L8c65a974;
L8c65a974: r[4]=sx(m,0x8c65a9a4u,2); /* mov.w 0x8c65a9a4,r4 */
 goto L8c65a976;
L8c65a976: goto L8c65a994; /* bra 0x8c65a994 */
 goto L8c65a97a;
L8c65a97a: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c65a97c;
L8c65a97c: if((*sr&1u))goto L8c65a9e6; /* bt 0x8c65a9e6 */
 goto L8c65a97e;
L8c65a97e: r[4]=sx(m,0x8c65a9a6u,2); /* mov.w 0x8c65a9a6,r4 */
 goto L8c65a980;
L8c65a980: goto L8c65a994; /* bra 0x8c65a994 */
 goto L8c65a984;
L8c65a984: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c65a986;
L8c65a986: if((*sr&1u))goto L8c65a9e6; /* bt 0x8c65a9e6 */
 goto L8c65a988;
L8c65a988: r[4]=sx(m,0x8c65a9a8u,2); /* mov.w 0x8c65a9a8,r4 */
 goto L8c65a98a;
L8c65a98a: goto L8c65a994; /* bra 0x8c65a994 */
 goto L8c65a98e;
L8c65a98e: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c65a990;
L8c65a990: if((*sr&1u))goto L8c65a9e6; /* bt 0x8c65a9e6 */
 goto L8c65a992;
L8c65a992: r[4]=sx(m,0x8c65a9aau,2); /* mov.w 0x8c65a9aa,r4 */
 goto L8c65a994;
L8c65a994: r[4]+=r[13]; /* add r13,r4 */
 goto L8c65a996;
L8c65a996: r[4]+=r[11];goto L8c65a9f6; /* bra 0x8c65a9f6 */
 goto L8c65a99a;
L8c65a99a: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a99c;
L8c65a99c: if(!(*sr&1u))goto L8c65a9e6; /* bf 0x8c65a9e6 */
 goto L8c65a99e;
L8c65a99e: r[4]=sx(m,0x8c65a9acu,2); /* mov.w 0x8c65a9ac,r4 */
 goto L8c65a9a0;
L8c65a9a0: goto L8c65a9ee; /* bra 0x8c65a9ee */
 goto L8c65a9d8;
L8c65a9d8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a9da;
L8c65a9da: if(!(*sr&1u))goto L8c65a9e6; /* bf 0x8c65a9e6 */
 goto L8c65a9dc;
L8c65a9dc: r[4]=sx(m,0x8c65aa22u,2); /* mov.w 0x8c65aa22,r4 */
 goto L8c65a9de;
L8c65a9de: goto L8c65a9ee; /* bra 0x8c65a9ee */
 goto L8c65a9e2;
L8c65a9e2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a9e4;
L8c65a9e4: if((*sr&1u))goto L8c65a9ec; /* bt 0x8c65a9ec */
 goto L8c65a9e6;
L8c65a9e6: r[12]=rd(m,0x8c65aa28u,4); /* mov.l 0x8c65aa28,r12 */
 goto L8c65a9e8;
L8c65a9e8: goto L8c65aa12; /* bra 0x8c65aa12 */
 goto L8c65a9ec;
L8c65a9ec: r[4]=sx(m,0x8c65aa24u,2); /* mov.w 0x8c65aa24,r4 */
 goto L8c65a9ee;
L8c65a9ee: r[4]+=r[13];goto L8c65a9f6; /* bra 0x8c65a9f6 */
 goto L8c65a9f2;
L8c65a9f2: r[12]=rd(m,0x8c65aa2cu,4); /* mov.l 0x8c65aa2c,r12 */
 goto L8c65a9f4;
L8c65a9f4: r[4]=r[9]; /* mov r9,r4 */
 goto L8c65a9f6;
L8c65a9f6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65a9f8;
L8c65a9f8: if((*sr&1u))goto L8c65aa04; /* bt 0x8c65aa04 */
 goto L8c65a9fa;
L8c65a9fa: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c65a9fc;
L8c65a9fc: wr(m,r[6],r[3],4); /* mov.l r3,@r6 */
 goto L8c65a9fe;
L8c65a9fe: r[2]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r2 */
 goto L8c65aa00;
L8c65aa00: wr(m,r[7],r[2],4);goto L8c65aa12; /* bra 0x8c65aa12 */
 goto L8c65aa04;
L8c65aa04: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c65aa06;
L8c65aa06: if((*sr&1u))goto L8c65aa0a; /* bt 0x8c65aa0a */
 goto L8c65aa08;
L8c65aa08: wr(m,r[6],r[9],4); /* mov.l r9,@r6 */
 goto L8c65aa0a;
L8c65aa0a: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c65aa0c;
L8c65aa0c: if((*sr&1u))goto L8c65aa12; /* bt 0x8c65aa12 */
 goto L8c65aa0e;
L8c65aa0e: r[2]=4294967295u; /* mov #-0x1,r2 */
 goto L8c65aa10;
L8c65aa10: wr(m,r[7],r[2],4); /* mov.l r2,@r7 */
 goto L8c65aa12;
L8c65aa12: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65aa14;
L8c65aa14: r[0]=r[12]; /* mov r12,r0 */
 goto L8c65aa16;
L8c65aa16: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65aa18;
L8c65aa18: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65aa1a;
L8c65aa1a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65aa1c;
L8c65aa1c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65aa1e;
L8c65aa1e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
