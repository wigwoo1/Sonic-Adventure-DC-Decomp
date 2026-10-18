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
/* Retail game-facing volume dispatch. Non-MIDI branch remains an external dependency. */
SA_STAGE_EXPORT int sa_sound_volume_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093be0;
L8c093be0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093be2;
L8c093be2: r[3]=32u; /* mov #0x20,r3 */
 goto L8c093be4;
L8c093be4: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093be6;
L8c093be6: r[13]=r[5]; /* mov r5,r13 */
 goto L8c093be8;
L8c093be8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093bea;
L8c093bea: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]); /* cmp/ge r3,r13 */
 goto L8c093bec;
L8c093bec: take=!(*sr&1u);r[14]=r[4];if(take)goto L8c093c08;goto L8c093bf0; /* bf/s 0x8c093c08 */
 goto L8c093bf0;
L8c093bf0: r[4]=r[13]; /* mov r13,r4 */
 goto L8c093bf2;
L8c093bf2: r[4]+=4294967266u; /* add #-0x1e,r4 */
 goto L8c093bf4;
L8c093bf4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093bf6;
L8c093bf6: r[0]=rd(m,0x8c093cc0u,4); /* mov.l 0x8c093cc0,r0 */
 goto L8c093bf8;
L8c093bf8: r[4]<<=2; /* shll2 r4 */
 goto L8c093bfa;
L8c093bfa: r[2]=rd(m,0x8c093cc4u,4); /* mov.l 0x8c093cc4,r2 */
 goto L8c093bfc;
L8c093bfc: r[5]=r[14]; /* mov r14,r5 */
 goto L8c093bfe;
L8c093bfe: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c093c00;
L8c093c00: r[6]=0u; /* mov #0x0,r6 */
 goto L8c093c02;
L8c093c02: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c093c04;
L8c093c04: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c093c08: *sr=(*sr&~1u)|((int32_t)r[14]>0); /* cmp/pl r14 */
 goto L8c093c0a;
L8c093c0a: if(!(*sr&1u))goto L8c093c16; /* bf 0x8c093c16 */
 goto L8c093c0c;
L8c093c0c: r[2]=rd(m,0x8c093cc8u,4); /* mov.l 0x8c093cc8,r2 */
 goto L8c093c0e;
L8c093c0e: r[0]=6u; /* mov #0x6,r0 */
 goto L8c093c10;
L8c093c10: target=r[2];*pr=0x8c093c14u;r[1]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093c14;
L8c093c14: r[14]=r[0]; /* mov r0,r14 */
 goto L8c093c16;
L8c093c16: r[14]+=106u; /* add #0x6a,r14 */
 goto L8c093c18;
L8c093c18: *sr=(*sr&~1u)|((int32_t)r[14]>=0); /* cmp/pz r14 */
 goto L8c093c1a;
L8c093c1a: take=(*sr&1u);r[4]=127u;if(take)goto L8c093c20;goto L8c093c1e; /* bt/s 0x8c093c20 */
 goto L8c093c1e;
L8c093c1e: r[14]=0u; /* mov #0x0,r14 */
 goto L8c093c20;
L8c093c20: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]); /* cmp/gt r4,r14 */
 goto L8c093c22;
L8c093c22: if(!(*sr&1u))goto L8c093c26; /* bf 0x8c093c26 */
 goto L8c093c24;
L8c093c24: r[14]=r[4]; /* mov r4,r14 */
 goto L8c093c26;
L8c093c26: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093c28;
L8c093c28: r[4]=r[13]; /* mov r13,r4 */
 goto L8c093c2a;
L8c093c2a: r[5]=r[14]; /* mov r14,r5 */
 goto L8c093c2c;
L8c093c2c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c093c2e;
L8c093c2e: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c093240u,m,f,sr,r,pr); /* bra 0x8c093240 */
}
