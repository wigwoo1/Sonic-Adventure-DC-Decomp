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
/* Retail backend readback66adc4..66ae8c; native integration verification pending. */
SA_STAGE_EXPORT int sa_sound_backend_readback(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66adc4;
L8c66adc4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66adc6;
L8c66adc6: r[14]=r[6]; /* mov r6,r14 */
 goto L8c66adc8;
L8c66adc8: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66adca;
L8c66adca: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66adcc;
L8c66adcc: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66adce;
L8c66adce: r[12]=r[5]; /* mov r5,r12 */
 goto L8c66add0;
L8c66add0: r[2]=rd(m,0x8c66ae8cu,4); /* mov.l 0x8c66ae8c,r2 */
 goto L8c66add2;
L8c66add2: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66add4;
L8c66add4: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66add6;
L8c66add6: r[10]=0u; /* mov #0x0,r10 */
 goto L8c66add8;
L8c66add8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66adda;
L8c66adda: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66addc;
L8c66addc: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66adde;
L8c66adde: take=!(*sr&1u);r[11]=r[10];if(take)goto L8c66ade8;goto L8c66ade2; /* bf/s 0x8c66ade8 */
 goto L8c66ade2;
L8c66ade2: r[11]=rd(m,0x8c66ae90u,4); /* mov.l 0x8c66ae90,r11 */
 goto L8c66ade4;
L8c66ade4: goto L8c66ae70; /* bra 0x8c66ae70 */
 goto L8c66ade8;
L8c66ade8: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c66adea;
L8c66adea: if(!(*sr&1u))goto L8c66adf2; /* bf 0x8c66adf2 */
 goto L8c66adec;
L8c66adec: r[11]=rd(m,0x8c66ae94u,4); /* mov.l 0x8c66ae94,r11 */
 goto L8c66adee;
L8c66adee: goto L8c66ae70; /* bra 0x8c66ae70 */
 goto L8c66adf2;
L8c66adf2: r[3]=rd(m,0x8c66ae98u,4); /* mov.l 0x8c66ae98,r3 */
 goto L8c66adf4;
L8c66adf4: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66adf6;
L8c66adf6: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66adf8;
L8c66adf8: r[6]+=20u; /* add #0x14,r6 */
 goto L8c66adfa;
L8c66adfa: target=r[3];*pr=0x8c66adfeu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66adfe;
L8c66adfe: r[2]=rd(m,0x8c66ae9cu,4); /* mov.l 0x8c66ae9c,r2 */
 goto L8c66ae00;
L8c66ae00: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae02;
L8c66ae02: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae04;
L8c66ae04: r[6]+=24u; /* add #0x18,r6 */
 goto L8c66ae06;
L8c66ae06: target=r[2];*pr=0x8c66ae0au;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ae0a;
L8c66ae0a: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae0c;
L8c66ae0c: r[3]=rd(m,0x8c66aea0u,4); /* mov.l 0x8c66aea0,r3 */
 goto L8c66ae0e;
L8c66ae0e: r[0]=r[10]; /* mov r10,r0 */
 goto L8c66ae10;
L8c66ae10: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae12;
L8c66ae12: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto L8c66ae14;
L8c66ae14: r[6]+=2u; /* add #0x2,r6 */
 goto L8c66ae16;
L8c66ae16: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c66ae18;
L8c66ae18: target=r[3];*pr=0x8c66ae1cu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ae1c;
L8c66ae1c: r[2]=rd(m,0x8c66aea4u,4); /* mov.l 0x8c66aea4,r2 */
 goto L8c66ae1e;
L8c66ae1e: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae20;
L8c66ae20: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae22;
L8c66ae22: r[6]+=3u; /* add #0x3,r6 */
 goto L8c66ae24;
L8c66ae24: target=r[2];*pr=0x8c66ae28u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ae28;
L8c66ae28: r[3]=rd(m,0x8c66aea8u,4); /* mov.l 0x8c66aea8,r3 */
 goto L8c66ae2a;
L8c66ae2a: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae2c;
L8c66ae2c: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae2e;
L8c66ae2e: r[6]+=6u; /* add #0x6,r6 */
 goto L8c66ae30;
L8c66ae30: target=r[3];*pr=0x8c66ae34u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ae34;
L8c66ae34: r[2]=rd(m,0x8c66aeacu,4); /* mov.l 0x8c66aeac,r2 */
 goto L8c66ae36;
L8c66ae36: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae38;
L8c66ae38: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae3a;
L8c66ae3a: r[6]+=8u; /* add #0x8,r6 */
 goto L8c66ae3c;
L8c66ae3c: target=r[2];*pr=0x8c66ae40u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ae40;
L8c66ae40: r[3]=rd(m,0x8c66aeb0u,4); /* mov.l 0x8c66aeb0,r3 */
 goto L8c66ae42;
L8c66ae42: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae44;
L8c66ae44: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae46;
L8c66ae46: r[6]+=4u; /* add #0x4,r6 */
 goto L8c66ae48;
L8c66ae48: target=r[3];*pr=0x8c66ae4cu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ae4c;
L8c66ae4c: r[2]=rd(m,0x8c66aeb4u,4); /* mov.l 0x8c66aeb4,r2 */
 goto L8c66ae4e;
L8c66ae4e: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae50;
L8c66ae50: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae52;
L8c66ae52: r[6]+=5u; /* add #0x5,r6 */
 goto L8c66ae54;
L8c66ae54: target=r[2];*pr=0x8c66ae58u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ae58;
L8c66ae58: r[3]=rd(m,0x8c66aeb8u,4); /* mov.l 0x8c66aeb8,r3 */
 goto L8c66ae5a;
L8c66ae5a: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae5c;
L8c66ae5c: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae5e;
L8c66ae5e: r[6]+=16u; /* add #0x10,r6 */
 goto L8c66ae60;
L8c66ae60: target=r[3];*pr=0x8c66ae64u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ae64;
L8c66ae64: r[2]=rd(m,0x8c66aebcu,4); /* mov.l 0x8c66aebc,r2 */
 goto L8c66ae66;
L8c66ae66: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66ae68;
L8c66ae68: r[5]=r[12]; /* mov r12,r5 */
 goto L8c66ae6a;
L8c66ae6a: r[6]+=12u; /* add #0xc,r6 */
 goto L8c66ae6c;
L8c66ae6c: target=r[2];*pr=0x8c66ae70u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ae70;
L8c66ae70: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c66ae72;
L8c66ae72: if((*sr&1u))goto L8c66ae7c; /* bt 0x8c66ae7c */
 goto L8c66ae74;
L8c66ae74: r[2]=rd(m,0x8c66aec4u,4); /* mov.l 0x8c66aec4,r2 */
 goto L8c66ae76;
L8c66ae76: r[4]=rd(m,0x8c66aec0u,4); /* mov.l 0x8c66aec0,r4 */
 goto L8c66ae78;
L8c66ae78: target=r[2];*pr=0x8c66ae7cu;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ae7c;
L8c66ae7c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ae7e;
L8c66ae7e: r[0]=r[11]; /* mov r11,r0 */
 goto L8c66ae80;
L8c66ae80: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66ae82;
L8c66ae82: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66ae84;
L8c66ae84: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66ae86;
L8c66ae86: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66ae88;
L8c66ae88: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
