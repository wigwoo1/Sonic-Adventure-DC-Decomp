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
/* Retail data-chunk encoding path 8c66a9fa..8c66ab2a.
 * Selector2 reconstructs stereo byte pairs(0,2)/(1,3);1 expands8-bit;0 copies16-bit.
 * Dispatcher skips unknown selectors. Native helpers connected by caller. */
SA_STAGE_EXPORT int sa_sound_data_bytepair_transfer_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a9fa;
L8c66a9fa: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a9fc;
L8c66a9fc: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a9fe;
L8c66a9fe: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66aa00;
L8c66aa00: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66aa02;
L8c66aa02: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66aa04;
L8c66aa04: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66aa06;
L8c66aa06: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66aa08;
L8c66aa08: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66aa0a;
L8c66aa0a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66aa0c;
L8c66aa0c: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66aa0e;
L8c66aa0e: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66aa10;
L8c66aa10: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66ab00;goto L8c66aa14; /* bf/s 0x8c66ab00 */
 goto L8c66aa14;
L8c66aa14: r[2]=rd(m,0x8c66aabcu,4); /* mov.l 0x8c66aabc,r2 */
 goto L8c66aa16;
L8c66aa16: target=r[2];*pr=0x8c66aa1au;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66aa1a;
L8c66aa1a: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66aa1c;
L8c66aa1c: if(!(*sr&1u))goto L8c66ab00; /* bf 0x8c66ab00 */
 goto L8c66aa1e;
L8c66aa1e: r[0]=sx(m,0x8c66aab8u,2); /* mov.w 0x8c66aab8,r0 */
 goto L8c66aa20;
L8c66aa20: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66aa22;
L8c66aa22: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66aa24;
L8c66aa24: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66aa26;
L8c66aa26: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66aa28;
L8c66aa28: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66aa2a;
L8c66aa2a: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66aa2c;
L8c66aa2c: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66aa2e;
L8c66aa2e: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66aa30;
L8c66aa30: target=r[2];*pr=0x8c66aa34u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66aa34;
L8c66aa34: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66aa36;
L8c66aa36: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66aa38;
L8c66aa38: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66aa3a;
L8c66aa3a: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66aa3c;
L8c66aa3c: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66aa3e;
L8c66aa3e: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66aa44;goto L8c66aa42; /* bf/s 0x8c66aa44 */
 goto L8c66aa42;
L8c66aa42: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66aa44;
L8c66aa44: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66aa46;
L8c66aa46: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c66aa4c;goto L8c66aa4a; /* bf/s 0x8c66aa4c */
 goto L8c66aa4a;
L8c66aa4a: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66aa4c;
L8c66aa4c: r[11]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r11 */
 goto L8c66aa4e;
L8c66aa4e: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66aa50;
L8c66aa50: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c66aa52;
L8c66aa52: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66aa54;
L8c66aa54: r[11]+=r[5]; /* add r5,r11 */
 goto L8c66aa56;
L8c66aa56: take=!(*sr&1u);r[7]=0u;if(take)goto L8c66aac0;goto L8c66aa5a; /* bf/s 0x8c66aac0 */
 goto L8c66aa5a;
L8c66aa5a: r[10]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r10 */
 goto L8c66aa5c;
L8c66aa5c: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66aa5e;
L8c66aa5e: r[13]=r[7]; /* mov r7,r13 */
 goto L8c66aa60;
L8c66aa60: r[10]+=r[3]; /* add r3,r10 */
 goto L8c66aa62;
L8c66aa62: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1; /* shll r10 */
 goto L8c66aa64;
L8c66aa64: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66aa66;
L8c66aa66: r[10]+=r[5]; /* add r5,r10 */
 goto L8c66aa68;
L8c66aa68: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66aa6a;
L8c66aa6a: take=!(*sr&1u);r[6]=r[7];if(take)goto L8c66aaec;goto L8c66aa6e; /* bf/s 0x8c66aaec */
 goto L8c66aa6e;
L8c66aa6e: r[2]=r[5]; /* mov r5,r2 */
 goto L8c66aa70;
L8c66aa70: r[2]+=2u; /* add #0x2,r2 */
 goto L8c66aa72;
L8c66aa72: r[2]+=r[12]; /* add r12,r2 */
 goto L8c66aa74;
L8c66aa74: r[0]=r[12]; /* mov r12,r0 */
 goto L8c66aa76;
L8c66aa76: r[3]=sx(m,r[2],1); /* mov.b @r2,r3 */
 goto L8c66aa78;
L8c66aa78: r[1]=r[11]; /* mov r11,r1 */
 goto L8c66aa7a;
L8c66aa7a: r[2]=sx(m,r[5]+r[0],1); /* mov.b @(r0,r5),r2 */
 goto L8c66aa7c;
L8c66aa7c: r[1]+=r[6]; /* add r6,r1 */
 goto L8c66aa7e;
L8c66aa7e: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66aa80;
L8c66aa80: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66aa82;
L8c66aa82: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66aa84;
L8c66aa84: r[3]<<=8; /* shll8 r3 */
 goto L8c66aa86;
L8c66aa86: r[3]|=r[2]; /* or r2,r3 */
 goto L8c66aa88;
L8c66aa88: r[2]=r[5]; /* mov r5,r2 */
 goto L8c66aa8a;
L8c66aa8a: wr(m,r[1],r[3],2); /* mov.w r3,@r1 */
 goto L8c66aa8c;
L8c66aa8c: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66aa8e;
L8c66aa8e: r[3]+=3u; /* add #0x3,r3 */
 goto L8c66aa90;
L8c66aa90: r[2]+=1u; /* add #0x1,r2 */
 goto L8c66aa92;
L8c66aa92: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66aa94;
L8c66aa94: r[2]+=r[12]; /* add r12,r2 */
 goto L8c66aa96;
L8c66aa96: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c66aa98;
L8c66aa98: r[2]=sx(m,r[2],1); /* mov.b @r2,r2 */
 goto L8c66aa9a;
L8c66aa9a: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66aa9c;
L8c66aa9c: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66aa9e;
L8c66aa9e: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66aaa0;
L8c66aaa0: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66aaa2;
L8c66aaa2: r[3]<<=8; /* shll8 r3 */
 goto L8c66aaa4;
L8c66aaa4: r[3]|=r[2]; /* or r2,r3 */
 goto L8c66aaa6;
L8c66aaa6: r[5]+=4u; /* add #0x4,r5 */
 goto L8c66aaa8;
L8c66aaa8: wr(m,r[0]+r[10],r[3],2); /* mov.w r3,@(r0,r10) */
 goto L8c66aaaa;
L8c66aaaa: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66aa6e;goto L8c66aaae; /* bf/s 0x8c66aa6e */
 goto L8c66aaae;
L8c66aaae: goto L8c66aaec; /* bra 0x8c66aaec */
 goto L8c66aac0;
L8c66aac0: r[6]=0u; /* mov #0x0,r6 */
 goto L8c66aac2;
L8c66aac2: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66aac4;
L8c66aac4: r[13]=r[7]; /* mov r7,r13 */
 goto L8c66aac6;
L8c66aac6: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66aac8;
L8c66aac8: take=!(*sr&1u);r[6]+=r[11];if(take)goto L8c66aaec;goto L8c66aacc; /* bf/s 0x8c66aaec */
 goto L8c66aacc;
L8c66aacc: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66aace;
L8c66aace: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66aad0;
L8c66aad0: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66aad2;
L8c66aad2: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66aad4;
L8c66aad4: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c66aad6;
L8c66aad6: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66aad8;
L8c66aad8: r[2]=sx(m,r[12]+r[0],1); /* mov.b @(r0,r12),r2 */
 goto L8c66aada;
L8c66aada: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66aadc;
L8c66aadc: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66aade;
L8c66aade: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66aae0;
L8c66aae0: r[3]<<=8; /* shll8 r3 */
 goto L8c66aae2;
L8c66aae2: r[3]|=r[2]; /* or r2,r3 */
 goto L8c66aae4;
L8c66aae4: r[5]+=2u; /* add #0x2,r5 */
 goto L8c66aae6;
L8c66aae6: wr(m,r[6],r[3],2); /* mov.w r3,@r6 */
 goto L8c66aae8;
L8c66aae8: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66aacc;goto L8c66aaec; /* bf/s 0x8c66aacc */
 goto L8c66aaec;
L8c66aaec: r[0]=sx(m,0x8c66ab86u,2); /* mov.w 0x8c66ab86,r0 */
 goto L8c66aaee;
L8c66aaee: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66aaf0;
L8c66aaf0: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66aaf2;
L8c66aaf2: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66aaf4;
L8c66aaf4: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66aaf6;
L8c66aaf6: r[0]=sx(m,0x8c66ab88u,2); /* mov.w 0x8c66ab88,r0 */
 goto L8c66aaf8;
L8c66aaf8: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66aafa;
L8c66aafa: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c66aafc;
L8c66aafc: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66aafe;
L8c66aafe: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66ab00;
L8c66ab00: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66ab02;
L8c66ab02: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66ab04;
L8c66ab04: if(!(*sr&1u))goto L8c66ab1c; /* bf 0x8c66ab1c */
 goto L8c66ab06;
L8c66ab06: r[0]=sx(m,0x8c66ab8au,2); /* mov.w 0x8c66ab8a,r0 */
 goto L8c66ab08;
L8c66ab08: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66ab0a;
L8c66ab0a: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66ab0c;
L8c66ab0c: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66ab0e;
L8c66ab0e: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66ab10;
L8c66ab10: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66ab12;
L8c66ab12: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66ab14;
L8c66ab14: target=r[3];*pr=0x8c66ab18u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ab18;
L8c66ab18: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66ab1a;
L8c66ab1a: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66ab1c;
L8c66ab1c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ab1e;
L8c66ab1e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66ab20;
L8c66ab20: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66ab22;
L8c66ab22: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66ab24;
L8c66ab24: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66ab26;
L8c66ab26: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
