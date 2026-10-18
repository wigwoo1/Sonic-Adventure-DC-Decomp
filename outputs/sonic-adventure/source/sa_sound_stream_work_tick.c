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
/* Retail stream work dispatcher 8c65bb4e..8c65bb88. Subordinate calls use explicit SH4 ABI services. */
SA_STAGE_EXPORT int sa_sound_stream_work_tick(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65bb4e;
L8c65bb4e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65bb50;
L8c65bb50: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65bb52;
L8c65bb52: r[13]=r[4]; /* mov r4,r13 */
 goto L8c65bb54;
L8c65bb54: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65bb56;
L8c65bb56: r[3]=rd(m,0x8c65bbccu,4); /* mov.l 0x8c65bbcc,r3 */
 goto L8c65bb58;
L8c65bb58: r[14]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r14 */
 goto L8c65bb5a;
L8c65bb5a: target=r[3];*pr=0x8c65bb5eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65bb5e;
L8c65bb5e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65bb60;
L8c65bb60: if(!(*sr&1u))goto L8c65bb66; /* bf 0x8c65bb66 */
 goto L8c65bb62;
L8c65bb62: *pr=0x8c65bb66u;r[4]=r[13];status=call(ctx,0x8c65b930u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65b930 */
 goto L8c65bb66;
L8c65bb66: r[2]=rd(m,0x8c65bbd0u,4); /* mov.l 0x8c65bbd0,r2 */
 goto L8c65bb68;
L8c65bb68: target=r[2];*pr=0x8c65bb6cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65bb6c;
L8c65bb6c: r[3]=rd(m,0x8c65bbccu,4); /* mov.l 0x8c65bbcc,r3 */
 goto L8c65bb6e;
L8c65bb6e: target=r[3];*pr=0x8c65bb72u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65bb72;
L8c65bb72: *sr=(*sr&~1u)|(r[0]==0x00000003u); /* cmp/eq #0x3,r0 */
 goto L8c65bb74;
L8c65bb74: if(!(*sr&1u))goto L8c65bb80; /* bf 0x8c65bb80 */
 goto L8c65bb76;
L8c65bb76: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65bb78;
L8c65bb78: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65bb7a;
L8c65bb7a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65bb7c;
L8c65bb7c: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65ba22u,m,f,sr,r,pr); /* bra 0x8c65ba22 */
 goto L8c65bb80;
L8c65bb80: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65bb82;
L8c65bb82: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65bb84;
L8c65bb84: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
