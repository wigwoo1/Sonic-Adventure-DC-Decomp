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
/* Static C recovery of retail Tikal helper 8c66bb4c..8c66bc02.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_pan_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66bb4c;
L8c66bb4c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66bb4e;
L8c66bb4e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66bb50;
L8c66bb50: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66bb52;
L8c66bb52: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66bb54;
L8c66bb54: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c66bb56;
L8c66bb56: r[3]=rd(m,0x8c66bc08u,4); /* mov.l 0x8c66bc08,r3 */
 goto L8c66bb58;
L8c66bb58: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66bb5a;
L8c66bb5a: wr(m,r[15]+0x8u,r[0],1); /* mov.b r0,@(0x8,r15) */
 goto L8c66bb5c;
L8c66bb5c: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66bb5e;
L8c66bb5e: r[13]=rd(m,r[3],4); /* mov.l @r3,r13 */
 goto L8c66bb60;
L8c66bb60: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c66bb62;
L8c66bb62: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66bb6c;goto L8c66bb66; /* bf/s 0x8c66bb6c */
 goto L8c66bb66;
L8c66bb66: r[14]=rd(m,0x8c66bc0cu,4); /* mov.l 0x8c66bc0c,r14 */
 goto L8c66bb68;
L8c66bb68: goto L8c66bbe8; /* bra 0x8c66bbe8 */
 goto L8c66bb6c;
L8c66bb6c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bb6e;
L8c66bb6e: if(!(*sr&1u))goto L8c66bb76; /* bf 0x8c66bb76 */
 goto L8c66bb70;
L8c66bb70: r[14]=rd(m,0x8c66bc10u,4); /* mov.l 0x8c66bc10,r14 */
 goto L8c66bb72;
L8c66bb72: goto L8c66bbe8; /* bra 0x8c66bbe8 */
 goto L8c66bb76;
L8c66bb76: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66bb78;
L8c66bb78: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66bb7a;
L8c66bb7a: if(!(*sr&1u))goto L8c66bb82; /* bf 0x8c66bb82 */
 goto L8c66bb7c;
L8c66bb7c: r[14]=rd(m,0x8c66bc14u,4); /* mov.l 0x8c66bc14,r14 */
 goto L8c66bb7e;
L8c66bb7e: goto L8c66bbe8; /* bra 0x8c66bbe8 */
 goto L8c66bb82;
L8c66bb82: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66bb84;
L8c66bb84: r[2]=44u; /* mov #0x2c,r2 */
 goto L8c66bb86;
L8c66bb86: r[1]=sx(m,0x8c66bc02u,2); /* mov.w 0x8c66bc02,r1 */
 goto L8c66bb88;
L8c66bb88: r[5]=r[15]; /* mov r15,r5 */
 goto L8c66bb8a;
L8c66bb8a: r[12]=r[0]; /* mov r0,r12 */
 goto L8c66bb8c;
L8c66bb8c: r[3]=(uint32_t)(int32_t)(int8_t)r[12]; /* exts.b r12,r3 */
 goto L8c66bb8e;
L8c66bb8e: *macl=(uint32_t)((int32_t)(int16_t)r[2]*(int32_t)(int16_t)r[3]); /* muls.w r2,r3 */
 goto L8c66bb90;
L8c66bb90: r[0]=sx(m,0x8c66bc04u,2); /* mov.w 0x8c66bc04,r0 */
 goto L8c66bb92;
L8c66bb92: r[1]+=r[13]; /* add r13,r1 */
 goto L8c66bb94;
L8c66bb94: r[12]=(uint32_t)(int32_t)(int8_t)r[12]; /* exts.b r12,r12 */
 goto L8c66bb96;
L8c66bb96: r[2]=rd(m,0x8c66bc18u,4); /* mov.l 0x8c66bc18,r2 */
 goto L8c66bb98;
L8c66bb98: r[12]<<=2; /* shll2 r12 */
 goto L8c66bb9a;
L8c66bb9a: r[3]=*macl; /* sts MACL,r3 */
 goto L8c66bb9c;
L8c66bb9c: r[12]<<=2; /* shll2 r12 */
 goto L8c66bb9e;
L8c66bb9e: r[12]<<=2; /* shll2 r12 */
 goto L8c66bba0;
L8c66bba0: r[3]+=r[1]; /* add r1,r3 */
 goto L8c66bba2;
L8c66bba2: wr(m,r[15]+0xcu,r[3],4); /* mov.l r3,@(0xc,r15) */
 goto L8c66bba4;
L8c66bba4: r[4]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r4 */
 goto L8c66bba6;
L8c66bba6: r[0]=sx(m,r[15]+0x8u,1); /* mov.b @(0x8,r15),r0 */
 goto L8c66bba8;
L8c66bba8: r[3]=sx(m,0x8c66bc06u,2); /* mov.w 0x8c66bc06,r3 */
 goto L8c66bbaa;
L8c66bbaa: r[0]<<=2; /* shll2 r0 */
 goto L8c66bbac;
L8c66bbac: r[0]<<=2; /* shll2 r0 */
 goto L8c66bbae;
L8c66bbae: r[4]+=r[3]; /* add r3,r4 */
 goto L8c66bbb0;
L8c66bbb0: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c66bbb2;
L8c66bbb2: r[4]+=r[12]; /* add r12,r4 */
 goto L8c66bbb4;
L8c66bbb4: r[4]+=r[0]; /* add r0,r4 */
 goto L8c66bbb6;
L8c66bbb6: target=r[2];*pr=0x8c66bbbau;r[4]+=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bbba;
L8c66bbba: r[0]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c66bbbc;
L8c66bbbc: r[4]=sx(m,r[15],1); /* mov.b @r15,r4 */
 goto L8c66bbbe;
L8c66bbbe: r[0]=sx(m,r[0]+0x5u,1); /* mov.b @(0x5,r0),r0 */
 goto L8c66bbc0;
L8c66bbc0: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66bbc2;
L8c66bbc2: if((*sr&1u))goto L8c66bbe4; /* bt 0x8c66bbe4 */
 goto L8c66bbc4;
L8c66bbc4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66bbc6;
L8c66bbc6: if(!(*sr&1u))goto L8c66bbe4; /* bf 0x8c66bbe4 */
 goto L8c66bbc8;
L8c66bbc8: r[0]=sx(m,r[15]+0x8u,1); /* mov.b @(0x8,r15),r0 */
 goto L8c66bbca;
L8c66bbca: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c66bbcc;
L8c66bbcc: if((*sr&1u))goto L8c66bbd6; /* bt 0x8c66bbd6 */
 goto L8c66bbce;
L8c66bbce: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66bbd0;
L8c66bbd0: if((*sr&1u))goto L8c66bbda; /* bt 0x8c66bbda */
 goto L8c66bbd2;
L8c66bbd2: goto L8c66bbe2; /* bra 0x8c66bbe2 */
 goto L8c66bbd6;
L8c66bbd6: r[4]+=64u;goto L8c66bbe2; /* bra 0x8c66bbe2 */
 goto L8c66bbda;
L8c66bbda: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bbdc;
L8c66bbdc: if(!(*sr&1u))goto L8c66bbe0; /* bf 0x8c66bbe0 */
 goto L8c66bbde;
L8c66bbde: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c66bbe0;
L8c66bbe0: r[4]+=4294967232u; /* add #-0x40,r4 */
 goto L8c66bbe2;
L8c66bbe2: r[4]+=r[4]; /* add r4,r4 */
 goto L8c66bbe4;
L8c66bbe4: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66bbe6;
L8c66bbe6: wr(m,r[3],r[4],1); /* mov.b r4,@r3 */
 goto L8c66bbe8;
L8c66bbe8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66bbea;
L8c66bbea: if((*sr&1u))goto L8c66bbf4; /* bt 0x8c66bbf4 */
 goto L8c66bbec;
L8c66bbec: r[2]=rd(m,0x8c66bc20u,4); /* mov.l 0x8c66bc20,r2 */
 goto L8c66bbee;
L8c66bbee: r[4]=rd(m,0x8c66bc1cu,4); /* mov.l 0x8c66bc1c,r4 */
 goto L8c66bbf0;
L8c66bbf0: target=r[2];*pr=0x8c66bbf4u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bbf4;
L8c66bbf4: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66bbf6;
L8c66bbf6: r[15]+=16u; /* add #0x10,r15 */
 goto L8c66bbf8;
L8c66bbf8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66bbfa;
L8c66bbfa: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66bbfc;
L8c66bbfc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66bbfe;
L8c66bbfe: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
