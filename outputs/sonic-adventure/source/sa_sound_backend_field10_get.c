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
/* Static C recovery of retail sound backend format-adjusted reader 8c66accc..8c66ad9a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_field10_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66accc;
L8c66accc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66acce;
L8c66acce: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66acd0;
L8c66acd0: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66acd2;
L8c66acd2: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66acd4;
L8c66acd4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66acd6;
L8c66acd6: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c66acd8;
L8c66acd8: r[3]=rd(m,0x8c66ada0u,4); /* mov.l 0x8c66ada0,r3 */
 goto L8c66acda;
L8c66acda: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66acdc;
L8c66acdc: wr(m,r[15]+0x4u,r[0],1); /* mov.b r0,@(0x4,r15) */
 goto L8c66acde;
L8c66acde: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c66ace0;
L8c66ace0: r[13]=rd(m,r[3],4); /* mov.l @r3,r13 */
 goto L8c66ace2;
L8c66ace2: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c66ace4;
L8c66ace4: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66acee;goto L8c66ace8; /* bf/s 0x8c66acee */
 goto L8c66ace8;
L8c66ace8: r[14]=rd(m,0x8c66ada4u,4); /* mov.l 0x8c66ada4,r14 */
 goto L8c66acea;
L8c66acea: goto L8c66ad7e; /* bra 0x8c66ad7e */
 goto L8c66acee;
L8c66acee: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66acf0;
L8c66acf0: if(!(*sr&1u))goto L8c66acf8; /* bf 0x8c66acf8 */
 goto L8c66acf2;
L8c66acf2: r[14]=rd(m,0x8c66ada8u,4); /* mov.l 0x8c66ada8,r14 */
 goto L8c66acf4;
L8c66acf4: goto L8c66ad7e; /* bra 0x8c66ad7e */
 goto L8c66acf8;
L8c66acf8: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c66acfa;
L8c66acfa: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66acfc;
L8c66acfc: if(!(*sr&1u))goto L8c66ad04; /* bf 0x8c66ad04 */
 goto L8c66acfe;
L8c66acfe: r[14]=rd(m,0x8c66adacu,4); /* mov.l 0x8c66adac,r14 */
 goto L8c66ad00;
L8c66ad00: goto L8c66ad7e; /* bra 0x8c66ad7e */
 goto L8c66ad04;
L8c66ad04: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66ad06;
L8c66ad06: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c66ad08;
L8c66ad08: r[2]=sx(m,0x8c66ad9au,2); /* mov.w 0x8c66ad9a,r2 */
 goto L8c66ad0a;
L8c66ad0a: r[5]=r[15]; /* mov r15,r5 */
 goto L8c66ad0c;
L8c66ad0c: r[11]=r[0]; /* mov r0,r11 */
 goto L8c66ad0e;
L8c66ad0e: r[12]=(uint32_t)(int32_t)(int8_t)r[11]; /* exts.b r11,r12 */
 goto L8c66ad10;
L8c66ad10: *macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[12]); /* muls.w r3,r12 */
 goto L8c66ad12;
L8c66ad12: r[0]=sx(m,0x8c66ad9cu,2); /* mov.w 0x8c66ad9c,r0 */
 goto L8c66ad14;
L8c66ad14: r[2]+=r[13]; /* add r13,r2 */
 goto L8c66ad16;
L8c66ad16: r[11]=(uint32_t)(int32_t)(int8_t)r[11]; /* exts.b r11,r11 */
 goto L8c66ad18;
L8c66ad18: r[3]=rd(m,0x8c66adb0u,4); /* mov.l 0x8c66adb0,r3 */
 goto L8c66ad1a;
L8c66ad1a: r[4]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r4 */
 goto L8c66ad1c;
L8c66ad1c: r[11]<<=2; /* shll2 r11 */
 goto L8c66ad1e;
L8c66ad1e: r[12]=*macl; /* sts MACL,r12 */
 goto L8c66ad20;
L8c66ad20: r[0]=sx(m,r[15]+0x4u,1); /* mov.b @(0x4,r15),r0 */
 goto L8c66ad22;
L8c66ad22: r[11]<<=2; /* shll2 r11 */
 goto L8c66ad24;
L8c66ad24: r[11]<<=2; /* shll2 r11 */
 goto L8c66ad26;
L8c66ad26: r[12]+=r[2]; /* add r2,r12 */
 goto L8c66ad28;
L8c66ad28: r[2]=sx(m,0x8c66ad9eu,2); /* mov.w 0x8c66ad9e,r2 */
 goto L8c66ad2a;
L8c66ad2a: r[0]<<=2; /* shll2 r0 */
 goto L8c66ad2c;
L8c66ad2c: r[0]<<=2; /* shll2 r0 */
 goto L8c66ad2e;
L8c66ad2e: r[4]+=r[2]; /* add r2,r4 */
 goto L8c66ad30;
L8c66ad30: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c66ad32;
L8c66ad32: r[4]+=r[11]; /* add r11,r4 */
 goto L8c66ad34;
L8c66ad34: r[4]+=r[0]; /* add r0,r4 */
 goto L8c66ad36;
L8c66ad36: target=r[3];*pr=0x8c66ad3au;r[4]+=24u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ad3a;
L8c66ad3a: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ad3c;
L8c66ad3c: r[0]=sx(m,r[15]+0x4u,1); /* mov.b @(0x4,r15),r0 */
 goto L8c66ad3e;
L8c66ad3e: r[3]=rd(m,0x8c66adb8u,4); /* mov.l 0x8c66adb8,r3 */
 goto L8c66ad40;
L8c66ad40: r[6]=r[15]; /* mov r15,r6 */
 goto L8c66ad42;
L8c66ad42: r[4]=rd(m,0x8c66adb4u,4); /* mov.l 0x8c66adb4,r4 */
 goto L8c66ad44;
L8c66ad44: r[7]=r[15]; /* mov r15,r7 */
 goto L8c66ad46;
L8c66ad46: r[7]+=16u; /* add #0x10,r7 */
 goto L8c66ad48;
L8c66ad48: r[6]+=12u; /* add #0xc,r6 */
 goto L8c66ad4a;
L8c66ad4a: r[5]+=7u; /* add #0x7,r5 */
 goto L8c66ad4c;
L8c66ad4c: target=r[3];*pr=0x8c66ad50u;r[5]=sx(m,r[5]+r[0],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ad50;
L8c66ad50: r[0]=rd(m,r[12],4); /* mov.l @r12,r0 */
 goto L8c66ad52;
L8c66ad52: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66ad54;
L8c66ad54: if((*sr&1u))goto L8c66ad62; /* bt 0x8c66ad62 */
 goto L8c66ad56;
L8c66ad56: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c66ad58;
L8c66ad58: if((*sr&1u))goto L8c66ad78; /* bt 0x8c66ad78 */
 goto L8c66ad5a;
L8c66ad5a: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66ad5c;
L8c66ad5c: if((*sr&1u))goto L8c66ad6c; /* bt 0x8c66ad6c */
 goto L8c66ad5e;
L8c66ad5e: goto L8c66ad78; /* bra 0x8c66ad78 */
 goto L8c66ad62;
L8c66ad62: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c66ad64;
L8c66ad64: r[2]=r[3]; /* mov r3,r2 */
 goto L8c66ad66;
L8c66ad66: r[2]+=r[3]; /* add r3,r2 */
 goto L8c66ad68;
L8c66ad68: wr(m,r[15],r[2],4);goto L8c66ad78; /* bra 0x8c66ad78 */
 goto L8c66ad6c;
L8c66ad6c: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c66ad6e;
L8c66ad6e: r[3]=0u; /* mov #0x0,r3 */
 goto L8c66ad70;
L8c66ad70: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[1]); /* cmp/gt r1,r3 */
 goto L8c66ad72;
L8c66ad72: {uint64_t sum=(uint64_t)r[1]+r[3]+(*sr&1u);r[1]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r3,r1 */
 goto L8c66ad74;
L8c66ad74: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c66ad76;
L8c66ad76: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c66ad78;
L8c66ad78: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c66ad7a;
L8c66ad7a: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c66ad7c;
L8c66ad7c: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66ad7e;
L8c66ad7e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66ad80;
L8c66ad80: if((*sr&1u))goto L8c66ad8a; /* bt 0x8c66ad8a */
 goto L8c66ad82;
L8c66ad82: r[2]=rd(m,0x8c66adc0u,4); /* mov.l 0x8c66adc0,r2 */
 goto L8c66ad84;
L8c66ad84: r[4]=rd(m,0x8c66adbcu,4); /* mov.l 0x8c66adbc,r4 */
 goto L8c66ad86;
L8c66ad86: target=r[2];*pr=0x8c66ad8au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ad8a;
L8c66ad8a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66ad8c;
L8c66ad8c: r[15]+=20u; /* add #0x14,r15 */
 goto L8c66ad8e;
L8c66ad8e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ad90;
L8c66ad90: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66ad92;
L8c66ad92: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66ad94;
L8c66ad94: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66ad96;
L8c66ad96: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
