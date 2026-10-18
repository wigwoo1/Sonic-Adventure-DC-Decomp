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
/* Retail data-chunk audio transfer 8c66a804..8c66a8fa.
 * Native pending getter; query/submission callbacks supplied by caller.
 * 8-bit mono stores raw sample; stereo stores sample shifted left8. */
SA_STAGE_EXPORT int sa_sound_data_transfer16_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a804;
L8c66a804: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a806;
L8c66a806: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a808;
L8c66a808: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a80a;
L8c66a80a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66a80c;
L8c66a80c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a80e;
L8c66a80e: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66a810;
L8c66a810: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a812;
L8c66a812: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a814;
L8c66a814: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a816;
L8c66a816: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a818;
L8c66a818: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66a8d2;goto L8c66a81c; /* bf/s 0x8c66a8d2 */
 goto L8c66a81c;
L8c66a81c: r[2]=rd(m,0x8c66a8a4u,4); /* mov.l 0x8c66a8a4,r2 */
 goto L8c66a81e;
L8c66a81e: target=r[2];*pr=0x8c66a822u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a822;
L8c66a822: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66a824;
L8c66a824: if(!(*sr&1u))goto L8c66a8d2; /* bf 0x8c66a8d2 */
 goto L8c66a826;
L8c66a826: r[0]=sx(m,0x8c66a89cu,2); /* mov.w 0x8c66a89c,r0 */
 goto L8c66a828;
L8c66a828: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66a82a;
L8c66a82a: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a82c;
L8c66a82c: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a82e;
L8c66a82e: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a830;
L8c66a830: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a832;
L8c66a832: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66a834;
L8c66a834: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66a836;
L8c66a836: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66a838;
L8c66a838: target=r[2];*pr=0x8c66a83cu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a83c;
L8c66a83c: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66a83e;
L8c66a83e: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a840;
L8c66a840: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66a842;
L8c66a842: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66a844;
L8c66a844: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a846;
L8c66a846: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66a84c;goto L8c66a84a; /* bf/s 0x8c66a84c */
 goto L8c66a84a;
L8c66a84a: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66a84c;
L8c66a84c: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a84e;
L8c66a84e: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c66a854;goto L8c66a852; /* bf/s 0x8c66a854 */
 goto L8c66a852;
L8c66a852: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66a854;
L8c66a854: r[11]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r11 */
 goto L8c66a856;
L8c66a856: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a858;
L8c66a858: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c66a85a;
L8c66a85a: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a85c;
L8c66a85c: r[11]+=r[5]; /* add r5,r11 */
 goto L8c66a85e;
L8c66a85e: take=!(*sr&1u);r[7]=0u;if(take)goto L8c66a8a8;goto L8c66a862; /* bf/s 0x8c66a8a8 */
 goto L8c66a862;
L8c66a862: r[1]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r1 */
 goto L8c66a864;
L8c66a864: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a866;
L8c66a866: r[13]=r[7]; /* mov r7,r13 */
 goto L8c66a868;
L8c66a868: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66a86a;
L8c66a86a: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c66a86c;
L8c66a86c: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a86e;
L8c66a86e: r[1]+=r[5]; /* add r5,r1 */
 goto L8c66a870;
L8c66a870: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66a872;
L8c66a872: take=!(*sr&1u);r[6]=r[7];if(take)goto L8c66a8be;goto L8c66a876; /* bf/s 0x8c66a8be */
 goto L8c66a876;
L8c66a876: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66a878;
L8c66a878: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66a87a;
L8c66a87a: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a87c;
L8c66a87c: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a87e;
L8c66a87e: r[3]=sx(m,r[3],2); /* mov.w @r3,r3 */
 goto L8c66a880;
L8c66a880: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66a882;
L8c66a882: wr(m,r[0]+r[11],r[3],2); /* mov.w r3,@(r0,r11) */
 goto L8c66a884;
L8c66a884: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66a886;
L8c66a886: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66a888;
L8c66a888: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66a88a;
L8c66a88a: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66a88c;
L8c66a88c: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a88e;
L8c66a88e: r[3]=sx(m,r[3],2); /* mov.w @r3,r3 */
 goto L8c66a890;
L8c66a890: r[5]+=2u; /* add #0x2,r5 */
 goto L8c66a892;
L8c66a892: wr(m,r[0]+r[1],r[3],2); /* mov.w r3,@(r0,r1) */
 goto L8c66a894;
L8c66a894: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a876;goto L8c66a898; /* bf/s 0x8c66a876 */
 goto L8c66a898;
L8c66a898: goto L8c66a8be; /* bra 0x8c66a8be */
 goto L8c66a8a8;
L8c66a8a8: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a8aa;
L8c66a8aa: r[6]=r[7]; /* mov r7,r6 */
 goto L8c66a8ac;
L8c66a8ac: take=!(*sr&1u);r[5]=r[7];if(take)goto L8c66a8be;goto L8c66a8b0; /* bf/s 0x8c66a8be */
 goto L8c66a8b0;
L8c66a8b0: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66a8b2;
L8c66a8b2: r[3]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r3 */
 goto L8c66a8b4;
L8c66a8b4: r[6]+=1u; /* add #0x1,r6 */
 goto L8c66a8b6;
L8c66a8b6: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c66a8b8;
L8c66a8b8: wr(m,r[0]+r[11],r[3],2); /* mov.w r3,@(r0,r11) */
 goto L8c66a8ba;
L8c66a8ba: take=!(*sr&1u);r[5]+=2u;if(take)goto L8c66a8b0;goto L8c66a8be; /* bf/s 0x8c66a8b0 */
 goto L8c66a8be;
L8c66a8be: r[0]=sx(m,0x8c66a998u,2); /* mov.w 0x8c66a998,r0 */
 goto L8c66a8c0;
L8c66a8c0: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a8c2;
L8c66a8c2: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a8c4;
L8c66a8c4: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a8c6;
L8c66a8c6: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66a8c8;
L8c66a8c8: r[0]=sx(m,0x8c66a99au,2); /* mov.w 0x8c66a99a,r0 */
 goto L8c66a8ca;
L8c66a8ca: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66a8cc;
L8c66a8cc: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c66a8ce;
L8c66a8ce: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a8d0;
L8c66a8d0: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a8d2;
L8c66a8d2: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a8d4;
L8c66a8d4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a8d6;
L8c66a8d6: if(!(*sr&1u))goto L8c66a8ee; /* bf 0x8c66a8ee */
 goto L8c66a8d8;
L8c66a8d8: r[0]=sx(m,0x8c66a99cu,2); /* mov.w 0x8c66a99c,r0 */
 goto L8c66a8da;
L8c66a8da: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a8dc;
L8c66a8dc: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a8de;
L8c66a8de: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66a8e0;
L8c66a8e0: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a8e2;
L8c66a8e2: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66a8e4;
L8c66a8e4: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66a8e6;
L8c66a8e6: target=r[3];*pr=0x8c66a8eau;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a8ea;
L8c66a8ea: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66a8ec;
L8c66a8ec: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a8ee;
L8c66a8ee: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a8f0;
L8c66a8f0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a8f2;
L8c66a8f2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a8f4;
L8c66a8f4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a8f6;
L8c66a8f6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
