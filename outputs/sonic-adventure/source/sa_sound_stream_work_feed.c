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
/* Retail stream source feeding 8c65b930..8c65ba22; subordinate services use explicit SH4 ABI. */
SA_STAGE_EXPORT int sa_sound_stream_work_feed(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b930;
L8c65b930: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65b932;
L8c65b932: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65b934;
L8c65b934: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65b936;
L8c65b936: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65b938;
L8c65b938: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65b93a;
L8c65b93a: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b93c;
L8c65b93c: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c65b93e;
L8c65b93e: r[12]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r12 */
 goto L8c65b940;
L8c65b940: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65b942;
L8c65b942: take=!(*sr&1u);r[13]=rd(m,r[14]+0x8u,4);if(take)goto L8c65b974;goto L8c65b946; /* bf/s 0x8c65b974 */
 goto L8c65b946;
L8c65b946: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c65b948;
L8c65b948: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65b94a;
L8c65b94a: r[3]=rd(m,r[2]+0x24u,4); /* mov.l @(0x24,r2),r3 */
 goto L8c65b94c;
L8c65b94c: target=r[3];*pr=0x8c65b950u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b950;
L8c65b950: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65b952;
L8c65b952: if(!(*sr&1u))goto L8c65b974; /* bf 0x8c65b974 */
 goto L8c65b954;
L8c65b954: r[0]=3u; /* mov #0x3,r0 */
 goto L8c65b956;
L8c65b956: wr(m,r[14]+0x1u,r[0],1);goto L8c65ba16; /* bra 0x8c65ba16 */
 goto L8c65b974;
L8c65b974: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c65b976;
L8c65b976: r[7]=r[14]; /* mov r14,r7 */
 goto L8c65b978;
L8c65b978: r[6]=rd(m,0x8c65bb9cu,4); /* mov.l 0x8c65bb9c,r6 */
 goto L8c65b97a;
L8c65b97a: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65b97c;
L8c65b97c: r[2]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r2 */
 goto L8c65b97e;
L8c65b97e: r[7]+=20u; /* add #0x14,r7 */
 goto L8c65b980;
L8c65b980: target=r[2];*pr=0x8c65b984u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b984;
L8c65b984: r[3]=rd(m,0x8c65bba0u,4); /* mov.l 0x8c65bba0,r3 */
 goto L8c65b986;
L8c65b986: target=r[3];*pr=0x8c65b98au;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b98a;
L8c65b98a: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65b98c;
L8c65b98c: if(!(*sr&1u))goto L8c65b9b6; /* bf 0x8c65b9b6 */
 goto L8c65b98e;
L8c65b98e: r[2]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r2 */
 goto L8c65b990;
L8c65b990: r[3]=2u; /* mov #0x2,r3 */
 goto L8c65b992;
L8c65b992: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]); /* cmp/ge r3,r2 */
 goto L8c65b994;
L8c65b994: if(!(*sr&1u))goto L8c65b9b6; /* bf 0x8c65b9b6 */
 goto L8c65b996;
L8c65b996: r[4]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r4 */
 goto L8c65b998;
L8c65b998: r[1]=4294967288u; /* mov #-0x8,r1 */
 goto L8c65b99a;
L8c65b99a: r[4]=sx(m,r[4],2); /* mov.w @r4,r4 */
 goto L8c65b99c;
L8c65b99c: r[2]=r[4]; /* mov r4,r2 */
 goto L8c65b99e;
L8c65b99e: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r2 */
 goto L8c65b9a0;
L8c65b9a0: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65b9a2;
L8c65b9a2: r[1]=rd(m,0x8c65bba4u,4); /* mov.l 0x8c65bba4,r1 */
 goto L8c65b9a4;
L8c65b9a4: r[0]<<=8; /* shll8 r0 */
 goto L8c65b9a6;
L8c65b9a6: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65b9a8;
L8c65b9a8: r[4]=r[2]; /* mov r2,r4 */
 goto L8c65b9aa;
L8c65b9aa: r[0]&=r[1]; /* and r1,r0 */
 goto L8c65b9ac;
L8c65b9ac: r[2]=rd(m,0x8c65bba8u,4); /* mov.l 0x8c65bba8,r2 */
 goto L8c65b9ae;
L8c65b9ae: r[4]|=r[0]; /* or r0,r4 */
 goto L8c65b9b0;
L8c65b9b0: r[4]=r[4]&65535u; /* extu.w r4,r4 */
 goto L8c65b9b2;
L8c65b9b2: *sr=(*sr&~1u)|(r[2]==r[4]); /* cmp/eq r2,r4 */
 goto L8c65b9b4;
L8c65b9b4: if((*sr&1u))goto L8c65b9c0; /* bt 0x8c65b9c0 */
 goto L8c65b9b6;
L8c65b9b6: *pr=0x8c65b9bau;r[4]=r[14];status=call(ctx,0x8c65bc84u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65bc84 */
 goto L8c65b9ba;
L8c65b9ba: r[3]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r3 */
 goto L8c65b9bc;
L8c65b9bc: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]); /* cmp/ge r0,r3 */
 goto L8c65b9be;
L8c65b9be: if(!(*sr&1u))goto L8c65b9c6; /* bf 0x8c65b9c6 */
 goto L8c65b9c0;
L8c65b9c0: r[0]=3u; /* mov #0x3,r0 */
 goto L8c65b9c2;
L8c65b9c2: wr(m,r[14]+0x1u,r[0],1);goto L8c65b9e4; /* bra 0x8c65b9e4 */
 goto L8c65b9c6;
L8c65b9c6: *pr=0x8c65b9cau;r[4]=r[14];status=call(ctx,0x8c65bc6cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65bc6c */
 goto L8c65b9ca;
L8c65b9ca: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65b9cc;
L8c65b9cc: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c65b9ce;
L8c65b9ce: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c65b9d0;
L8c65b9d0: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c65b9d2;
L8c65b9d2: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c65b9d4;
L8c65b9d4: r[1]=rd(m,r[2]+0x24u,4); /* mov.l @(0x24,r2),r1 */
 goto L8c65b9d6;
L8c65b9d6: target=r[1];*pr=0x8c65b9dau;r[4]=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c65b9da;
L8c65b9da: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c65b9dc;
L8c65b9dc: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65b9de;
L8c65b9de: *sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1; /* shlr r4 */
 goto L8c65b9e0;
L8c65b9e0: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c65b9e2;
L8c65b9e2: if((*sr&1u))goto L8c65b9fc; /* bt 0x8c65b9fc */
 goto L8c65b9e4;
L8c65b9e4: r[1]=rd(m,r[13],4); /* mov.l @r13,r1 */
 goto L8c65b9e6;
L8c65b9e6: r[6]=r[14]; /* mov r14,r6 */
 goto L8c65b9e8;
L8c65b9e8: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65b9ea;
L8c65b9ea: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65b9ec;
L8c65b9ec: r[3]=rd(m,r[1]+0x1cu,4); /* mov.l @(0x1c,r1),r3 */
 goto L8c65b9ee;
L8c65b9ee: r[6]+=20u; /* add #0x14,r6 */
 goto L8c65b9f0;
L8c65b9f0: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65b9f2;
L8c65b9f2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65b9f4;
L8c65b9f4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65b9f6;
L8c65b9f6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65b9f8;
L8c65b9f8: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c65b9fc: r[2]=rd(m,0x8c65bbacu,4); /* mov.l 0x8c65bbac,r2 */
 goto L8c65b9fe;
L8c65b9fe: r[5]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r5 */
 goto L8c65ba00;
L8c65ba00: r[6]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r6 */
 goto L8c65ba02;
L8c65ba02: target=r[2];*pr=0x8c65ba06u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ba06;
L8c65ba06: r[4]=r[12]; /* mov r12,r4 */
 goto L8c65ba08;
L8c65ba08: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65ba0a;
L8c65ba0a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ba0c;
L8c65ba0c: r[3]=rd(m,0x8c65bbb0u,4); /* mov.l 0x8c65bbb0,r3 */
 goto L8c65ba0e;
L8c65ba0e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65ba10;
L8c65ba10: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65ba12;
L8c65ba12: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c65ba16: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65ba18;
L8c65ba18: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ba1a;
L8c65ba1a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65ba1c;
L8c65ba1c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65ba1e;
L8c65ba1e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
