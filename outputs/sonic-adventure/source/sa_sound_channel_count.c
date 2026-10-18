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
/* Static C recovery of retail Tikal helper 8c66b5ac..8c66b64c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_channel_count(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b5ac;
L8c66b5ac: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b5ae;
L8c66b5ae: r[7]=0u; /* mov #0x0,r7 */
 goto L8c66b5b0;
L8c66b5b0: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66b5b2;
L8c66b5b2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66b5b4;
L8c66b5b4: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66b5b6;
L8c66b5b6: r[3]=rd(m,0x8c66b650u,4); /* mov.l 0x8c66b650,r3 */
 goto L8c66b5b8;
L8c66b5b8: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66b5ba;
L8c66b5ba: r[10]=r[4]; /* mov r4,r10 */
 goto L8c66b5bc;
L8c66b5bc: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c66b5be;
L8c66b5be: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b5c0;
L8c66b5c0: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c66b5c2;
L8c66b5c2: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b5c4;
L8c66b5c4: take=!(*sr&1u);r[13]=r[7];if(take)goto L8c66b5ce;goto L8c66b5c8; /* bf/s 0x8c66b5ce */
 goto L8c66b5c8;
L8c66b5c8: r[13]=rd(m,0x8c66b654u,4); /* mov.l 0x8c66b654,r13 */
 goto L8c66b5ca;
L8c66b5ca: goto L8c66b62e; /* bra 0x8c66b62e */
 goto L8c66b5ce;
L8c66b5ce: *sr=(*sr&~1u)|!(r[10]&r[10]); /* tst r10,r10 */
 goto L8c66b5d0;
L8c66b5d0: if(!(*sr&1u))goto L8c66b5d8; /* bf 0x8c66b5d8 */
 goto L8c66b5d2;
L8c66b5d2: r[13]=rd(m,0x8c66b658u,4); /* mov.l 0x8c66b658,r13 */
 goto L8c66b5d4;
L8c66b5d4: goto L8c66b62e; /* bra 0x8c66b62e */
 goto L8c66b5d8;
L8c66b5d8: r[2]=1u; /* mov #0x1,r2 */
 goto L8c66b5da;
L8c66b5da: r[12]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r12 */
 goto L8c66b5dc;
L8c66b5dc: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]); /* cmp/ge r2,r12 */
 goto L8c66b5de;
L8c66b5de: if(!(*sr&1u))goto L8c66b602; /* bf 0x8c66b602 */
 goto L8c66b5e0;
L8c66b5e0: r[1]=2u; /* mov #0x2,r1 */
 goto L8c66b5e2;
L8c66b5e2: *sr=(*sr&~1u)|((int32_t)r[12]>(int32_t)r[1]); /* cmp/gt r1,r12 */
 goto L8c66b5e4;
L8c66b5e4: if((*sr&1u))goto L8c66b602; /* bt 0x8c66b602 */
 goto L8c66b5e6;
L8c66b5e6: r[0]=sx(m,r[10]+0x1u,1); /* mov.b @(0x1,r10),r0 */
 goto L8c66b5e8;
L8c66b5e8: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c66b5ea;
L8c66b5ea: r[2]=sx(m,0x8c66b64cu,2); /* mov.w 0x8c66b64c,r2 */
 goto L8c66b5ec;
L8c66b5ec: *sr=(*sr&~1u)|((int32_t)r[12]>=0); /* cmp/pz r12 */
 goto L8c66b5ee;
L8c66b5ee: r[6]=r[0]; /* mov r0,r6 */
 goto L8c66b5f0;
L8c66b5f0: r[11]=(uint32_t)(int32_t)(int8_t)r[6]; /* exts.b r6,r11 */
 goto L8c66b5f2;
L8c66b5f2: *macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[11]); /* muls.w r3,r11 */
 goto L8c66b5f4;
L8c66b5f4: r[2]+=r[4]; /* add r4,r2 */
 goto L8c66b5f6;
L8c66b5f6: r[11]=*macl; /* sts MACL,r11 */
 goto L8c66b5f8;
L8c66b5f8: take=!(*sr&1u);r[11]+=r[2];if(take)goto L8c66b602;goto L8c66b5fc; /* bf/s 0x8c66b602 */
 goto L8c66b5fc;
L8c66b5fc: r[0]=sx(m,r[11]+0x4u,1); /* mov.b @(0x4,r11),r0 */
 goto L8c66b5fe;
L8c66b5fe: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[12]); /* cmp/ge r12,r0 */
 goto L8c66b600;
L8c66b600: if((*sr&1u))goto L8c66b608; /* bt 0x8c66b608 */
 goto L8c66b602;
L8c66b602: r[13]=rd(m,0x8c66b65cu,4); /* mov.l 0x8c66b65c,r13 */
 goto L8c66b604;
L8c66b604: goto L8c66b62e; /* bra 0x8c66b62e */
 goto L8c66b608;
L8c66b608: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66b60a;
L8c66b60a: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c66b60c;
L8c66b60c: wr(m,r[11]+0x5u,r[0],1); /* mov.b r0,@(0x5,r11) */
 goto L8c66b60e;
L8c66b60e: take=!(*sr&1u);r[14]=r[7];if(take)goto L8c66b62e;goto L8c66b612; /* bf/s 0x8c66b62e */
 goto L8c66b612;
L8c66b612: r[9]=rd(m,0x8c66b660u,4); /* mov.l 0x8c66b660,r9 */
 goto L8c66b614;
L8c66b614: r[3]=r[11]; /* mov r11,r3 */
 goto L8c66b616;
L8c66b616: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66b618;
L8c66b618: r[3]+=28u; /* add #0x1c,r3 */
 goto L8c66b61a;
L8c66b61a: r[5]=r[14]; /* mov r14,r5 */
 goto L8c66b61c;
L8c66b61c: r[6]<<=2; /* shll2 r6 */
 goto L8c66b61e;
L8c66b61e: r[6]+=r[3]; /* add r3,r6 */
 goto L8c66b620;
L8c66b620: r[6]=rd(m,r[6],4); /* mov.l @r6,r6 */
 goto L8c66b622;
L8c66b622: r[7]=0u; /* mov #0x0,r7 */
 goto L8c66b624;
L8c66b624: target=r[9];*pr=0x8c66b628u;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c66b628;
L8c66b628: r[14]+=1u; /* add #0x1,r14 */
 goto L8c66b62a;
L8c66b62a: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]); /* cmp/ge r12,r14 */
 goto L8c66b62c;
L8c66b62c: if(!(*sr&1u))goto L8c66b614; /* bf 0x8c66b614 */
 goto L8c66b62e;
L8c66b62e: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c66b630;
L8c66b630: if((*sr&1u))goto L8c66b63a; /* bt 0x8c66b63a */
 goto L8c66b632;
L8c66b632: r[2]=rd(m,0x8c66b668u,4); /* mov.l 0x8c66b668,r2 */
 goto L8c66b634;
L8c66b634: r[4]=rd(m,0x8c66b664u,4); /* mov.l 0x8c66b664,r4 */
 goto L8c66b636;
L8c66b636: target=r[2];*pr=0x8c66b63au;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66b63a;
L8c66b63a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b63c;
L8c66b63c: r[0]=r[13]; /* mov r13,r0 */
 goto L8c66b63e;
L8c66b63e: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66b640;
L8c66b640: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66b642;
L8c66b642: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66b644;
L8c66b644: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66b646;
L8c66b646: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66b648;
L8c66b648: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
