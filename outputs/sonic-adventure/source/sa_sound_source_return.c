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
/* Retail source return65e644..65e712, explicit SH4 ABI.
 * Rolls back matching ring reservations; scope: sound_source_return_verification.json. */
SA_STAGE_EXPORT int sa_sound_source_return(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e644;
L8c65e644: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e646;
L8c65e646: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e648;
L8c65e648: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e64a;
L8c65e64a: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65e64c;
L8c65e64c: r[13]=r[6]; /* mov r6,r13 */
 goto L8c65e64e;
L8c65e64e: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e650;
L8c65e650: *pr=0x8c65e654u;r[14]=r[4];status=call(ctx,0x8c65e368u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e368 */
 goto L8c65e654;
L8c65e654: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c65e656;
L8c65e656: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e658;
L8c65e658: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e65a;
L8c65e65a: if(!(*sr&1u))goto L8c65e69e; /* bf 0x8c65e69e */
 goto L8c65e65c;
L8c65e65c: r[1]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r1 */
 goto L8c65e65e;
L8c65e65e: r[5]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r5 */
 goto L8c65e660;
L8c65e660: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e662;
L8c65e662: r[1]+=r[5]; /* add r5,r1 */
 goto L8c65e664;
L8c65e664: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c65e666;
L8c65e666: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65e668;
L8c65e668:  /* nop */
 goto L8c65e66a;
L8c65e66a: r[2]=rd(m,0x8c65e6e8u,4); /* mov.l 0x8c65e6e8,r2 */
 goto L8c65e66c;
L8c65e66c: target=r[2];*pr=0x8c65e670u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e670;
L8c65e670: r[1]=rd(m,r[13],4); /* mov.l @r13,r1 */
 goto L8c65e672;
L8c65e672: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65e674;
L8c65e674: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e676;
L8c65e676: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c65e678;
L8c65e678: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65e67a;
L8c65e67a:  /* nop */
 goto L8c65e67c;
L8c65e67c: r[2]=rd(m,0x8c65e6e8u,4); /* mov.l 0x8c65e6e8,r2 */
 goto L8c65e67e;
L8c65e67e: target=r[2];*pr=0x8c65e682u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e682;
L8c65e682: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65e684;
L8c65e684: *sr=(*sr&~1u)|(r[5]==r[4]); /* cmp/eq r5,r4 */
 goto L8c65e686;
L8c65e686: if(!(*sr&1u))goto L8c65e694; /* bf 0x8c65e694 */
 goto L8c65e688;
L8c65e688: wr(m,r[14]+0x14u,r[4],4); /* mov.l r4,@(0x14,r14) */
 goto L8c65e68a;
L8c65e68a: r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c65e68c;
L8c65e68c: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e68e;
L8c65e68e: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e690;
L8c65e690: wr(m,r[14]+0x10u,r[2],4);goto L8c65e704; /* bra 0x8c65e704 */
 goto L8c65e694;
L8c65e694: r[0]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r0 */
 goto L8c65e696;
L8c65e696: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65e698;
L8c65e698: if(!(*sr&1u))goto L8c65e6fc; /* bf 0x8c65e6fc */
 goto L8c65e69a;
L8c65e69a: goto L8c65e704; /* bra 0x8c65e704 */
 goto L8c65e69e;
L8c65e69e: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e6a0;
L8c65e6a0: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e6a2;
L8c65e6a2: if(!(*sr&1u))goto L8c65e6f0; /* bf 0x8c65e6f0 */
 goto L8c65e6a4;
L8c65e6a4: r[1]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r1 */
 goto L8c65e6a6;
L8c65e6a6: r[5]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r5 */
 goto L8c65e6a8;
L8c65e6a8: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e6aa;
L8c65e6aa: r[1]+=r[5]; /* add r5,r1 */
 goto L8c65e6ac;
L8c65e6ac: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c65e6ae;
L8c65e6ae: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65e6b0;
L8c65e6b0:  /* nop */
 goto L8c65e6b2;
L8c65e6b2: r[2]=rd(m,0x8c65e6e8u,4); /* mov.l 0x8c65e6e8,r2 */
 goto L8c65e6b4;
L8c65e6b4: target=r[2];*pr=0x8c65e6b8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e6b8;
L8c65e6b8: r[1]=rd(m,r[13],4); /* mov.l @r13,r1 */
 goto L8c65e6ba;
L8c65e6ba: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65e6bc;
L8c65e6bc: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e6be;
L8c65e6be: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c65e6c0;
L8c65e6c0: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65e6c2;
L8c65e6c2:  /* nop */
 goto L8c65e6c4;
L8c65e6c4: r[2]=rd(m,0x8c65e6e8u,4); /* mov.l 0x8c65e6e8,r2 */
 goto L8c65e6c6;
L8c65e6c6: target=r[2];*pr=0x8c65e6cau;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e6ca;
L8c65e6ca: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65e6cc;
L8c65e6cc: *sr=(*sr&~1u)|(r[5]==r[4]); /* cmp/eq r5,r4 */
 goto L8c65e6ce;
L8c65e6ce: if(!(*sr&1u))goto L8c65e6dc; /* bf 0x8c65e6dc */
 goto L8c65e6d0;
L8c65e6d0: wr(m,r[14]+0x18u,r[4],4); /* mov.l r4,@(0x18,r14) */
 goto L8c65e6d2;
L8c65e6d2: r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c65e6d4;
L8c65e6d4: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e6d6;
L8c65e6d6: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e6d8;
L8c65e6d8: wr(m,r[14]+0xcu,r[2],4);goto L8c65e704; /* bra 0x8c65e704 */
 goto L8c65e6dc;
L8c65e6dc: r[0]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r0 */
 goto L8c65e6de;
L8c65e6de: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65e6e0;
L8c65e6e0: if((*sr&1u))goto L8c65e704; /* bt 0x8c65e704 */
 goto L8c65e6e2;
L8c65e6e2: goto L8c65e6fc; /* bra 0x8c65e6fc */
 goto L8c65e6f0;
L8c65e6f0: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65e6f2;
L8c65e6f2: wr(m,r[13]+0x4u,r[4],4); /* mov.l r4,@(0x4,r13) */
 goto L8c65e6f4;
L8c65e6f4: wr(m,r[13],r[4],4); /* mov.l r4,@r13 */
 goto L8c65e6f6;
L8c65e6f6: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e6f8;
L8c65e6f8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e6fa;
L8c65e6fa: if((*sr&1u))goto L8c65e704; /* bt 0x8c65e704 */
 goto L8c65e6fc;
L8c65e6fc: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e6fe;
L8c65e6fe: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e700;
L8c65e700: target=r[3];*pr=0x8c65e704u;r[4]=rd(m,r[14]+0x2cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e704;
L8c65e704: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65e706;
L8c65e706: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65e708;
L8c65e708: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65e70a;
L8c65e70a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e70c;
L8c65e70c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e70e;
L8c65e70e: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65e380u,m,f,sr,r,pr); /* bra 0x8c65e380 */
}
