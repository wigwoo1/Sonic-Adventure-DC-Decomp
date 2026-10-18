#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 8c08a120..8c08a178.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_counter_award(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c08a120;
L8c08a120: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c08a122;
L8c08a122: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c08a124;
L8c08a124: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c08a126;
L8c08a126: wr(m,r[15],r[4],2); /* mov.w r4,@r15 */
 goto L8c08a128;
L8c08a128: r[14]=sx(m,r[15],2); /* mov.w @r15,r14 */
 goto L8c08a12a;
L8c08a12a: *sr=(*sr&~1u)|((int32_t)r[14]>0); /* cmp/pl r14 */
 goto L8c08a12c;
L8c08a12c: if(!(*sr&1u))goto L8c08a13e; /* bf 0x8c08a13e */
 goto L8c08a12e;
L8c08a12e: r[4]=3u; /* mov #0x3,r4 */
 goto L8c08a130;
L8c08a130: r[2]=rd(m,0x8c08a220u,4); /* mov.l 0x8c08a220,r2 */
 goto L8c08a132;
L8c08a132: r[4]<<=8; /* shll8 r4 */
 goto L8c08a134;
L8c08a134: r[7]=0u; /* mov #0x0,r7 */
 goto L8c08a136;
L8c08a136: r[6]=r[7]; /* mov r7,r6 */
 goto L8c08a138;
L8c08a138: r[4]+=4294967232u; /* add #-0x40,r4 */
 goto L8c08a13a;
L8c08a13a: target=r[2];*pr=0x8c08a13eu;r[5]=r[7];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c08a13e;
L8c08a13e: r[4]=rd(m,0x8c08a224u,4); /* mov.l 0x8c08a224,r4 */
 goto L8c08a140;
L8c08a140: r[3]=sx(m,r[15],1); /* mov.b @r15,r3 */
 goto L8c08a142;
L8c08a142: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c08a144;
L8c08a144: r[2]+=r[3]; /* add r3,r2 */
 goto L8c08a146;
L8c08a146: wr(m,r[4],r[2],1); /* mov.b r2,@r4 */
 goto L8c08a148;
L8c08a148: r[1]=sx(m,r[4],1); /* mov.b @r4,r1 */
 goto L8c08a14a;
L8c08a14a: *sr=(*sr&~1u)|((int32_t)r[1]>=0); /* cmp/pz r1 */
 goto L8c08a14c;
L8c08a14c: if((*sr&1u))goto L8c08a156; /* bt 0x8c08a156 */
 goto L8c08a14e;
L8c08a14e: *sr=(*sr&~1u)|((int32_t)r[14]>0); /* cmp/pl r14 */
 goto L8c08a150;
L8c08a150: if(!(*sr&1u))goto L8c08a156; /* bf 0x8c08a156 */
 goto L8c08a152;
L8c08a152: r[3]=127u; /* mov #0x7f,r3 */
 goto L8c08a154;
L8c08a154: wr(m,r[4],r[3],1); /* mov.b r3,@r4 */
 goto L8c08a156;
L8c08a156: r[2]=rd(m,0x8c08a228u,4); /* mov.l 0x8c08a228,r2 */
 goto L8c08a158;
L8c08a158: target=r[2];*pr=0x8c08a15cu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c08a15c;
L8c08a15c: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c08a15e;
L8c08a15e: if(!(*sr&1u))goto L8c08a170; /* bf 0x8c08a170 */
 goto L8c08a160;
L8c08a160: r[6]=rd(m,0x8c08a22cu,4); /* mov.l 0x8c08a22c,r6 */
 goto L8c08a162;
L8c08a162: r[4]=8u; /* mov #0x8,r4 */
 goto L8c08a164;
L8c08a164: r[5]=6u; /* mov #0x6,r5 */
 goto L8c08a166;
L8c08a166: r[15]+=4u; /* add #0x4,r15 */
 goto L8c08a168;
L8c08a168: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c08a16a;
L8c08a16a: r[2]=rd(m,0x8c08a230u,4); /* mov.l 0x8c08a230,r2 */
 goto L8c08a16c;
L8c08a16c: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c08a170: r[15]+=4u; /* add #0x4,r15 */
 goto L8c08a172;
L8c08a172: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c08a174;
L8c08a174: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
