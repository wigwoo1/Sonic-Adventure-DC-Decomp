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
/* Static C recovery of USA Rev A direct queue consumer. Subordinate services use the supplied dispatcher. */
SA_STAGE_EXPORT int sa_engine_queue_consume_direct(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a5dc;
L8c65a5dc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a5de;
L8c65a5de: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65a5e0;
L8c65a5e0: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65a5e2;
L8c65a5e2: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65a5e4;
L8c65a5e4: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65a5e6;
L8c65a5e6: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65a5e8;
L8c65a5e8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a5ea;
L8c65a5ea: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c65a5ec;
L8c65a5ec: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a5ee;
L8c65a5ee: r[3]=sx(m,0x8c65a6b2u,2); /* mov.w 0x8c65a6b2,r3 */
 goto L8c65a5f0;
L8c65a5f0: r[9]=0u; /* mov #0x0,r9 */
 goto L8c65a5f2;
L8c65a5f2: r[11]=r[4]; /* mov r4,r11 */
 goto L8c65a5f4;
L8c65a5f4: r[13]=r[11]; /* mov r11,r13 */
 goto L8c65a5f6;
L8c65a5f6: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a5f8;
L8c65a5f8: wr(m,r[15]+0xcu,r[5],4); /* mov.l r5,@(0xc,r15) */
 goto L8c65a5fa;
L8c65a5fa: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a5fc;
L8c65a5fc: r[14]=r[9]; /* mov r9,r14 */
 goto L8c65a5fe;
L8c65a5fe: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a600;
L8c65a600: r[12]=r[0]; /* mov r0,r12 */
 goto L8c65a602;
L8c65a602: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a604;
L8c65a604: r[0]&=r[3]; /* and r3,r0 */
 goto L8c65a606;
L8c65a606: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c65a608;
L8c65a608: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a60a;
L8c65a60a: r[10]=rd(m,0x8c65a6b8u,4); /* mov.l 0x8c65a6b8,r10 */
 goto L8c65a60c;
L8c65a60c: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c65a60e;
L8c65a60e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65a610;
L8c65a610: if(!(*sr&1u))goto L8c65a618; /* bf 0x8c65a618 */
 goto L8c65a612;
L8c65a612: r[14]=rd(m,0x8c65a6bcu,4); /* mov.l 0x8c65a6bc,r14 */
 goto L8c65a614;
L8c65a614: goto L8c65a6ee; /* bra 0x8c65a6ee */
 goto L8c65a618;
L8c65a618: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c65a61a;
L8c65a61a: if(!(*sr&1u))goto L8c65a622; /* bf 0x8c65a622 */
 goto L8c65a61c;
L8c65a61c: r[14]=rd(m,0x8c65a6c0u,4); /* mov.l 0x8c65a6c0,r14 */
 goto L8c65a61e;
L8c65a61e: goto L8c65a6ee; /* bra 0x8c65a6ee */
 goto L8c65a622;
L8c65a622: r[3]=rd(m,0x8c65a6c4u,4); /* mov.l 0x8c65a6c4,r3 */
 goto L8c65a624;
L8c65a624: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65a626;
L8c65a626: r[5]+=8u; /* add #0x8,r5 */
 goto L8c65a628;
L8c65a628: target=r[3];*pr=0x8c65a62cu;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a62c;
L8c65a62c: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c65a62e;
L8c65a62e: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c65a630;
L8c65a630: if(!(*sr&1u))goto L8c65a638; /* bf 0x8c65a638 */
 goto L8c65a632;
L8c65a632: r[14]=rd(m,0x8c65a6c8u,4); /* mov.l 0x8c65a6c8,r14 */
 goto L8c65a634;
L8c65a634: goto L8c65a6ee; /* bra 0x8c65a6ee */
 goto L8c65a638;
L8c65a638: r[1]=sx(m,0x8c65a6b4u,2); /* mov.w 0x8c65a6b4,r1 */
 goto L8c65a63a;
L8c65a63a: r[2]=rd(m,0x8c65a6ccu,4); /* mov.l 0x8c65a6cc,r2 */
 goto L8c65a63c;
L8c65a63c: wr(m,r[2],r[9],4); /* mov.l r9,@r2 */
 goto L8c65a63e;
L8c65a63e: wr(m,r[13]+0x18u,r[9],4); /* mov.l r9,@(0x18,r13) */
 goto L8c65a640;
L8c65a640: r[0]=rd(m,r[10],4); /* mov.l @r10,r0 */
 goto L8c65a642;
L8c65a642: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65a644;
L8c65a644: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c65a646;
L8c65a646: if((*sr&1u))goto L8c65a650; /* bt 0x8c65a650 */
 goto L8c65a648;
L8c65a648: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65a64a;
L8c65a64a: if((*sr&1u))goto L8c65a662; /* bt 0x8c65a662 */
 goto L8c65a64c;
L8c65a64c: goto L8c65a6ee; /* bra 0x8c65a6ee */
 goto L8c65a650;
L8c65a650: r[3]=rd(m,0x8c65a6d0u,4); /* mov.l 0x8c65a6d0,r3 */
 goto L8c65a652;
L8c65a652: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c65a654;
L8c65a654: target=r[3];*pr=0x8c65a658u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a658;
L8c65a658: r[14]=r[0]; /* mov r0,r14 */
 goto L8c65a65a;
L8c65a65a: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a65c;
L8c65a65c: if((*sr&1u))goto L8c65a6fa; /* bt 0x8c65a6fa */
 goto L8c65a65e;
L8c65a65e: goto L8c65a6ee; /* bra 0x8c65a6ee */
 goto L8c65a662;
L8c65a662: r[3]=rd(m,0x8c65a6d4u,4); /* mov.l 0x8c65a6d4,r3 */
 goto L8c65a664;
L8c65a664: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c65a666;
L8c65a666: target=r[3];*pr=0x8c65a66au;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a66a;
L8c65a66a: r[14]=r[0]; /* mov r0,r14 */
 goto L8c65a66c;
L8c65a66c: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a66e;
L8c65a66e: if(!(*sr&1u))goto L8c65a6ee; /* bf 0x8c65a6ee */
 goto L8c65a670;
L8c65a670: r[0]=sx(m,0x8c65a6b6u,2); /* mov.w 0x8c65a6b6,r0 */
 goto L8c65a672;
L8c65a672: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c65a674;
L8c65a674: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65a676;
L8c65a676: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65a678;
L8c65a678: if(!(*sr&1u))goto L8c65a6ee; /* bf 0x8c65a6ee */
 goto L8c65a67a;
L8c65a67a: r[2]=rd(m,0x8c65a6d8u,4); /* mov.l 0x8c65a6d8,r2 */
 goto L8c65a67c;
L8c65a67c: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65a67e;
L8c65a67e: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65a680;
L8c65a680: target=r[2];*pr=0x8c65a684u;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a684;
L8c65a684: r[3]=rd(m,0x8c65a6dcu,4); /* mov.l 0x8c65a6dc,r3 */
 goto L8c65a686;
L8c65a686: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65a688;
L8c65a688: target=r[3];*pr=0x8c65a68cu;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a68c;
L8c65a68c: r[14]=r[0]; /* mov r0,r14 */
 goto L8c65a68e;
L8c65a68e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a690;
L8c65a690: if(!(*sr&1u))goto L8c65a6ee; /* bf 0x8c65a6ee */
 goto L8c65a692;
L8c65a692: r[2]=*sr; /* stc SR,r2 */
 goto L8c65a694;
L8c65a694: r[0]=r[12]; /* mov r12,r0 */
 goto L8c65a696;
L8c65a696: r[3]=sx(m,0x8c65a6b2u,2); /* mov.w 0x8c65a6b2,r3 */
 goto L8c65a698;
L8c65a698: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a69a;
L8c65a69a: r[0]<<=2; /* shll2 r0 */
 goto L8c65a69c;
L8c65a69c: r[2]&=r[3]; /* and r3,r2 */
 goto L8c65a69e;
L8c65a69e: r[0]<<=2; /* shll2 r0 */
 goto L8c65a6a0;
L8c65a6a0: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65a6a2;
L8c65a6a2: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a6a4;
L8c65a6a4: r[0]=rd(m,r[13]+0x10u,4); /* mov.l @(0x10,r13),r0 */
 goto L8c65a6a6;
L8c65a6a6: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c65a6a8;
L8c65a6a8: take=!(*sr&1u);r[12]=4294967295u;if(take)goto L8c65a6ee;goto L8c65a6ac; /* bf/s 0x8c65a6ee */
 goto L8c65a6ac;
L8c65a6ac: r[11]=rd(m,0x8c65a6e0u,4); /* mov.l 0x8c65a6e0,r11 */
 goto L8c65a6ae;
L8c65a6ae: goto L8c65a6e8; /* bra 0x8c65a6e8 */
 goto L8c65a6e4;
L8c65a6e4: target=r[11];*pr=0x8c65a6e8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65a6e8;
L8c65a6e8: r[0]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r0 */
 goto L8c65a6ea;
L8c65a6ea: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65a6ec;
L8c65a6ec: if(!(*sr&1u))goto L8c65a6e4; /* bf 0x8c65a6e4 */
 goto L8c65a6ee;
L8c65a6ee: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a6f0;
L8c65a6f0: if((*sr&1u))goto L8c65a6fa; /* bt 0x8c65a6fa */
 goto L8c65a6f2;
L8c65a6f2: r[3]=rd(m,0x8c65a72cu,4); /* mov.l 0x8c65a72c,r3 */
 goto L8c65a6f4;
L8c65a6f4: r[4]=rd(m,0x8c65a728u,4); /* mov.l 0x8c65a728,r4 */
 goto L8c65a6f6;
L8c65a6f6: target=r[3];*pr=0x8c65a6fau;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a6fa;
L8c65a6fa: *sr=(*sr&~1u)|((int32_t)r[12]>=0); /* cmp/pz r12 */
 goto L8c65a6fc;
L8c65a6fc: if(!(*sr&1u))goto L8c65a710; /* bf 0x8c65a710 */
 goto L8c65a6fe;
L8c65a6fe: r[3]=*sr; /* stc SR,r3 */
 goto L8c65a700;
L8c65a700: r[0]=r[12]; /* mov r12,r0 */
 goto L8c65a702;
L8c65a702: r[2]=sx(m,0x8c65a724u,2); /* mov.w 0x8c65a724,r2 */
 goto L8c65a704;
L8c65a704: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a706;
L8c65a706: r[0]<<=2; /* shll2 r0 */
 goto L8c65a708;
L8c65a708: r[3]&=r[2]; /* and r2,r3 */
 goto L8c65a70a;
L8c65a70a: r[0]<<=2; /* shll2 r0 */
 goto L8c65a70c;
L8c65a70c: r[0]|=r[3]; /* or r3,r0 */
 goto L8c65a70e;
L8c65a70e: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a710;
L8c65a710: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65a712;
L8c65a712: r[15]+=16u; /* add #0x10,r15 */
 goto L8c65a714;
L8c65a714: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a716;
L8c65a716: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65a718;
L8c65a718: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65a71a;
L8c65a71a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65a71c;
L8c65a71c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65a71e;
L8c65a71e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65a720;
L8c65a720: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
