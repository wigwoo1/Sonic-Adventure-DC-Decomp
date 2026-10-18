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
/* Static C recovery of retail sound backend field reader 8c66bcb4..8c66bd24.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_field8_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66bcb4;
L8c66bcb4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66bcb6;
L8c66bcb6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66bcb8;
L8c66bcb8: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66bcba;
L8c66bcba: r[3]=rd(m,0x8c66bd28u,4); /* mov.l 0x8c66bd28,r3 */
 goto L8c66bcbc;
L8c66bcbc: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66bcbe;
L8c66bcbe: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66bcc0;
L8c66bcc0: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66bcc2;
L8c66bcc2: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66bccc;goto L8c66bcc6; /* bf/s 0x8c66bccc */
 goto L8c66bcc6;
L8c66bcc6: r[14]=rd(m,0x8c66bd2cu,4); /* mov.l 0x8c66bd2c,r14 */
 goto L8c66bcc8;
L8c66bcc8: goto L8c66bd0e; /* bra 0x8c66bd0e */
 goto L8c66bccc;
L8c66bccc: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bcce;
L8c66bcce: if(!(*sr&1u))goto L8c66bcd6; /* bf 0x8c66bcd6 */
 goto L8c66bcd0;
L8c66bcd0: r[14]=rd(m,0x8c66bd30u,4); /* mov.l 0x8c66bd30,r14 */
 goto L8c66bcd2;
L8c66bcd2: goto L8c66bd0e; /* bra 0x8c66bd0e */
 goto L8c66bcd6;
L8c66bcd6: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66bcd8;
L8c66bcd8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66bcda;
L8c66bcda: if(!(*sr&1u))goto L8c66bce2; /* bf 0x8c66bce2 */
 goto L8c66bcdc;
L8c66bcdc: r[14]=rd(m,0x8c66bd34u,4); /* mov.l 0x8c66bd34,r14 */
 goto L8c66bcde;
L8c66bcde: goto L8c66bd0e; /* bra 0x8c66bd0e */
 goto L8c66bce2;
L8c66bce2: r[0]=sx(m,0x8c66bd24u,2); /* mov.w 0x8c66bd24,r0 */
 goto L8c66bce4;
L8c66bce4: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66bce6;
L8c66bce6: r[3]=sx(m,0x8c66bd26u,2); /* mov.w 0x8c66bd26,r3 */
 goto L8c66bce8;
L8c66bce8: r[5]<<=2; /* shll2 r5 */
 goto L8c66bcea;
L8c66bcea: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66bcec;
L8c66bcec: r[5]<<=2; /* shll2 r5 */
 goto L8c66bcee;
L8c66bcee: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66bcf0;
L8c66bcf0: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66bcf2;
L8c66bcf2: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66bcf4;
L8c66bcf4: r[2]=rd(m,0x8c66bd38u,4); /* mov.l 0x8c66bd38,r2 */
 goto L8c66bcf6;
L8c66bcf6: r[0]<<=2; /* shll2 r0 */
 goto L8c66bcf8;
L8c66bcf8: r[0]<<=2; /* shll2 r0 */
 goto L8c66bcfa;
L8c66bcfa: r[0]<<=2; /* shll2 r0 */
 goto L8c66bcfc;
L8c66bcfc: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66bcfe;
L8c66bcfe: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66bd00;
L8c66bd00: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66bd02;
L8c66bd02: r[4]+=4u; /* add #0x4,r4 */
 goto L8c66bd04;
L8c66bd04: target=r[2];*pr=0x8c66bd08u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bd08;
L8c66bd08: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c66bd0a;
L8c66bd0a: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66bd0c;
L8c66bd0c: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c66bd0e;
L8c66bd0e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66bd10;
L8c66bd10: if((*sr&1u))goto L8c66bd1a; /* bt 0x8c66bd1a */
 goto L8c66bd12;
L8c66bd12: r[2]=rd(m,0x8c66bd40u,4); /* mov.l 0x8c66bd40,r2 */
 goto L8c66bd14;
L8c66bd14: r[4]=rd(m,0x8c66bd3cu,4); /* mov.l 0x8c66bd3c,r4 */
 goto L8c66bd16;
L8c66bd16: target=r[2];*pr=0x8c66bd1au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bd1a;
L8c66bd1a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66bd1c;
L8c66bd1c: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66bd1e;
L8c66bd1e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66bd20;
L8c66bd20: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
