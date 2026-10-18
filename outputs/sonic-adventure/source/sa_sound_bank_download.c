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
/* Retail bank download orchestration: native allocation/configuration/download/release. Multi-unit dependency and behavioral verification pending. */
SA_STAGE_EXPORT int sa_sound_bank_download(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093ce0;
L8c093ce0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093ce2;
L8c093ce2: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c093ce4;
L8c093ce4: r[3]=0u; /* mov #0x0,r3 */
 goto L8c093ce6;
L8c093ce6: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c093ce8;
L8c093ce8: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c093cea;
L8c093cea: r[3]=rd(m,0x8c093e40u,4); /* mov.l 0x8c093e40,r3 */
 goto L8c093cec;
L8c093cec: target=r[3];*pr=0x8c093cf0u;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093cf0;
L8c093cf0: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093cf2;
L8c093cf2: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093cf4;
L8c093cf4: if(!(*sr&1u))goto L8c093d18; /* bf 0x8c093d18 */
 goto L8c093cf6;
L8c093cf6: r[2]=0u; /* mov #0x0,r2 */
 goto L8c093cf8;
L8c093cf8: r[3]=rd(m,0x8c093e44u,4); /* mov.l 0x8c093e44,r3 */
 goto L8c093cfa;
L8c093cfa: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c093cfc;
L8c093cfc: r[7]=4294967295u; /* mov #-0x1,r7 */
 goto L8c093cfe;
L8c093cfe: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c093d00;
L8c093d00: r[6]=r[2]; /* mov r2,r6 */
 goto L8c093d02;
L8c093d02: target=r[3];*pr=0x8c093d06u;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093d06;
L8c093d06: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093d08;
L8c093d08: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093d0a;
L8c093d0a: take=!(*sr&1u);r[15]+=4u;if(take)goto L8c093d18;goto L8c093d0e; /* bf/s 0x8c093d18 */
 goto L8c093d0e;
L8c093d0e: r[3]=rd(m,0x8c093e48u,4); /* mov.l 0x8c093e48,r3 */
 goto L8c093d10;
L8c093d10: target=r[3];*pr=0x8c093d14u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093d14;
L8c093d14: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093d16;
L8c093d16: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093d18;
L8c093d18: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c093d1a;
L8c093d1a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c093d1c;
L8c093d1c: if((*sr&1u))goto L8c093d24; /* bt 0x8c093d24 */
 goto L8c093d1e;
L8c093d1e: r[3]=rd(m,0x8c093e4cu,4); /* mov.l 0x8c093e4c,r3 */
 goto L8c093d20;
L8c093d20: target=r[3];*pr=0x8c093d24u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093d24;
L8c093d24: r[15]+=8u; /* add #0x8,r15 */
 goto L8c093d26;
L8c093d26: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093d28;
L8c093d28: return 0; /* rts */
}
