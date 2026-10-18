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
/* Static C recovery of retail Sound bank section transfer helper 8c643904..8c64395c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_bank_section_transfer(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643904;
L8c643904: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643906;
L8c643906: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643908;
L8c643908: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c64390a;
L8c64390a: r[0]=16u; /* mov #0x10,r0 */
 goto L8c64390c;
L8c64390c: r[3]=rd(m,0x8c64395cu,4); /* mov.l 0x8c64395c,r3 */
 goto L8c64390e;
L8c64390e: r[7]=r[15]; /* mov r15,r7 */
 goto L8c643910;
L8c643910: wr(m,r[15]+0x8u,r[4],4); /* mov.l r4,@(0x8,r15) */
 goto L8c643912;
L8c643912: wr(m,r[15]+0xcu,r[5],4); /* mov.l r5,@(0xc,r15) */
 goto L8c643914;
L8c643914: r[7]+=4u; /* add #0x4,r7 */
 goto L8c643916;
L8c643916: wr(m,r[15]+r[0],r[6],1); /* mov.b r6,@(r0,r15) */
 goto L8c643918;
L8c643918: r[0]=16u; /* mov #0x10,r0 */
 goto L8c64391a;
L8c64391a: r[5]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r5 */
 goto L8c64391c;
L8c64391c: r[6]=r[15]; /* mov r15,r6 */
 goto L8c64391e;
L8c64391e: target=r[3];*pr=0x8c643922u;r[4]=rd(m,r[15]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643922;
L8c643922: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643924;
L8c643924: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643926;
L8c643926: if(!(*sr&1u))goto L8c643946; /* bf 0x8c643946 */
 goto L8c643928;
L8c643928: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c64392a;
L8c64392a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c64392c;
L8c64392c: if((*sr&1u))goto L8c643934; /* bt 0x8c643934 */
 goto L8c64392e;
L8c64392e: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c643930;
L8c643930: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c643932;
L8c643932: if((*sr&1u))goto L8c64393a; /* bt 0x8c64393a */
 goto L8c643934;
L8c643934: r[14]=rd(m,0x8c643960u,4); /* mov.l 0x8c643960,r14 */
 goto L8c643936;
L8c643936: goto L8c643946; /* bra 0x8c643946 */
 goto L8c64393a;
L8c64393a: r[3]=rd(m,0x8c643964u,4); /* mov.l 0x8c643964,r3 */
 goto L8c64393c;
L8c64393c: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c64393e;
L8c64393e: target=r[3];*pr=0x8c643942u;r[4]=rd(m,r[15]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643942;
L8c643942: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643944;
L8c643944: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643946;
L8c643946: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643948;
L8c643948: if((*sr&1u))goto L8c643952; /* bt 0x8c643952 */
 goto L8c64394a;
L8c64394a: r[3]=rd(m,0x8c64396cu,4); /* mov.l 0x8c64396c,r3 */
 goto L8c64394c;
L8c64394c: r[4]=rd(m,0x8c643968u,4); /* mov.l 0x8c643968,r4 */
 goto L8c64394e;
L8c64394e: target=r[3];*pr=0x8c643952u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643952;
L8c643952: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643954;
L8c643954: r[15]+=20u; /* add #0x14,r15 */
 goto L8c643956;
L8c643956: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643958;
L8c643958: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
