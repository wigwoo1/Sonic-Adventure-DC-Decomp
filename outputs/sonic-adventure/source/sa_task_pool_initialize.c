#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 8c09844a..8c09852e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_pool_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09844a;
L8c09844a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c09844c;
L8c09844c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c09844e;
L8c09844e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098450;
L8c098450: r[4]=rd(m,0x8c0985a4u,4); /* mov.l 0x8c0985a4,r4 */
 goto L8c098452;
L8c098452: *pr=0x8c098456u;status=call(ctx,0x8c098a80u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098a80 */
 goto L8c098456;
L8c098456: r[2]=rd(m,0x8c0985a8u,4); /* mov.l 0x8c0985a8,r2 */
 goto L8c098458;
L8c098458: r[3]=r[0]; /* mov r0,r3 */
 goto L8c09845a;
L8c09845a: r[6]=rd(m,0x8c0985acu,4); /* mov.l 0x8c0985ac,r6 */
 goto L8c09845c;
L8c09845c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09845e;
L8c09845e: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c098460;
L8c098460: take=(*sr&1u);wr(m,r[6],r[0],4);if(take)goto L8c098526;goto L8c098464; /* bt/s 0x8c098526 */
 goto L8c098464;
L8c098464: r[4]=rd(m,0x8c0985b0u,4); /* mov.l 0x8c0985b0,r4 */
 goto L8c098466;
L8c098466: r[7]=r[4]; /* mov r4,r7 */
 goto L8c098468;
L8c098468: r[7]+=32u; /* add #0x20,r7 */
 goto L8c09846a;
L8c09846a: r[5]=r[4]; /* mov r4,r5 */
 goto L8c09846c;
L8c09846c: *sr=(*sr&~1u)|(r[5]>=r[7]); /* cmp/hs r7,r5 */
 goto L8c09846e;
L8c09846e: take=(*sr&1u);r[4]=0u;if(take)goto L8c09847e;goto L8c098472; /* bt/s 0x8c09847e */
 goto L8c098472;
L8c098472: wr(m,r[5],r[4],4); /* mov.l r4,@r5 */
 goto L8c098474;
L8c098474: r[5]+=4u; /* add #0x4,r5 */
 goto L8c098476;
L8c098476: wr(m,r[5],r[4],4); /* mov.l r4,@r5 */
 goto L8c098478;
L8c098478: r[5]+=4u; /* add #0x4,r5 */
 goto L8c09847a;
L8c09847a: *sr=(*sr&~1u)|(r[5]>=r[7]); /* cmp/hs r7,r5 */
 goto L8c09847c;
L8c09847c: if(!(*sr&1u))goto L8c098472; /* bf 0x8c098472 */
 goto L8c09847e;
L8c09847e: r[12]=16u; /* mov #0x10,r12 */
 goto L8c098480;
L8c098480: r[6]=rd(m,r[6],4); /* mov.l @r6,r6 */
 goto L8c098482;
L8c098482: r[11]=16u; /* mov #0x10,r11 */
 goto L8c098484;
L8c098484: r[1]=r[4]; /* mov r4,r1 */
 goto L8c098486;
L8c098486: r[12]<<=8; /* shll8 r12 */
 goto L8c098488;
L8c098488: r[5]=r[6]; /* mov r6,r5 */
 goto L8c09848a;
L8c09848a: r[12]+=4294967295u; /* add #-0x1,r12 */
 goto L8c09848c;
L8c09848c: r[7]=r[4]; /* mov r4,r7 */
 goto L8c09848e;
L8c09848e: r[11]<<=8; /* shll8 r11 */
 goto L8c098490;
L8c098490: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]); /* cmp/ge r12,r7 */
 goto L8c098492;
L8c098492: take=!(*sr&1u);r[5]+=48u;if(take)goto L8c098498;goto L8c098496; /* bf/s 0x8c098498 */
 goto L8c098496;
L8c098496: r[5]=r[4]; /* mov r4,r5 */
 goto L8c098498;
L8c098498: r[7]+=1u; /* add #0x1,r7 */
 goto L8c09849a;
L8c09849a: wr(m,r[6],r[5],4); /* mov.l r5,@r6 */
 goto L8c09849c;
L8c09849c: wr(m,r[6]+0x4u,r[1],4); /* mov.l r1,@(0x4,r6) */
 goto L8c09849e;
L8c09849e: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]); /* cmp/ge r12,r7 */
 goto L8c0984a0;
L8c0984a0: r[1]=r[5]; /* mov r5,r1 */
 goto L8c0984a2;
L8c0984a2: wr(m,r[6]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r6) */
 goto L8c0984a4;
L8c0984a4: wr(m,r[6]+0x20u,r[4],4); /* mov.l r4,@(0x20,r6) */
 goto L8c0984a6;
L8c0984a6: wr(m,r[6]+0x24u,r[4],4); /* mov.l r4,@(0x24,r6) */
 goto L8c0984a8;
L8c0984a8: wr(m,r[6]+0x28u,r[4],4); /* mov.l r4,@(0x28,r6) */
 goto L8c0984aa;
L8c0984aa: wr(m,r[6]+0x2cu,r[4],4); /* mov.l r4,@(0x2c,r6) */
 goto L8c0984ac;
L8c0984ac: wr(m,r[6]+0xcu,r[4],4); /* mov.l r4,@(0xc,r6) */
 goto L8c0984ae;
L8c0984ae: wr(m,r[6]+0x8u,r[4],4); /* mov.l r4,@(0x8,r6) */
 goto L8c0984b0;
L8c0984b0: wr(m,r[6]+0x10u,r[4],4); /* mov.l r4,@(0x10,r6) */
 goto L8c0984b2;
L8c0984b2: wr(m,r[6]+0x14u,r[4],4); /* mov.l r4,@(0x14,r6) */
 goto L8c0984b4;
L8c0984b4: wr(m,r[6]+0x18u,r[4],4); /* mov.l r4,@(0x18,r6) */
 goto L8c0984b6;
L8c0984b6: take=!(*sr&1u);r[5]+=48u;if(take)goto L8c0984bc;goto L8c0984ba; /* bf/s 0x8c0984bc */
 goto L8c0984ba;
L8c0984ba: r[5]=r[4]; /* mov r4,r5 */
 goto L8c0984bc;
L8c0984bc: r[7]+=1u; /* add #0x1,r7 */
 goto L8c0984be;
L8c0984be: r[0]=r[5]; /* mov r5,r0 */
 goto L8c0984c0;
L8c0984c0: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]); /* cmp/ge r12,r7 */
 goto L8c0984c2;
L8c0984c2: wr(m,r[1],r[5],4); /* mov.l r5,@r1 */
 goto L8c0984c4;
L8c0984c4: wr(m,r[1]+0x4u,r[6],4); /* mov.l r6,@(0x4,r1) */
 goto L8c0984c6;
L8c0984c6: wr(m,r[1]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r1) */
 goto L8c0984c8;
L8c0984c8: wr(m,r[1]+0x20u,r[4],4); /* mov.l r4,@(0x20,r1) */
 goto L8c0984ca;
L8c0984ca: wr(m,r[1]+0x24u,r[4],4); /* mov.l r4,@(0x24,r1) */
 goto L8c0984cc;
L8c0984cc: wr(m,r[1]+0x28u,r[4],4); /* mov.l r4,@(0x28,r1) */
 goto L8c0984ce;
L8c0984ce: wr(m,r[1]+0x2cu,r[4],4); /* mov.l r4,@(0x2c,r1) */
 goto L8c0984d0;
L8c0984d0: wr(m,r[1]+0xcu,r[4],4); /* mov.l r4,@(0xc,r1) */
 goto L8c0984d2;
L8c0984d2: wr(m,r[1]+0x8u,r[4],4); /* mov.l r4,@(0x8,r1) */
 goto L8c0984d4;
L8c0984d4: wr(m,r[1]+0x10u,r[4],4); /* mov.l r4,@(0x10,r1) */
 goto L8c0984d6;
L8c0984d6: wr(m,r[1]+0x14u,r[4],4); /* mov.l r4,@(0x14,r1) */
 goto L8c0984d8;
L8c0984d8: wr(m,r[1]+0x18u,r[4],4); /* mov.l r4,@(0x18,r1) */
 goto L8c0984da;
L8c0984da: take=!(*sr&1u);r[5]+=48u;if(take)goto L8c0984e0;goto L8c0984de; /* bf/s 0x8c0984e0 */
 goto L8c0984de;
L8c0984de: r[5]=r[4]; /* mov r4,r5 */
 goto L8c0984e0;
L8c0984e0: r[7]+=1u; /* add #0x1,r7 */
 goto L8c0984e2;
L8c0984e2: r[6]=r[5]; /* mov r5,r6 */
 goto L8c0984e4;
L8c0984e4: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]); /* cmp/ge r12,r7 */
 goto L8c0984e6;
L8c0984e6: wr(m,r[0],r[5],4); /* mov.l r5,@r0 */
 goto L8c0984e8;
L8c0984e8: wr(m,r[0]+0x4u,r[1],4); /* mov.l r1,@(0x4,r0) */
 goto L8c0984ea;
L8c0984ea: wr(m,r[0]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r0) */
 goto L8c0984ec;
L8c0984ec: wr(m,r[0]+0x20u,r[4],4); /* mov.l r4,@(0x20,r0) */
 goto L8c0984ee;
L8c0984ee: wr(m,r[0]+0x24u,r[4],4); /* mov.l r4,@(0x24,r0) */
 goto L8c0984f0;
L8c0984f0: wr(m,r[0]+0x28u,r[4],4); /* mov.l r4,@(0x28,r0) */
 goto L8c0984f2;
L8c0984f2: wr(m,r[0]+0x2cu,r[4],4); /* mov.l r4,@(0x2c,r0) */
 goto L8c0984f4;
L8c0984f4: wr(m,r[0]+0xcu,r[4],4); /* mov.l r4,@(0xc,r0) */
 goto L8c0984f6;
L8c0984f6: wr(m,r[0]+0x8u,r[4],4); /* mov.l r4,@(0x8,r0) */
 goto L8c0984f8;
L8c0984f8: wr(m,r[0]+0x10u,r[4],4); /* mov.l r4,@(0x10,r0) */
 goto L8c0984fa;
L8c0984fa: wr(m,r[0]+0x14u,r[4],4); /* mov.l r4,@(0x14,r0) */
 goto L8c0984fc;
L8c0984fc: wr(m,r[0]+0x18u,r[4],4); /* mov.l r4,@(0x18,r0) */
 goto L8c0984fe;
L8c0984fe: take=!(*sr&1u);r[5]+=48u;if(take)goto L8c098504;goto L8c098502; /* bf/s 0x8c098504 */
 goto L8c098502;
L8c098502: r[5]=r[4]; /* mov r4,r5 */
 goto L8c098504;
L8c098504: r[7]+=1u; /* add #0x1,r7 */
 goto L8c098506;
L8c098506: r[1]=r[6]; /* mov r6,r1 */
 goto L8c098508;
L8c098508: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[11]); /* cmp/ge r11,r7 */
 goto L8c09850a;
L8c09850a: wr(m,r[6],r[5],4); /* mov.l r5,@r6 */
 goto L8c09850c;
L8c09850c: wr(m,r[6]+0x4u,r[0],4); /* mov.l r0,@(0x4,r6) */
 goto L8c09850e;
L8c09850e: wr(m,r[6]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r6) */
 goto L8c098510;
L8c098510: wr(m,r[6]+0x20u,r[4],4); /* mov.l r4,@(0x20,r6) */
 goto L8c098512;
L8c098512: wr(m,r[6]+0x24u,r[4],4); /* mov.l r4,@(0x24,r6) */
 goto L8c098514;
L8c098514: wr(m,r[6]+0x28u,r[4],4); /* mov.l r4,@(0x28,r6) */
 goto L8c098516;
L8c098516: wr(m,r[6]+0x2cu,r[4],4); /* mov.l r4,@(0x2c,r6) */
 goto L8c098518;
L8c098518: wr(m,r[6]+0xcu,r[4],4); /* mov.l r4,@(0xc,r6) */
 goto L8c09851a;
L8c09851a: wr(m,r[6]+0x8u,r[4],4); /* mov.l r4,@(0x8,r6) */
 goto L8c09851c;
L8c09851c: wr(m,r[6]+0x10u,r[4],4); /* mov.l r4,@(0x10,r6) */
 goto L8c09851e;
L8c09851e: wr(m,r[6]+0x14u,r[4],4); /* mov.l r4,@(0x14,r6) */
 goto L8c098520;
L8c098520: wr(m,r[6]+0x18u,r[4],4); /* mov.l r4,@(0x18,r6) */
 goto L8c098522;
L8c098522: take=!(*sr&1u);r[6]=r[5];if(take)goto L8c098490;goto L8c098526; /* bf/s 0x8c098490 */
 goto L8c098526;
L8c098526: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c098528;
L8c098528: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c09852a;
L8c09852a: r[12]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
