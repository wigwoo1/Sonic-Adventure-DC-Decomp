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
/* Retail magic8000 extension-word getter65af64..65afca.
 * Requires signed size>=18 and BE header offset>13; copies BE word at16.
 * Field semantic meaning not established; no external calls. */
SA_STAGE_EXPORT int sa_sound_magic_extension_word(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65af64;
L8c65af64: r[3]=18u; /* mov #0x12,r3 */
 goto L8c65af66;
L8c65af66: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c65af68;
L8c65af68: if(!(*sr&1u))goto L8c65afa8; /* bf 0x8c65afa8 */
 goto L8c65af6a;
L8c65af6a: r[5]=sx(m,r[4],2); /* mov.w @r4,r5 */
 goto L8c65af6c;
L8c65af6c: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c65af6e;
L8c65af6e: r[7]=rd(m,0x8c65b050u,4); /* mov.l 0x8c65b050,r7 */
 goto L8c65af70;
L8c65af70: r[5]=r[5]&65535u; /* extu.w r5,r5 */
 goto L8c65af72;
L8c65af72: r[3]=r[5]; /* mov r5,r3 */
 goto L8c65af74;
L8c65af74: r[1]=r[5]; /* mov r5,r1 */
 goto L8c65af76;
L8c65af76: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c65af78;
L8c65af78: r[1]<<=8; /* shll8 r1 */
 goto L8c65af7a;
L8c65af7a: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c65af7c;
L8c65af7c: r[5]=r[3]; /* mov r3,r5 */
 goto L8c65af7e;
L8c65af7e: r[1]&=r[7]; /* and r7,r1 */
 goto L8c65af80;
L8c65af80: r[3]=rd(m,0x8c65b054u,4); /* mov.l 0x8c65b054,r3 */
 goto L8c65af82;
L8c65af82: r[5]|=r[1]; /* or r1,r5 */
 goto L8c65af84;
L8c65af84: r[5]=r[5]&65535u; /* extu.w r5,r5 */
 goto L8c65af86;
L8c65af86: *sr=(*sr&~1u)|(r[3]==r[5]); /* cmp/eq r3,r5 */
 goto L8c65af88;
L8c65af88: if((*sr&1u))goto L8c65af8e; /* bt 0x8c65af8e */
 goto L8c65af8a;
L8c65af8a: r[0]=4294967294u;return 0; /* rts */
L8c65af8e: r[0]=sx(m,r[4]+0x2u,2); /* mov.w @(0x2,r4),r0 */
 goto L8c65af90;
L8c65af90: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c65af92;
L8c65af92: r[1]=r[0]; /* mov r0,r1 */
 goto L8c65af94;
L8c65af94: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65af96;
L8c65af96: r[1]<<=8; /* shll8 r1 */
 goto L8c65af98;
L8c65af98: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65af9a;
L8c65af9a: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c65af9c;
L8c65af9c: r[1]&=r[7]; /* and r7,r1 */
 goto L8c65af9e;
L8c65af9e: r[5]=r[3]&255u; /* extu.b r3,r5 */
 goto L8c65afa0;
L8c65afa0: r[3]=14u; /* mov #0xe,r3 */
 goto L8c65afa2;
L8c65afa2: r[5]|=r[1]; /* or r1,r5 */
 goto L8c65afa4;
L8c65afa4: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c65afa6;
L8c65afa6: if((*sr&1u))goto L8c65afac; /* bt 0x8c65afac */
 goto L8c65afa8;
L8c65afa8: r[0]=4294967295u;return 0; /* rts */
L8c65afac: r[0]=sx(m,r[4]+0x10u,2); /* mov.w @(0x10,r4),r0 */
 goto L8c65afae;
L8c65afae: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c65afb0;
L8c65afb0: r[5]=r[0]&65535u; /* extu.w r0,r5 */
 goto L8c65afb2;
L8c65afb2: r[3]=r[5]; /* mov r5,r3 */
 goto L8c65afb4;
L8c65afb4: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c65afb6;
L8c65afb6: r[2]=sx(m,0x8c65b04eu,2); /* mov.w 0x8c65b04e,r2 */
 goto L8c65afb8;
L8c65afb8: r[1]=r[5]; /* mov r5,r1 */
 goto L8c65afba;
L8c65afba: r[1]<<=8; /* shll8 r1 */
 goto L8c65afbc;
L8c65afbc: r[1]&=r[2]; /* and r2,r1 */
 goto L8c65afbe;
L8c65afbe: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c65afc0;
L8c65afc0: r[3]|=r[1]; /* or r1,r3 */
 goto L8c65afc2;
L8c65afc2: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65afc4;
L8c65afc4: wr(m,r[6],r[3],2); /* mov.w r3,@r6 */
 goto L8c65afc6;
L8c65afc6: return 0; /* rts */
}
