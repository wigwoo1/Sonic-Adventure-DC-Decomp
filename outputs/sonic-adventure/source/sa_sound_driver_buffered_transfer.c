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
/* Static C recovery of retail Tikal helper 8c6687f4..8c668916.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_buffered_transfer(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6687f4;
L8c6687f4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6687f6;
L8c6687f6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6687f8;
L8c6687f8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6687fa;
L8c6687fa: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6687fc;
L8c6687fc: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6687fe;
L8c6687fe: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c668800;
L8c668800: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c668802;
L8c668802: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668804;
L8c668804: r[15]+=4294967252u; /* add #-0x2c,r15 */
 goto L8c668806;
L8c668806: r[9]=r[4]; /* mov r4,r9 */
 goto L8c668808;
L8c668808: r[4]=r[15]; /* mov r15,r4 */
 goto L8c66880a;
L8c66880a: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c66880c;
L8c66880c: wr(m,r[15]+0xcu,r[5],4); /* mov.l r5,@(0xc,r15) */
 goto L8c66880e;
L8c66880e: r[4]+=28u; /* add #0x1c,r4 */
 goto L8c668810;
L8c668810: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c668812;
L8c668812: r[14]=r[4]; /* mov r4,r14 */
 goto L8c668814;
L8c668814: r[12]=r[14]; /* mov r14,r12 */
 goto L8c668816;
L8c668816: r[12]+=4u; /* add #0x4,r12 */
 goto L8c668818;
L8c668818: r[4]=r[15]; /* mov r15,r4 */
 goto L8c66881a;
L8c66881a: r[4]+=36u; /* add #0x24,r4 */
 goto L8c66881c;
L8c66881c: wr(m,r[15]+0x18u,r[7],4); /* mov.l r7,@(0x18,r15) */
 goto L8c66881e;
L8c66881e: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c668820;
L8c668820: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c668822;
L8c668822: r[1]=0u; /* mov #0x0,r1 */
 goto L8c668824;
L8c668824: r[11]=r[4]; /* mov r4,r11 */
 goto L8c668826;
L8c668826: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c668828;
L8c668828: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66882a;
L8c66882a: r[3]=sx(m,0x8c66884cu,2); /* mov.w 0x8c66884c,r3 */
 goto L8c66882c;
L8c66882c: r[11]+=4u; /* add #0x4,r11 */
 goto L8c66882e;
L8c66882e: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c668830;
L8c668830: r[2]+=r[3]; /* add r3,r2 */
 goto L8c668832;
L8c668832: r[3]=rd(m,0x8c668858u,4); /* mov.l 0x8c668858,r3 */
 goto L8c668834;
L8c668834: wr(m,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c668836;
L8c668836: wr(m,r[15]+0x10u,r[1],4); /* mov.l r1,@(0x10,r15) */
 goto L8c668838;
L8c668838: r[1]=64u; /* mov #0x40,r1 */
 goto L8c66883a;
L8c66883a: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c66883c;
L8c66883c: wr(m,r[15]+0x14u,r[2],4); /* mov.l r2,@(0x14,r15) */
 goto L8c66883e;
L8c66883e: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c668840;
L8c668840: r[2]<<=2; /* shll2 r2 */
 goto L8c668842;
L8c668842: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c668844;
L8c668844: target=r[3];*pr=0x8c668848u;r[0]=rd(m,r[15]+0x18u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668848;
L8c668848: r[8]=r[0];goto L8c6688fa; /* bra 0x8c6688fa */
 goto L8c66885c;
L8c66885c: r[0]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r0 */
 goto L8c66885e;
L8c66885e: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c668860;
L8c668860: if(!(*sr&1u))goto L8c668866; /* bf 0x8c668866 */
 goto L8c668862;
L8c668862: r[0]&=0x3fu;goto L8c668870; /* bra 0x8c668870 */
 goto L8c668866;
L8c668866: r[0]=~r[0]; /* not r0,r0 */
 goto L8c668868;
L8c668868: r[0]+=1u; /* add #0x1,r0 */
 goto L8c66886a;
L8c66886a: r[0]&=0x3fu; /* and #0x3f,r0 */
 goto L8c66886c;
L8c66886c: r[0]=~r[0]; /* not r0,r0 */
 goto L8c66886e;
L8c66886e: r[0]+=1u; /* add #0x1,r0 */
 goto L8c668870;
L8c668870: r[4]=r[9]; /* mov r9,r4 */
 goto L8c668872;
L8c668872: r[4]+=48u; /* add #0x30,r4 */
 goto L8c668874;
L8c668874: r[6]=sx(m,0x8c668958u,2); /* mov.w 0x8c668958,r6 */
 goto L8c668876;
L8c668876: r[1]=rd(m,0x8c66895cu,4); /* mov.l 0x8c66895c,r1 */
 goto L8c668878;
L8c668878: wr(m,r[13],r[0],4); /* mov.l r0,@r13 */
 goto L8c66887a;
L8c66887a: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c66887c;
L8c66887c: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c66887e;
L8c66887e: r[4]+=r[3]; /* add r3,r4 */
 goto L8c668880;
L8c668880: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c668882;
L8c668882: r[2]<<=8; /* shll8 r2 */
 goto L8c668884;
L8c668884: r[4]+=r[2]; /* add r2,r4 */
 goto L8c668886;
L8c668886: target=r[1];*pr=0x8c66888au;r[5]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c66888a;
L8c66888a: r[3]=0u; /* mov #0x0,r3 */
 goto L8c66888c;
L8c66888c: *sr=(*sr&~1u)|((int32_t)r[8]>0); /* cmp/pl r8 */
 goto L8c66888e;
L8c66888e: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto L8c668890;
L8c668890: if(!(*sr&1u))goto L8c6688ee; /* bf 0x8c6688ee */
 goto L8c668892;
L8c668892: r[0]=rd(m,r[13],4); /* mov.l @r13,r0 */
 goto L8c668894;
L8c668894: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c668896;
L8c668896: r[0]-=r[3]; /* sub r3,r0 */
 goto L8c668898;
L8c668898: r[0]+=64u; /* add #0x40,r0 */
 goto L8c66889a;
L8c66889a: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c66889c;
L8c66889c: if(!(*sr&1u))goto L8c6688a2; /* bf 0x8c6688a2 */
 goto L8c66889e;
L8c66889e: r[0]&=0x3fu;goto L8c6688ac; /* bra 0x8c6688ac */
 goto L8c6688a2;
L8c6688a2: r[0]=~r[0]; /* not r0,r0 */
 goto L8c6688a4;
L8c6688a4: r[0]+=1u; /* add #0x1,r0 */
 goto L8c6688a6;
L8c6688a6: r[0]&=0x3fu; /* and #0x3f,r0 */
 goto L8c6688a8;
L8c6688a8: r[0]=~r[0]; /* not r0,r0 */
 goto L8c6688aa;
L8c6688aa: r[0]+=1u; /* add #0x1,r0 */
 goto L8c6688ac;
L8c6688ac: r[10]=r[9]; /* mov r9,r10 */
 goto L8c6688ae;
L8c6688ae: r[10]+=48u; /* add #0x30,r10 */
 goto L8c6688b0;
L8c6688b0: r[6]=sx(m,0x8c668958u,2); /* mov.w 0x8c668958,r6 */
 goto L8c6688b2;
L8c6688b2: r[1]=rd(m,0x8c66895cu,4); /* mov.l 0x8c66895c,r1 */
 goto L8c6688b4;
L8c6688b4: wr(m,r[11],r[0],4); /* mov.l r0,@r11 */
 goto L8c6688b6;
L8c6688b6: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c6688b8;
L8c6688b8: r[2]=rd(m,r[11],4); /* mov.l @r11,r2 */
 goto L8c6688ba;
L8c6688ba: r[10]+=r[3]; /* add r3,r10 */
 goto L8c6688bc;
L8c6688bc: r[5]=rd(m,r[12],4); /* mov.l @r12,r5 */
 goto L8c6688be;
L8c6688be: r[10]=rd(m,r[10],4); /* mov.l @r10,r10 */
 goto L8c6688c0;
L8c6688c0: r[2]<<=8; /* shll8 r2 */
 goto L8c6688c2;
L8c6688c2: r[10]+=r[2]; /* add r2,r10 */
 goto L8c6688c4;
L8c6688c4: target=r[1];*pr=0x8c6688c8u;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6688c8;
L8c6688c8: r[3]=sx(m,0x8c668958u,2); /* mov.w 0x8c668958,r3 */
 goto L8c6688ca;
L8c6688ca: r[6]=r[10]; /* mov r10,r6 */
 goto L8c6688cc;
L8c6688cc: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6688ce;
L8c6688ce: r[5]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r5 */
 goto L8c6688d0;
L8c6688d0: r[7]=rd(m,r[14],4); /* mov.l @r14,r7 */
 goto L8c6688d2;
L8c6688d2: *pr=0x8c6688d6u;r[4]=r[9];status=call(ctx,0x8c6684c4u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6684c4 */
 goto L8c6688d6;
L8c6688d6: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6688d8;
L8c6688d8: r[3]=rd(m,r[11],4); /* mov.l @r11,r3 */
 goto L8c6688da;
L8c6688da: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c6688dc;
L8c6688dc: r[2]=rd(m,r[12],4); /* mov.l @r12,r2 */
 goto L8c6688de;
L8c6688de: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c6688e0;
L8c6688e0: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c6688e2;
L8c6688e2: wr(m,r[12],r[4],4); /* mov.l r4,@r12 */
 goto L8c6688e4;
L8c6688e4: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c6688e6;
L8c6688e6: r[3]+=1u; /* add #0x1,r3 */
 goto L8c6688e8;
L8c6688e8: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[8]); /* cmp/ge r8,r3 */
 goto L8c6688ea;
L8c6688ea: take=!(*sr&1u);wr(m,r[15]+0x4u,r[3],4);if(take)goto L8c668892;goto L8c6688ee; /* bf/s 0x8c668892 */
 goto L8c6688ee;
L8c6688ee: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c6688f0;
L8c6688f0: r[3]+=1u; /* add #0x1,r3 */
 goto L8c6688f2;
L8c6688f2: wr(m,r[15]+0x10u,r[3],4); /* mov.l r3,@(0x10,r15) */
 goto L8c6688f4;
L8c6688f4: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c6688f6;
L8c6688f6: r[2]+=1u; /* add #0x1,r2 */
 goto L8c6688f8;
L8c6688f8: wr(m,r[15]+0x14u,r[2],4); /* mov.l r2,@(0x14,r15) */
 goto L8c6688fa;
L8c6688fa: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c6688fc;
L8c6688fc: r[3]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r3 */
 goto L8c6688fe;
L8c6688fe: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[3]); /* cmp/ge r3,r1 */
 goto L8c668900;
L8c668900: if(!(*sr&1u))goto L8c66885c; /* bf 0x8c66885c */
 goto L8c668902;
L8c668902: r[15]+=44u; /* add #0x2c,r15 */
 goto L8c668904;
L8c668904: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668906;
L8c668906: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c668908;
L8c668908: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66890a;
L8c66890a: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66890c;
L8c66890c: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66890e;
L8c66890e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c668910;
L8c668910: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668912;
L8c668912: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
