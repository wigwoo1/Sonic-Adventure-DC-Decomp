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
/* Static C recovery of retail Sound bank section configuration helper 8c65a730..8c65a8a4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_bank_section_configure(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a730;
L8c65a730: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a732;
L8c65a732: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65a734;
L8c65a734: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65a736;
L8c65a736: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65a738;
L8c65a738: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a73a;
L8c65a73a: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65a73c;
L8c65a73c: r[3]=rd(m,0x8c65a7dcu,4); /* mov.l 0x8c65a7dc,r3 */
 goto L8c65a73e;
L8c65a73e: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c65a740;
L8c65a740: wr(m,r[15]+0x4u,r[7],4); /* mov.l r7,@(0x4,r15) */
 goto L8c65a742;
L8c65a742: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c65a744;
L8c65a744: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c65a746;
L8c65a746: take=!(*sr&1u);r[12]=0u;if(take)goto L8c65a750;goto L8c65a74a; /* bf/s 0x8c65a750 */
 goto L8c65a74a;
L8c65a74a: r[12]=rd(m,0x8c65a7e0u,4); /* mov.l 0x8c65a7e0,r12 */
 goto L8c65a74c;
L8c65a74c: goto L8c65a894; /* bra 0x8c65a894 */
 goto L8c65a750;
L8c65a750: r[7]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r7 */
 goto L8c65a752;
L8c65a752: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c65a754;
L8c65a754: if((*sr&1u))goto L8c65a75a; /* bt 0x8c65a75a */
 goto L8c65a756;
L8c65a756: goto L8c65a860; /* bra 0x8c65a860 */
 goto L8c65a75a;
L8c65a75a: r[1]=rd(m,0x8c65a7e4u,4); /* mov.l 0x8c65a7e4,r1 */
 goto L8c65a75c;
L8c65a75c: r[11]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r11 */
 goto L8c65a75e;
L8c65a75e: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65a760;
L8c65a760: r[11]<<=2; /* shll2 r11 */
 goto L8c65a762;
L8c65a762: r[13]=1u; /* mov #0x1,r13 */
 goto L8c65a764;
L8c65a764: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c65a766;
L8c65a766: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a768;
L8c65a768: take=(*sr&1u);r[14]=16u;if(take)goto L8c65a7b2;goto L8c65a76c; /* bt/s 0x8c65a7b2 */
 goto L8c65a76c;
L8c65a76c: r[1]=rd(m,0x8c65a7e8u,4); /* mov.l 0x8c65a7e8,r1 */
 goto L8c65a76e;
L8c65a76e: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a770;
L8c65a770: if((*sr&1u))goto L8c65a7c0; /* bt 0x8c65a7c0 */
 goto L8c65a772;
L8c65a772: r[1]=rd(m,0x8c65a7ecu,4); /* mov.l 0x8c65a7ec,r1 */
 goto L8c65a774;
L8c65a774: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a776;
L8c65a776: if((*sr&1u))goto L8c65a814; /* bt 0x8c65a814 */
 goto L8c65a778;
L8c65a778: r[1]=rd(m,0x8c65a7f0u,4); /* mov.l 0x8c65a7f0,r1 */
 goto L8c65a77a;
L8c65a77a: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a77c;
L8c65a77c: if((*sr&1u))goto L8c65a826; /* bt 0x8c65a826 */
 goto L8c65a77e;
L8c65a77e: r[1]=rd(m,0x8c65a7f4u,4); /* mov.l 0x8c65a7f4,r1 */
 goto L8c65a780;
L8c65a780: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a782;
L8c65a782: if((*sr&1u))goto L8c65a83c; /* bt 0x8c65a83c */
 goto L8c65a784;
L8c65a784: r[1]=rd(m,0x8c65a7f8u,4); /* mov.l 0x8c65a7f8,r1 */
 goto L8c65a786;
L8c65a786: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a788;
L8c65a788: if((*sr&1u))goto L8c65a84c; /* bt 0x8c65a84c */
 goto L8c65a78a;
L8c65a78a: r[1]=rd(m,0x8c65a7fcu,4); /* mov.l 0x8c65a7fc,r1 */
 goto L8c65a78c;
L8c65a78c: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a78e;
L8c65a78e: if((*sr&1u))goto L8c65a85c; /* bt 0x8c65a85c */
 goto L8c65a790;
L8c65a790: r[1]=rd(m,0x8c65a800u,4); /* mov.l 0x8c65a800,r1 */
 goto L8c65a792;
L8c65a792: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a794;
L8c65a794: if((*sr&1u))goto L8c65a874; /* bt 0x8c65a874 */
 goto L8c65a796;
L8c65a796: r[1]=rd(m,0x8c65a804u,4); /* mov.l 0x8c65a804,r1 */
 goto L8c65a798;
L8c65a798: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a79a;
L8c65a79a: if((*sr&1u))goto L8c65a874; /* bt 0x8c65a874 */
 goto L8c65a79c;
L8c65a79c: r[1]=rd(m,0x8c65a808u,4); /* mov.l 0x8c65a808,r1 */
 goto L8c65a79e;
L8c65a79e: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a7a0;
L8c65a7a0: if((*sr&1u))goto L8c65a874; /* bt 0x8c65a874 */
 goto L8c65a7a2;
L8c65a7a2: r[1]=rd(m,0x8c65a80cu,4); /* mov.l 0x8c65a80c,r1 */
 goto L8c65a7a4;
L8c65a7a4: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a7a6;
L8c65a7a6: if((*sr&1u))goto L8c65a874; /* bt 0x8c65a874 */
 goto L8c65a7a8;
L8c65a7a8: r[1]=rd(m,0x8c65a810u,4); /* mov.l 0x8c65a810,r1 */
 goto L8c65a7aa;
L8c65a7aa: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65a7ac;
L8c65a7ac: if((*sr&1u))goto L8c65a874; /* bt 0x8c65a874 */
 goto L8c65a7ae;
L8c65a7ae: goto L8c65a874; /* bra 0x8c65a874 */
 goto L8c65a7b2;
L8c65a7b2: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[14]); /* cmp/ge r14,r7 */
 goto L8c65a7b4;
L8c65a7b4: if((*sr&1u))goto L8c65a860; /* bt 0x8c65a860 */
 goto L8c65a7b6;
L8c65a7b6: r[0]=sx(m,0x8c65a7d2u,2); /* mov.w 0x8c65a7d2,r0 */
 goto L8c65a7b8;
L8c65a7b8: r[14]=sx(m,0x8c65a7d4u,2); /* mov.w 0x8c65a7d4,r14 */
 goto L8c65a7ba;
L8c65a7ba: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a7bc;
L8c65a7bc: r[13]+=r[11];goto L8c65a836; /* bra 0x8c65a836 */
 goto L8c65a7c0;
L8c65a7c0: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[14]); /* cmp/ge r14,r7 */
 goto L8c65a7c2;
L8c65a7c2: if((*sr&1u))goto L8c65a860; /* bt 0x8c65a860 */
 goto L8c65a7c4;
L8c65a7c4: r[0]=sx(m,0x8c65a7d2u,2); /* mov.w 0x8c65a7d2,r0 */
 goto L8c65a7c6;
L8c65a7c6: r[3]=sx(m,0x8c65a7d6u,2); /* mov.w 0x8c65a7d6,r3 */
 goto L8c65a7c8;
L8c65a7c8: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a7ca;
L8c65a7ca: r[14]=sx(m,0x8c65a7d8u,2); /* mov.w 0x8c65a7d8,r14 */
 goto L8c65a7cc;
L8c65a7cc: r[13]+=r[3]; /* add r3,r13 */
 goto L8c65a7ce;
L8c65a7ce: r[13]+=r[11];goto L8c65a836; /* bra 0x8c65a836 */
 goto L8c65a814;
L8c65a814: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[14]); /* cmp/ge r14,r7 */
 goto L8c65a816;
L8c65a816: if((*sr&1u))goto L8c65a860; /* bt 0x8c65a860 */
 goto L8c65a818;
L8c65a818: r[0]=sx(m,0x8c65a8a4u,2); /* mov.w 0x8c65a8a4,r0 */
 goto L8c65a81a;
L8c65a81a: r[3]=sx(m,0x8c65a8a6u,2); /* mov.w 0x8c65a8a6,r3 */
 goto L8c65a81c;
L8c65a81c: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a81e;
L8c65a81e: r[14]=sx(m,0x8c65a8a8u,2); /* mov.w 0x8c65a8a8,r14 */
 goto L8c65a820;
L8c65a820: r[13]+=r[3]; /* add r3,r13 */
 goto L8c65a822;
L8c65a822: r[13]+=r[11];goto L8c65a836; /* bra 0x8c65a836 */
 goto L8c65a826;
L8c65a826: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[14]); /* cmp/ge r14,r7 */
 goto L8c65a828;
L8c65a828: if((*sr&1u))goto L8c65a860; /* bt 0x8c65a860 */
 goto L8c65a82a;
L8c65a82a: r[0]=sx(m,0x8c65a8a4u,2); /* mov.w 0x8c65a8a4,r0 */
 goto L8c65a82c;
L8c65a82c: r[3]=sx(m,0x8c65a8aau,2); /* mov.w 0x8c65a8aa,r3 */
 goto L8c65a82e;
L8c65a82e: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a830;
L8c65a830: r[14]=sx(m,0x8c65a8acu,2); /* mov.w 0x8c65a8ac,r14 */
 goto L8c65a832;
L8c65a832: r[13]+=r[3]; /* add r3,r13 */
 goto L8c65a834;
L8c65a834: r[13]+=r[11]; /* add r11,r13 */
 goto L8c65a836;
L8c65a836: r[14]+=r[6]; /* add r6,r14 */
 goto L8c65a838;
L8c65a838: r[14]+=r[11];goto L8c65a87a; /* bra 0x8c65a87a */
 goto L8c65a83c;
L8c65a83c: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]); /* cmp/ge r13,r7 */
 goto L8c65a83e;
L8c65a83e: if((*sr&1u))goto L8c65a860; /* bt 0x8c65a860 */
 goto L8c65a840;
L8c65a840: r[0]=sx(m,0x8c65a8a4u,2); /* mov.w 0x8c65a8a4,r0 */
 goto L8c65a842;
L8c65a842: r[14]=sx(m,0x8c65a8b0u,2); /* mov.w 0x8c65a8b0,r14 */
 goto L8c65a844;
L8c65a844: r[3]=sx(m,0x8c65a8aeu,2); /* mov.w 0x8c65a8ae,r3 */
 goto L8c65a846;
L8c65a846: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a848;
L8c65a848: r[13]+=r[3];goto L8c65a870; /* bra 0x8c65a870 */
 goto L8c65a84c;
L8c65a84c: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]); /* cmp/ge r13,r7 */
 goto L8c65a84e;
L8c65a84e: if((*sr&1u))goto L8c65a860; /* bt 0x8c65a860 */
 goto L8c65a850;
L8c65a850: r[0]=sx(m,0x8c65a8a4u,2); /* mov.w 0x8c65a8a4,r0 */
 goto L8c65a852;
L8c65a852: r[14]=sx(m,0x8c65a8b4u,2); /* mov.w 0x8c65a8b4,r14 */
 goto L8c65a854;
L8c65a854: r[3]=sx(m,0x8c65a8b2u,2); /* mov.w 0x8c65a8b2,r3 */
 goto L8c65a856;
L8c65a856: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a858;
L8c65a858: r[13]+=r[3];goto L8c65a870; /* bra 0x8c65a870 */
 goto L8c65a85c;
L8c65a85c: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]); /* cmp/ge r13,r7 */
 goto L8c65a85e;
L8c65a85e: if(!(*sr&1u))goto L8c65a866; /* bf 0x8c65a866 */
 goto L8c65a860;
L8c65a860: r[12]=rd(m,0x8c65a8bcu,4); /* mov.l 0x8c65a8bc,r12 */
 goto L8c65a862;
L8c65a862: goto L8c65a894; /* bra 0x8c65a894 */
 goto L8c65a866;
L8c65a866: r[0]=sx(m,0x8c65a8a4u,2); /* mov.w 0x8c65a8a4,r0 */
 goto L8c65a868;
L8c65a868: r[3]=sx(m,0x8c65a8b6u,2); /* mov.w 0x8c65a8b6,r3 */
 goto L8c65a86a;
L8c65a86a: r[13]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r13 */
 goto L8c65a86c;
L8c65a86c: r[14]=sx(m,0x8c65a8b8u,2); /* mov.w 0x8c65a8b8,r14 */
 goto L8c65a86e;
L8c65a86e: r[13]+=r[3]; /* add r3,r13 */
 goto L8c65a870;
L8c65a870: r[14]+=r[6];goto L8c65a87a; /* bra 0x8c65a87a */
 goto L8c65a874;
L8c65a874: r[12]=rd(m,0x8c65a8c0u,4); /* mov.l 0x8c65a8c0,r12 */
 goto L8c65a876;
L8c65a876: goto L8c65a894; /* bra 0x8c65a894 */
 goto L8c65a87a;
L8c65a87a: r[3]=rd(m,0x8c65a8c4u,4); /* mov.l 0x8c65a8c4,r3 */
 goto L8c65a87c;
L8c65a87c: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c65a87e;
L8c65a87e: target=r[3];*pr=0x8c65a882u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a882;
L8c65a882: r[3]=rd(m,0x8c65a8c4u,4); /* mov.l 0x8c65a8c4,r3 */
 goto L8c65a884;
L8c65a884: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65a886;
L8c65a886: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65a888;
L8c65a888: target=r[3];*pr=0x8c65a88cu;r[4]+=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a88c;
L8c65a88c: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65a88e;
L8c65a88e: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c65a890;
L8c65a890: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65a892;
L8c65a892: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c65a894;
L8c65a894: r[0]=r[12]; /* mov r12,r0 */
 goto L8c65a896;
L8c65a896: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65a898;
L8c65a898: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a89a;
L8c65a89a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65a89c;
L8c65a89c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65a89e;
L8c65a89e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65a8a0;
L8c65a8a0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
