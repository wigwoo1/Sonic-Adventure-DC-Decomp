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
/* Retail stop-all voice entry 8c094762..8c0947ba.
 * Clears lifetime and sets stop flag for each of38 records before dispatch. */
SA_STAGE_EXPORT int sa_sound_voice_stop_all(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c094762;
L8c094762: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c094764;
L8c094764: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c094766;
L8c094766: r[13]=0u; /* mov #0x0,r13 */
 goto L8c094768;
L8c094768: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c09476a;
L8c09476a: r[12]=16u; /* mov #0x10,r12 */
 goto L8c09476c;
L8c09476c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c09476e;
L8c09476e: r[11]=52u; /* mov #0x34,r11 */
 goto L8c094770;
L8c094770: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c094772;
L8c094772: r[14]=r[13]; /* mov r13,r14 */
 goto L8c094774;
L8c094774: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c094776;
L8c094776: r[9]=38u; /* mov #0x26,r9 */
 goto L8c094778;
L8c094778: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09477a;
L8c09477a: r[10]=rd(m,0x8c09486cu,4); /* mov.l 0x8c09486c,r10 */
 goto L8c09477c;
L8c09477c: *macl=r[11]*r[14]; /* mul.l r11,r14 */
 goto L8c09477e;
L8c09477e: r[4]=*macl; /* sts MACL,r4 */
 goto L8c094780;
L8c094780: r[4]+=r[10]; /* add r10,r4 */
 goto L8c094782;
L8c094782: wr(m,r[4]+0x4u,r[13],4); /* mov.l r13,@(0x4,r4) */
 goto L8c094784;
L8c094784: r[3]=rd(m,r[4]+0xcu,4); /* mov.l @(0xc,r4),r3 */
 goto L8c094786;
L8c094786: r[3]|=r[12]; /* or r12,r3 */
 goto L8c094788;
L8c094788: wr(m,r[4]+0xcu,r[3],4); /* mov.l r3,@(0xc,r4) */
 goto L8c09478a;
L8c09478a: *pr=0x8c09478eu;r[4]=r[14];status=call(ctx,0x8c093b70u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093b70 */
 goto L8c09478e;
L8c09478e: r[14]+=1u; /* add #0x1,r14 */
 goto L8c094790;
L8c094790: *macl=r[11]*r[14]; /* mul.l r11,r14 */
 goto L8c094792;
L8c094792: r[4]=*macl; /* sts MACL,r4 */
 goto L8c094794;
L8c094794: r[4]+=r[10]; /* add r10,r4 */
 goto L8c094796;
L8c094796: wr(m,r[4]+0x4u,r[13],4); /* mov.l r13,@(0x4,r4) */
 goto L8c094798;
L8c094798: r[3]=rd(m,r[4]+0xcu,4); /* mov.l @(0xc,r4),r3 */
 goto L8c09479a;
L8c09479a: r[3]|=r[12]; /* or r12,r3 */
 goto L8c09479c;
L8c09479c: wr(m,r[4]+0xcu,r[3],4); /* mov.l r3,@(0xc,r4) */
 goto L8c09479e;
L8c09479e: *pr=0x8c0947a2u;r[4]=r[14];status=call(ctx,0x8c093b70u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093b70 */
 goto L8c0947a2;
L8c0947a2: r[14]+=1u; /* add #0x1,r14 */
 goto L8c0947a4;
L8c0947a4: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[9]); /* cmp/ge r9,r14 */
 goto L8c0947a6;
L8c0947a6: if(!(*sr&1u))goto L8c09477c; /* bf 0x8c09477c */
 goto L8c0947a8;
L8c0947a8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0947aa;
L8c0947aa: r[0]=0u; /* mov #0x0,r0 */
 goto L8c0947ac;
L8c0947ac: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c0947ae;
L8c0947ae: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c0947b0;
L8c0947b0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c0947b2;
L8c0947b2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c0947b4;
L8c0947b4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0947b6;
L8c0947b6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
