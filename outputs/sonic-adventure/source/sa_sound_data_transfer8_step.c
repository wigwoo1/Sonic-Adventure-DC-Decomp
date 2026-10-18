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
/* Retail data-chunk audio transfer 8c66a8fa..8c66a9fa.
 * Native pending getter; query/submission callbacks supplied by caller.
 * 8-bit mono stores raw sample; stereo stores sample shifted left8. */
SA_STAGE_EXPORT int sa_sound_data_transfer8_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a8fa;
L8c66a8fa: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a8fc;
L8c66a8fc: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a8fe;
L8c66a8fe: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a900;
L8c66a900: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66a902;
L8c66a902: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a904;
L8c66a904: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66a906;
L8c66a906: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a908;
L8c66a908: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a90a;
L8c66a90a: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a90c;
L8c66a90c: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a90e;
L8c66a90e: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66a9d2;goto L8c66a912; /* bf/s 0x8c66a9d2 */
 goto L8c66a912;
L8c66a912: r[2]=rd(m,0x8c66a9a0u,4); /* mov.l 0x8c66a9a0,r2 */
 goto L8c66a914;
L8c66a914: target=r[2];*pr=0x8c66a918u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a918;
L8c66a918: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66a91a;
L8c66a91a: if(!(*sr&1u))goto L8c66a9d2; /* bf 0x8c66a9d2 */
 goto L8c66a91c;
L8c66a91c: r[0]=sx(m,0x8c66a99eu,2); /* mov.w 0x8c66a99e,r0 */
 goto L8c66a91e;
L8c66a91e: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66a920;
L8c66a920: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a922;
L8c66a922: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a924;
L8c66a924: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a926;
L8c66a926: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a928;
L8c66a928: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66a92a;
L8c66a92a: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66a92c;
L8c66a92c: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66a92e;
L8c66a92e: target=r[2];*pr=0x8c66a932u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a932;
L8c66a932: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66a934;
L8c66a934: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a936;
L8c66a936: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66a938;
L8c66a938: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66a93a;
L8c66a93a: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a93c;
L8c66a93c: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66a942;goto L8c66a940; /* bf/s 0x8c66a942 */
 goto L8c66a940;
L8c66a940: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66a942;
L8c66a942: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a944;
L8c66a944: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c66a94a;goto L8c66a948; /* bf/s 0x8c66a94a */
 goto L8c66a948;
L8c66a948: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66a94a;
L8c66a94a: r[11]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r11 */
 goto L8c66a94c;
L8c66a94c: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a94e;
L8c66a94e: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c66a950;
L8c66a950: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a952;
L8c66a952: r[11]+=r[5]; /* add r5,r11 */
 goto L8c66a954;
L8c66a954: take=!(*sr&1u);r[7]=0u;if(take)goto L8c66a9a4;goto L8c66a958; /* bf/s 0x8c66a9a4 */
 goto L8c66a958;
L8c66a958: r[1]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r1 */
 goto L8c66a95a;
L8c66a95a: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a95c;
L8c66a95c: r[13]=r[7]; /* mov r7,r13 */
 goto L8c66a95e;
L8c66a95e: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66a960;
L8c66a960: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c66a962;
L8c66a962: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a964;
L8c66a964: r[1]+=r[5]; /* add r5,r1 */
 goto L8c66a966;
L8c66a966: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66a968;
L8c66a968: take=!(*sr&1u);r[6]=r[7];if(take)goto L8c66a9c0;goto L8c66a96c; /* bf/s 0x8c66a9c0 */
 goto L8c66a96c;
L8c66a96c: r[0]=r[12]; /* mov r12,r0 */
 goto L8c66a96e;
L8c66a96e: r[2]=sx(m,r[5]+r[0],1); /* mov.b @(r0,r5),r2 */
 goto L8c66a970;
L8c66a970: r[3]=r[11]; /* mov r11,r3 */
 goto L8c66a972;
L8c66a972: r[3]+=r[6]; /* add r6,r3 */
 goto L8c66a974;
L8c66a974: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66a976;
L8c66a976: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a978;
L8c66a978: r[2]<<=8; /* shll8 r2 */
 goto L8c66a97a;
L8c66a97a: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c66a97c;
L8c66a97c: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66a97e;
L8c66a97e: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66a980;
L8c66a980: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a982;
L8c66a982: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c66a984;
L8c66a984: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66a986;
L8c66a986: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66a988;
L8c66a988: r[5]+=2u; /* add #0x2,r5 */
 goto L8c66a98a;
L8c66a98a: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66a98c;
L8c66a98c: r[3]<<=8; /* shll8 r3 */
 goto L8c66a98e;
L8c66a98e: wr(m,r[0]+r[1],r[3],2); /* mov.w r3,@(r0,r1) */
 goto L8c66a990;
L8c66a990: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a96c;goto L8c66a994; /* bf/s 0x8c66a96c */
 goto L8c66a994;
L8c66a994: goto L8c66a9c0; /* bra 0x8c66a9c0 */
 goto L8c66a9a4;
L8c66a9a4: r[13]=0u; /* mov #0x0,r13 */
 goto L8c66a9a6;
L8c66a9a6: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a9a8;
L8c66a9a8: r[5]=0u; /* mov #0x0,r5 */
 goto L8c66a9aa;
L8c66a9aa: r[6]=r[7]; /* mov r7,r6 */
 goto L8c66a9ac;
L8c66a9ac: r[13]+=r[12]; /* add r12,r13 */
 goto L8c66a9ae;
L8c66a9ae: take=!(*sr&1u);r[5]+=r[11];if(take)goto L8c66a9c0;goto L8c66a9b2; /* bf/s 0x8c66a9c0 */
 goto L8c66a9b2;
L8c66a9b2: r[3]=sx(m,r[13],1);r[13]+=1; /* mov.b @r13+,r3 */
 goto L8c66a9b4;
L8c66a9b4: r[6]+=1u; /* add #0x1,r6 */
 goto L8c66a9b6;
L8c66a9b6: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c66a9b8;
L8c66a9b8: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66a9ba;
L8c66a9ba: wr(m,r[5],r[3],2); /* mov.w r3,@r5 */
 goto L8c66a9bc;
L8c66a9bc: take=!(*sr&1u);r[5]+=2u;if(take)goto L8c66a9b2;goto L8c66a9c0; /* bf/s 0x8c66a9b2 */
 goto L8c66a9c0;
L8c66a9c0: r[0]=sx(m,0x8c66aab2u,2); /* mov.w 0x8c66aab2,r0 */
 goto L8c66a9c2;
L8c66a9c2: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a9c4;
L8c66a9c4: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a9c6;
L8c66a9c6: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a9c8;
L8c66a9c8: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66a9ca;
L8c66a9ca: r[0]=sx(m,0x8c66aab4u,2); /* mov.w 0x8c66aab4,r0 */
 goto L8c66a9cc;
L8c66a9cc: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66a9ce;
L8c66a9ce: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a9d0;
L8c66a9d0: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a9d2;
L8c66a9d2: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a9d4;
L8c66a9d4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a9d6;
L8c66a9d6: if(!(*sr&1u))goto L8c66a9ee; /* bf 0x8c66a9ee */
 goto L8c66a9d8;
L8c66a9d8: r[0]=sx(m,0x8c66aab6u,2); /* mov.w 0x8c66aab6,r0 */
 goto L8c66a9da;
L8c66a9da: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a9dc;
L8c66a9dc: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a9de;
L8c66a9de: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66a9e0;
L8c66a9e0: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a9e2;
L8c66a9e2: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66a9e4;
L8c66a9e4: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66a9e6;
L8c66a9e6: target=r[3];*pr=0x8c66a9eau;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a9ea;
L8c66a9ea: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66a9ec;
L8c66a9ec: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a9ee;
L8c66a9ee: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a9f0;
L8c66a9f0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a9f2;
L8c66a9f2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a9f4;
L8c66a9f4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a9f6;
L8c66a9f6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
