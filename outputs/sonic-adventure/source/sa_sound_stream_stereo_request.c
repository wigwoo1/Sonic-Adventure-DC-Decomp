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
/* Static C recovery of retail audio stream helper 8c66780e..8c667844.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_stream_stereo_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66780e;
L8c66780e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c667810;
L8c667810: r[6]=r[4]; /* mov r4,r6 */
 goto L8c667812;
L8c667812: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667814;
L8c667814: r[0]=80u; /* mov #0x50,r0 */
 goto L8c667816;
L8c667816: r[6]+=80u; /* add #0x50,r6 */
 goto L8c667818;
L8c667818: r[15]-=4;wr(m,r[15],r[5],4); /* mov.l r5,@-r15 */
 goto L8c66781a;
L8c66781a: r[3]=rd(m,r[6]+0x20u,4); /* mov.l @(0x20,r6),r3 */
 goto L8c66781c;
L8c66781c: r[5]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r5 */
 goto L8c66781e;
L8c66781e: r[14]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r14 */
 goto L8c667820;
L8c667820: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c667822;
L8c667822: r[4]=rd(m,r[6]+0x1cu,4); /* mov.l @(0x1c,r6),r4 */
 goto L8c667824;
L8c667824: r[7]=rd(m,r[6]+0x14u,4); /* mov.l @(0x14,r6),r7 */
 goto L8c667826;
L8c667826: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c667828;
L8c667828: r[7]+=r[3]; /* add r3,r7 */
 goto L8c66782a;
L8c66782a: r[3]=rd(m,0x8c667920u,4); /* mov.l 0x8c667920,r3 */
 goto L8c66782c;
L8c66782c: r[4]+=r[7]; /* add r7,r4 */
 goto L8c66782e;
L8c66782e: r[15]-=4;wr(m,r[15],r[4],4); /* mov.l r4,@-r15 */
 goto L8c667830;
L8c667830: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c667832;
L8c667832: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1; /* shll r6 */
 goto L8c667834;
L8c667834: target=r[3];*pr=0x8c667838u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667838;
L8c667838: r[4]=r[14]; /* mov r14,r4 */
 goto L8c66783a;
L8c66783a: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66783c;
L8c66783c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66783e;
L8c66783e: r[2]=rd(m,0x8c66791cu,4); /* mov.l 0x8c66791c,r2 */
 goto L8c667840;
L8c667840: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
}
