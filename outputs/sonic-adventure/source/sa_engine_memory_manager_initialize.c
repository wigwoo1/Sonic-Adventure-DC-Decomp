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
/* Static C recovery of retail memory-manager initializer 8c663140..8c6632cc.
 * Option selector, register writer, manager reset and node allocator remain services;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_memory_manager_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c663140;
L8c663140: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c663142;
L8c663142: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c663144;
L8c663144: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c663146;
L8c663146: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c663148;
L8c663148: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66314a;
L8c66314a: r[3]=rd(m,0x8c6631fcu,4); /* mov.l 0x8c6631fc,r3 */
 goto L8c66314c;
L8c66314c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c66314e;
L8c66314e: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c663150;
L8c663150: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c663152;
L8c663152: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c663154;
L8c663154: r[13]=rd(m,0x8c6631f4u,4); /* mov.l 0x8c6631f4,r13 */
 goto L8c663156;
L8c663156: r[14]=rd(m,0x8c6631f8u,4); /* mov.l 0x8c6631f8,r14 */
 goto L8c663158;
L8c663158: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c66315a;
L8c66315a: r[9]=rd(m,0x8c6631e8u,4); /* mov.l 0x8c6631e8,r9 */
 goto L8c66315c;
L8c66315c: r[8]=rd(m,0x8c6631e4u,4); /* mov.l 0x8c6631e4,r8 */
 goto L8c66315e;
L8c66315e: r[12]=rd(m,0x8c6631f0u,4); /* mov.l 0x8c6631f0,r12 */
 goto L8c663160;
L8c663160: r[10]=rd(m,0x8c6631ecu,4); /* mov.l 0x8c6631ec,r10 */
 goto L8c663162;
L8c663162: take=!(*sr&1u);r[11]=0u;if(take)goto L8c663220;goto L8c663166; /* bf/s 0x8c663220 */
 goto L8c663166;
L8c663166: r[1]=rd(m,0x8c663208u,4); /* mov.l 0x8c663208,r1 */
 goto L8c663168;
L8c663168: r[4]=rd(m,0x8c663204u,4); /* mov.l 0x8c663204,r4 */
 goto L8c66316a;
L8c66316a: r[5]=rd(m,0x8c663200u,4); /* mov.l 0x8c663200,r5 */
 goto L8c66316c;
L8c66316c: target=r[1];*pr=0x8c663170u;r[6]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c663170;
L8c663170: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c663172;
L8c663172: if(!(*sr&1u))goto L8c663194; /* bf 0x8c663194 */
 goto L8c663174;
L8c663174: r[3]=rd(m,0x8c663210u,4); /* mov.l 0x8c663210,r3 */
 goto L8c663176;
L8c663176: r[2]=rd(m,0x8c66320cu,4); /* mov.l 0x8c66320c,r2 */
 goto L8c663178;
L8c663178: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66317a;
L8c66317a: r[0]=rd(m,0x8c663214u,4); /* mov.l 0x8c663214,r0 */
 goto L8c66317c;
L8c66317c: r[1]=r[2]; /* mov r2,r1 */
 goto L8c66317e;
L8c66317e: r[4]=sx(m,0x8c6631e0u,2); /* mov.w 0x8c6631e0,r4 */
 goto L8c663180;
L8c663180: wr(m,r[0],r[2],4); /* mov.l r2,@r0 */
 goto L8c663182;
L8c663182: wr(m,r[9],r[11],4); /* mov.l r11,@r9 */
 goto L8c663184;
L8c663184: wr(m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c663186;
L8c663186: r[2]=rd(m,0x8c663218u,4); /* mov.l 0x8c663218,r2 */
 goto L8c663188;
L8c663188: wr(m,r[8],r[1],4); /* mov.l r1,@r8 */
 goto L8c66318a;
L8c66318a: wr(m,r[10],r[11],4); /* mov.l r11,@r10 */
 goto L8c66318c;
L8c66318c: target=r[2];*pr=0x8c663190u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c663190;
L8c663190: goto L8c6631ac; /* bra 0x8c6631ac */
 goto L8c663194;
L8c663194: r[2]=rd(m,0x8c663210u,4); /* mov.l 0x8c663210,r2 */
 goto L8c663196;
L8c663196: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c663198;
L8c663198: r[3]=rd(m,0x8c663214u,4); /* mov.l 0x8c663214,r3 */
 goto L8c66319a;
L8c66319a: r[4]=sx(m,0x8c6631e0u,2); /* mov.w 0x8c6631e0,r4 */
 goto L8c66319c;
L8c66319c: r[1]=rd(m,0x8c663218u,4); /* mov.l 0x8c663218,r1 */
 goto L8c66319e;
L8c66319e: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6631a0;
L8c6631a0: wr(m,r[9],r[11],4); /* mov.l r11,@r9 */
 goto L8c6631a2;
L8c6631a2: wr(m,r[13],r[14],4); /* mov.l r14,@r13 */
 goto L8c6631a4;
L8c6631a4: wr(m,r[8],r[14],4); /* mov.l r14,@r8 */
 goto L8c6631a6;
L8c6631a6: wr(m,r[10],r[11],4); /* mov.l r11,@r10 */
 goto L8c6631a8;
L8c6631a8: target=r[1];*pr=0x8c6631acu;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6631ac;
L8c6631ac: *pr=0x8c6631b0u;status=call(ctx,0x8c663080u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c663080 */
 goto L8c6631b0;
L8c6631b0: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c6631b2;
L8c6631b2: wr(m,r[12]+0x10u,r[2],4); /* mov.l r2,@(0x10,r12) */
 goto L8c6631b4;
L8c6631b4: r[2]=rd(m,0x8c66321cu,4); /* mov.l 0x8c66321c,r2 */
 goto L8c6631b6;
L8c6631b6: r[3]=rd(m,r[10],4); /* mov.l @r10,r3 */
 goto L8c6631b8;
L8c6631b8: target=r[2];*pr=0x8c6631bcu;wr(m,r[12]+0x24u,r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6631bc;
L8c6631bc: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6631be;
L8c6631be: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6631c0;
L8c6631c0: if(!(*sr&1u))goto L8c6631c6; /* bf 0x8c6631c6 */
 goto L8c6631c2;
L8c6631c2: goto L8c66329c; /* bra 0x8c66329c */
 goto L8c6631c6;
L8c6631c6: r[2]=rd(m,r[9],4); /* mov.l @r9,r2 */
 goto L8c6631c8;
L8c6631c8: wr(m,r[4]+0xcu,r[2],4); /* mov.l r2,@(0xc,r4) */
 goto L8c6631ca;
L8c6631ca: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c6631cc;
L8c6631cc: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto L8c6631ce;
L8c6631ce: r[5]=r[12];goto L8c6632ac; /* bra 0x8c6632ac */
 goto L8c663220;
L8c663220: r[2]=rd(m,0x8c6632ecu,4); /* mov.l 0x8c6632ec,r2 */
 goto L8c663222;
L8c663222: r[4]=rd(m,0x8c6632e8u,4); /* mov.l 0x8c6632e8,r4 */
 goto L8c663224;
L8c663224: r[5]=rd(m,0x8c6632e4u,4); /* mov.l 0x8c6632e4,r5 */
 goto L8c663226;
L8c663226: target=r[2];*pr=0x8c66322au;r[6]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66322a;
L8c66322a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c66322c;
L8c66322c: if(!(*sr&1u))goto L8c66324c; /* bf 0x8c66324c */
 goto L8c66322e;
L8c66322e: r[3]=rd(m,0x8c6632f4u,4); /* mov.l 0x8c6632f4,r3 */
 goto L8c663230;
L8c663230: r[2]=rd(m,0x8c6632f0u,4); /* mov.l 0x8c6632f0,r2 */
 goto L8c663232;
L8c663232: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c663234;
L8c663234: r[1]=rd(m,0x8c6632f8u,4); /* mov.l 0x8c6632f8,r1 */
 goto L8c663236;
L8c663236: r[4]=sx(m,0x8c6632e0u,2); /* mov.w 0x8c6632e0,r4 */
 goto L8c663238;
L8c663238: r[2]=rd(m,0x8c6632fcu,4); /* mov.l 0x8c6632fc,r2 */
 goto L8c66323a;
L8c66323a: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c66323c;
L8c66323c: wr(m,r[9],r[11],4); /* mov.l r11,@r9 */
 goto L8c66323e;
L8c66323e: wr(m,r[13],r[14],4); /* mov.l r14,@r13 */
 goto L8c663240;
L8c663240: wr(m,r[8],r[14],4); /* mov.l r14,@r8 */
 goto L8c663242;
L8c663242: wr(m,r[10],r[14],4); /* mov.l r14,@r10 */
 goto L8c663244;
L8c663244: target=r[2];*pr=0x8c663248u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c663248;
L8c663248: goto L8c663266; /* bra 0x8c663266 */
 goto L8c66324c;
L8c66324c: r[2]=rd(m,0x8c6632f4u,4); /* mov.l 0x8c6632f4,r2 */
 goto L8c66324e;
L8c66324e: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c663250;
L8c663250: r[3]=rd(m,0x8c6632f8u,4); /* mov.l 0x8c6632f8,r3 */
 goto L8c663252;
L8c663252: r[4]=rd(m,0x8c663300u,4); /* mov.l 0x8c663300,r4 */
 goto L8c663254;
L8c663254: r[1]=rd(m,0x8c6632fcu,4); /* mov.l 0x8c6632fc,r1 */
 goto L8c663256;
L8c663256: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c663258;
L8c663258: wr(m,r[9],r[11],4); /* mov.l r11,@r9 */
 goto L8c66325a;
L8c66325a: wr(m,r[13],r[4],4); /* mov.l r4,@r13 */
 goto L8c66325c;
L8c66325c: wr(m,r[8],r[4],4); /* mov.l r4,@r8 */
 goto L8c66325e;
L8c66325e: wr(m,r[10],r[4],4); /* mov.l r4,@r10 */
 goto L8c663260;
L8c663260: r[4]=sx(m,0x8c6632e0u,2); /* mov.w 0x8c6632e0,r4 */
 goto L8c663262;
L8c663262: target=r[1];*pr=0x8c663266u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c663266;
L8c663266: *pr=0x8c66326au;status=call(ctx,0x8c663080u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c663080 */
 goto L8c66326a;
L8c66326a: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c66326c;
L8c66326c: wr(m,r[12]+0x10u,r[2],4); /* mov.l r2,@(0x10,r12) */
 goto L8c66326e;
L8c66326e: r[2]=rd(m,0x8c663304u,4); /* mov.l 0x8c663304,r2 */
 goto L8c663270;
L8c663270: r[3]=rd(m,r[10],4); /* mov.l @r10,r3 */
 goto L8c663272;
L8c663272: target=r[2];*pr=0x8c663276u;wr(m,r[12]+0x24u,r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c663276;
L8c663276: r[4]=r[0]; /* mov r0,r4 */
 goto L8c663278;
L8c663278: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66327a;
L8c66327a: if((*sr&1u))goto L8c66329c; /* bt 0x8c66329c */
 goto L8c66327c;
L8c66327c: r[2]=rd(m,r[9],4); /* mov.l @r9,r2 */
 goto L8c66327e;
L8c66327e: r[5]=r[12]; /* mov r12,r5 */
 goto L8c663280;
L8c663280: wr(m,r[4]+0xcu,r[2],4); /* mov.l r2,@(0xc,r4) */
 goto L8c663282;
L8c663282: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c663284;
L8c663284: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto L8c663286;
L8c663286: r[2]=rd(m,r[5]+0x8u,4); /* mov.l @(0x8,r5),r2 */
 goto L8c663288;
L8c663288: wr(m,r[4]+0x8u,r[2],4); /* mov.l r2,@(0x8,r4) */
 goto L8c66328a;
L8c66328a: r[2]=rd(m,0x8c663304u,4); /* mov.l 0x8c663304,r2 */
 goto L8c66328c;
L8c66328c: wr(m,r[5]+0x8u,r[4],4); /* mov.l r4,@(0x8,r5) */
 goto L8c66328e;
L8c66328e: r[3]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r3 */
 goto L8c663290;
L8c663290: wr(m,r[4]+0x4u,r[3],4); /* mov.l r3,@(0x4,r4) */
 goto L8c663292;
L8c663292: target=r[2];*pr=0x8c663296u;wr(m,r[5]+0xcu,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c663296;
L8c663296: r[4]=r[0]; /* mov r0,r4 */
 goto L8c663298;
L8c663298: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66329a;
L8c66329a: if(!(*sr&1u))goto L8c6632a0; /* bf 0x8c6632a0 */
 goto L8c66329c;
L8c66329c: r[0]=3u;goto L8c6632ba; /* bra 0x8c6632ba */
 goto L8c6632a0;
L8c6632a0: r[2]=rd(m,r[8],4); /* mov.l @r8,r2 */
 goto L8c6632a2;
L8c6632a2: r[5]=r[12]; /* mov r12,r5 */
 goto L8c6632a4;
L8c6632a4: r[5]+=20u; /* add #0x14,r5 */
 goto L8c6632a6;
L8c6632a6: wr(m,r[4]+0xcu,r[2],4); /* mov.l r2,@(0xc,r4) */
 goto L8c6632a8;
L8c6632a8: r[3]=rd(m,r[10],4); /* mov.l @r10,r3 */
 goto L8c6632aa;
L8c6632aa: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto L8c6632ac;
L8c6632ac: r[2]=rd(m,r[5]+0x8u,4); /* mov.l @(0x8,r5),r2 */
 goto L8c6632ae;
L8c6632ae: r[0]=0u; /* mov #0x0,r0 */
 goto L8c6632b0;
L8c6632b0: wr(m,r[4]+0x8u,r[2],4); /* mov.l r2,@(0x8,r4) */
 goto L8c6632b2;
L8c6632b2: wr(m,r[5]+0x8u,r[4],4); /* mov.l r4,@(0x8,r5) */
 goto L8c6632b4;
L8c6632b4: r[3]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r3 */
 goto L8c6632b6;
L8c6632b6: wr(m,r[4]+0x4u,r[3],4); /* mov.l r3,@(0x4,r4) */
 goto L8c6632b8;
L8c6632b8: wr(m,r[5]+0xcu,r[4],4); /* mov.l r4,@(0xc,r5) */
 goto L8c6632ba;
L8c6632ba: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6632bc;
L8c6632bc: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c6632be;
L8c6632be: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6632c0;
L8c6632c0: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6632c2;
L8c6632c2: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6632c4;
L8c6632c4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6632c6;
L8c6632c6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6632c8;
L8c6632c8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
