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
/* Retail channel-volume command25 builder66b66c; selector-dependent
 * level/auxiliary fields; commit and error services external. */
SA_STAGE_EXPORT int sa_sound_channel_volume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b66c;
L8c66b66c: r[3]=rd(m,0x8c66b700u,4); /* mov.l 0x8c66b700,r3 */
 goto L8c66b66e;
L8c66b66e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b670;
L8c66b670: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b672;
L8c66b672: r[14]=rd(m,r[3],4); /* mov.l @r3,r14 */
 goto L8c66b674;
L8c66b674: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b676;
L8c66b676: if(!(*sr&1u))goto L8c66b67e; /* bf 0x8c66b67e */
 goto L8c66b678;
L8c66b678: r[14]=rd(m,0x8c66b704u,4); /* mov.l 0x8c66b704,r14 */
 goto L8c66b67a;
L8c66b67a: goto L8c66b6e8; /* bra 0x8c66b6e8 */
 goto L8c66b67e;
L8c66b67e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b680;
L8c66b680: if(!(*sr&1u))goto L8c66b688; /* bf 0x8c66b688 */
 goto L8c66b682;
L8c66b682: r[14]=rd(m,0x8c66b708u,4); /* mov.l 0x8c66b708,r14 */
 goto L8c66b684;
L8c66b684: goto L8c66b6e8; /* bra 0x8c66b6e8 */
 goto L8c66b688;
L8c66b688: r[3]=sx(m,0x8c66b6fcu,2); /* mov.w 0x8c66b6fc,r3 */
 goto L8c66b68a;
L8c66b68a: r[6]=(uint32_t)(int32_t)(int8_t)r[6]; /* exts.b r6,r6 */
 goto L8c66b68c;
L8c66b68c: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b68e;
L8c66b68e: r[14]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r14 */
 goto L8c66b690;
L8c66b690: r[1]=rd(m,0x8c66b70cu,4); /* mov.l 0x8c66b70c,r1 */
 goto L8c66b692;
L8c66b692: r[6]+=r[3]; /* add r3,r6 */
 goto L8c66b694;
L8c66b694: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66b696;
L8c66b696: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66b698;
L8c66b698: r[0]=rd(m,0x8c66b710u,4); /* mov.l 0x8c66b710,r0 */
 goto L8c66b69a;
L8c66b69a: r[2]=37u; /* mov #0x25,r2 */
 goto L8c66b69c;
L8c66b69c: r[3]&=r[14]; /* and r14,r3 */
 goto L8c66b69e;
L8c66b69e: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c66b6a0;
L8c66b6a0: r[2]=sx(m,r[3]+r[0],1); /* mov.b @(r0,r3),r2 */
 goto L8c66b6a2;
L8c66b6a2: r[1]+=4u; /* add #0x4,r1 */
 goto L8c66b6a4;
L8c66b6a4: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c66b6a6;
L8c66b6a6: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66b6a8;
L8c66b6a8: r[2]|=r[4]; /* or r4,r2 */
 goto L8c66b6aa;
L8c66b6aa: r[4]=r[7]; /* mov r7,r4 */
 goto L8c66b6ac;
L8c66b6ac: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b6ae;
L8c66b6ae: wr(m,r[1],r[2],1); /* mov.b r2,@r1 */
 goto L8c66b6b0;
L8c66b6b0: take=(*sr&1u);{uint32_t shift=r[3];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}if(take)goto L8c66b6ba;goto L8c66b6b4; /* bt/s 0x8c66b6ba */
 goto L8c66b6b4;
L8c66b6b4: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b6b6;
L8c66b6b6: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #0xff,r0 */
 goto L8c66b6b8;
L8c66b6b8: if(!(*sr&1u))goto L8c66b6c6; /* bf 0x8c66b6c6 */
 goto L8c66b6ba;
L8c66b6ba: r[3]=rd(m,0x8c66b718u,4); /* mov.l 0x8c66b718,r3 */
 goto L8c66b6bc;
L8c66b6bc: r[1]=rd(m,0x8c66b71cu,4); /* mov.l 0x8c66b71c,r1 */
 goto L8c66b6be;
L8c66b6be: r[2]=rd(m,0x8c66b714u,4); /* mov.l 0x8c66b714,r2 */
 goto L8c66b6c0;
L8c66b6c0: wr(m,r[2],r[6],1); /* mov.b r6,@r2 */
 goto L8c66b6c2;
L8c66b6c2: wr(m,r[3],r[5],1); /* mov.b r5,@r3 */
 goto L8c66b6c4;
L8c66b6c4: wr(m,r[1],r[4],1); /* mov.b r4,@r1 */
 goto L8c66b6c6;
L8c66b6c6: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b6c8;
L8c66b6c8: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66b6ca;
L8c66b6ca: if((*sr&1u))goto L8c66b6d2; /* bt 0x8c66b6d2 */
 goto L8c66b6cc;
L8c66b6cc: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b6ce;
L8c66b6ce: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #0xff,r0 */
 goto L8c66b6d0;
L8c66b6d0: if(!(*sr&1u))goto L8c66b6de; /* bf 0x8c66b6de */
 goto L8c66b6d2;
L8c66b6d2: r[2]=rd(m,0x8c66b724u,4); /* mov.l 0x8c66b724,r2 */
 goto L8c66b6d4;
L8c66b6d4: r[1]=rd(m,0x8c66b728u,4); /* mov.l 0x8c66b728,r1 */
 goto L8c66b6d6;
L8c66b6d6: r[3]=rd(m,0x8c66b720u,4); /* mov.l 0x8c66b720,r3 */
 goto L8c66b6d8;
L8c66b6d8: wr(m,r[3],r[6],1); /* mov.b r6,@r3 */
 goto L8c66b6da;
L8c66b6da: wr(m,r[2],r[5],1); /* mov.b r5,@r2 */
 goto L8c66b6dc;
L8c66b6dc: wr(m,r[1],r[4],1); /* mov.b r4,@r1 */
 goto L8c66b6de;
L8c66b6de: r[3]=rd(m,0x8c66b72cu,4); /* mov.l 0x8c66b72c,r3 */
 goto L8c66b6e0;
L8c66b6e0: r[4]=rd(m,0x8c66b70cu,4); /* mov.l 0x8c66b70c,r4 */
 goto L8c66b6e2;
L8c66b6e2: target=r[3];*pr=0x8c66b6e6u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b6e6;
L8c66b6e6: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b6e8;
L8c66b6e8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b6ea;
L8c66b6ea: if((*sr&1u))goto L8c66b6f4; /* bt 0x8c66b6f4 */
 goto L8c66b6ec;
L8c66b6ec: r[3]=rd(m,0x8c66b734u,4); /* mov.l 0x8c66b734,r3 */
 goto L8c66b6ee;
L8c66b6ee: r[4]=rd(m,0x8c66b730u,4); /* mov.l 0x8c66b730,r4 */
 goto L8c66b6f0;
L8c66b6f0: target=r[3];*pr=0x8c66b6f4u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b6f4;
L8c66b6f4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b6f6;
L8c66b6f6: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b6f8;
L8c66b6f8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
