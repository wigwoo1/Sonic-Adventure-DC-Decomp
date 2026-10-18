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
/* Retail driver control6682be: cached control update and converted level
 * requests across channels. Higher-level semantic naming remains provisional. */
SA_STAGE_EXPORT int sa_sound_driver_control_6682be(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6682be;
L8c6682be: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6682c0;
L8c6682c0: r[3]=4294967254u; /* mov #-0x2a,r3 */
 goto L8c6682c2;
L8c6682c2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6682c4;
L8c6682c4: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]); /* cmp/ge r3,r6 */
 goto L8c6682c6;
L8c6682c6: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6682c8;
L8c6682c8: r[13]=r[4]; /* mov r4,r13 */
 goto L8c6682ca;
L8c6682ca: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6682cc;
L8c6682cc: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6682ce;
L8c6682ce: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6682d0;
L8c6682d0: r[9]=r[5]; /* mov r5,r9 */
 goto L8c6682d2;
L8c6682d2: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6682d4;
L8c6682d4: r[8]=0u; /* mov #0x0,r8 */
 goto L8c6682d6;
L8c6682d6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6682d8;
L8c6682d8: take=(*sr&1u);r[14]=r[8];if(take)goto L8c6682e0;goto L8c6682dc; /* bt/s 0x8c6682e0 */
 goto L8c6682dc;
L8c6682dc: r[10]=4294967169u;goto L8c668314; /* bra 0x8c668314 */
 goto L8c6682e0;
L8c6682e0: *sr=(*sr&~1u)|((int32_t)r[6]>=0); /* cmp/pz r6 */
 goto L8c6682e2;
L8c6682e2: if(!(*sr&1u))goto L8c668304; /* bf 0x8c668304 */
 goto L8c6682e4;
L8c6682e4: r[10]=127u;goto L8c668314; /* bra 0x8c668314 */
 goto L8c668304;
L8c668304: r[2]=r[6]; /* mov r6,r2 */
 goto L8c668306;
L8c668306: r[1]=0u; /* mov #0x0,r1 */
 goto L8c668308;
L8c668308: r[10]=r[6]; /* mov r6,r10 */
 goto L8c66830a;
L8c66830a: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1; /* shll r10 */
 goto L8c66830c;
L8c66830c: *sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[2]); /* cmp/gt r2,r1 */
 goto L8c66830e;
L8c66830e: {uint64_t sum=(uint64_t)r[2]+r[1]+(*sr&1u);r[2]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r1,r2 */
 goto L8c668310;
L8c668310: *sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u); /* shar r2 */
 goto L8c668312;
L8c668312: r[10]+=r[2]; /* add r2,r10 */
 goto L8c668314;
L8c668314: r[0]=64u; /* mov #0x40,r0 */
 goto L8c668316;
L8c668316: r[11]=rd(m,0x8c668400u,4); /* mov.l 0x8c668400,r11 */
 goto L8c668318;
L8c668318: r[3]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r3 */
 goto L8c66831a;
L8c66831a: *sr=(*sr&~1u)|(r[9]==r[3]); /* cmp/eq r9,r3 */
 goto L8c66831c;
L8c66831c: if((*sr&1u))goto L8c668346; /* bt 0x8c668346 */
 goto L8c66831e;
L8c66831e: r[12]=r[8];goto L8c66833c; /* bra 0x8c66833c */
 goto L8c668322;
L8c668322: r[2]=rd(m,0x8c668404u,4); /* mov.l 0x8c668404,r2 */
 goto L8c668324;
L8c668324: r[5]=r[12]; /* mov r12,r5 */
 goto L8c668326;
L8c668326: r[7]=127u; /* mov #0x7f,r7 */
 goto L8c668328;
L8c668328: r[6]=r[9]; /* mov r9,r6 */
 goto L8c66832a;
L8c66832a: target=r[2];*pr=0x8c66832eu;r[4]=rd(m,r[13]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66832e;
L8c66832e: r[14]=r[0]; /* mov r0,r14 */
 goto L8c668330;
L8c668330: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c668332;
L8c668332: if((*sr&1u))goto L8c66833a; /* bt 0x8c66833a */
 goto L8c668334;
L8c668334: r[4]=rd(m,0x8c668408u,4); /* mov.l 0x8c668408,r4 */
 goto L8c668336;
L8c668336: target=r[11];*pr=0x8c66833au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c66833a;
L8c66833a: r[12]+=1u; /* add #0x1,r12 */
 goto L8c66833c;
L8c66833c: r[2]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r2 */
 goto L8c66833e;
L8c66833e: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]); /* cmp/ge r2,r12 */
 goto L8c668340;
L8c668340: if(!(*sr&1u))goto L8c668322; /* bf 0x8c668322 */
 goto L8c668342;
L8c668342: r[0]=64u; /* mov #0x40,r0 */
 goto L8c668344;
L8c668344: wr(m,r[13]+r[0],r[9],4); /* mov.l r9,@(r0,r13) */
 goto L8c668346;
L8c668346: r[9]=rd(m,0x8c66840cu,4); /* mov.l 0x8c66840c,r9 */
 goto L8c668348;
L8c668348: r[12]=r[8];goto L8c668362; /* bra 0x8c668362 */
 goto L8c66834c;
L8c66834c: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66834e;
L8c66834e: r[6]=r[10]; /* mov r10,r6 */
 goto L8c668350;
L8c668350: target=r[9];*pr=0x8c668354u;r[4]=rd(m,r[13]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c668354;
L8c668354: r[14]=r[0]; /* mov r0,r14 */
 goto L8c668356;
L8c668356: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c668358;
L8c668358: if((*sr&1u))goto L8c668360; /* bt 0x8c668360 */
 goto L8c66835a;
L8c66835a: r[4]=rd(m,0x8c668410u,4); /* mov.l 0x8c668410,r4 */
 goto L8c66835c;
L8c66835c: target=r[11];*pr=0x8c668360u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c668360;
L8c668360: r[12]+=1u; /* add #0x1,r12 */
 goto L8c668362;
L8c668362: r[3]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r3 */
 goto L8c668364;
L8c668364: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[3]); /* cmp/ge r3,r12 */
 goto L8c668366;
L8c668366: if(!(*sr&1u))goto L8c66834c; /* bf 0x8c66834c */
 goto L8c668368;
L8c668368: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66836a;
L8c66836a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66836c;
L8c66836c: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c66836e;
L8c66836e: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c668370;
L8c668370: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c668372;
L8c668372: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c668374;
L8c668374: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c668376;
L8c668376: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668378;
L8c668378: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
