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
/* Retail subordinate effect setter65dd26..65ddb2;span includes skipped literals. */
SA_STAGE_EXPORT int sa_sound_subordinate_effect(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65dd26;
L8c65dd26: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65dd28;
L8c65dd28: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65dd2a;
L8c65dd2a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c65dd2c;
L8c65dd2c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65dd2e;
L8c65dd2e: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c65dd30;
L8c65dd30: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65dd32;
L8c65dd32: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65dd34;
L8c65dd34: r[11]=rd(m,0x8c65dd70u,4); /* mov.l 0x8c65dd70,r11 */
 goto L8c65dd36;
L8c65dd36: take=!(*sr&1u);r[12]=r[5];if(take)goto L8c65dd40;goto L8c65dd3a; /* bf/s 0x8c65dd40 */
 goto L8c65dd3a;
L8c65dd3a: r[4]=rd(m,0x8c65dd74u,4); /* mov.l 0x8c65dd74,r4 */
 goto L8c65dd3c;
L8c65dd3c: goto L8c65dd4c; /* bra 0x8c65dd4c */
 goto L8c65dd40;
L8c65dd40: *sr=(*sr&~1u)|((int32_t)r[12]>=0); /* cmp/pz r12 */
 goto L8c65dd42;
L8c65dd42: if(!(*sr&1u))goto L8c65dd4a; /* bf 0x8c65dd4a */
 goto L8c65dd44;
L8c65dd44: r[3]=16u; /* mov #0x10,r3 */
 goto L8c65dd46;
L8c65dd46: *sr=(*sr&~1u)|((int32_t)r[12]>(int32_t)r[3]); /* cmp/gt r3,r12 */
 goto L8c65dd48;
L8c65dd48: if(!(*sr&1u))goto L8c65dd54; /* bf 0x8c65dd54 */
 goto L8c65dd4a;
L8c65dd4a: r[4]=rd(m,0x8c65dd80u,4); /* mov.l 0x8c65dd80,r4 */
 goto L8c65dd4c;
L8c65dd4c: target=r[11];*pr=0x8c65dd50u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65dd50;
L8c65dd50: goto L8c65dda6; /* bra 0x8c65dda6 */
 goto L8c65dd54;
L8c65dd54: r[4]=4294967251u; /* mov #-0x2d,r4 */
 goto L8c65dd56;
L8c65dd56: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c65dd58;
L8c65dd58: if((*sr&1u))goto L8c65dd5e; /* bt 0x8c65dd5e */
 goto L8c65dd5a;
L8c65dd5a: r[14]=r[4];goto L8c65dd86; /* bra 0x8c65dd86 */
 goto L8c65dd5e;
L8c65dd5e: *sr=(*sr&~1u)|((int32_t)r[6]>0); /* cmp/pl r6 */
 goto L8c65dd60;
L8c65dd60: if(!(*sr&1u))goto L8c65dd84; /* bf 0x8c65dd84 */
 goto L8c65dd62;
L8c65dd62: r[14]=0u;goto L8c65dd86; /* bra 0x8c65dd86 */
 goto L8c65dd84;
L8c65dd84: r[14]=r[6]; /* mov r6,r14 */
 goto L8c65dd86;
L8c65dd86: r[3]=rd(m,0x8c65dea4u,4); /* mov.l 0x8c65dea4,r3 */
 goto L8c65dd88;
L8c65dd88: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65dd8a;
L8c65dd8a: r[6]=r[14]; /* mov r14,r6 */
 goto L8c65dd8c;
L8c65dd8c: r[5]=r[12]; /* mov r12,r5 */
 goto L8c65dd8e;
L8c65dd8e: target=r[3];*pr=0x8c65dd92u;r[4]=rd(m,r[13]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65dd92;
L8c65dd92: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65dd94;
L8c65dd94: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65dd96;
L8c65dd96: if((*sr&1u))goto L8c65dd9e; /* bt 0x8c65dd9e */
 goto L8c65dd98;
L8c65dd98: r[4]=rd(m,0x8c65dea8u,4); /* mov.l 0x8c65dea8,r4 */
 goto L8c65dd9a;
L8c65dd9a: target=r[11];*pr=0x8c65dd9eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65dd9e;
L8c65dd9e: r[0]=120u; /* mov #0x78,r0 */
 goto L8c65dda0;
L8c65dda0: wr(m,r[13]+r[0],r[12],4); /* mov.l r12,@(r0,r13) */
 goto L8c65dda2;
L8c65dda2: r[0]=124u; /* mov #0x7c,r0 */
 goto L8c65dda4;
L8c65dda4: wr(m,r[13]+r[0],r[14],4); /* mov.l r14,@(r0,r13) */
 goto L8c65dda6;
L8c65dda6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65dda8;
L8c65dda8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65ddaa;
L8c65ddaa: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65ddac;
L8c65ddac: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65ddae;
L8c65ddae: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
