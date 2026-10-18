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
/* Static C recovery of retail Tikal helper 8c65aaf4..8c65ac64.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_info(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65aaf4;
L8c65aaf4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65aaf6;
L8c65aaf6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65aaf8;
L8c65aaf8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65aafa;
L8c65aafa: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65aafc;
L8c65aafc: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65aafe;
L8c65aafe: r[10]=0u; /* mov #0x0,r10 */
 goto L8c65ab00;
L8c65ab00: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65ab02;
L8c65ab02: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ab04;
L8c65ab04: r[3]=rd(m,0x8c65ab70u,4); /* mov.l 0x8c65ab70,r3 */
 goto L8c65ab06;
L8c65ab06: r[2]=rd(m,0x8c65ab74u,4); /* mov.l 0x8c65ab74,r2 */
 goto L8c65ab08;
L8c65ab08: r[12]=rd(m,0x8c65ab6cu,4); /* mov.l 0x8c65ab6c,r12 */
 goto L8c65ab0a;
L8c65ab0a: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c65ab0c;
L8c65ab0c: target=r[12];*pr=0x8c65ab10u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ab10;
L8c65ab10: r[14]=rd(m,0x8c65ab78u,4); /* mov.l 0x8c65ab78,r14 */
 goto L8c65ab12;
L8c65ab12: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65ab14;
L8c65ab14: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65ab16;
L8c65ab16: take=!(*sr&1u);r[15]+=8u;if(take)goto L8c65ab20;goto L8c65ab1a; /* bf/s 0x8c65ab20 */
 goto L8c65ab1a;
L8c65ab1a: r[10]=rd(m,0x8c65ab7cu,4); /* mov.l 0x8c65ab7c,r10 */
 goto L8c65ab1c;
L8c65ab1c: goto L8c65ac46; /* bra 0x8c65ac46 */
 goto L8c65ab20;
L8c65ab20: r[0]=sx(m,0x8c65ab62u,2); /* mov.w 0x8c65ab62,r0 */
 goto L8c65ab22;
L8c65ab22: r[3]=sx(m,0x8c65ab64u,2); /* mov.w 0x8c65ab64,r3 */
 goto L8c65ab24;
L8c65ab24: r[13]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r13 */
 goto L8c65ab26;
L8c65ab26: r[0]+=4294967268u; /* add #-0x1c,r0 */
 goto L8c65ab28;
L8c65ab28: r[11]=rd(m,0x8c65ab80u,4); /* mov.l 0x8c65ab80,r11 */
 goto L8c65ab2a;
L8c65ab2a: r[13]+=32u; /* add #0x20,r13 */
 goto L8c65ab2c;
L8c65ab2c: wr(m,r[4]+r[0],r[13],4); /* mov.l r13,@(r0,r4) */
 goto L8c65ab2e;
L8c65ab2e: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65ab30;
L8c65ab30: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65ab32;
L8c65ab32: target=r[11];*pr=0x8c65ab36u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65ab36;
L8c65ab36: r[3]=sx(m,0x8c65ab66u,2); /* mov.w 0x8c65ab66,r3 */
 goto L8c65ab38;
L8c65ab38: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65ab3a;
L8c65ab3a: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65ab3c;
L8c65ab3c: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65ab3e;
L8c65ab3e: target=r[11];*pr=0x8c65ab42u;r[4]+=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65ab42;
L8c65ab42: r[9]=rd(m,r[14],4); /* mov.l @r14,r9 */
 goto L8c65ab44;
L8c65ab44: r[1]=sx(m,0x8c65ab64u,2); /* mov.w 0x8c65ab64,r1 */
 goto L8c65ab46;
L8c65ab46: r[0]=r[9]; /* mov r9,r0 */
 goto L8c65ab48;
L8c65ab48: r[0]=sx(m,r[1]+r[0],1); /* mov.b @(r0,r1),r0 */
 goto L8c65ab4a;
L8c65ab4a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65ab4c;
L8c65ab4c: if(!(*sr&1u))goto L8c65ab84; /* bf 0x8c65ab84 */
 goto L8c65ab4e;
L8c65ab4e: r[0]=sx(m,0x8c65ab66u,2); /* mov.w 0x8c65ab66,r0 */
 goto L8c65ab50;
L8c65ab50: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65ab52;
L8c65ab52: r[3]=sx(m,r[2]+r[0],1); /* mov.b @(r0,r2),r3 */
 goto L8c65ab54;
L8c65ab54: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65ab56;
L8c65ab56: if(!(*sr&1u))goto L8c65ab84; /* bf 0x8c65ab84 */
 goto L8c65ab58;
L8c65ab58: r[5]=sx(m,0x8c65ab68u,2); /* mov.w 0x8c65ab68,r5 */
 goto L8c65ab5a;
L8c65ab5a: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65ab5c;
L8c65ab5c: r[5]+=r[9]; /* add r9,r5 */
 goto L8c65ab5e;
L8c65ab5e: r[4]+=3u;goto L8c65ab8c; /* bra 0x8c65ab8c */
 goto L8c65ab84;
L8c65ab84: r[5]=sx(m,0x8c65ac64u,2); /* mov.w 0x8c65ac64,r5 */
 goto L8c65ab86;
L8c65ab86: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65ab88;
L8c65ab88: r[4]+=2u; /* add #0x2,r4 */
 goto L8c65ab8a;
L8c65ab8a: r[5]+=r[9]; /* add r9,r5 */
 goto L8c65ab8c;
L8c65ab8c: target=r[11];*pr=0x8c65ab90u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65ab90;
L8c65ab90: r[0]=sx(m,0x8c65ac66u,2); /* mov.w 0x8c65ac66,r0 */
 goto L8c65ab92;
L8c65ab92: r[3]=0u; /* mov #0x0,r3 */
 goto L8c65ab94;
L8c65ab94: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65ab96;
L8c65ab96: r[4]=sx(m,0x8c65ac6au,2); /* mov.w 0x8c65ac6a,r4 */
 goto L8c65ab98;
L8c65ab98: wr(m,r[2]+r[0],r[3],1); /* mov.b r3,@(r0,r2) */
 goto L8c65ab9a;
L8c65ab9a: r[3]=sx(m,0x8c65ac68u,2); /* mov.w 0x8c65ac68,r3 */
 goto L8c65ab9c;
L8c65ab9c: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65ab9e;
L8c65ab9e: r[11]=rd(m,0x8c65ac84u,4); /* mov.l 0x8c65ac84,r11 */
 goto L8c65aba0;
L8c65aba0: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65aba2;
L8c65aba2: target=r[11];*pr=0x8c65aba6u;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65aba6;
L8c65aba6: r[3]=sx(m,0x8c65ac6cu,2); /* mov.w 0x8c65ac6c,r3 */
 goto L8c65aba8;
L8c65aba8: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65abaa;
L8c65abaa: r[4]=sx(m,0x8c65ac6eu,2); /* mov.w 0x8c65ac6e,r4 */
 goto L8c65abac;
L8c65abac: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65abae;
L8c65abae: target=r[11];*pr=0x8c65abb2u;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65abb2;
L8c65abb2: r[3]=sx(m,0x8c65ac70u,2); /* mov.w 0x8c65ac70,r3 */
 goto L8c65abb4;
L8c65abb4: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65abb6;
L8c65abb6: r[4]=sx(m,0x8c65ac72u,2); /* mov.w 0x8c65ac72,r4 */
 goto L8c65abb8;
L8c65abb8: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65abba;
L8c65abba: target=r[11];*pr=0x8c65abbeu;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65abbe;
L8c65abbe: r[3]=sx(m,0x8c65ac74u,2); /* mov.w 0x8c65ac74,r3 */
 goto L8c65abc0;
L8c65abc0: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65abc2;
L8c65abc2: r[4]=sx(m,0x8c65ac76u,2); /* mov.w 0x8c65ac76,r4 */
 goto L8c65abc4;
L8c65abc4: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65abc6;
L8c65abc6: target=r[11];*pr=0x8c65abcau;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65abca;
L8c65abca: r[3]=sx(m,0x8c65ac78u,2); /* mov.w 0x8c65ac78,r3 */
 goto L8c65abcc;
L8c65abcc: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65abce;
L8c65abce: r[4]=sx(m,0x8c65ac7au,2); /* mov.w 0x8c65ac7a,r4 */
 goto L8c65abd0;
L8c65abd0: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65abd2;
L8c65abd2: target=r[11];*pr=0x8c65abd6u;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65abd6;
L8c65abd6: r[3]=sx(m,0x8c65ac7cu,2); /* mov.w 0x8c65ac7c,r3 */
 goto L8c65abd8;
L8c65abd8: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c65abda;
L8c65abda: r[4]=sx(m,0x8c65ac7eu,2); /* mov.w 0x8c65ac7e,r4 */
 goto L8c65abdc;
L8c65abdc: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65abde;
L8c65abde: target=r[11];*pr=0x8c65abe2u;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65abe2;
L8c65abe2: r[0]=sx(m,0x8c65ac80u,2); /* mov.w 0x8c65ac80,r0 */
 goto L8c65abe4;
L8c65abe4: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65abe6;
L8c65abe6: r[1]=rd(m,0x8c65ac88u,4); /* mov.l 0x8c65ac88,r1 */
 goto L8c65abe8;
L8c65abe8: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65abea;
L8c65abea: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c65abec;
L8c65abec: target=r[12];*pr=0x8c65abf0u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65abf0;
L8c65abf0: r[0]=sx(m,0x8c65ac68u,2); /* mov.w 0x8c65ac68,r0 */
 goto L8c65abf2;
L8c65abf2: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65abf4;
L8c65abf4: r[1]=rd(m,0x8c65ac8cu,4); /* mov.l 0x8c65ac8c,r1 */
 goto L8c65abf6;
L8c65abf6: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65abf8;
L8c65abf8: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c65abfa;
L8c65abfa: target=r[12];*pr=0x8c65abfeu;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65abfe;
L8c65abfe: r[0]=sx(m,0x8c65ac6cu,2); /* mov.w 0x8c65ac6c,r0 */
 goto L8c65ac00;
L8c65ac00: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ac02;
L8c65ac02: r[1]=rd(m,0x8c65ac90u,4); /* mov.l 0x8c65ac90,r1 */
 goto L8c65ac04;
L8c65ac04: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ac06;
L8c65ac06: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c65ac08;
L8c65ac08: target=r[12];*pr=0x8c65ac0cu;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ac0c;
L8c65ac0c: r[0]=sx(m,0x8c65ac70u,2); /* mov.w 0x8c65ac70,r0 */
 goto L8c65ac0e;
L8c65ac0e: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ac10;
L8c65ac10: r[1]=rd(m,0x8c65ac94u,4); /* mov.l 0x8c65ac94,r1 */
 goto L8c65ac12;
L8c65ac12: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ac14;
L8c65ac14: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c65ac16;
L8c65ac16: target=r[12];*pr=0x8c65ac1au;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ac1a;
L8c65ac1a: r[0]=sx(m,0x8c65ac74u,2); /* mov.w 0x8c65ac74,r0 */
 goto L8c65ac1c;
L8c65ac1c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ac1e;
L8c65ac1e: r[1]=rd(m,0x8c65ac98u,4); /* mov.l 0x8c65ac98,r1 */
 goto L8c65ac20;
L8c65ac20: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ac22;
L8c65ac22: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c65ac24;
L8c65ac24: target=r[12];*pr=0x8c65ac28u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ac28;
L8c65ac28: r[0]=sx(m,0x8c65ac78u,2); /* mov.w 0x8c65ac78,r0 */
 goto L8c65ac2a;
L8c65ac2a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ac2c;
L8c65ac2c: r[1]=rd(m,0x8c65ac9cu,4); /* mov.l 0x8c65ac9c,r1 */
 goto L8c65ac2e;
L8c65ac2e: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ac30;
L8c65ac30: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c65ac32;
L8c65ac32: target=r[12];*pr=0x8c65ac36u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ac36;
L8c65ac36: r[0]=sx(m,0x8c65ac7cu,2); /* mov.w 0x8c65ac7c,r0 */
 goto L8c65ac38;
L8c65ac38: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ac3a;
L8c65ac3a: r[1]=rd(m,0x8c65aca0u,4); /* mov.l 0x8c65aca0,r1 */
 goto L8c65ac3c;
L8c65ac3c: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ac3e;
L8c65ac3e: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c65ac40;
L8c65ac40: target=r[12];*pr=0x8c65ac44u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ac44;
L8c65ac44: r[15]+=56u; /* add #0x38,r15 */
 goto L8c65ac46;
L8c65ac46: r[3]=rd(m,0x8c65aca4u,4); /* mov.l 0x8c65aca4,r3 */
 goto L8c65ac48;
L8c65ac48: r[2]=rd(m,0x8c65aca8u,4); /* mov.l 0x8c65aca8,r2 */
 goto L8c65ac4a;
L8c65ac4a: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c65ac4c;
L8c65ac4c: target=r[12];*pr=0x8c65ac50u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c65ac50;
L8c65ac50: r[0]=r[10]; /* mov r10,r0 */
 goto L8c65ac52;
L8c65ac52: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65ac54;
L8c65ac54: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ac56;
L8c65ac56: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65ac58;
L8c65ac58: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65ac5a;
L8c65ac5a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65ac5c;
L8c65ac5c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65ac5e;
L8c65ac5e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65ac60;
L8c65ac60: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
