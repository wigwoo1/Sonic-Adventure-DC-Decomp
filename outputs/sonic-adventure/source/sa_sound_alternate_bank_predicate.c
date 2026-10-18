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
/* Static C recovery of retail Tikal helper 8c095612..8c0956be.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_alternate_bank_predicate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c095612;
L8c095612: r[3]=rd(m,0x8c0956f0u,4); /* mov.l 0x8c0956f0,r3 */
 goto L8c095614;
L8c095614: r[5]=rd(m,r[3],4); /* mov.l @r3,r5 */
 goto L8c095616;
L8c095616: *sr=(*sr&~1u)|((int32_t)r[5]>=0); /* cmp/pz r5 */
 goto L8c095618;
L8c095618: if(!(*sr&1u))goto L8c0956b8; /* bf 0x8c0956b8 */
 goto L8c09561a;
L8c09561a: r[3]=rd(m,0x8c0956f4u,4); /* mov.l 0x8c0956f4,r3 */
 goto L8c09561c;
L8c09561c: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c09561e;
L8c09561e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c095620;
L8c095620: if((*sr&1u))goto L8c0956b8; /* bt 0x8c0956b8 */
 goto L8c095622;
L8c095622: r[0]=r[5]; /* mov r5,r0 */
 goto L8c095624;
L8c095624: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c095626;
L8c095626: if((*sr&1u))goto L8c095634; /* bt 0x8c095634 */
 goto L8c095628;
L8c095628: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c09562a;
L8c09562a: if((*sr&1u))goto L8c095660; /* bt 0x8c095660 */
 goto L8c09562c;
L8c09562c: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c09562e;
L8c09562e: if((*sr&1u))goto L8c09568c; /* bt 0x8c09568c */
 goto L8c095630;
L8c095630: goto L8c0956b8; /* bra 0x8c0956b8 */
 goto L8c095634;
L8c095634: r[2]=3u; /* mov #0x3,r2 */
 goto L8c095636;
L8c095636: r[2]<<=8; /* shll8 r2 */
 goto L8c095638;
L8c095638: r[2]+=4294967250u; /* add #-0x2e,r2 */
 goto L8c09563a;
L8c09563a: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c09563c;
L8c09563c: if(!(*sr&1u))goto L8c095648; /* bf 0x8c095648 */
 goto L8c09563e;
L8c09563e: r[0]=3u; /* mov #0x3,r0 */
 goto L8c095640;
L8c095640: r[0]<<=8; /* shll8 r0 */
 goto L8c095642;
L8c095642: r[0]+=4294967264u; /* add #-0x20,r0 */
 goto L8c095644;
L8c095644: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[0]); /* cmp/ge r0,r4 */
 goto L8c095646;
L8c095646: if(!(*sr&1u))goto L8c0956b4; /* bf 0x8c0956b4 */
 goto L8c095648;
L8c095648: r[2]=4u; /* mov #0x4,r2 */
 goto L8c09564a;
L8c09564a: r[2]<<=8; /* shll8 r2 */
 goto L8c09564c;
L8c09564c: r[2]+=87u; /* add #0x57,r2 */
 goto L8c09564e;
L8c09564e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c095650;
L8c095650: if(!(*sr&1u))goto L8c0956b8; /* bf 0x8c0956b8 */
 goto L8c095652;
L8c095652: r[1]=4u; /* mov #0x4,r1 */
 goto L8c095654;
L8c095654: r[1]<<=8; /* shll8 r1 */
 goto L8c095656;
L8c095656: r[1]+=120u; /* add #0x78,r1 */
 goto L8c095658;
L8c095658: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[1]); /* cmp/ge r1,r4 */
 goto L8c09565a;
L8c09565a: if((*sr&1u))goto L8c0956b8; /* bt 0x8c0956b8 */
 goto L8c09565c;
L8c09565c: goto L8c0956b4; /* bra 0x8c0956b4 */
 goto L8c095660;
L8c095660: r[1]=3u; /* mov #0x3,r1 */
 goto L8c095662;
L8c095662: r[1]<<=8; /* shll8 r1 */
 goto L8c095664;
L8c095664: r[1]+=4294967264u; /* add #-0x20,r1 */
 goto L8c095666;
L8c095666: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]); /* cmp/gt r1,r4 */
 goto L8c095668;
L8c095668: if(!(*sr&1u))goto L8c095674; /* bf 0x8c095674 */
 goto L8c09566a;
L8c09566a: r[0]=3u; /* mov #0x3,r0 */
 goto L8c09566c;
L8c09566c: r[0]<<=8; /* shll8 r0 */
 goto L8c09566e;
L8c09566e: r[0]+=4294967279u; /* add #-0x11,r0 */
 goto L8c095670;
L8c095670: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[0]); /* cmp/ge r0,r4 */
 goto L8c095672;
L8c095672: if(!(*sr&1u))goto L8c0956b4; /* bf 0x8c0956b4 */
 goto L8c095674;
L8c095674: r[2]=4u; /* mov #0x4,r2 */
 goto L8c095676;
L8c095676: r[2]<<=8; /* shll8 r2 */
 goto L8c095678;
L8c095678: r[2]+=120u; /* add #0x78,r2 */
 goto L8c09567a;
L8c09567a: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c09567c;
L8c09567c: if(!(*sr&1u))goto L8c0956b8; /* bf 0x8c0956b8 */
 goto L8c09567e;
L8c09567e: r[1]=5u; /* mov #0x5,r1 */
 goto L8c095680;
L8c095680: r[1]<<=8; /* shll8 r1 */
 goto L8c095682;
L8c095682: r[1]+=4294967184u; /* add #-0x70,r1 */
 goto L8c095684;
L8c095684: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[1]); /* cmp/ge r1,r4 */
 goto L8c095686;
L8c095686: if((*sr&1u))goto L8c0956b8; /* bt 0x8c0956b8 */
 goto L8c095688;
L8c095688: goto L8c0956b4; /* bra 0x8c0956b4 */
 goto L8c09568c;
L8c09568c: r[1]=3u; /* mov #0x3,r1 */
 goto L8c09568e;
L8c09568e: r[1]<<=8; /* shll8 r1 */
 goto L8c095690;
L8c095690: r[1]+=8u; /* add #0x8,r1 */
 goto L8c095692;
L8c095692: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]); /* cmp/gt r1,r4 */
 goto L8c095694;
L8c095694: if(!(*sr&1u))goto L8c0956a0; /* bf 0x8c0956a0 */
 goto L8c095696;
L8c095696: r[0]=3u; /* mov #0x3,r0 */
 goto L8c095698;
L8c095698: r[0]<<=8; /* shll8 r0 */
 goto L8c09569a;
L8c09569a: r[0]+=27u; /* add #0x1b,r0 */
 goto L8c09569c;
L8c09569c: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[0]); /* cmp/ge r0,r4 */
 goto L8c09569e;
L8c09569e: if(!(*sr&1u))goto L8c0956b4; /* bf 0x8c0956b4 */
 goto L8c0956a0;
L8c0956a0: r[2]=5u; /* mov #0x5,r2 */
 goto L8c0956a2;
L8c0956a2: r[2]<<=8; /* shll8 r2 */
 goto L8c0956a4;
L8c0956a4: r[2]+=4294967202u; /* add #-0x5e,r2 */
 goto L8c0956a6;
L8c0956a6: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c0956a8;
L8c0956a8: if(!(*sr&1u))goto L8c0956b8; /* bf 0x8c0956b8 */
 goto L8c0956aa;
L8c0956aa: r[1]=5u; /* mov #0x5,r1 */
 goto L8c0956ac;
L8c0956ac: r[1]<<=8; /* shll8 r1 */
 goto L8c0956ae;
L8c0956ae: r[1]+=4294967215u; /* add #-0x51,r1 */
 goto L8c0956b0;
L8c0956b0: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[1]); /* cmp/ge r1,r4 */
 goto L8c0956b2;
L8c0956b2: if((*sr&1u))goto L8c0956b8; /* bt 0x8c0956b8 */
 goto L8c0956b4;
L8c0956b4: r[0]=1u;return 0; /* rts */
L8c0956b8: r[0]=0u; /* mov #0x0,r0 */
 goto L8c0956ba;
L8c0956ba: return 0; /* rts */
}
