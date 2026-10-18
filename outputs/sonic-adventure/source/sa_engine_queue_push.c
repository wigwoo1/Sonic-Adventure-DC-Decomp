/* Sonic Adventure USA Rev A main executable queue push, 0x8c667080. */
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
/* Static C recovery of retail Tikal helper 8c667080..8c667134.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_queue_push(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667080;
L8c667080: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c667082;
L8c667082: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667084;
L8c667084: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c667086;
L8c667086: r[0]=*sr; /* stc SR,r0 */
 goto L8c667088;
L8c667088: r[3]=sx(m,0x8c667134u,2); /* mov.w 0x8c667134,r3 */
 goto L8c66708a;
L8c66708a: r[14]=0u; /* mov #0x0,r14 */
 goto L8c66708c;
L8c66708c: r[0]>>=2; /* shlr2 r0 */
 goto L8c66708e;
L8c66708e: r[0]>>=2; /* shlr2 r0 */
 goto L8c667090;
L8c667090: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c667092;
L8c667092: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c667094;
L8c667094: r[0]=*sr; /* stc SR,r0 */
 goto L8c667096;
L8c667096: r[0]&=r[3]; /* and r3,r0 */
 goto L8c667098;
L8c667098: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c66709a;
L8c66709a: *sr=r[0]; /* ldc r0,SR */
 goto L8c66709c;
L8c66709c: r[6]=rd(m,0x8c66713cu,4); /* mov.l 0x8c66713c,r6 */
 goto L8c66709e;
L8c66709e: r[7]=rd(m,r[6],4); /* mov.l @r6,r7 */
 goto L8c6670a0;
L8c6670a0: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c6670a2;
L8c6670a2: if(!(*sr&1u))goto L8c6670aa; /* bf 0x8c6670aa */
 goto L8c6670a4;
L8c6670a4: r[14]=rd(m,0x8c667140u,4); /* mov.l 0x8c667140,r14 */
 goto L8c6670a6;
L8c6670a6: goto L8c66710c; /* bra 0x8c66710c */
 goto L8c6670aa;
L8c6670aa: r[0]=sx(m,0x8c667136u,2); /* mov.w 0x8c667136,r0 */
 goto L8c6670ac;
L8c6670ac: r[3]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r3 */
 goto L8c6670ae;
L8c6670ae: r[0]+=4294967284u; /* add #-0xc,r0 */
 goto L8c6670b0;
L8c6670b0: r[2]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r2 */
 goto L8c6670b2;
L8c6670b2: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]); /* cmp/gt r3,r2 */
 goto L8c6670b4;
L8c6670b4: if((*sr&1u))goto L8c6670bc; /* bt 0x8c6670bc */
 goto L8c6670b6;
L8c6670b6: r[14]=rd(m,0x8c667144u,4); /* mov.l 0x8c667144,r14 */
 goto L8c6670b8;
L8c6670b8: goto L8c66710c; /* bra 0x8c66710c */
 goto L8c6670bc;
L8c6670bc: r[0]=sx(m,0x8c667138u,2); /* mov.w 0x8c667138,r0 */
 goto L8c6670be;
L8c6670be: r[2]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r2 */
 goto L8c6670c0;
L8c6670c0: r[0]+=4294967288u; /* add #-0x8,r0 */
 goto L8c6670c2;
L8c6670c2: r[3]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r3 */
 goto L8c6670c4;
L8c6670c4: r[0]+=8u; /* add #0x8,r0 */
 goto L8c6670c6;
L8c6670c6: r[2]<<=2; /* shll2 r2 */
 goto L8c6670c8;
L8c6670c8: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c6670ca;
L8c6670ca: r[2]+=r[3]; /* add r3,r2 */
 goto L8c6670cc;
L8c6670cc: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c6670ce;
L8c6670ce: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c6670d0;
L8c6670d0: r[2]=r[3]; /* mov r3,r2 */
 goto L8c6670d2;
L8c6670d2: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c6670d4;
L8c6670d4: r[0]+=4294967288u; /* add #-0x8,r0 */
 goto L8c6670d6;
L8c6670d6: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c6670d8;
L8c6670d8: r[1]<<=2; /* shll2 r1 */
 goto L8c6670da;
L8c6670da: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c6670dc;
L8c6670dc: r[1]+=r[3]; /* add r3,r1 */
 goto L8c6670de;
L8c6670de: r[0]+=8u; /* add #0x8,r0 */
 goto L8c6670e0;
L8c6670e0: wr(m,r[1]+0x4u,r[5],4); /* mov.l r5,@(0x4,r1) */
 goto L8c6670e2;
L8c6670e2: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c6670e4;
L8c6670e4: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c6670e6;
L8c6670e6: r[2]+=1u; /* add #0x1,r2 */
 goto L8c6670e8;
L8c6670e8: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c6670ea;
L8c6670ea: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c6670ec;
L8c6670ec: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c6670ee;
L8c6670ee: r[2]=r[3]; /* mov r3,r2 */
 goto L8c6670f0;
L8c6670f0: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c6670f2;
L8c6670f2: r[0]+=4u; /* add #0x4,r0 */
 goto L8c6670f4;
L8c6670f4: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c6670f6;
L8c6670f6: *sr=(*sr&~1u)|(r[1]==r[3]); /* cmp/eq r1,r3 */
 goto L8c6670f8;
L8c6670f8: if(!(*sr&1u))goto L8c667102; /* bf 0x8c667102 */
 goto L8c6670fa;
L8c6670fa: r[0]=sx(m,0x8c667138u,2); /* mov.w 0x8c667138,r0 */
 goto L8c6670fc;
L8c6670fc: r[2]=0u; /* mov #0x0,r2 */
 goto L8c6670fe;
L8c6670fe: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c667100;
L8c667100: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c667102;
L8c667102: r[0]=sx(m,0x8c667136u,2); /* mov.w 0x8c667136,r0 */
 goto L8c667104;
L8c667104: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c667106;
L8c667106: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c667108;
L8c667108: r[2]+=1u; /* add #0x1,r2 */
 goto L8c66710a;
L8c66710a: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c66710c;
L8c66710c: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66710e;
L8c66710e: if((*sr&1u))goto L8c667118; /* bt 0x8c667118 */
 goto L8c667110;
L8c667110: r[2]=rd(m,0x8c66714cu,4); /* mov.l 0x8c66714c,r2 */
 goto L8c667112;
L8c667112: r[4]=rd(m,0x8c667148u,4); /* mov.l 0x8c667148,r4 */
 goto L8c667114;
L8c667114: target=r[2];*pr=0x8c667118u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667118;
L8c667118: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c66711a;
L8c66711a: r[3]=*sr; /* stc SR,r3 */
 goto L8c66711c;
L8c66711c: r[2]=sx(m,0x8c667134u,2); /* mov.w 0x8c667134,r2 */
 goto L8c66711e;
L8c66711e: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c667120;
L8c667120: r[0]<<=2; /* shll2 r0 */
 goto L8c667122;
L8c667122: r[3]&=r[2]; /* and r2,r3 */
 goto L8c667124;
L8c667124: r[0]<<=2; /* shll2 r0 */
 goto L8c667126;
L8c667126: r[0]|=r[3]; /* or r3,r0 */
 goto L8c667128;
L8c667128: *sr=r[0]; /* ldc r0,SR */
 goto L8c66712a;
L8c66712a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66712c;
L8c66712c: r[15]+=4u; /* add #0x4,r15 */
 goto L8c66712e;
L8c66712e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667130;
L8c667130: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
