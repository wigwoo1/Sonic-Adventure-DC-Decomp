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
/* Static C recovery of retail Tikal helper 8c10e7b8..8c10e856.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_atan_scaling(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e7b8;
L8c10e7b8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10e7ba;
L8c10e7ba: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c10e7bc;
L8c10e7bc: r[6]=r[15]; /* mov r15,r6 */
 goto L8c10e7be;
L8c10e7be: r[2]=rd(m,0x8c10e860u,4); /* mov.l 0x8c10e860,r2 */
 goto L8c10e7c0;
L8c10e7c0: r[3]=rd(m,0x8c10e858u,4); /* mov.l 0x8c10e858,r3 */
 goto L8c10e7c2;
L8c10e7c2: r[1]=rd(m,0x8c10e864u,4); /* mov.l 0x8c10e864,r1 */
 goto L8c10e7c4;
L8c10e7c4: r[5]=rd(m,0x8c10e85cu,4); /* mov.l 0x8c10e85c,r5 */
 goto L8c10e7c6;
L8c10e7c6: wr(m,r[6],fr[4],4); /* fmov.s fr4,@r6 */
 goto L8c10e7c8;
L8c10e7c8: r[7]=rd(m,r[6],4); /* mov.l @r6,r7 */
 goto L8c10e7ca;
L8c10e7ca: fr[7]=rd(m,r[2],4); /* fmov.s @r2,fr7 */
 goto L8c10e7cc;
L8c10e7cc: r[14]=rd(m,r[6],4); /* mov.l @r6,r14 */
 goto L8c10e7ce;
L8c10e7ce: r[7]&=r[3]; /* and r3,r7 */
 goto L8c10e7d0;
L8c10e7d0: *sr=(*sr&~1u)|(r[3]==r[7]); /* cmp/eq r3,r7 */
 goto L8c10e7d2;
L8c10e7d2: take=!(*sr&1u);fr[5]=rd(m,r[1],4);if(take)goto L8c10e7ee;goto L8c10e7d6; /* bf/s 0x8c10e7ee */
 goto L8c10e7d6;
L8c10e7d6: r[0]=rd(m,0x8c10e868u,4); /* mov.l 0x8c10e868,r0 */
 goto L8c10e7d8;
L8c10e7d8: *sr=(*sr&~1u)|!(r[14]&r[0]); /* tst r14,r0 */
 goto L8c10e7da;
L8c10e7da: if(!(*sr&1u))goto L8c10e7e4; /* bf 0x8c10e7e4 */
 goto L8c10e7dc;
L8c10e7dc: *sr=(*sr&~1u)|!(r[14]&r[5]); /* tst r14,r5 */
 goto L8c10e7de;
L8c10e7de: if(!(*sr&1u))goto L8c10e822; /* bf 0x8c10e822 */
 goto L8c10e7e0;
L8c10e7e0: goto L8c10e81a; /* bra 0x8c10e81a */
 goto L8c10e7e4;
L8c10e7e4: r[3]=rd(m,0x8c10e86cu,4); /* mov.l 0x8c10e86c,r3 */
 goto L8c10e7e6;
L8c10e7e6: fr[0]=rd(m,r[3],4); /* fmov.s @r3,fr0 */
 goto L8c10e7e8;
L8c10e7e8: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10e7ea;
L8c10e7ea: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10e7ee: r[2]=rd(m,0x8c10e870u,4); /* mov.l 0x8c10e870,r2 */
 goto L8c10e7f0;
L8c10e7f0: *sr=(*sr&~1u)|((int32_t)r[7]>0); /* cmp/pl r7 */
 goto L8c10e7f2;
L8c10e7f2: r[3]=rd(m,0x8c10e874u,4); /* mov.l 0x8c10e874,r3 */
 goto L8c10e7f4;
L8c10e7f4: fr[4]=rd(m,r[2],4); /* fmov.s @r2,fr4 */
 goto L8c10e7f6;
L8c10e7f6: take=(*sr&1u);fr[6]=rd(m,r[3],4);if(take)goto L8c10e802;goto L8c10e7fa; /* bt/s 0x8c10e802 */
 goto L8c10e7fa;
L8c10e7fa: *sr=(*sr&~1u)|!(r[14]&r[5]); /* tst r14,r5 */
 goto L8c10e7fc;
L8c10e7fc: if(!(*sr&1u))goto L8c10e836; /* bf 0x8c10e836 */
 goto L8c10e7fe;
L8c10e7fe: goto L8c10e82e; /* bra 0x8c10e82e */
 goto L8c10e802;
L8c10e802: r[3]=4294967273u; /* mov #-0x17,r3 */
 goto L8c10e804;
L8c10e804: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[7]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[7]=n?(uint32_t)((int32_t)r[7]>>n):((r[7]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r7 */
 goto L8c10e806;
L8c10e806: r[7]+=r[4]; /* add r4,r7 */
 goto L8c10e808;
L8c10e808: *sr=(*sr&~1u)|((int32_t)r[7]>0); /* cmp/pl r7 */
 goto L8c10e80a;
L8c10e80a: if(!(*sr&1u))goto L8c10e812; /* bf 0x8c10e812 */
 goto L8c10e80c;
L8c10e80c: r[3]=sx(m,0x8c10e856u,2); /* mov.w 0x8c10e856,r3 */
 goto L8c10e80e;
L8c10e80e: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]); /* cmp/ge r3,r7 */
 goto L8c10e810;
L8c10e810: if(!(*sr&1u))goto L8c10e83e; /* bf 0x8c10e83e */
 goto L8c10e812;
L8c10e812: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c10e814;
L8c10e814: if(!(*sr&1u))goto L8c10e82a; /* bf 0x8c10e82a */
 goto L8c10e816;
L8c10e816: *sr=(*sr&~1u)|!(r[14]&r[5]); /* tst r14,r5 */
 goto L8c10e818;
L8c10e818: if(!(*sr&1u))goto L8c10e822; /* bf 0x8c10e822 */
 goto L8c10e81a;
L8c10e81a: fr[0]=fr[7]; /* fmov fr7,fr0 */
 goto L8c10e81c;
L8c10e81c: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10e81e;
L8c10e81e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10e822: fr[0]=fr[5]; /* fmov fr5,fr0 */
 goto L8c10e824;
L8c10e824: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10e826;
L8c10e826: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10e82a: *sr=(*sr&~1u)|!(r[14]&r[5]); /* tst r14,r5 */
 goto L8c10e82c;
L8c10e82c: if(!(*sr&1u))goto L8c10e836; /* bf 0x8c10e836 */
 goto L8c10e82e;
L8c10e82e: fr[0]=fr[4]; /* fmov fr4,fr0 */
 goto L8c10e830;
L8c10e830: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10e832;
L8c10e832: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10e836: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c10e838;
L8c10e838: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10e83a;
L8c10e83a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10e83e: r[2]=rd(m,0x8c10e868u,4); /* mov.l 0x8c10e868,r2 */
 goto L8c10e840;
L8c10e840: r[3]=23u; /* mov #0x17,r3 */
 goto L8c10e842;
L8c10e842: r[5]&=r[14]; /* and r14,r5 */
 goto L8c10e844;
L8c10e844: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[7]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[7]=n?(uint32_t)((int32_t)r[7]>>n):((r[7]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r7 */
 goto L8c10e846;
L8c10e846: r[2]&=r[14]; /* and r14,r2 */
 goto L8c10e848;
L8c10e848: r[5]|=r[7]; /* or r7,r5 */
 goto L8c10e84a;
L8c10e84a: r[5]|=r[2]; /* or r2,r5 */
 goto L8c10e84c;
L8c10e84c: wr(m,r[6],r[5],4); /* mov.l r5,@r6 */
 goto L8c10e84e;
L8c10e84e: fr[0]=rd(m,r[6],4); /* fmov.s @r6,fr0 */
 goto L8c10e850;
L8c10e850: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10e852;
L8c10e852: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
