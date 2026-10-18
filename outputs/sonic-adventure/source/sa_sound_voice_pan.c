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
/* Static C recovery of retail Tikal helper 8c0955c0..8c095612.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_sound_voice_pan(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0955c0;
L8c0955c0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0955c2;
L8c0955c2: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c0955c4;
L8c0955c4: r[5]=52u; /* mov #0x34,r5 */
 goto L8c0955c6;
L8c0955c6: r[3]=rd(m,0x8c0956e0u,4); /* mov.l 0x8c0956e0,r3 */
 goto L8c0955c8;
L8c0955c8: *macl=r[5]*r[4]; /* mul.l r5,r4 */
 goto L8c0955ca;
L8c0955ca: r[0]=36u; /* mov #0x24,r0 */
 goto L8c0955cc;
L8c0955cc: r[2]=rd(m,0x8c0956e4u,4); /* mov.l 0x8c0956e4,r2 */
 goto L8c0955ce;
L8c0955ce: r[5]=*macl; /* sts MACL,r5 */
 goto L8c0955d0;
L8c0955d0: r[5]+=r[3]; /* add r3,r5 */
 goto L8c0955d2;
L8c0955d2: fr[3]=rd(m,r[0]+r[5],4); /* fmov.s @(r0,r5),fr3 */
 goto L8c0955d4;
L8c0955d4: r[0]=40u; /* mov #0x28,r0 */
 goto L8c0955d6;
L8c0955d6: wr(m,r[15],fr[3],4); /* fmov.s fr3,@r15 */
 goto L8c0955d8;
L8c0955d8: fr[3]=rd(m,r[0]+r[5],4); /* fmov.s @(r0,r5),fr3 */
 goto L8c0955da;
L8c0955da: r[0]=4u; /* mov #0x4,r0 */
 goto L8c0955dc;
L8c0955dc: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(3u&1u);unsigned pair=3u&~1u;wr(m,r[0]+r[15],f->bank[bank][pair],4);wr(m,r[0]+r[15]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto L8c0955de;
L8c0955de: r[0]=44u; /* mov #0x2c,r0 */
 goto L8c0955e0;
L8c0955e0: fr[3]=rd(m,r[0]+r[5],4); /* fmov.s @(r0,r5),fr3 */
 goto L8c0955e2;
L8c0955e2: r[0]=8u; /* mov #0x8,r0 */
 goto L8c0955e4;
L8c0955e4: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(3u&1u);unsigned pair=3u&~1u;wr(m,r[0]+r[15],f->bank[bank][pair],4);wr(m,r[0]+r[15]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto L8c0955e6;
L8c0955e6: target=r[2];*pr=0x8c0955eau;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c0955ea;
L8c0955ea: r[3]=rd(m,0x8c0956e8u,4); /* mov.l 0x8c0956e8,r3 */
 goto L8c0955ec;
L8c0955ec: r[0]=4u; /* mov #0x4,r0 */
 goto L8c0955ee;
L8c0955ee: fr[3]=0; /* fldi0 fr3 */
 goto L8c0955f0;
L8c0955f0: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(3u&1u);unsigned pair=3u&~1u;wr(m,r[0]+r[15],f->bank[bank][pair],4);wr(m,r[0]+r[15]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto L8c0955f2;
L8c0955f2: target=r[3];*pr=0x8c0955f6u;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0955f6;
L8c0955f6: fr[2]=rd(m,r[15],4); /* fmov.s @r15,fr2 */
 goto L8c0955f8;
L8c0955f8: r[0]=0x8c0956ecu; /* mova 0x8c0956ec,r0 */
 goto L8c0955fa;
L8c0955fa: fr[3]=0x3f800000u; /* fldi1 fr3 */
 goto L8c0955fc;
L8c0955fc: status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr3,fr2 */
 goto L8c0955fe;
L8c0955fe: fr[1]=rd(m,r[0],4); /* fmov.s @r0,fr1 */
 goto L8c095600;
L8c095600: fr[4]=fr[2]; /* fmov fr2,fr4 */
 goto L8c095602;
L8c095602: status=sa_sh4_multiply(&fr[4],&fr[1],&f->fpscr,&fr[4]);if(status)return status; /* fmul fr1,fr4 */
 goto L8c095604;
L8c095604: status=sa_sh4_truncate_i32(fr[4],&f->fpscr,fpul);if(status)return status; /* ftrc fr4,FPUL */
 goto L8c095606;
L8c095606: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c095608;
L8c095608: r[4]<<=2; /* shll2 r4 */
 goto L8c09560a;
L8c09560a: r[15]+=12u; /* add #0xc,r15 */
 goto L8c09560c;
L8c09560c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c09560e;
L8c09560e: r[0]=r[4];return 0; /* rts */
}
