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
/* Retail audio data-chunk metadata, 8c66a63c..8c66a714.
 * Scans aligned words for retail data marker; signed division computes frames.
 * Width4 normalizes to16 with type4 and unsigned frame calculation. */
SA_STAGE_EXPORT int sa_sound_data_chunk_metadata(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a63c;
L8c66a63c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a63e;
L8c66a63e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a640;
L8c66a640: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a642;
L8c66a642: r[14]+=20u; /* add #0x14,r14 */
 goto L8c66a644;
L8c66a644: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a646;
L8c66a646: r[12]=r[5]; /* mov r5,r12 */
 goto L8c66a648;
L8c66a648: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a64a;
L8c66a64a: r[12]>>=2; /* shlr2 r12 */
 goto L8c66a64c;
L8c66a64c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66a64e;
L8c66a64e: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c66a650;
L8c66a650: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c66a652;
L8c66a652: r[11]=r[4]; /* mov r4,r11 */
 goto L8c66a654;
L8c66a654: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c66a656;
L8c66a656: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a658;
L8c66a658: r[8]=rd(m,0x8c66a76cu,4); /* mov.l 0x8c66a76c,r8 */
 goto L8c66a65a;
L8c66a65a: r[9]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r9 */
 goto L8c66a65c;
L8c66a65c: r[13]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r13 */
 goto L8c66a65e;
L8c66a65e: r[10]=rd(m,r[15]+0x38u,4); /* mov.l @(0x38,r15),r10 */
 goto L8c66a660;
L8c66a660: take=!(*sr&1u);r[4]=0u;if(take)goto L8c66a678;goto L8c66a664; /* bf/s 0x8c66a678 */
 goto L8c66a664;
L8c66a664: r[2]=rd(m,r[8],4); /* mov.l @r8,r2 */
 goto L8c66a666;
L8c66a666: r[0]=r[4]; /* mov r4,r0 */
 goto L8c66a668;
L8c66a668: r[0]<<=2; /* shll2 r0 */
 goto L8c66a66a;
L8c66a66a: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c66a66c;
L8c66a66c: r[3]=rd(m,r[11]+r[0],4); /* mov.l @(r0,r11),r3 */
 goto L8c66a66e;
L8c66a66e: *sr=(*sr&~1u)|(r[1]==r[3]); /* cmp/eq r1,r3 */
 goto L8c66a670;
L8c66a670: if((*sr&1u))goto L8c66a678; /* bt 0x8c66a678 */
 goto L8c66a672;
L8c66a672: r[4]+=1u; /* add #0x1,r4 */
 goto L8c66a674;
L8c66a674: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]); /* cmp/ge r12,r4 */
 goto L8c66a676;
L8c66a676: if(!(*sr&1u))goto L8c66a664; /* bf 0x8c66a664 */
 goto L8c66a678;
L8c66a678: r[5]>>=2; /* shlr2 r5 */
 goto L8c66a67a;
L8c66a67a: *sr=(*sr&~1u)|(r[5]==r[4]); /* cmp/eq r5,r4 */
 goto L8c66a67c;
L8c66a67c: if(!(*sr&1u))goto L8c66a682; /* bf 0x8c66a682 */
 goto L8c66a67e;
L8c66a67e: r[0]=4294967295u;goto L8c66a702; /* bra 0x8c66a702 */
 goto L8c66a682;
L8c66a682: r[0]=r[4]; /* mov r4,r0 */
 goto L8c66a684;
L8c66a684: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c66a686;
L8c66a686: r[0]+=1u; /* add #0x1,r0 */
 goto L8c66a688;
L8c66a688: r[0]<<=2; /* shll2 r0 */
 goto L8c66a68a;
L8c66a68a: r[5]=rd(m,r[11]+r[0],4); /* mov.l @(r0,r11),r5 */
 goto L8c66a68c;
L8c66a68c: r[4]+=2u; /* add #0x2,r4 */
 goto L8c66a68e;
L8c66a68e: r[4]<<=2; /* shll2 r4 */
 goto L8c66a690;
L8c66a690: wr(m,r[6],r[4],2); /* mov.w r4,@r6 */
 goto L8c66a692;
L8c66a692: wr(m,r[7],r[3],1); /* mov.b r3,@r7 */
 goto L8c66a694;
L8c66a694: r[2]=rd(m,r[15]+0x2cu,4); /* mov.l @(0x2c,r15),r2 */
 goto L8c66a696;
L8c66a696: r[3]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c66a698;
L8c66a698: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c66a69a;
L8c66a69a: r[0]=sx(m,r[14]+0x2u,2); /* mov.w @(0x2,r14),r0 */
 goto L8c66a69c;
L8c66a69c: r[2]=r[0]; /* mov r0,r2 */
 goto L8c66a69e;
L8c66a69e: wr(m,r[9],r[2],1); /* mov.b r2,@r9 */
 goto L8c66a6a0;
L8c66a6a0: r[0]=sx(m,r[14]+0xeu,2); /* mov.w @(0xe,r14),r0 */
 goto L8c66a6a2;
L8c66a6a2: r[3]=r[0]; /* mov r0,r3 */
 goto L8c66a6a4;
L8c66a6a4: wr(m,r[13],r[3],1); /* mov.b r3,@r13 */
 goto L8c66a6a6;
L8c66a6a6: r[0]=sx(m,r[14]+0xcu,2); /* mov.w @(0xc,r14),r0 */
 goto L8c66a6a8;
L8c66a6a8: r[2]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r2 */
 goto L8c66a6aa;
L8c66a6aa: r[3]=r[0]; /* mov r0,r3 */
 goto L8c66a6ac;
L8c66a6ac: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c66a6ae;
L8c66a6ae: r[0]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r0 */
 goto L8c66a6b0;
L8c66a6b0: r[3]=rd(m,0x8c66a770u,4); /* mov.l 0x8c66a770,r3 */
 goto L8c66a6b2;
L8c66a6b2: r[0]=sx(m,r[0],1); /* mov.b @r0,r0 */
 goto L8c66a6b4;
L8c66a6b4: r[2]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r2 */
 goto L8c66a6b6;
L8c66a6b6: target=r[3];*pr=0x8c66a6bau;r[1]=r[5];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a6ba;
L8c66a6ba: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c66a6bc;
L8c66a6bc: r[3]=1u; /* mov #0x1,r3 */
 goto L8c66a6be;
L8c66a6be: r[2]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r2 */
 goto L8c66a6c0;
L8c66a6c0: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c66a6c2;
L8c66a6c2: r[0]=sx(m,r[13],1); /* mov.b @r13,r0 */
 goto L8c66a6c4;
L8c66a6c4: *sr=(*sr&~1u)|(r[0]==0x00000010u); /* cmp/eq #0x10,r0 */
 goto L8c66a6c6;
L8c66a6c6: if(!(*sr&1u))goto L8c66a6cc; /* bf 0x8c66a6cc */
 goto L8c66a6c8;
L8c66a6c8: r[2]=0u;goto L8c66a6d2; /* bra 0x8c66a6d2 */
 goto L8c66a6cc;
L8c66a6cc: *sr=(*sr&~1u)|(r[0]==0x00000008u); /* cmp/eq #0x8,r0 */
 goto L8c66a6ce;
L8c66a6ce: if(!(*sr&1u))goto L8c66a6d6; /* bf 0x8c66a6d6 */
 goto L8c66a6d0;
L8c66a6d0: r[2]=1u; /* mov #0x1,r2 */
 goto L8c66a6d2;
L8c66a6d2: wr(m,r[10],r[2],2);goto L8c66a700; /* bra 0x8c66a700 */
 goto L8c66a6d6;
L8c66a6d6: r[0]=sx(m,r[13],1); /* mov.b @r13,r0 */
 goto L8c66a6d8;
L8c66a6d8: *sr=(*sr&~1u)|(r[0]==0x00000004u); /* cmp/eq #0x4,r0 */
 goto L8c66a6da;
L8c66a6da: if(!(*sr&1u))goto L8c66a700; /* bf 0x8c66a700 */
 goto L8c66a6dc;
L8c66a6dc: r[3]=sx(m,r[9],1); /* mov.b @r9,r3 */
 goto L8c66a6de;
L8c66a6de: r[2]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r2 */
 goto L8c66a6e0;
L8c66a6e0: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66a6e2;
L8c66a6e2: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c66a6e4;
L8c66a6e4: r[3]=4u; /* mov #0x4,r3 */
 goto L8c66a6e6;
L8c66a6e6: r[2]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r2 */
 goto L8c66a6e8;
L8c66a6e8: *sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1; /* shlr r5 */
 goto L8c66a6ea;
L8c66a6ea: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c66a6ec;
L8c66a6ec: r[0]=sx(m,r[9],1); /* mov.b @r9,r0 */
 goto L8c66a6ee;
L8c66a6ee: r[3]=rd(m,0x8c66a774u,4); /* mov.l 0x8c66a774,r3 */
 goto L8c66a6f0;
L8c66a6f0: r[2]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r2 */
 goto L8c66a6f2;
L8c66a6f2: target=r[3];*pr=0x8c66a6f6u;r[1]=r[5];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a6f6;
L8c66a6f6: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c66a6f8;
L8c66a6f8: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a6fa;
L8c66a6fa: r[2]=16u; /* mov #0x10,r2 */
 goto L8c66a6fc;
L8c66a6fc: wr(m,r[13],r[2],1); /* mov.b r2,@r13 */
 goto L8c66a6fe;
L8c66a6fe: wr(m,r[10],r[3],2); /* mov.w r3,@r10 */
 goto L8c66a700;
L8c66a700: r[0]=0u; /* mov #0x0,r0 */
 goto L8c66a702;
L8c66a702: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a704;
L8c66a704: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c66a706;
L8c66a706: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66a708;
L8c66a708: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66a70a;
L8c66a70a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a70c;
L8c66a70c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a70e;
L8c66a70e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a710;
L8c66a710: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
