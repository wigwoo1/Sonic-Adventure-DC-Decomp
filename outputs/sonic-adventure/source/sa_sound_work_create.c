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
/* Recovery of retail audio work constructor65b624..65b700.
 * Pool allocation, stream creation and callback configuration.
 * Matched against retail with subordinate stream services modeled.
 * Full native subordinate integration remains unverified. */
SA_STAGE_EXPORT int sa_sound_work_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b624;
L8c65b624: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65b626;
L8c65b626: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65b628;
L8c65b628: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65b62a;
L8c65b62a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65b62c;
L8c65b62c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65b62e;
L8c65b62e: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65b630;
L8c65b630: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65b632;
L8c65b632: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65b634;
L8c65b634: r[9]=r[6]; /* mov r6,r9 */
 goto L8c65b636;
L8c65b636: r[13]=0u; /* mov #0x0,r13 */
 goto L8c65b638;
L8c65b638: r[6]=rd(m,0x8c65b708u,4); /* mov.l 0x8c65b708,r6 */
 goto L8c65b63a;
L8c65b63a: r[12]=r[5]; /* mov r5,r12 */
 goto L8c65b63c;
L8c65b63c: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c65b63e;
L8c65b63e: r[5]=8u; /* mov #0x8,r5 */
 goto L8c65b640;
L8c65b640: r[10]=rd(m,r[9],4); /* mov.l @r9,r10 */
 goto L8c65b642;
L8c65b642: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65b644;
L8c65b644: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65b646;
L8c65b646: r[3]<<=2; /* shll2 r3 */
 goto L8c65b648;
L8c65b648: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c65b64a;
L8c65b64a: r[2]=r[4]; /* mov r4,r2 */
 goto L8c65b64c;
L8c65b64c: r[3]+=r[2]; /* add r2,r3 */
 goto L8c65b64e;
L8c65b64e: r[3]<<=2; /* shll2 r3 */
 goto L8c65b650;
L8c65b650: r[3]<<=2; /* shll2 r3 */
 goto L8c65b652;
L8c65b652: r[3]+=r[6]; /* add r6,r3 */
 goto L8c65b654;
L8c65b654: r[1]=sx(m,r[3],1); /* mov.b @r3,r1 */
 goto L8c65b656;
L8c65b656: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65b658;
L8c65b658: if((*sr&1u))goto L8c65b660; /* bt 0x8c65b660 */
 goto L8c65b65a;
L8c65b65a: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65b65c;
L8c65b65c: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65b65e;
L8c65b65e: if(!(*sr&1u))goto L8c65b644; /* bf 0x8c65b644 */
 goto L8c65b660;
L8c65b660: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65b662;
L8c65b662: *sr=(*sr&~1u)|(r[0]==0x00000008u); /* cmp/eq #0x8,r0 */
 goto L8c65b664;
L8c65b664: if((*sr&1u))goto L8c65b6b0; /* bt 0x8c65b6b0 */
 goto L8c65b666;
L8c65b666: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b668;
L8c65b668: r[14]<<=2; /* shll2 r14 */
 goto L8c65b66a;
L8c65b66a: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1; /* shll r14 */
 goto L8c65b66c;
L8c65b66c: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65b66e;
L8c65b66e: r[14]+=r[3]; /* add r3,r14 */
 goto L8c65b670;
L8c65b670: r[2]=rd(m,0x8c65b714u,4); /* mov.l 0x8c65b714,r2 */
 goto L8c65b672;
L8c65b672: r[14]<<=2; /* shll2 r14 */
 goto L8c65b674;
L8c65b674: r[14]<<=2; /* shll2 r14 */
 goto L8c65b676;
L8c65b676: r[14]+=r[6]; /* add r6,r14 */
 goto L8c65b678;
L8c65b678: target=r[2];*pr=0x8c65b67cu;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b67c;
L8c65b67c: r[3]=rd(m,0x8c65b718u,4); /* mov.l 0x8c65b718,r3 */
 goto L8c65b67e;
L8c65b67e: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c65b680;
L8c65b680: target=r[3];*pr=0x8c65b684u;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b684;
L8c65b684: r[2]=rd(m,0x8c65b71cu,4); /* mov.l 0x8c65b71c,r2 */
 goto L8c65b686;
L8c65b686: r[11]=r[0]; /* mov r0,r11 */
 goto L8c65b688;
L8c65b688: *sr=(*sr&~1u)|(r[11]&1u);r[11]>>=1; /* shlr r11 */
 goto L8c65b68a;
L8c65b68a: target=r[2];*pr=0x8c65b68eu;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b68e;
L8c65b68e: r[3]=rd(m,0x8c65b720u,4); /* mov.l 0x8c65b720,r3 */
 goto L8c65b690;
L8c65b690: *sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1; /* shlr r0 */
 goto L8c65b692;
L8c65b692: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65b694;
L8c65b694: r[7]=r[11]; /* mov r11,r7 */
 goto L8c65b696;
L8c65b696: r[7]+=r[0]; /* add r0,r7 */
 goto L8c65b698;
L8c65b698: r[6]=r[11]; /* mov r11,r6 */
 goto L8c65b69a;
L8c65b69a: target=r[3];*pr=0x8c65b69eu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b69e;
L8c65b69e: r[5]=rd(m,0x8c65b724u,4); /* mov.l 0x8c65b724,r5 */
 goto L8c65b6a0;
L8c65b6a0: r[6]=r[14]; /* mov r14,r6 */
 goto L8c65b6a2;
L8c65b6a2: r[2]=rd(m,0x8c65b728u,4); /* mov.l 0x8c65b728,r2 */
 goto L8c65b6a4;
L8c65b6a4: wr(m,r[14]+0x4u,r[0],4); /* mov.l r0,@(0x4,r14) */
 goto L8c65b6a6;
L8c65b6a6: target=r[2];*pr=0x8c65b6aau;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b6aa;
L8c65b6aa: r[3]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c65b6ac;
L8c65b6ac: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65b6ae;
L8c65b6ae: if(!(*sr&1u))goto L8c65b6b4; /* bf 0x8c65b6b4 */
 goto L8c65b6b0;
L8c65b6b0: r[0]=0u;goto L8c65b6ee; /* bra 0x8c65b6ee */
 goto L8c65b6b4;
L8c65b6b4: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65b6b6;
L8c65b6b6: r[0]=r[12]; /* mov r12,r0 */
 goto L8c65b6b8;
L8c65b6b8: r[5]=r[13]; /* mov r13,r5 */
 goto L8c65b6ba;
L8c65b6ba: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c65b6bc;
L8c65b6bc: wr(m,r[14]+0x8u,r[3],4); /* mov.l r3,@(0x8,r14) */
 goto L8c65b6be;
L8c65b6be: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c65b6c0;
L8c65b6c0: take=!(*sr&1u);r[4]=r[13];if(take)goto L8c65b6d8;goto L8c65b6c4; /* bf/s 0x8c65b6d8 */
 goto L8c65b6c4;
L8c65b6c4: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65b6c6;
L8c65b6c6: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65b6c8;
L8c65b6c8: r[2]=rd(m,r[9]+r[0],4); /* mov.l @(r0,r9),r2 */
 goto L8c65b6ca;
L8c65b6ca: r[3]+=12u; /* add #0xc,r3 */
 goto L8c65b6cc;
L8c65b6cc: r[5]+=1u; /* add #0x1,r5 */
 goto L8c65b6ce;
L8c65b6ce: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65b6d0;
L8c65b6d0: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[12]); /* cmp/ge r12,r5 */
 goto L8c65b6d2;
L8c65b6d2: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65b6d4;
L8c65b6d4: take=!(*sr&1u);r[4]+=4u;if(take)goto L8c65b6c4;goto L8c65b6d8; /* bf/s 0x8c65b6c4 */
 goto L8c65b6d8;
L8c65b6d8: r[0]=r[13]; /* mov r13,r0 */
 goto L8c65b6da;
L8c65b6da: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c65b6dc;
L8c65b6dc: *pr=0x8c65b6e0u;r[4]=r[14];status=call(ctx,0x8c65b606u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65b606 */
 goto L8c65b6e0;
L8c65b6e0: r[0]=72u; /* mov #0x48,r0 */
 goto L8c65b6e2;
L8c65b6e2: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c65b6e4;
L8c65b6e4: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c65b6e6;
L8c65b6e6: r[3]=1u; /* mov #0x1,r3 */
 goto L8c65b6e8;
L8c65b6e8: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c65b6ea;
L8c65b6ea: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65b6ec;
L8c65b6ec: wr(m,r[14],r[3],1); /* mov.b r3,@r14 */
 goto L8c65b6ee;
L8c65b6ee: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65b6f0;
L8c65b6f0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65b6f2;
L8c65b6f2: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65b6f4;
L8c65b6f4: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65b6f6;
L8c65b6f6: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65b6f8;
L8c65b6f8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65b6fa;
L8c65b6fa: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65b6fc;
L8c65b6fc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
