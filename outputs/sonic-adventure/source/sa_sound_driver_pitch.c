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
/* Static C recovery of retail Tikal helper 8c6683be..8c66846a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_pitch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6683be;
L8c6683be: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6683c0;
L8c6683c0: r[14]=4294967196u; /* mov #-0x64,r14 */
 goto L8c6683c2;
L8c6683c2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6683c4;
L8c6683c4: r[7]=(uint32_t)(int32_t)(int16_t)r[5]; /* exts.w r5,r7 */
 goto L8c6683c6;
L8c6683c6: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6683c8;
L8c6683c8: *sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[14]); /* cmp/gt r14,r7 */
 goto L8c6683ca;
L8c6683ca: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6683cc;
L8c6683cc: r[12]=r[4]; /* mov r4,r12 */
 goto L8c6683ce;
L8c6683ce: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6683d0;
L8c6683d0: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6683d2;
L8c6683d2: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6683d4;
L8c6683d4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6683d6;
L8c6683d6: take=(*sr&1u);r[4]=100u;if(take)goto L8c6683de;goto L8c6683da; /* bt/s 0x8c6683de */
 goto L8c6683da;
L8c6683da: r[7]=r[14];goto L8c6683e4; /* bra 0x8c6683e4 */
 goto L8c6683de;
L8c6683de: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c6683e0;
L8c6683e0: if(!(*sr&1u))goto L8c6683e4; /* bf 0x8c6683e4 */
 goto L8c6683e2;
L8c6683e2: r[7]=r[4]; /* mov r4,r7 */
 goto L8c6683e4;
L8c6683e4: r[5]=(uint32_t)(int32_t)(int16_t)r[6]; /* exts.w r6,r5 */
 goto L8c6683e6;
L8c6683e6: *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[14]); /* cmp/gt r14,r5 */
 goto L8c6683e8;
L8c6683e8: take=(*sr&1u);r[7]<<=8;if(take)goto L8c6683f2;goto L8c6683ec; /* bt/s 0x8c6683f2 */
 goto L8c6683ec;
L8c6683ec: r[5]=sx(m,0x8c6683fcu,2); /* mov.w 0x8c6683fc,r5 */
 goto L8c6683ee;
L8c6683ee: goto L8c66842c; /* bra 0x8c66842c */
 goto L8c6683f2;
L8c6683f2: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[4]); /* cmp/ge r4,r5 */
 goto L8c6683f4;
L8c6683f4: if(!(*sr&1u))goto L8c668420; /* bf 0x8c668420 */
 goto L8c6683f6;
L8c6683f6: r[5]=sx(m,0x8c6683feu,2); /* mov.w 0x8c6683fe,r5 */
 goto L8c6683f8;
L8c6683f8: goto L8c66842c; /* bra 0x8c66842c */
 goto L8c668420;
L8c668420: r[3]=rd(m,0x8c6686b8u,4); /* mov.l 0x8c6686b8,r3 */
 goto L8c668422;
L8c668422: r[1]=(uint32_t)(int32_t)(int16_t)r[6]; /* exts.w r6,r1 */
 goto L8c668424;
L8c668424: r[1]<<=8; /* shll8 r1 */
 goto L8c668426;
L8c668426: target=r[3];*pr=0x8c66842au;r[0]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66842a;
L8c66842a: r[5]=r[0]; /* mov r0,r5 */
 goto L8c66842c;
L8c66842c: r[10]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r10 */
 goto L8c66842e;
L8c66842e: r[8]=r[7]; /* mov r7,r8 */
 goto L8c668430;
L8c668430: r[11]=rd(m,0x8c6686c4u,4); /* mov.l 0x8c6686c4,r11 */
 goto L8c668432;
L8c668432: r[8]+=r[5]; /* add r5,r8 */
 goto L8c668434;
L8c668434: r[9]=rd(m,0x8c6686bcu,4); /* mov.l 0x8c6686bc,r9 */
 goto L8c668436;
L8c668436: r[14]=0u;goto L8c668450; /* bra 0x8c668450 */
 goto L8c66843a;
L8c66843a: r[5]=r[14]; /* mov r14,r5 */
 goto L8c66843c;
L8c66843c: r[6]=r[8]; /* mov r8,r6 */
 goto L8c66843e;
L8c66843e: r[7]=0u; /* mov #0x0,r7 */
 goto L8c668440;
L8c668440: target=r[11];*pr=0x8c668444u;r[4]=rd(m,r[12]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c668444;
L8c668444: r[13]=r[0]; /* mov r0,r13 */
 goto L8c668446;
L8c668446: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c668448;
L8c668448: if((*sr&1u))goto L8c66844e; /* bt 0x8c66844e */
 goto L8c66844a;
L8c66844a: target=r[10];*pr=0x8c66844eu;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c66844e;
L8c66844e: r[14]+=1u; /* add #0x1,r14 */
 goto L8c668450;
L8c668450: r[2]=rd(m,r[12]+0x8u,4); /* mov.l @(0x8,r12),r2 */
 goto L8c668452;
L8c668452: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[2]); /* cmp/ge r2,r14 */
 goto L8c668454;
L8c668454: if(!(*sr&1u))goto L8c66843a; /* bf 0x8c66843a */
 goto L8c668456;
L8c668456: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668458;
L8c668458: r[0]=r[13]; /* mov r13,r0 */
 goto L8c66845a;
L8c66845a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c66845c;
L8c66845c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66845e;
L8c66845e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c668460;
L8c668460: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c668462;
L8c668462: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c668464;
L8c668464: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668466;
L8c668466: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
