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
/* Retail driver volume6681e8; integer level conversion and per-channel
 * volume requests, stopping on lower-service errors. */
SA_STAGE_EXPORT int sa_sound_driver_volume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6681e8;
L8c6681e8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6681ea;
L8c6681ea: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6681ec;
L8c6681ec: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6681ee;
L8c6681ee: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6681f0;
L8c6681f0: r[11]=r[4]; /* mov r4,r11 */
 goto L8c6681f2;
L8c6681f2: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6681f4;
L8c6681f4: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6681f6;
L8c6681f6: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6681f8;
L8c6681f8: r[4]=sx(m,0x8c6682e8u,2); /* mov.w 0x8c6682e8,r4 */
 goto L8c6681fa;
L8c6681fa: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6681fc;
L8c6681fc: *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[4]); /* cmp/gt r4,r5 */
 goto L8c6681fe;
L8c6681fe: take=!(*sr&1u);r[6]=4294967169u;if(take)goto L8c668232;goto L8c668202; /* bf/s 0x8c668232 */
 goto L8c668202;
L8c668202: *sr=(*sr&~1u)|((int32_t)r[5]>=0); /* cmp/pz r5 */
 goto L8c668204;
L8c668204: if(!(*sr&1u))goto L8c66820a; /* bf 0x8c66820a */
 goto L8c668206;
L8c668206: r[14]=127u;goto L8c668234; /* bra 0x8c668234 */
 goto L8c66820a;
L8c66820a: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66820c;
L8c66820c: r[5]<<=8; /* shll8 r5 */
 goto L8c66820e;
L8c66820e: r[1]=sx(m,0x8c6682eau,2); /* mov.w 0x8c6682ea,r1 */
 goto L8c668210;
L8c668210: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c668212;
L8c668212: r[3]=rd(m,0x8c6682ecu,4); /* mov.l 0x8c6682ec,r3 */
 goto L8c668214;
L8c668214: r[1]-=r[5]; /* sub r5,r1 */
 goto L8c668216;
L8c668216: target=r[3];*pr=0x8c66821au;r[0]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66821a;
L8c66821a: r[1]=r[0]; /* mov r0,r1 */
 goto L8c66821c;
L8c66821c: r[1]<<=2; /* shll2 r1 */
 goto L8c66821e;
L8c66821e: r[3]=r[0]; /* mov r0,r3 */
 goto L8c668220;
L8c668220: r[2]=rd(m,0x8c6682ecu,4); /* mov.l 0x8c6682ec,r2 */
 goto L8c668222;
L8c668222: r[1]+=r[3]; /* add r3,r1 */
 goto L8c668224;
L8c668224: r[4]=r[0]; /* mov r0,r4 */
 goto L8c668226;
L8c668226: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c668228;
L8c668228: target=r[2];*pr=0x8c66822cu;r[0]=18u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66822c;
L8c66822c: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66822e;
L8c66822e: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[6]); /* cmp/ge r6,r14 */
 goto L8c668230;
L8c668230: if((*sr&1u))goto L8c668234; /* bt 0x8c668234 */
 goto L8c668232;
L8c668232: r[14]=r[6]; /* mov r6,r14 */
 goto L8c668234;
L8c668234: r[10]=rd(m,0x8c6682f8u,4); /* mov.l 0x8c6682f8,r10 */
 goto L8c668236;
L8c668236: r[9]=rd(m,0x8c6682f4u,4); /* mov.l 0x8c6682f4,r9 */
 goto L8c668238;
L8c668238: r[8]=rd(m,0x8c6682f0u,4); /* mov.l 0x8c6682f0,r8 */
 goto L8c66823a;
L8c66823a: r[13]=0u;goto L8c668258; /* bra 0x8c668258 */
 goto L8c66823e;
L8c66823e: r[5]=r[13]; /* mov r13,r5 */
 goto L8c668240;
L8c668240: r[6]=r[14]; /* mov r14,r6 */
 goto L8c668242;
L8c668242: r[7]=0u; /* mov #0x0,r7 */
 goto L8c668244;
L8c668244: target=r[10];*pr=0x8c668248u;r[4]=rd(m,r[11]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c668248;
L8c668248: r[12]=r[0]; /* mov r0,r12 */
 goto L8c66824a;
L8c66824a: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c66824c;
L8c66824c: if((*sr&1u))goto L8c668256; /* bt 0x8c668256 */
 goto L8c66824e;
L8c66824e: target=r[9];*pr=0x8c668252u;r[4]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c668252;
L8c668252: goto L8c66825e; /* bra 0x8c66825e */
 goto L8c668256;
L8c668256: r[13]+=1u; /* add #0x1,r13 */
 goto L8c668258;
L8c668258: r[3]=rd(m,r[11]+0x8u,4); /* mov.l @(0x8,r11),r3 */
 goto L8c66825a;
L8c66825a: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]); /* cmp/ge r3,r13 */
 goto L8c66825c;
L8c66825c: if(!(*sr&1u))goto L8c66823e; /* bf 0x8c66823e */
 goto L8c66825e;
L8c66825e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668260;
L8c668260: r[0]=r[12]; /* mov r12,r0 */
 goto L8c668262;
L8c668262: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c668264;
L8c668264: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c668266;
L8c668266: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c668268;
L8c668268: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66826a;
L8c66826a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66826c;
L8c66826c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66826e;
L8c66826e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
