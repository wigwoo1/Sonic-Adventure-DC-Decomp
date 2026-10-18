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
/* Static C recovery of retail filesystem completion processor
 * 8c5ffa12..8c5ffc98. External routines are dispatched through call.
 * Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_completion_process(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ffa12;
L8c5ffa12: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c5ffa14;
L8c5ffa14: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c5ffa16;
L8c5ffa16: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c5ffa18;
L8c5ffa18: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5ffa1a;
L8c5ffa1a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5ffa1c;
L8c5ffa1c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5ffa1e;
L8c5ffa1e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c5ffa20;
L8c5ffa20: r[15]+=4294967256u; /* add #-0x28,r15 */
 goto L8c5ffa22;
L8c5ffa22: r[13]=rd(m,0x8c5ffc0cu,4); /* mov.l 0x8c5ffc0c,r13 */
 goto L8c5ffa24;
L8c5ffa24: r[2]=1u; /* mov #0x1,r2 */
 goto L8c5ffa26;
L8c5ffa26: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffa28;
L8c5ffa28: wr(m,r[3]+0x20u,r[2],4); /* mov.l r2,@(0x20,r3) */
 goto L8c5ffa2a;
L8c5ffa2a: r[14]=rd(m,r[13],4); /* mov.l @r13,r14 */
 goto L8c5ffa2c;
L8c5ffa2c: r[14]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r14 */
 goto L8c5ffa2e;
L8c5ffa2e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c5ffa30;
L8c5ffa30: take=!(*sr&1u);r[12]=0u;if(take)goto L8c5ffa38;goto L8c5ffa34; /* bf/s 0x8c5ffa38 */
 goto L8c5ffa34;
L8c5ffa34: r[4]=0u;goto L8c5ffa3a; /* bra 0x8c5ffa3a */
 goto L8c5ffa38;
L8c5ffa38: r[4]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r4 */
 goto L8c5ffa3a;
L8c5ffa3a: r[11]=rd(m,r[13],4); /* mov.l @r13,r11 */
 goto L8c5ffa3c;
L8c5ffa3c: r[10]=r[15]; /* mov r15,r10 */
 goto L8c5ffa3e;
L8c5ffa3e: r[10]+=28u; /* add #0x1c,r10 */
 goto L8c5ffa40;
L8c5ffa40: r[11]=rd(m,r[11]+0xcu,4); /* mov.l @(0xc,r11),r11 */
 goto L8c5ffa42;
L8c5ffa42: r[11]=rd(m,r[11]+0x10u,4); /* mov.l @(0x10,r11),r11 */
 goto L8c5ffa44;
L8c5ffa44: target=r[11];*pr=0x8c5ffa48u;r[5]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c5ffa48;
L8c5ffa48: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c5ffa4a;
L8c5ffa4a: take=!(*sr&1u);r[11]=r[0];if(take)goto L8c5ffa52;goto L8c5ffa4e; /* bf/s 0x8c5ffa52 */
 goto L8c5ffa4e;
L8c5ffa4e: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffa52;
L8c5ffa52: r[0]=r[11]; /* mov r11,r0 */
 goto L8c5ffa54;
L8c5ffa54: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c5ffa56;
L8c5ffa56: take=(*sr&1u);r[9]=2u;if(take)goto L8c5ffa8a;goto L8c5ffa5a; /* bt/s 0x8c5ffa8a */
 goto L8c5ffa5a;
L8c5ffa5a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5ffa5c;
L8c5ffa5c: if((*sr&1u))goto L8c5ffa8a; /* bt 0x8c5ffa8a */
 goto L8c5ffa5e;
L8c5ffa5e: *sr=(*sr&~1u)|(r[0]==0x41u); /* cmp/eq #0x41,r0 */
 goto L8c5ffa60;
L8c5ffa60: if(!(*sr&1u))goto L8c5ffa66; /* bf 0x8c5ffa66 */
 goto L8c5ffa62;
L8c5ffa62: goto L8c5ffb84; /* bra 0x8c5ffb84 */
 goto L8c5ffa66;
L8c5ffa66: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c5ffa68;
L8c5ffa68: if(!(*sr&1u))goto L8c5ffa6e; /* bf 0x8c5ffa6e */
 goto L8c5ffa6a;
L8c5ffa6a: goto L8c5ffbb8; /* bra 0x8c5ffbb8 */
 goto L8c5ffa6e;
L8c5ffa6e: *sr=(*sr&~1u)|(r[0]==0x43u); /* cmp/eq #0x43,r0 */
 goto L8c5ffa70;
L8c5ffa70: if(!(*sr&1u))goto L8c5ffa76; /* bf 0x8c5ffa76 */
 goto L8c5ffa72;
L8c5ffa72: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffa76;
L8c5ffa76: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c5ffa78;
L8c5ffa78: if(!(*sr&1u))goto L8c5ffa7e; /* bf 0x8c5ffa7e */
 goto L8c5ffa7a;
L8c5ffa7a: goto L8c5ffbcc; /* bra 0x8c5ffbcc */
 goto L8c5ffa7e;
L8c5ffa7e: *sr=(*sr&~1u)|(r[0]==0x40u); /* cmp/eq #0x40,r0 */
 goto L8c5ffa80;
L8c5ffa80: if(!(*sr&1u))goto L8c5ffa86; /* bf 0x8c5ffa86 */
 goto L8c5ffa82;
L8c5ffa82: goto L8c5ffbcc; /* bra 0x8c5ffbcc */
 goto L8c5ffa86;
L8c5ffa86: goto L8c5ffc74; /* bra 0x8c5ffc74 */
 goto L8c5ffa8a;
L8c5ffa8a: r[0]=72u; /* mov #0x48,r0 */
 goto L8c5ffa8c;
L8c5ffa8c: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffa8e;
L8c5ffa8e: r[4]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r4 */
 goto L8c5ffa90;
L8c5ffa90: wr(m,r[3]+0x1cu,r[12],4); /* mov.l r12,@(0x1c,r3) */
 goto L8c5ffa92;
L8c5ffa92: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffa94;
L8c5ffa94: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c5ffa96;
L8c5ffa96: r[0]=72u; /* mov #0x48,r0 */
 goto L8c5ffa98;
L8c5ffa98: wr(m,r[14]+0x3cu,r[12],4); /* mov.l r12,@(0x3c,r14) */
 goto L8c5ffa9a;
L8c5ffa9a: wr(m,r[14]+r[0],r[12],2); /* mov.w r12,@(r0,r14) */
 goto L8c5ffa9c;
L8c5ffa9c: r[0]=r[11]; /* mov r11,r0 */
 goto L8c5ffa9e;
L8c5ffa9e: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5ffaa0;
L8c5ffaa0: if((*sr&1u))goto L8c5ffaa6; /* bt 0x8c5ffaa6 */
 goto L8c5ffaa2;
L8c5ffaa2: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffaa6;
L8c5ffaa6: r[0]=r[4]; /* mov r4,r0 */
 goto L8c5ffaa8;
L8c5ffaa8: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5ffaaa;
L8c5ffaaa: if(!(*sr&1u))goto L8c5ffad0; /* bf 0x8c5ffad0 */
 goto L8c5ffaac;
L8c5ffaac: r[3]=rd(m,r[10],4); /* mov.l @r10,r3 */
 goto L8c5ffaae;
L8c5ffaae: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c5ffab0;
L8c5ffab0: wr(m,r[14]+0x18u,r[3],4); /* mov.l r3,@(0x18,r14) */
 goto L8c5ffab2;
L8c5ffab2: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c5ffab4;
L8c5ffab4: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c5ffab6;
L8c5ffab6: r[2]+=r[3]; /* add r3,r2 */
 goto L8c5ffab8;
L8c5ffab8: wr(m,r[14]+0x14u,r[2],4); /* mov.l r2,@(0x14,r14) */
 goto L8c5ffaba;
L8c5ffaba: wr(m,r[14]+r[0],r[9],2); /* mov.w r9,@(r0,r14) */
 goto L8c5ffabc;
L8c5ffabc: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c5ffabe;
L8c5ffabe: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c5ffac0;
L8c5ffac0: if(!(*sr&1u))goto L8c5ffac6; /* bf 0x8c5ffac6 */
 goto L8c5ffac2;
L8c5ffac2: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffac6;
L8c5ffac6: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c5ffac8;
L8c5ffac8: target=r[3];*pr=0x8c5ffaccu;r[4]=rd(m,r[14]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5ffacc;
L8c5ffacc: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffad0;
L8c5ffad0: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c5ffad2;
L8c5ffad2: if((*sr&1u))goto L8c5ffae6; /* bt 0x8c5ffae6 */
 goto L8c5ffad4;
L8c5ffad4: r[0]=r[4]; /* mov r4,r0 */
 goto L8c5ffad6;
L8c5ffad6: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c5ffad8;
L8c5ffad8: if(!(*sr&1u))goto L8c5ffaec; /* bf 0x8c5ffaec */
 goto L8c5ffada;
L8c5ffada: r[3]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r3 */
 goto L8c5ffadc;
L8c5ffadc: wr(m,r[14]+0x18u,r[3],4); /* mov.l r3,@(0x18,r14) */
 goto L8c5ffade;
L8c5ffade: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c5ffae0;
L8c5ffae0: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c5ffae2;
L8c5ffae2: r[2]+=r[3]; /* add r3,r2 */
 goto L8c5ffae4;
L8c5ffae4: wr(m,r[14]+0x14u,r[2],4); /* mov.l r2,@(0x14,r14) */
 goto L8c5ffae6;
L8c5ffae6: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c5ffae8;
L8c5ffae8: wr(m,r[14]+r[0],r[9],2);goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffaec;
L8c5ffaec: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c5ffaee;
L8c5ffaee: if(!(*sr&1u))goto L8c5ffb38; /* bf 0x8c5ffb38 */
 goto L8c5ffaf0;
L8c5ffaf0: r[4]=rd(m,r[13],4); /* mov.l @r13,r4 */
 goto L8c5ffaf2;
L8c5ffaf2: r[5]=r[15]; /* mov r15,r5 */
 goto L8c5ffaf4;
L8c5ffaf4: r[3]=rd(m,0x8c5ffc10u,4); /* mov.l 0x8c5ffc10,r3 */
 goto L8c5ffaf6;
L8c5ffaf6: r[5]+=4u; /* add #0x4,r5 */
 goto L8c5ffaf8;
L8c5ffaf8: target=r[3];*pr=0x8c5ffafcu;r[4]+=64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5ffafc;
L8c5ffafc: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffafe;
L8c5ffafe: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffb00;
L8c5ffb00: if((*sr&1u))goto L8c5ffb14; /* bt 0x8c5ffb14 */
 goto L8c5ffb02;
L8c5ffb02: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffb04;
L8c5ffb04: r[3]=5u; /* mov #0x5,r3 */
 goto L8c5ffb06;
L8c5ffb06: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c5ffb08;
L8c5ffb08: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffb0a;
L8c5ffb0a: r[2]=4294967277u; /* mov #-0x13,r2 */
 goto L8c5ffb0c;
L8c5ffb0c: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c5ffb0e;
L8c5ffb0e: r[0]=68u; /* mov #0x44,r0 */
 goto L8c5ffb10;
L8c5ffb10: wr(m,r[14]+r[0],r[12],4);goto L8c5ffc34; /* bra 0x8c5ffc34 */
 goto L8c5ffb14;
L8c5ffb14: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c5ffb16;
L8c5ffb16: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c5ffb18;
L8c5ffb18: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c5ffb1a;
L8c5ffb1a: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c5ffb1c;
L8c5ffb1c: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c5ffb1e;
L8c5ffb1e: wr(m,r[2]+0x4u,r[3],4); /* mov.l r3,@(0x4,r2) */
 goto L8c5ffb20;
L8c5ffb20: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c5ffb22;
L8c5ffb22: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c5ffb24;
L8c5ffb24: wr(m,r[2]+0x8u,r[3],4); /* mov.l r3,@(0x8,r2) */
 goto L8c5ffb26;
L8c5ffb26: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c5ffb28;
L8c5ffb28: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c5ffb2a;
L8c5ffb2a: wr(m,r[2]+0xcu,r[3],4); /* mov.l r3,@(0xc,r2) */
 goto L8c5ffb2c;
L8c5ffb2c: r[1]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r1 */
 goto L8c5ffb2e;
L8c5ffb2e: r[3]=rd(m,0x8c5ffc14u,4); /* mov.l 0x8c5ffc14,r3 */
 goto L8c5ffb30;
L8c5ffb30: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c5ffb32;
L8c5ffb32: r[1]&=r[3]; /* and r3,r1 */
 goto L8c5ffb34;
L8c5ffb34: wr(m,r[2]+0x10u,r[1],4);goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffb38;
L8c5ffb38: r[0]=r[4]; /* mov r4,r0 */
 goto L8c5ffb3a;
L8c5ffb3a: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c5ffb3c;
L8c5ffb3c: if((*sr&1u))goto L8c5ffb42; /* bt 0x8c5ffb42 */
 goto L8c5ffb3e;
L8c5ffb3e: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffb42;
L8c5ffb42: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffb44;
L8c5ffb44: r[0]=72u; /* mov #0x48,r0 */
 goto L8c5ffb46;
L8c5ffb46: r[5]=rd(m,0x8c5ffc1cu,4); /* mov.l 0x8c5ffc1c,r5 */
 goto L8c5ffb48;
L8c5ffb48: r[2]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r2 */
 goto L8c5ffb4a;
L8c5ffb4a: r[4]=rd(m,0x8c5ffc20u,4); /* mov.l 0x8c5ffc20,r4 */
 goto L8c5ffb4c;
L8c5ffb4c: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c5ffb4e;
L8c5ffb4e: r[7]=rd(m,0x8c5ffc18u,4); /* mov.l 0x8c5ffc18,r7 */
 goto L8c5ffb50;
L8c5ffb50: target=r[1];*pr=0x8c5ffb54u;r[6]=rd(m,r[14]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c5ffb54;
L8c5ffb54: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffb56;
L8c5ffb56: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffb58;
L8c5ffb58: if((*sr&1u))goto L8c5ffb6e; /* bt 0x8c5ffb6e */
 goto L8c5ffb5a;
L8c5ffb5a: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffb5c;
L8c5ffb5c: wr(m,r[14]+0x3cu,r[4],4); /* mov.l r4,@(0x3c,r14) */
 goto L8c5ffb5e;
L8c5ffb5e: r[2]=4u; /* mov #0x4,r2 */
 goto L8c5ffb60;
L8c5ffb60: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c5ffb62;
L8c5ffb62: r[0]=72u; /* mov #0x48,r0 */
 goto L8c5ffb64;
L8c5ffb64: r[3]=6u; /* mov #0x6,r3 */
 goto L8c5ffb66;
L8c5ffb66: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c5ffb68;
L8c5ffb68: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c5ffb6a;
L8c5ffb6a: wr(m,r[2]+0x1cu,r[14],4);goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffb6e;
L8c5ffb6e: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffb70;
L8c5ffb70: r[2]=5u; /* mov #0x5,r2 */
 goto L8c5ffb72;
L8c5ffb72: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c5ffb74;
L8c5ffb74: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffb76;
L8c5ffb76: r[3]=4294967268u; /* mov #-0x1c,r3 */
 goto L8c5ffb78;
L8c5ffb78: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c5ffb7a;
L8c5ffb7a: r[0]=68u; /* mov #0x44,r0 */
 goto L8c5ffb7c;
L8c5ffb7c: wr(m,r[14]+r[0],r[12],4); /* mov.l r12,@(r0,r14) */
 goto L8c5ffb7e;
L8c5ffb7e: r[3]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c5ffb80;
L8c5ffb80: wr(m,r[3],r[12],4);goto L8c5ffc34; /* bra 0x8c5ffc34 */
 goto L8c5ffb84;
L8c5ffb84: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c5ffb86;
L8c5ffb86: r[5]=r[15]; /* mov r15,r5 */
 goto L8c5ffb88;
L8c5ffb88: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c5ffb8a;
L8c5ffb8a: r[1]=rd(m,r[3]+0x24u,4); /* mov.l @(0x24,r3),r1 */
 goto L8c5ffb8c;
L8c5ffb8c: target=r[1];*pr=0x8c5ffb90u;r[4]=rd(m,r[14]+0x3cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c5ffb90;
L8c5ffb90: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffb92;
L8c5ffb92: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffb94;
L8c5ffb94: if(!(*sr&1u))goto L8c5ffbac; /* bf 0x8c5ffbac */
 goto L8c5ffb96;
L8c5ffb96: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c5ffb98;
L8c5ffb98: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c5ffb9a;
L8c5ffb9a: if(!(*sr&1u))goto L8c5ffba2; /* bf 0x8c5ffba2 */
 goto L8c5ffb9c;
L8c5ffb9c: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c5ffb9e;
L8c5ffb9e: wr(m,r[14]+r[0],r[12],2);goto L8c5ffba6; /* bra 0x8c5ffba6 */
 goto L8c5ffba2;
L8c5ffba2: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c5ffba4;
L8c5ffba4: wr(m,r[14]+r[0],r[9],2); /* mov.w r9,@(r0,r14) */
 goto L8c5ffba6;
L8c5ffba6: r[3]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r3 */
 goto L8c5ffba8;
L8c5ffba8: wr(m,r[14]+0x18u,r[3],4);goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffbac;
L8c5ffbac: r[0]=r[4]; /* mov r4,r0 */
 goto L8c5ffbae;
L8c5ffbae: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5ffbb0;
L8c5ffbb0: if(!(*sr&1u))goto L8c5ffc80; /* bf 0x8c5ffc80 */
 goto L8c5ffbb2;
L8c5ffbb2: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c5ffbb4;
L8c5ffbb4: wr(m,r[14]+0x18u,r[2],4);goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffbb8;
L8c5ffbb8: r[1]=rd(m,r[10]+0x8u,4); /* mov.l @(0x8,r10),r1 */
 goto L8c5ffbba;
L8c5ffbba: r[0]=64u; /* mov #0x40,r0 */
 goto L8c5ffbbc;
L8c5ffbbc: wr(m,r[14]+r[0],r[1],4); /* mov.l r1,@(r0,r14) */
 goto L8c5ffbbe;
L8c5ffbbe: r[0]=72u; /* mov #0x48,r0 */
 goto L8c5ffbc0;
L8c5ffbc0: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c5ffbc2;
L8c5ffbc2: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5ffbc4;
L8c5ffbc4: if(!(*sr&1u))goto L8c5ffc80; /* bf 0x8c5ffc80 */
 goto L8c5ffbc6;
L8c5ffbc6: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c5ffbc8;
L8c5ffbc8: wr(m,r[14]+0x18u,r[2],4);goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffbcc;
L8c5ffbcc: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffbce;
L8c5ffbce: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c5ffbd0;
L8c5ffbd0: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffbd2;
L8c5ffbd2: r[3]=rd(m,r[10],4); /* mov.l @r10,r3 */
 goto L8c5ffbd4;
L8c5ffbd4: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c5ffbd6;
L8c5ffbd6: r[0]=68u; /* mov #0x44,r0 */
 goto L8c5ffbd8;
L8c5ffbd8: r[2]=rd(m,r[10]+0x4u,4); /* mov.l @(0x4,r10),r2 */
 goto L8c5ffbda;
L8c5ffbda: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c5ffbdc;
L8c5ffbdc: wr(m,r[14]+0x3cu,r[12],4); /* mov.l r12,@(0x3c,r14) */
 goto L8c5ffbde;
L8c5ffbde: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffbe0;
L8c5ffbe0: wr(m,r[3]+0x1cu,r[12],4); /* mov.l r12,@(0x1c,r3) */
 goto L8c5ffbe2;
L8c5ffbe2: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffbe4;
L8c5ffbe4: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c5ffbe6;
L8c5ffbe6: *sr=(*sr&~1u)|(r[0]==0xffffffe3u); /* cmp/eq #-0x1d,r0 */
 goto L8c5ffbe8;
L8c5ffbe8: if(!(*sr&1u))goto L8c5ffc34; /* bf 0x8c5ffc34 */
 goto L8c5ffbea;
L8c5ffbea: r[0]=68u; /* mov #0x44,r0 */
 goto L8c5ffbec;
L8c5ffbec: r[0]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r0 */
 goto L8c5ffbee;
L8c5ffbee: *sr=(*sr&~1u)|(r[0]==0x3au); /* cmp/eq #0x3a,r0 */
 goto L8c5ffbf0;
L8c5ffbf0: if(!(*sr&1u))goto L8c5ffc24; /* bf 0x8c5ffc24 */
 goto L8c5ffbf2;
L8c5ffbf2: *pr=0x8c5ffbf6u;status=call(ctx,0x8c600688u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c600688 */
 goto L8c5ffbf6;
L8c5ffbf6: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c5ffbf8;
L8c5ffbf8: take=(*sr&1u);r[4]=r[0];if(take)goto L8c5ffc00;goto L8c5ffbfc; /* bt/s 0x8c5ffc00 */
 goto L8c5ffbfc;
L8c5ffbfc: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffbfe;
L8c5ffbfe: if(!(*sr&1u))goto L8c5ffc34; /* bf 0x8c5ffc34 */
 goto L8c5ffc00;
L8c5ffc00: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffc02;
L8c5ffc02: r[3]=4294967273u; /* mov #-0x17,r3 */
 goto L8c5ffc04;
L8c5ffc04: wr(m,r[14]+r[0],r[3],2);goto L8c5ffc34; /* bra 0x8c5ffc34 */
 goto L8c5ffc24;
L8c5ffc24: r[0]=68u; /* mov #0x44,r0 */
 goto L8c5ffc26;
L8c5ffc26: r[1]=sx(m,0x8c5ffd14u,2); /* mov.w 0x8c5ffd14,r1 */
 goto L8c5ffc28;
L8c5ffc28: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c5ffc2a;
L8c5ffc2a: *sr=(*sr&~1u)|(r[1]==r[2]); /* cmp/eq r1,r2 */
 goto L8c5ffc2c;
L8c5ffc2c: if(!(*sr&1u))goto L8c5ffc34; /* bf 0x8c5ffc34 */
 goto L8c5ffc2e;
L8c5ffc2e: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffc30;
L8c5ffc30: r[3]=4294967259u; /* mov #-0x25,r3 */
 goto L8c5ffc32;
L8c5ffc32: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c5ffc34;
L8c5ffc34: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffc36;
L8c5ffc36: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c5ffc38;
L8c5ffc38: *sr=(*sr&~1u)|(r[0]==0xffffffe9u); /* cmp/eq #-0x17,r0 */
 goto L8c5ffc3a;
L8c5ffc3a: take=(*sr&1u);r[4]=r[0];if(take)goto L8c5ffc44;goto L8c5ffc3e; /* bt/s 0x8c5ffc44 */
 goto L8c5ffc3e;
L8c5ffc3e: r[0]=r[4]; /* mov r4,r0 */
 goto L8c5ffc40;
L8c5ffc40: *sr=(*sr&~1u)|(r[0]==0xffffffdfu); /* cmp/eq #-0x21,r0 */
 goto L8c5ffc42;
L8c5ffc42: if(!(*sr&1u))goto L8c5ffc4a; /* bf 0x8c5ffc4a */
 goto L8c5ffc44;
L8c5ffc44: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffc46;
L8c5ffc46: r[2]=1u; /* mov #0x1,r2 */
 goto L8c5ffc48;
L8c5ffc48: wr(m,r[3]+0x34u,r[2],4); /* mov.l r2,@(0x34,r3) */
 goto L8c5ffc4a;
L8c5ffc4a: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffc4c;
L8c5ffc4c: r[2]=rd(m,r[3]+0x24u,4); /* mov.l @(0x24,r3),r2 */
 goto L8c5ffc4e;
L8c5ffc4e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c5ffc50;
L8c5ffc50: if((*sr&1u))goto L8c5ffc60; /* bt 0x8c5ffc60 */
 goto L8c5ffc52;
L8c5ffc52: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c5ffc54;
L8c5ffc54: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffc56;
L8c5ffc56: r[5]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r5 */
 goto L8c5ffc58;
L8c5ffc58: r[3]=rd(m,r[2]+0x24u,4); /* mov.l @(0x24,r2),r3 */
 goto L8c5ffc5a;
L8c5ffc5a: r[4]=r[2]; /* mov r2,r4 */
 goto L8c5ffc5c;
L8c5ffc5c: target=r[3];*pr=0x8c5ffc60u;r[4]=rd(m,r[4]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5ffc60;
L8c5ffc60: r[3]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r3 */
 goto L8c5ffc62;
L8c5ffc62: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c5ffc64;
L8c5ffc64: if((*sr&1u))goto L8c5ffc80; /* bt 0x8c5ffc80 */
 goto L8c5ffc66;
L8c5ffc66: r[0]=82u; /* mov #0x52,r0 */
 goto L8c5ffc68;
L8c5ffc68: r[3]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r3 */
 goto L8c5ffc6a;
L8c5ffc6a: r[5]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r5 */
 goto L8c5ffc6c;
L8c5ffc6c: target=r[3];*pr=0x8c5ffc70u;r[4]=rd(m,r[14]+0x38u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5ffc70;
L8c5ffc70: goto L8c5ffc80; /* bra 0x8c5ffc80 */
 goto L8c5ffc74;
L8c5ffc74: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffc76;
L8c5ffc76: r[2]=r[11]&255u; /* extu.b r11,r2 */
 goto L8c5ffc78;
L8c5ffc78: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c5ffc7a;
L8c5ffc7a: wr(m,r[14]+0x3cu,r[12],4); /* mov.l r12,@(0x3c,r14) */
 goto L8c5ffc7c;
L8c5ffc7c: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffc7e;
L8c5ffc7e: wr(m,r[3]+0x1cu,r[12],4); /* mov.l r12,@(0x1c,r3) */
 goto L8c5ffc80;
L8c5ffc80: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffc82;
L8c5ffc82: wr(m,r[3]+0x20u,r[12],4); /* mov.l r12,@(0x20,r3) */
 goto L8c5ffc84;
L8c5ffc84: r[0]=r[11]; /* mov r11,r0 */
 goto L8c5ffc86;
L8c5ffc86: r[15]+=40u; /* add #0x28,r15 */
 goto L8c5ffc88;
L8c5ffc88: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c5ffc8a;
L8c5ffc8a: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5ffc8c;
L8c5ffc8c: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5ffc8e;
L8c5ffc8e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5ffc90;
L8c5ffc90: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c5ffc92;
L8c5ffc92: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c5ffc94;
L8c5ffc94: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
