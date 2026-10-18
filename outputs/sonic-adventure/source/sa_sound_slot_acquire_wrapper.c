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
/* Retail sound slot setup/allocation. Typed slot exhaustion requires separate verification. */
SA_STAGE_EXPORT int sa_sound_slot_acquire_wrapper(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643ac0;
L8c643ac0: r[2]=rd(m,0x8c643af4u,4); /* mov.l 0x8c643af4,r2 */
 goto L8c643ac2;
L8c643ac2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643ac4;
L8c643ac4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643ac6;
L8c643ac6: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643ac8;
L8c643ac8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643aca;
L8c643aca: take=!(*sr&1u);r[14]=0u;if(take)goto L8c643ad4;goto L8c643ace; /* bf/s 0x8c643ad4 */
 goto L8c643ace;
L8c643ace: r[14]=rd(m,0x8c643af8u,4); /* mov.l 0x8c643af8,r14 */
 goto L8c643ad0;
L8c643ad0: goto L8c643ae0; /* bra 0x8c643ae0 */
 goto L8c643ad4;
L8c643ad4: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643ad6;
L8c643ad6: if((*sr&1u))goto L8c643ae0; /* bt 0x8c643ae0 */
 goto L8c643ad8;
L8c643ad8: r[3]=rd(m,0x8c643afcu,4); /* mov.l 0x8c643afc,r3 */
 goto L8c643ada;
L8c643ada: target=r[3];*pr=0x8c643adeu;r[5]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643ade;
L8c643ade: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643ae0;
L8c643ae0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643ae2;
L8c643ae2: if((*sr&1u))goto L8c643aec; /* bt 0x8c643aec */
 goto L8c643ae4;
L8c643ae4: r[3]=rd(m,0x8c643b04u,4); /* mov.l 0x8c643b04,r3 */
 goto L8c643ae6;
L8c643ae6: r[4]=rd(m,0x8c643b00u,4); /* mov.l 0x8c643b00,r4 */
 goto L8c643ae8;
L8c643ae8: target=r[3];*pr=0x8c643aecu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643aec;
L8c643aec: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643aee;
L8c643aee: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643af0;
L8c643af0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
