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
/* Retail sound subordinate pan setter: signed clamp -15..15,channel cache,and driver call. */
SA_STAGE_EXPORT int sa_sound_subordinate_pan(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65dcca;
L8c65dcca: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65dccc;
L8c65dccc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65dcce;
L8c65dcce: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65dcd0;
L8c65dcd0: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65dcd2;
L8c65dcd2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65dcd4;
L8c65dcd4: take=!(*sr&1u);wr(m,r[15],r[5],4);if(take)goto L8c65dce4;goto L8c65dcd8; /* bf/s 0x8c65dce4 */
 goto L8c65dcd8;
L8c65dcd8: r[4]=rd(m,0x8c65dd74u,4); /* mov.l 0x8c65dd74,r4 */
 goto L8c65dcda;
L8c65dcda: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65dcdc;
L8c65dcdc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65dcde;
L8c65dcde: r[2]=rd(m,0x8c65dd70u,4); /* mov.l 0x8c65dd70,r2 */
 goto L8c65dce0;
L8c65dce0: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c65dce4: r[4]=4294967281u; /* mov #-0xf,r4 */
 goto L8c65dce6;
L8c65dce6: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c65dce8;
L8c65dce8: if((*sr&1u))goto L8c65dcee; /* bt 0x8c65dcee */
 goto L8c65dcea;
L8c65dcea: r[6]=r[4];goto L8c65dcf6; /* bra 0x8c65dcf6 */
 goto L8c65dcee;
L8c65dcee: r[4]=15u; /* mov #0xf,r4 */
 goto L8c65dcf0;
L8c65dcf0: *sr=(*sr&~1u)|((int32_t)r[6]>(int32_t)r[4]); /* cmp/gt r4,r6 */
 goto L8c65dcf2;
L8c65dcf2: if(!(*sr&1u))goto L8c65dcf6; /* bf 0x8c65dcf6 */
 goto L8c65dcf4;
L8c65dcf4: r[6]=r[4]; /* mov r4,r6 */
 goto L8c65dcf6;
L8c65dcf6: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65dcf8;
L8c65dcf8: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65dcfa;
L8c65dcfa: r[3]+=112u; /* add #0x70,r3 */
 goto L8c65dcfc;
L8c65dcfc: r[2]<<=2; /* shll2 r2 */
 goto L8c65dcfe;
L8c65dcfe: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65dd00;
L8c65dd00: r[3]=rd(m,0x8c65dd78u,4); /* mov.l 0x8c65dd78,r3 */
 goto L8c65dd02;
L8c65dd02: wr(m,r[2],r[6],4); /* mov.l r6,@r2 */
 goto L8c65dd04;
L8c65dd04: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dd06;
L8c65dd06: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c65dd08;
L8c65dd08: target=r[3];*pr=0x8c65dd0cu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dd0c;
L8c65dd0c: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65dd0e;
L8c65dd0e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65dd10;
L8c65dd10: if((*sr&1u))goto L8c65dd1e; /* bt 0x8c65dd1e */
 goto L8c65dd12;
L8c65dd12: r[4]=rd(m,0x8c65dd7cu,4); /* mov.l 0x8c65dd7c,r4 */
 goto L8c65dd14;
L8c65dd14: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65dd16;
L8c65dd16: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65dd18;
L8c65dd18: r[3]=rd(m,0x8c65dd70u,4); /* mov.l 0x8c65dd70,r3 */
 goto L8c65dd1a;
L8c65dd1a: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c65dd1e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65dd20;
L8c65dd20: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65dd22;
L8c65dd22: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
