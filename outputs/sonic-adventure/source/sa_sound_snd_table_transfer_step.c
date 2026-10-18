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
/* Retail SND encoding path, 8c66a244..8c66a348.
 * Selector 2 uses retail table67c3ec; selector1 expands raw bytes; otherwise BE16.
 * Native transfer/pending calls connected; query/submission callbacks supplied by caller. */
SA_STAGE_EXPORT int sa_sound_snd_table_transfer_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a244;
L8c66a244: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a246;
L8c66a246: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a248;
L8c66a248: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a24a;
L8c66a24a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66a24c;
L8c66a24c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a24e;
L8c66a24e: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66a250;
L8c66a250: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a252;
L8c66a252: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66a254;
L8c66a254: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a256;
L8c66a256: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a258;
L8c66a258: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a25a;
L8c66a25a: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66a31e;goto L8c66a25e; /* bf/s 0x8c66a31e */
 goto L8c66a25e;
L8c66a25e: r[2]=rd(m,0x8c66a370u,4); /* mov.l 0x8c66a370,r2 */
 goto L8c66a260;
L8c66a260: target=r[2];*pr=0x8c66a264u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a264;
L8c66a264: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66a266;
L8c66a266: if(!(*sr&1u))goto L8c66a31e; /* bf 0x8c66a31e */
 goto L8c66a268;
L8c66a268: r[0]=sx(m,0x8c66a364u,2); /* mov.w 0x8c66a364,r0 */
 goto L8c66a26a;
L8c66a26a: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66a26c;
L8c66a26c: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a26e;
L8c66a26e: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a270;
L8c66a270: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a272;
L8c66a272: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a274;
L8c66a274: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66a276;
L8c66a276: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66a278;
L8c66a278: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66a27a;
L8c66a27a: target=r[2];*pr=0x8c66a27eu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a27e;
L8c66a27e: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66a280;
L8c66a280: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a282;
L8c66a282: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66a284;
L8c66a284: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66a286;
L8c66a286: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a288;
L8c66a288: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66a28e;goto L8c66a28c; /* bf/s 0x8c66a28e */
 goto L8c66a28c;
L8c66a28c: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66a28e;
L8c66a28e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a290;
L8c66a290: take=!(*sr&1u);r[6]=rd(m,r[13]+0x14u,4);if(take)goto L8c66a296;goto L8c66a294; /* bf/s 0x8c66a296 */
 goto L8c66a294;
L8c66a294: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66a296;
L8c66a296: r[10]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r10 */
 goto L8c66a298;
L8c66a298: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a29a;
L8c66a29a: r[5]=rd(m,0x8c66a374u,4); /* mov.l 0x8c66a374,r5 */
 goto L8c66a29c;
L8c66a29c: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1; /* shll r10 */
 goto L8c66a29e;
L8c66a29e: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a2a0;
L8c66a2a0: r[10]+=r[6]; /* add r6,r10 */
 goto L8c66a2a2;
L8c66a2a2: take=!(*sr&1u);r[11]=0u;if(take)goto L8c66a2ec;goto L8c66a2a6; /* bf/s 0x8c66a2ec */
 goto L8c66a2a6;
L8c66a2a6: r[1]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r1 */
 goto L8c66a2a8;
L8c66a2a8: r[7]=r[11]; /* mov r11,r7 */
 goto L8c66a2aa;
L8c66a2aa: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a2ac;
L8c66a2ac: r[13]=r[11]; /* mov r11,r13 */
 goto L8c66a2ae;
L8c66a2ae: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66a2b0;
L8c66a2b0: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c66a2b2;
L8c66a2b2: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a2b4;
L8c66a2b4: r[1]+=r[6]; /* add r6,r1 */
 goto L8c66a2b6;
L8c66a2b6: take=!(*sr&1u);r[6]=r[11];if(take)goto L8c66a30c;goto L8c66a2ba; /* bf/s 0x8c66a30c */
 goto L8c66a2ba;
L8c66a2ba: r[0]=r[12]; /* mov r12,r0 */
 goto L8c66a2bc;
L8c66a2bc: r[0]=sx(m,r[7]+r[0],1); /* mov.b @(r0,r7),r0 */
 goto L8c66a2be;
L8c66a2be: r[2]=r[10]; /* mov r10,r2 */
 goto L8c66a2c0;
L8c66a2c0: r[2]+=r[6]; /* add r6,r2 */
 goto L8c66a2c2;
L8c66a2c2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66a2c4;
L8c66a2c4: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c66a2c6;
L8c66a2c6: r[3]=sx(m,r[0]+r[5],2); /* mov.w @(r0,r5),r3 */
 goto L8c66a2c8;
L8c66a2c8: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66a2ca;
L8c66a2ca: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a2cc;
L8c66a2cc: wr(m,r[2],r[3],2); /* mov.w r3,@r2 */
 goto L8c66a2ce;
L8c66a2ce: r[3]=r[7]; /* mov r7,r3 */
 goto L8c66a2d0;
L8c66a2d0: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66a2d2;
L8c66a2d2: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a2d4;
L8c66a2d4: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c66a2d6;
L8c66a2d6: r[7]+=2u; /* add #0x2,r7 */
 goto L8c66a2d8;
L8c66a2d8: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66a2da;
L8c66a2da: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66a2dc;
L8c66a2dc: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66a2de;
L8c66a2de: r[3]+=r[5]; /* add r5,r3 */
 goto L8c66a2e0;
L8c66a2e0: r[3]=sx(m,r[3],2); /* mov.w @r3,r3 */
 goto L8c66a2e2;
L8c66a2e2: wr(m,r[0]+r[1],r[3],2); /* mov.w r3,@(r0,r1) */
 goto L8c66a2e4;
L8c66a2e4: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a2ba;goto L8c66a2e8; /* bf/s 0x8c66a2ba */
 goto L8c66a2e8;
L8c66a2e8: goto L8c66a30c; /* bra 0x8c66a30c */
 goto L8c66a2ec;
L8c66a2ec: r[6]=0u; /* mov #0x0,r6 */
 goto L8c66a2ee;
L8c66a2ee: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a2f0;
L8c66a2f0: r[7]=0u; /* mov #0x0,r7 */
 goto L8c66a2f2;
L8c66a2f2: r[13]=r[11]; /* mov r11,r13 */
 goto L8c66a2f4;
L8c66a2f4: r[6]+=r[12]; /* add r12,r6 */
 goto L8c66a2f6;
L8c66a2f6: take=!(*sr&1u);r[7]+=r[10];if(take)goto L8c66a30c;goto L8c66a2fa; /* bf/s 0x8c66a30c */
 goto L8c66a2fa;
L8c66a2fa: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c66a2fc;
L8c66a2fc: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66a2fe;
L8c66a2fe: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66a300;
L8c66a300: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c66a302;
L8c66a302: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66a304;
L8c66a304: r[3]=sx(m,r[0]+r[5],2); /* mov.w @(r0,r5),r3 */
 goto L8c66a306;
L8c66a306: wr(m,r[7],r[3],2); /* mov.w r3,@r7 */
 goto L8c66a308;
L8c66a308: take=!(*sr&1u);r[7]+=2u;if(take)goto L8c66a2fa;goto L8c66a30c; /* bf/s 0x8c66a2fa */
 goto L8c66a30c;
L8c66a30c: r[0]=sx(m,0x8c66a366u,2); /* mov.w 0x8c66a366,r0 */
 goto L8c66a30e;
L8c66a30e: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a310;
L8c66a310: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a312;
L8c66a312: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a314;
L8c66a314: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66a316;
L8c66a316: r[0]=sx(m,0x8c66a368u,2); /* mov.w 0x8c66a368,r0 */
 goto L8c66a318;
L8c66a318: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66a31a;
L8c66a31a: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a31c;
L8c66a31c: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a31e;
L8c66a31e: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a320;
L8c66a320: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a322;
L8c66a322: if(!(*sr&1u))goto L8c66a33a; /* bf 0x8c66a33a */
 goto L8c66a324;
L8c66a324: r[0]=sx(m,0x8c66a36au,2); /* mov.w 0x8c66a36a,r0 */
 goto L8c66a326;
L8c66a326: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a328;
L8c66a328: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a32a;
L8c66a32a: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66a32c;
L8c66a32c: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a32e;
L8c66a32e: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66a330;
L8c66a330: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66a332;
L8c66a332: target=r[3];*pr=0x8c66a336u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a336;
L8c66a336: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66a338;
L8c66a338: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a33a;
L8c66a33a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a33c;
L8c66a33c: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66a33e;
L8c66a33e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a340;
L8c66a340: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a342;
L8c66a342: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a344;
L8c66a344: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
