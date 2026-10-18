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
/* Static C recovery of retail Tikal helper 8c65bbd4..8c65bc02.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_work_pool_update(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65bbd4;
L8c65bbd4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65bbd6;
L8c65bbd6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65bbd8;
L8c65bbd8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65bbda;
L8c65bbda: r[12]=sx(m,0x8c65bcecu,2); /* mov.w 0x8c65bcec,r12 */
 goto L8c65bbdc;
L8c65bbdc: r[4]=rd(m,0x8c65bcf0u,4); /* mov.l 0x8c65bcf0,r4 */
 goto L8c65bbde;
L8c65bbde: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65bbe0;
L8c65bbe0: r[13]=sx(m,0x8c65bceau,2); /* mov.w 0x8c65bcea,r13 */
 goto L8c65bbe2;
L8c65bbe2: r[12]+=r[4]; /* add r4,r12 */
 goto L8c65bbe4;
L8c65bbe4: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65bbe6;
L8c65bbe6: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65bbe8;
L8c65bbe8: r[0]=sx(m,r[4],1); /* mov.b @r4,r0 */
 goto L8c65bbea;
L8c65bbea: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65bbec;
L8c65bbec: if(!(*sr&1u))goto L8c65bbf2; /* bf 0x8c65bbf2 */
 goto L8c65bbee;
L8c65bbee: *pr=0x8c65bbf2u;status=call(ctx,0x8c65bb88u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65bb88 */
 goto L8c65bbf2;
L8c65bbf2: r[14]+=r[13]; /* add r13,r14 */
 goto L8c65bbf4;
L8c65bbf4: *sr=(*sr&~1u)|(r[14]>=r[12]); /* cmp/hs r12,r14 */
 goto L8c65bbf6;
L8c65bbf6: if(!(*sr&1u))goto L8c65bbe6; /* bf 0x8c65bbe6 */
 goto L8c65bbf8;
L8c65bbf8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65bbfa;
L8c65bbfa: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65bbfc;
L8c65bbfc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65bbfe;
L8c65bbfe: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
