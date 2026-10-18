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
SA_STAGE_EXPORT int sa_sound_slot_acquire(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65aa30;
L8c65aa30: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65aa32;
L8c65aa32: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65aa34;
L8c65aa34: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65aa36;
L8c65aa36: r[13]=0u; /* mov #0x0,r13 */
 goto L8c65aa38;
L8c65aa38: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65aa3a;
L8c65aa3a: r[3]=rd(m,0x8c65aae0u,4); /* mov.l 0x8c65aae0,r3 */
 goto L8c65aa3c;
L8c65aa3c: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65aa3e;
L8c65aa3e: r[14]=r[13]; /* mov r13,r14 */
 goto L8c65aa40;
L8c65aa40: r[6]=8u; /* mov #0x8,r6 */
 goto L8c65aa42;
L8c65aa42: take=(*sr&1u);r[7]=rd(m,r[3],4);if(take)goto L8c65aa52;goto L8c65aa46; /* bt/s 0x8c65aa52 */
 goto L8c65aa46;
L8c65aa46: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c65aa48;
L8c65aa48: if((*sr&1u))goto L8c65aa70; /* bt 0x8c65aa70 */
 goto L8c65aa4a;
L8c65aa4a: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c65aa4c;
L8c65aa4c: if((*sr&1u))goto L8c65aa8e; /* bt 0x8c65aa8e */
 goto L8c65aa4e;
L8c65aa4e: goto L8c65aaae; /* bra 0x8c65aaae */
 goto L8c65aa52;
L8c65aa52: r[5]=r[7]; /* mov r7,r5 */
 goto L8c65aa54;
L8c65aa54: r[7]=r[13]; /* mov r13,r7 */
 goto L8c65aa56;
L8c65aa56: r[5]+=8u; /* add #0x8,r5 */
 goto L8c65aa58;
L8c65aa58: r[0]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r0 */
 goto L8c65aa5a;
L8c65aa5a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65aa5c;
L8c65aa5c: if(!(*sr&1u))goto L8c65aa66; /* bf 0x8c65aa66 */
 goto L8c65aa5e;
L8c65aa5e: r[7]+=1u; /* add #0x1,r7 */
 goto L8c65aa60;
L8c65aa60: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]); /* cmp/ge r6,r7 */
 goto L8c65aa62;
L8c65aa62: take=!(*sr&1u);r[5]+=8u;if(take)goto L8c65aa58;goto L8c65aa66; /* bf/s 0x8c65aa58 */
 goto L8c65aa66;
L8c65aa66: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65aa68;
L8c65aa68: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c65aa6a;
L8c65aa6a: if((*sr&1u))goto L8c65aaaa; /* bt 0x8c65aaaa */
 goto L8c65aa6c;
L8c65aa6c: goto L8c65aab4; /* bra 0x8c65aab4 */
 goto L8c65aa70;
L8c65aa70: r[5]=r[7]; /* mov r7,r5 */
 goto L8c65aa72;
L8c65aa72: r[7]=r[13]; /* mov r13,r7 */
 goto L8c65aa74;
L8c65aa74: r[5]+=72u; /* add #0x48,r5 */
 goto L8c65aa76;
L8c65aa76: r[0]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r0 */
 goto L8c65aa78;
L8c65aa78: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65aa7a;
L8c65aa7a: if(!(*sr&1u))goto L8c65aa84; /* bf 0x8c65aa84 */
 goto L8c65aa7c;
L8c65aa7c: r[7]+=1u; /* add #0x1,r7 */
 goto L8c65aa7e;
L8c65aa7e: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]); /* cmp/ge r6,r7 */
 goto L8c65aa80;
L8c65aa80: take=!(*sr&1u);r[5]+=8u;if(take)goto L8c65aa76;goto L8c65aa84; /* bf/s 0x8c65aa76 */
 goto L8c65aa84;
L8c65aa84: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65aa86;
L8c65aa86: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c65aa88;
L8c65aa88: if(!(*sr&1u))goto L8c65aab4; /* bf 0x8c65aab4 */
 goto L8c65aa8a;
L8c65aa8a: goto L8c65aaaa; /* bra 0x8c65aaaa */
 goto L8c65aa8e;
L8c65aa8e: r[5]=sx(m,0x8c65aadeu,2); /* mov.w 0x8c65aade,r5 */
 goto L8c65aa90;
L8c65aa90: r[6]=r[13]; /* mov r13,r6 */
 goto L8c65aa92;
L8c65aa92: r[5]+=r[7]; /* add r7,r5 */
 goto L8c65aa94;
L8c65aa94: r[7]=4u; /* mov #0x4,r7 */
 goto L8c65aa96;
L8c65aa96: r[0]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r0 */
 goto L8c65aa98;
L8c65aa98: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65aa9a;
L8c65aa9a: if(!(*sr&1u))goto L8c65aaa4; /* bf 0x8c65aaa4 */
 goto L8c65aa9c;
L8c65aa9c: r[6]+=1u; /* add #0x1,r6 */
 goto L8c65aa9e;
L8c65aa9e: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]); /* cmp/ge r7,r6 */
 goto L8c65aaa0;
L8c65aaa0: take=!(*sr&1u);r[5]+=8u;if(take)goto L8c65aa96;goto L8c65aaa4; /* bf/s 0x8c65aa96 */
 goto L8c65aaa4;
L8c65aaa4: r[0]=r[6]; /* mov r6,r0 */
 goto L8c65aaa6;
L8c65aaa6: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c65aaa8;
L8c65aaa8: if(!(*sr&1u))goto L8c65aab4; /* bf 0x8c65aab4 */
 goto L8c65aaaa;
L8c65aaaa: r[5]=r[13];goto L8c65aab4; /* bra 0x8c65aab4 */
 goto L8c65aaae;
L8c65aaae: r[14]=rd(m,0x8c65aae4u,4); /* mov.l 0x8c65aae4,r14 */
 goto L8c65aab0;
L8c65aab0: goto L8c65aac8; /* bra 0x8c65aac8 */
 goto L8c65aab4;
L8c65aab4: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c65aab6;
L8c65aab6: if((*sr&1u))goto L8c65aac4; /* bt 0x8c65aac4 */
 goto L8c65aab8;
L8c65aab8: r[3]=1u; /* mov #0x1,r3 */
 goto L8c65aaba;
L8c65aaba: r[2]=r[5]; /* mov r5,r2 */
 goto L8c65aabc;
L8c65aabc: wr(m,r[4],r[5],4); /* mov.l r5,@r4 */
 goto L8c65aabe;
L8c65aabe: wr(m,r[2]+0x4u,r[3],4); /* mov.l r3,@(0x4,r2) */
 goto L8c65aac0;
L8c65aac0: goto L8c65aac8; /* bra 0x8c65aac8 */
 goto L8c65aac4;
L8c65aac4: r[14]=rd(m,0x8c65aae8u,4); /* mov.l 0x8c65aae8,r14 */
 goto L8c65aac6;
L8c65aac6: wr(m,r[4],r[13],4); /* mov.l r13,@r4 */
 goto L8c65aac8;
L8c65aac8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65aaca;
L8c65aaca: if((*sr&1u))goto L8c65aad4; /* bt 0x8c65aad4 */
 goto L8c65aacc;
L8c65aacc: r[3]=rd(m,0x8c65aaf0u,4); /* mov.l 0x8c65aaf0,r3 */
 goto L8c65aace;
L8c65aace: r[4]=rd(m,0x8c65aaecu,4); /* mov.l 0x8c65aaec,r4 */
 goto L8c65aad0;
L8c65aad0: target=r[3];*pr=0x8c65aad4u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65aad4;
L8c65aad4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65aad6;
L8c65aad6: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65aad8;
L8c65aad8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65aada;
L8c65aada: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
