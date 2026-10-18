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
/* Recovered sound request producer/copy helper; external calls dispatch to recovered native routines. */
SA_STAGE_EXPORT int sa_sound_request_commit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a464;
L8c65a464: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a466;
L8c65a466: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a468;
L8c65a468: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65a46a;
L8c65a46a: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a46c;
L8c65a46c: r[3]=sx(m,0x8c65a4f8u,2); /* mov.w 0x8c65a4f8,r3 */
 goto L8c65a46e;
L8c65a46e: r[14]=0u; /* mov #0x0,r14 */
 goto L8c65a470;
L8c65a470: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a472;
L8c65a472: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a474;
L8c65a474: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a476;
L8c65a476: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65a478;
L8c65a478: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a47a;
L8c65a47a: r[0]&=r[3]; /* and r3,r0 */
 goto L8c65a47c;
L8c65a47c: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c65a47e;
L8c65a47e: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a480;
L8c65a480: r[2]=rd(m,0x8c65a500u,4); /* mov.l 0x8c65a500,r2 */
 goto L8c65a482;
L8c65a482: r[5]=rd(m,r[2],4); /* mov.l @r2,r5 */
 goto L8c65a484;
L8c65a484: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c65a486;
L8c65a486: if(!(*sr&1u))goto L8c65a48e; /* bf 0x8c65a48e */
 goto L8c65a488;
L8c65a488: r[14]=rd(m,0x8c65a504u,4); /* mov.l 0x8c65a504,r14 */
 goto L8c65a48a;
L8c65a48a: goto L8c65a4d0; /* bra 0x8c65a4d0 */
 goto L8c65a48e;
L8c65a48e: r[2]=sx(m,0x8c65a4fau,2); /* mov.w 0x8c65a4fa,r2 */
 goto L8c65a490;
L8c65a490: r[3]=sx(m,0x8c65a4fcu,2); /* mov.w 0x8c65a4fc,r3 */
 goto L8c65a492;
L8c65a492: r[5]+=r[2]; /* add r2,r5 */
 goto L8c65a494;
L8c65a494: r[6]=rd(m,r[5]+0x14u,4); /* mov.l @(0x14,r5),r6 */
 goto L8c65a496;
L8c65a496: *sr=(*sr&~1u)|(r[3]==r[6]); /* cmp/eq r3,r6 */
 goto L8c65a498;
L8c65a498: take=!(*sr&1u);r[7]=rd(m,r[5]+0x10u,4);if(take)goto L8c65a4a2;goto L8c65a49c; /* bf/s 0x8c65a4a2 */
 goto L8c65a49c;
L8c65a49c: r[14]=rd(m,0x8c65a508u,4); /* mov.l 0x8c65a508,r14 */
 goto L8c65a49e;
L8c65a49e: goto L8c65a4d0; /* bra 0x8c65a4d0 */
 goto L8c65a4a2;
L8c65a4a2: r[6]=rd(m,r[5]+0x18u,4); /* mov.l @(0x18,r5),r6 */
 goto L8c65a4a4;
L8c65a4a4: r[3]=28u; /* mov #0x1c,r3 */
 goto L8c65a4a6;
L8c65a4a6: *macl=r[3]*r[6]; /* mul.l r3,r6 */
 goto L8c65a4a8;
L8c65a4a8: r[6]+=1u; /* add #0x1,r6 */
 goto L8c65a4aa;
L8c65a4aa: wr(m,r[5]+0x18u,r[6],4); /* mov.l r6,@(0x18,r5) */
 goto L8c65a4ac;
L8c65a4ac: r[1]=rd(m,r[5]+0x18u,4); /* mov.l @(0x18,r5),r1 */
 goto L8c65a4ae;
L8c65a4ae: r[6]=*macl; /* sts MACL,r6 */
 goto L8c65a4b0;
L8c65a4b0: r[2]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r2 */
 goto L8c65a4b2;
L8c65a4b2: *sr=(*sr&~1u)|(r[7]==r[1]); /* cmp/eq r7,r1 */
 goto L8c65a4b4;
L8c65a4b4: r[6]+=r[2]; /* add r2,r6 */
 goto L8c65a4b6;
L8c65a4b6: take=!(*sr&1u);r[3]=1u;if(take)goto L8c65a4be;goto L8c65a4ba; /* bf/s 0x8c65a4be */
 goto L8c65a4ba;
L8c65a4ba: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65a4bc;
L8c65a4bc: wr(m,r[5]+0x18u,r[0],4); /* mov.l r0,@(0x18,r5) */
 goto L8c65a4be;
L8c65a4be: wr(m,r[6]+0x14u,r[3],4); /* mov.l r3,@(0x14,r6) */
 goto L8c65a4c0;
L8c65a4c0: r[2]=r[4]; /* mov r4,r2 */
 goto L8c65a4c2;
L8c65a4c2: r[3]=rd(m,0x8c65a50cu,4); /* mov.l 0x8c65a50c,r3 */
 goto L8c65a4c4;
L8c65a4c4: r[1]=r[6]; /* mov r6,r1 */
 goto L8c65a4c6;
L8c65a4c6: target=r[3];*pr=0x8c65a4cau;r[0]=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a4ca;
L8c65a4ca: r[2]=rd(m,r[5]+0x14u,4); /* mov.l @(0x14,r5),r2 */
 goto L8c65a4cc;
L8c65a4cc: r[2]+=1u; /* add #0x1,r2 */
 goto L8c65a4ce;
L8c65a4ce: wr(m,r[5]+0x14u,r[2],4); /* mov.l r2,@(0x14,r5) */
 goto L8c65a4d0;
L8c65a4d0: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65a4d2;
L8c65a4d2: r[3]=*sr; /* stc SR,r3 */
 goto L8c65a4d4;
L8c65a4d4: r[1]=sx(m,0x8c65a4f8u,2); /* mov.w 0x8c65a4f8,r1 */
 goto L8c65a4d6;
L8c65a4d6: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a4d8;
L8c65a4d8: r[0]<<=2; /* shll2 r0 */
 goto L8c65a4da;
L8c65a4da: r[3]&=r[1]; /* and r1,r3 */
 goto L8c65a4dc;
L8c65a4dc: r[0]<<=2; /* shll2 r0 */
 goto L8c65a4de;
L8c65a4de: r[0]|=r[3]; /* or r3,r0 */
 goto L8c65a4e0;
L8c65a4e0: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a4e2;
L8c65a4e2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a4e4;
L8c65a4e4: if((*sr&1u))goto L8c65a4ee; /* bt 0x8c65a4ee */
 goto L8c65a4e6;
L8c65a4e6: r[3]=rd(m,0x8c65a514u,4); /* mov.l 0x8c65a514,r3 */
 goto L8c65a4e8;
L8c65a4e8: r[4]=rd(m,0x8c65a510u,4); /* mov.l 0x8c65a510,r4 */
 goto L8c65a4ea;
L8c65a4ea: target=r[3];*pr=0x8c65a4eeu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a4ee;
L8c65a4ee: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65a4f0;
L8c65a4f0: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65a4f2;
L8c65a4f2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a4f4;
L8c65a4f4: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
