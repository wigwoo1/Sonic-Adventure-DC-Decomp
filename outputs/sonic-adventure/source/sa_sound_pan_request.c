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
/* Static C recovery of retail Tikal helper 8c66b374..8c66b4e0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_pan_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b374;
L8c66b374: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b376;
L8c66b376: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66b378;
L8c66b378: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66b37a;
L8c66b37a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66b37c;
L8c66b37c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b37e;
L8c66b37e: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66b380;
L8c66b380: r[3]=rd(m,0x8c66b46cu,4); /* mov.l 0x8c66b46c,r3 */
 goto L8c66b382;
L8c66b382: r[14]=rd(m,r[3],4); /* mov.l @r3,r14 */
 goto L8c66b384;
L8c66b384: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b386;
L8c66b386: if(!(*sr&1u))goto L8c66b38e; /* bf 0x8c66b38e */
 goto L8c66b388;
L8c66b388: r[14]=rd(m,0x8c66b470u,4); /* mov.l 0x8c66b470,r14 */
 goto L8c66b38a;
L8c66b38a: goto L8c66b4c4; /* bra 0x8c66b4c4 */
 goto L8c66b38e;
L8c66b38e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b390;
L8c66b390: if(!(*sr&1u))goto L8c66b398; /* bf 0x8c66b398 */
 goto L8c66b392;
L8c66b392: r[14]=rd(m,0x8c66b474u,4); /* mov.l 0x8c66b474,r14 */
 goto L8c66b394;
L8c66b394: goto L8c66b4c4; /* bra 0x8c66b4c4 */
 goto L8c66b398;
L8c66b398: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b39a;
L8c66b39a: r[2]=44u; /* mov #0x2c,r2 */
 goto L8c66b39c;
L8c66b39c: r[1]=sx(m,0x8c66b466u,2); /* mov.w 0x8c66b466,r1 */
 goto L8c66b39e;
L8c66b39e: r[12]=(uint32_t)(int32_t)(int8_t)r[6]; /* exts.b r6,r12 */
 goto L8c66b3a0;
L8c66b3a0: r[11]=r[0]; /* mov r0,r11 */
 goto L8c66b3a2;
L8c66b3a2: r[3]=(uint32_t)(int32_t)(int8_t)r[11]; /* exts.b r11,r3 */
 goto L8c66b3a4;
L8c66b3a4: *macl=(uint32_t)((int32_t)(int16_t)r[2]*(int32_t)(int16_t)r[3]); /* muls.w r2,r3 */
 goto L8c66b3a6;
L8c66b3a6: r[14]+=r[1]; /* add r1,r14 */
 goto L8c66b3a8;
L8c66b3a8: r[6]=0u; /* mov #0x0,r6 */
 goto L8c66b3aa;
L8c66b3aa: r[13]=r[6]; /* mov r6,r13 */
 goto L8c66b3ac;
L8c66b3ac: r[3]=*macl; /* sts MACL,r3 */
 goto L8c66b3ae;
L8c66b3ae: r[14]+=r[3]; /* add r3,r14 */
 goto L8c66b3b0;
L8c66b3b0: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b3b2;
L8c66b3b2: r[3]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r3 */
 goto L8c66b3b4;
L8c66b3b4: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c66b3b6;
L8c66b3b6: r[3]<<=2; /* shll2 r3 */
 goto L8c66b3b8;
L8c66b3b8: r[3]+=r[0]; /* add r0,r3 */
 goto L8c66b3ba;
L8c66b3ba: wr(m,r[3],r[12],4); /* mov.l r12,@r3 */
 goto L8c66b3bc;
L8c66b3bc: r[4]=r[15];goto L8c66b3c8; /* bra 0x8c66b3c8 */
 goto L8c66b3c0;
L8c66b3c0: r[0]=r[13]; /* mov r13,r0 */
 goto L8c66b3c2;
L8c66b3c2: r[0]<<=2; /* shll2 r0 */
 goto L8c66b3c4;
L8c66b3c4: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66b3c6;
L8c66b3c6: wr(m,r[4]+r[0],r[12],4); /* mov.l r12,@(r0,r4) */
 goto L8c66b3c8;
L8c66b3c8: r[0]=sx(m,r[14]+0x5u,1); /* mov.b @(0x5,r14),r0 */
 goto L8c66b3ca;
L8c66b3ca: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]); /* cmp/ge r0,r13 */
 goto L8c66b3cc;
L8c66b3cc: if(!(*sr&1u))goto L8c66b3c0; /* bf 0x8c66b3c0 */
 goto L8c66b3ce;
L8c66b3ce: r[0]=sx(m,r[14]+0x5u,1); /* mov.b @(0x5,r14),r0 */
 goto L8c66b3d0;
L8c66b3d0: r[13]=sx(m,0x8c66b468u,2); /* mov.w 0x8c66b468,r13 */
 goto L8c66b3d2;
L8c66b3d2: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66b3d4;
L8c66b3d4: take=(*sr&1u);r[12]=(uint32_t)(int32_t)(int8_t)r[5];if(take)goto L8c66b424;goto L8c66b3d8; /* bt/s 0x8c66b424 */
 goto L8c66b3d8;
L8c66b3d8: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66b3da;
L8c66b3da: if(!(*sr&1u))goto L8c66b424; /* bf 0x8c66b424 */
 goto L8c66b3dc;
L8c66b3dc: r[0]=r[12]; /* mov r12,r0 */
 goto L8c66b3de;
L8c66b3de: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c66b3e0;
L8c66b3e0: if((*sr&1u))goto L8c66b3ea; /* bt 0x8c66b3ea */
 goto L8c66b3e2;
L8c66b3e2: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66b3e4;
L8c66b3e4: if((*sr&1u))goto L8c66b3f2; /* bt 0x8c66b3f2 */
 goto L8c66b3e6;
L8c66b3e6: goto L8c66b408; /* bra 0x8c66b408 */
 goto L8c66b3ea;
L8c66b3ea: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c66b3ec;
L8c66b3ec: r[2]+=r[13]; /* add r13,r2 */
 goto L8c66b3ee;
L8c66b3ee: wr(m,r[4],r[2],4);goto L8c66b408; /* bra 0x8c66b408 */
 goto L8c66b3f2;
L8c66b3f2: r[5]=r[4]; /* mov r4,r5 */
 goto L8c66b3f4;
L8c66b3f4: r[5]+=4u; /* add #0x4,r5 */
 goto L8c66b3f6;
L8c66b3f6: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c66b3f8;
L8c66b3f8: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c66b3fa;
L8c66b3fa: r[2]-=r[13]; /* sub r13,r2 */
 goto L8c66b3fc;
L8c66b3fc: r[1]=r[2]; /* mov r2,r1 */
 goto L8c66b3fe;
L8c66b3fe: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[3]); /* cmp/ge r3,r1 */
 goto L8c66b400;
L8c66b400: wr(m,r[5],r[2],4); /* mov.l r2,@r5 */
 goto L8c66b402;
L8c66b402: if(!(*sr&1u))goto L8c66b408; /* bf 0x8c66b408 */
 goto L8c66b404;
L8c66b404: r[0]=4294967294u; /* mov #-0x2,r0 */
 goto L8c66b406;
L8c66b406: wr(m,r[4]+0x4u,r[0],4); /* mov.l r0,@(0x4,r4) */
 goto L8c66b408;
L8c66b408: r[5]=r[6];goto L8c66b41e; /* bra 0x8c66b41e */
 goto L8c66b40c;
L8c66b40c: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66b40e;
L8c66b40e: r[0]<<=2; /* shll2 r0 */
 goto L8c66b410;
L8c66b410: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c66b412;
L8c66b412: r[2]=0u; /* mov #0x0,r2 */
 goto L8c66b414;
L8c66b414: r[5]+=1u; /* add #0x1,r5 */
 goto L8c66b416;
L8c66b416: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]); /* cmp/gt r3,r2 */
 goto L8c66b418;
L8c66b418: {uint64_t sum=(uint64_t)r[3]+r[2]+(*sr&1u);r[3]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r2,r3 */
 goto L8c66b41a;
L8c66b41a: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c66b41c;
L8c66b41c: wr(m,r[4]+r[0],r[3],4); /* mov.l r3,@(r0,r4) */
 goto L8c66b41e;
L8c66b41e: r[0]=sx(m,r[14]+0x5u,1); /* mov.b @(0x5,r14),r0 */
 goto L8c66b420;
L8c66b420: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[0]); /* cmp/ge r0,r5 */
 goto L8c66b422;
L8c66b422: if(!(*sr&1u))goto L8c66b40c; /* bf 0x8c66b40c */
 goto L8c66b424;
L8c66b424: r[5]=r[6];goto L8c66b434; /* bra 0x8c66b434 */
 goto L8c66b428;
L8c66b428: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66b42a;
L8c66b42a: r[0]<<=2; /* shll2 r0 */
 goto L8c66b42c;
L8c66b42c: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c66b42e;
L8c66b42e: r[5]+=1u; /* add #0x1,r5 */
 goto L8c66b430;
L8c66b430: r[3]+=r[13]; /* add r13,r3 */
 goto L8c66b432;
L8c66b432: wr(m,r[4]+r[0],r[3],4); /* mov.l r3,@(r0,r4) */
 goto L8c66b434;
L8c66b434: r[0]=sx(m,r[14]+0x5u,1); /* mov.b @(0x5,r14),r0 */
 goto L8c66b436;
L8c66b436: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[0]); /* cmp/ge r0,r5 */
 goto L8c66b438;
L8c66b438: if(!(*sr&1u))goto L8c66b428; /* bf 0x8c66b428 */
 goto L8c66b43a;
L8c66b43a: r[3]=rd(m,0x8c66b478u,4); /* mov.l 0x8c66b478,r3 */
 goto L8c66b43c;
L8c66b43c: r[2]=38u; /* mov #0x26,r2 */
 goto L8c66b43e;
L8c66b43e: r[0]=rd(m,0x8c66b480u,4); /* mov.l 0x8c66b480,r0 */
 goto L8c66b440;
L8c66b440: r[14]=r[7]; /* mov r7,r14 */
 goto L8c66b442;
L8c66b442: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66b444;
L8c66b444: r[2]=3u; /* mov #0x3,r2 */
 goto L8c66b446;
L8c66b446: r[2]&=r[12]; /* and r12,r2 */
 goto L8c66b448;
L8c66b448: r[1]=rd(m,0x8c66b47cu,4); /* mov.l 0x8c66b47c,r1 */
 goto L8c66b44a;
L8c66b44a: r[3]=sx(m,r[2]+r[0],1); /* mov.b @(r0,r2),r3 */
 goto L8c66b44c;
L8c66b44c: r[0]=r[12]; /* mov r12,r0 */
 goto L8c66b44e;
L8c66b44e: r[5]=rd(m,0x8c66b484u,4); /* mov.l 0x8c66b484,r5 */
 goto L8c66b450;
L8c66b450: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c66b452;
L8c66b452: r[3]|=r[11]; /* or r11,r3 */
 goto L8c66b454;
L8c66b454: r[13]=r[7]; /* mov r7,r13 */
 goto L8c66b456;
L8c66b456: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c66b458;
L8c66b458: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c66b45a;
L8c66b45a: take=(*sr&1u);{uint32_t shift=r[3];if(!(shift&0x80000000u))r[13]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[13]=n?(uint32_t)((int32_t)r[13]>>n):((r[13]&0x80000000u)?0xffffffffu:0u);}}if(take)goto L8c66b488;goto L8c66b45e; /* bt/s 0x8c66b488 */
 goto L8c66b45e;
L8c66b45e: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66b460;
L8c66b460: if((*sr&1u))goto L8c66b4a2; /* bt 0x8c66b4a2 */
 goto L8c66b462;
L8c66b462: goto L8c66b4ba; /* bra 0x8c66b4ba */
 goto L8c66b488;
L8c66b488: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c66b48a;
L8c66b48a: r[3]=rd(m,0x8c66b4e0u,4); /* mov.l 0x8c66b4e0,r3 */
 goto L8c66b48c;
L8c66b48c: wr(m,r[5],r[2],1); /* mov.b r2,@r5 */
 goto L8c66b48e;
L8c66b48e: r[2]=rd(m,0x8c66b4e4u,4); /* mov.l 0x8c66b4e4,r2 */
 goto L8c66b490;
L8c66b490: r[1]=rd(m,0x8c66b4e8u,4); /* mov.l 0x8c66b4e8,r1 */
 goto L8c66b492;
L8c66b492: wr(m,r[3],r[14],1); /* mov.b r14,@r3 */
 goto L8c66b494;
L8c66b494: r[3]+=4u; /* add #0x4,r3 */
 goto L8c66b496;
L8c66b496: wr(m,r[2],r[13],1); /* mov.b r13,@r2 */
 goto L8c66b498;
L8c66b498: r[2]+=4u; /* add #0x4,r2 */
 goto L8c66b49a;
L8c66b49a: wr(m,r[1],r[6],1); /* mov.b r6,@r1 */
 goto L8c66b49c;
L8c66b49c: wr(m,r[3],r[6],1); /* mov.b r6,@r3 */
 goto L8c66b49e;
L8c66b49e: wr(m,r[2],r[6],1);goto L8c66b4ba; /* bra 0x8c66b4ba */
 goto L8c66b4a2;
L8c66b4a2: r[3]=rd(m,0x8c66b4e0u,4); /* mov.l 0x8c66b4e0,r3 */
 goto L8c66b4a4;
L8c66b4a4: r[1]=rd(m,0x8c66b4e8u,4); /* mov.l 0x8c66b4e8,r1 */
 goto L8c66b4a6;
L8c66b4a6: r[0]=rd(m,0x8c66b4ecu,4); /* mov.l 0x8c66b4ec,r0 */
 goto L8c66b4a8;
L8c66b4a8: r[2]=rd(m,0x8c66b4e4u,4); /* mov.l 0x8c66b4e4,r2 */
 goto L8c66b4aa;
L8c66b4aa: wr(m,r[5],r[6],1); /* mov.b r6,@r5 */
 goto L8c66b4ac;
L8c66b4ac: wr(m,r[3],r[6],1); /* mov.b r6,@r3 */
 goto L8c66b4ae;
L8c66b4ae: wr(m,r[2],r[6],1); /* mov.b r6,@r2 */
 goto L8c66b4b0;
L8c66b4b0: r[3]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r3 */
 goto L8c66b4b2;
L8c66b4b2: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c66b4b4;
L8c66b4b4: r[3]=rd(m,0x8c66b4f0u,4); /* mov.l 0x8c66b4f0,r3 */
 goto L8c66b4b6;
L8c66b4b6: wr(m,r[0],r[14],1); /* mov.b r14,@r0 */
 goto L8c66b4b8;
L8c66b4b8: wr(m,r[3],r[13],1); /* mov.b r13,@r3 */
 goto L8c66b4ba;
L8c66b4ba: r[3]=rd(m,0x8c66b4f8u,4); /* mov.l 0x8c66b4f8,r3 */
 goto L8c66b4bc;
L8c66b4bc: r[4]=rd(m,0x8c66b4f4u,4); /* mov.l 0x8c66b4f4,r4 */
 goto L8c66b4be;
L8c66b4be: target=r[3];*pr=0x8c66b4c2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b4c2;
L8c66b4c2: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b4c4;
L8c66b4c4: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b4c6;
L8c66b4c6: if((*sr&1u))goto L8c66b4d0; /* bt 0x8c66b4d0 */
 goto L8c66b4c8;
L8c66b4c8: r[3]=rd(m,0x8c66b500u,4); /* mov.l 0x8c66b500,r3 */
 goto L8c66b4ca;
L8c66b4ca: r[4]=rd(m,0x8c66b4fcu,4); /* mov.l 0x8c66b4fc,r4 */
 goto L8c66b4cc;
L8c66b4cc: target=r[3];*pr=0x8c66b4d0u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b4d0;
L8c66b4d0: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b4d2;
L8c66b4d2: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66b4d4;
L8c66b4d4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b4d6;
L8c66b4d6: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66b4d8;
L8c66b4d8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66b4da;
L8c66b4da: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66b4dc;
L8c66b4dc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
