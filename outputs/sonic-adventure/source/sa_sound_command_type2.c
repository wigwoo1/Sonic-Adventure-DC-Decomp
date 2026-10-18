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
/* Retail type-2 sound command wrapper. Semantic command name remains unconfirmed. */
SA_STAGE_EXPORT int sa_sound_command_type2(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643e08;
L8c643e08: r[2]=rd(m,0x8c643e4cu,4); /* mov.l 0x8c643e4c,r2 */
 goto L8c643e0a;
L8c643e0a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643e0c;
L8c643e0c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643e0e;
L8c643e0e: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643e10;
L8c643e10: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643e12;
L8c643e12: if(!(*sr&1u))goto L8c643e1a; /* bf 0x8c643e1a */
 goto L8c643e14;
L8c643e14: r[14]=rd(m,0x8c643e50u,4); /* mov.l 0x8c643e50,r14 */
 goto L8c643e16;
L8c643e16: goto L8c643e38; /* bra 0x8c643e38 */
 goto L8c643e1a;
L8c643e1a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643e1c;
L8c643e1c: if(!(*sr&1u))goto L8c643e24; /* bf 0x8c643e24 */
 goto L8c643e1e;
L8c643e1e: r[14]=rd(m,0x8c643e54u,4); /* mov.l 0x8c643e54,r14 */
 goto L8c643e20;
L8c643e20: goto L8c643e38; /* bra 0x8c643e38 */
 goto L8c643e24;
L8c643e24: r[2]=rd(m,0x8c643e58u,4); /* mov.l 0x8c643e58,r2 */
 goto L8c643e26;
L8c643e26: r[3]=2u; /* mov #0x2,r3 */
 goto L8c643e28;
L8c643e28: r[1]=rd(m,0x8c643e5cu,4); /* mov.l 0x8c643e5c,r1 */
 goto L8c643e2a;
L8c643e2a: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c643e2c;
L8c643e2c: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c643e2e;
L8c643e2e: r[3]=rd(m,0x8c643e60u,4); /* mov.l 0x8c643e60,r3 */
 goto L8c643e30;
L8c643e30: wr(m,r[1],r[0],1); /* mov.b r0,@r1 */
 goto L8c643e32;
L8c643e32: target=r[3];*pr=0x8c643e36u;r[4]=r[2];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643e36;
L8c643e36: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643e38;
L8c643e38: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643e3a;
L8c643e3a: if((*sr&1u))goto L8c643e44; /* bt 0x8c643e44 */
 goto L8c643e3c;
L8c643e3c: r[3]=rd(m,0x8c643e68u,4); /* mov.l 0x8c643e68,r3 */
 goto L8c643e3e;
L8c643e3e: r[4]=rd(m,0x8c643e64u,4); /* mov.l 0x8c643e64,r4 */
 goto L8c643e40;
L8c643e40: target=r[3];*pr=0x8c643e44u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643e44;
L8c643e44: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643e46;
L8c643e46: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643e48;
L8c643e48: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
