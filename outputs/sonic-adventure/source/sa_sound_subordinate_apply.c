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
/* Retail control apply65df78..65e062; null path verified, non-null integration pending. */
SA_STAGE_EXPORT int sa_sound_subordinate_apply(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65df78;
L8c65df78: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65df7a;
L8c65df7a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65df7c;
L8c65df7c: r[15]+=4294967252u; /* add #-0x2c,r15 */
 goto L8c65df7e;
L8c65df7e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65df80;
L8c65df80: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65df82;
L8c65df82: if(!(*sr&1u))goto L8c65dfb4; /* bf 0x8c65dfb4 */
 goto L8c65df84;
L8c65df84: r[2]=rd(m,0x8c65dfa0u,4); /* mov.l 0x8c65dfa0,r2 */
 goto L8c65df86;
L8c65df86: r[4]=rd(m,0x8c65df9cu,4); /* mov.l 0x8c65df9c,r4 */
 goto L8c65df88;
L8c65df88: target=r[2];*pr=0x8c65df8cu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65df8c;
L8c65df8c: r[15]+=44u; /* add #0x2c,r15 */
 goto L8c65df8e;
L8c65df8e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65df90;
L8c65df90: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65dfb4: r[3]=rd(m,0x8c65e06cu,4); /* mov.l 0x8c65e06c,r3 */
 goto L8c65dfb6;
L8c65dfb6: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dfb8;
L8c65dfb8: target=r[3];*pr=0x8c65dfbcu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dfbc;
L8c65dfbc: r[3]=rd(m,0x8c65e070u,4); /* mov.l 0x8c65e070,r3 */
 goto L8c65dfbe;
L8c65dfbe: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dfc0;
L8c65dfc0: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65dfc2;
L8c65dfc2: target=r[3];*pr=0x8c65dfc6u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dfc6;
L8c65dfc6: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65dfc8;
L8c65dfc8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65dfca;
L8c65dfca: if((*sr&1u))goto L8c65e05a; /* bt 0x8c65e05a */
 goto L8c65dfcc;
L8c65dfcc: r[3]=rd(m,0x8c65e074u,4); /* mov.l 0x8c65e074,r3 */
 goto L8c65dfce;
L8c65dfce: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65dfd0;
L8c65dfd0: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dfd2;
L8c65dfd2: r[5]+=24u; /* add #0x18,r5 */
 goto L8c65dfd4;
L8c65dfd4: target=r[3];*pr=0x8c65dfd8u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dfd8;
L8c65dfd8: r[3]=rd(m,0x8c65e078u,4); /* mov.l 0x8c65e078,r3 */
 goto L8c65dfda;
L8c65dfda: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65dfdc;
L8c65dfdc: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dfde;
L8c65dfde: r[5]+=20u; /* add #0x14,r5 */
 goto L8c65dfe0;
L8c65dfe0: target=r[3];*pr=0x8c65dfe4u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dfe4;
L8c65dfe4: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65dfe6;
L8c65dfe6: r[3]=rd(m,0x8c65e07cu,4); /* mov.l 0x8c65e07c,r3 */
 goto L8c65dfe8;
L8c65dfe8: r[6]+=36u; /* add #0x24,r6 */
 goto L8c65dfea;
L8c65dfea: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dfec;
L8c65dfec: wr(m,r[15]+0x20u,r[6],4); /* mov.l r6,@(0x20,r15) */
 goto L8c65dfee;
L8c65dfee: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65dff0;
L8c65dff0: target=r[3];*pr=0x8c65dff4u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dff4;
L8c65dff4: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65dff6;
L8c65dff6: r[3]=rd(m,0x8c65e07cu,4); /* mov.l 0x8c65e07c,r3 */
 goto L8c65dff8;
L8c65dff8: r[6]+=40u; /* add #0x28,r6 */
 goto L8c65dffa;
L8c65dffa: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dffc;
L8c65dffc: wr(m,r[15]+0x1cu,r[6],4); /* mov.l r6,@(0x1c,r15) */
 goto L8c65dffe;
L8c65dffe: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65e000;
L8c65e000: target=r[3];*pr=0x8c65e004u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e004;
L8c65e004: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65e006;
L8c65e006: r[3]=rd(m,0x8c65e080u,4); /* mov.l 0x8c65e080,r3 */
 goto L8c65e008;
L8c65e008: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65e00a;
L8c65e00a: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65e00c;
L8c65e00c: r[6]+=16u; /* add #0x10,r6 */
 goto L8c65e00e;
L8c65e00e: r[5]+=12u; /* add #0xc,r5 */
 goto L8c65e010;
L8c65e010: target=r[3];*pr=0x8c65e014u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e014;
L8c65e014: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65e016;
L8c65e016: r[3]=rd(m,0x8c65e084u,4); /* mov.l 0x8c65e084,r3 */
 goto L8c65e018;
L8c65e018: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65e01a;
L8c65e01a: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65e01c;
L8c65e01c: r[6]+=8u; /* add #0x8,r6 */
 goto L8c65e01e;
L8c65e01e: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65e020;
L8c65e020: target=r[3];*pr=0x8c65e024u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e024;
L8c65e024: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c65e026;
L8c65e026: *pr=0x8c65e02au;r[4]=r[14];status=call(ctx,0x8c65dc1cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65dc1c */
 goto L8c65e02a;
L8c65e02a: r[5]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r5 */
 goto L8c65e02c;
L8c65e02c: *pr=0x8c65e030u;r[4]=r[14];status=call(ctx,0x8c65dc54u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65dc54 */
 goto L8c65e030;
L8c65e030: r[5]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r5 */
 goto L8c65e032;
L8c65e032: *pr=0x8c65e036u;r[4]=r[14];status=call(ctx,0x8c65dc64u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65dc64 */
 goto L8c65e036;
L8c65e036: r[6]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r6 */
 goto L8c65e038;
L8c65e038: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65e03a;
L8c65e03a: r[6]=rd(m,r[6],4); /* mov.l @r6,r6 */
 goto L8c65e03c;
L8c65e03c: *pr=0x8c65e040u;r[4]=r[14];status=call(ctx,0x8c65dccau,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65dcca */
 goto L8c65e040;
L8c65e040: r[6]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r6 */
 goto L8c65e042;
L8c65e042: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65e044;
L8c65e044: r[6]=rd(m,r[6],4); /* mov.l @r6,r6 */
 goto L8c65e046;
L8c65e046: *pr=0x8c65e04au;r[4]=r[14];status=call(ctx,0x8c65dccau,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65dcca */
 goto L8c65e04a;
L8c65e04a: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c65e04c;
L8c65e04c: r[6]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r6 */
 goto L8c65e04e;
L8c65e04e: *pr=0x8c65e052u;r[4]=r[14];status=call(ctx,0x8c65dd26u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65dd26 */
 goto L8c65e052;
L8c65e052: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65e054;
L8c65e054: r[6]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r6 */
 goto L8c65e056;
L8c65e056: *pr=0x8c65e05au;r[4]=r[14];status=call(ctx,0x8c65df2au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65df2a */
 goto L8c65e05a;
L8c65e05a: r[15]+=44u; /* add #0x2c,r15 */
 goto L8c65e05c;
L8c65e05c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e05e;
L8c65e05e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
