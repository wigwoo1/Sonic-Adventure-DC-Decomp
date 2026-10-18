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
/* Retail magic stream update667a5c..667ad4. Verified within the scope of
 * analysis/sound_stream_magic_update_verification.json; downstream routines
 * and callbacks are modeled in that comparison. */
SA_STAGE_EXPORT int sa_sound_stream_magic_update(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667a5c;
L8c667a5c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c667a5e;
L8c667a5e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c667a60;
L8c667a60: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c667a62;
L8c667a62: r[13]=r[4]; /* mov r4,r13 */
 goto L8c667a64;
L8c667a64: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667a66;
L8c667a66: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c667a68;
L8c667a68: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c667a6a;
L8c667a6a: take=!(*sr&1u);r[13]+=80u;if(take)goto L8c667a96;goto L8c667a6e; /* bf/s 0x8c667a96 */
 goto L8c667a6e;
L8c667a6e: r[2]=rd(m,0x8c667b50u,4); /* mov.l 0x8c667b50,r2 */
 goto L8c667a70;
L8c667a70: target=r[2];*pr=0x8c667a74u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667a74;
L8c667a74: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c667a76;
L8c667a76: if(!(*sr&1u))goto L8c667a96; /* bf 0x8c667a96 */
 goto L8c667a78;
L8c667a78: r[0]=sx(m,0x8c667b44u,2); /* mov.w 0x8c667b44,r0 */
 goto L8c667a7a;
L8c667a7a: r[6]=r[13]; /* mov r13,r6 */
 goto L8c667a7c;
L8c667a7c: r[5]=r[13]; /* mov r13,r5 */
 goto L8c667a7e;
L8c667a7e: r[7]=r[13]; /* mov r13,r7 */
 goto L8c667a80;
L8c667a80: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c667a82;
L8c667a82: r[0]+=4u; /* add #0x4,r0 */
 goto L8c667a84;
L8c667a84: r[5]+=32u; /* add #0x20,r5 */
 goto L8c667a86;
L8c667a86: r[7]+=40u; /* add #0x28,r7 */
 goto L8c667a88;
L8c667a88: r[6]+=36u; /* add #0x24,r6 */
 goto L8c667a8a;
L8c667a8a: target=r[2];*pr=0x8c667a8eu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667a8e;
L8c667a8e: *pr=0x8c667a92u;r[4]=r[14];status=call(ctx,0x8c667844u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c667844 */
 goto L8c667a92;
L8c667a92: r[2]=2u; /* mov #0x2,r2 */
 goto L8c667a94;
L8c667a94: wr(m,r[14]+0x4u,r[2],4); /* mov.l r2,@(0x4,r14) */
 goto L8c667a96;
L8c667a96: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c667a98;
L8c667a98: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c667a9a;
L8c667a9a: if(!(*sr&1u))goto L8c667acc; /* bf 0x8c667acc */
 goto L8c667a9c;
L8c667a9c: r[2]=rd(m,0x8c667b54u,4); /* mov.l 0x8c667b54,r2 */
 goto L8c667a9e;
L8c667a9e: target=r[2];*pr=0x8c667aa2u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667aa2;
L8c667aa2: r[3]=rd(m,0x8c667b50u,4); /* mov.l 0x8c667b50,r3 */
 goto L8c667aa4;
L8c667aa4: target=r[3];*pr=0x8c667aa8u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667aa8;
L8c667aa8: *sr=(*sr&~1u)|(r[0]==0x00000003u); /* cmp/eq #0x3,r0 */
 goto L8c667aaa;
L8c667aaa: if(!(*sr&1u))goto L8c667acc; /* bf 0x8c667acc */
 goto L8c667aac;
L8c667aac: *pr=0x8c667ab0u;r[4]=r[14];status=call(ctx,0x8c667978u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c667978 */
 goto L8c667ab0;
L8c667ab0: r[2]=rd(m,0x8c667b58u,4); /* mov.l 0x8c667b58,r2 */
 goto L8c667ab2;
L8c667ab2: target=r[2];*pr=0x8c667ab6u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667ab6;
L8c667ab6: r[0]=sx(m,0x8c667b46u,2); /* mov.w 0x8c667b46,r0 */
 goto L8c667ab8;
L8c667ab8: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c667aba;
L8c667aba: r[0]+=16u; /* add #0x10,r0 */
 goto L8c667abc;
L8c667abc: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c667abe;
L8c667abe: r[0]+=4u; /* add #0x4,r0 */
 goto L8c667ac0;
L8c667ac0: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c667ac2;
L8c667ac2: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c667ac4;
L8c667ac4: target=r[3];*pr=0x8c667ac8u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667ac8;
L8c667ac8: r[3]=3u; /* mov #0x3,r3 */
 goto L8c667aca;
L8c667aca: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c667acc;
L8c667acc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667ace;
L8c667ace: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667ad0;
L8c667ad0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
