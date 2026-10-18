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
/* Static C recovery of retail Tikal helper 8c600038..8c600156.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_context_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c600038;
L8c600038: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c60003a;
L8c60003a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c60003c;
L8c60003c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c60003e;
L8c60003e: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c600040;
L8c600040: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c600042;
L8c600042: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c600044;
L8c600044: r[14]=rd(m,0x8c6000e4u,4); /* mov.l 0x8c6000e4,r14 */
 goto L8c600046;
L8c600046: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c600048;
L8c600048: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c60004a;
L8c60004a: wr(m,r[15]+0x10u,r[6],4); /* mov.l r6,@(0x10,r15) */
 goto L8c60004c;
L8c60004c: wr(m,r[15]+0xcu,r[7],4); /* mov.l r7,@(0xc,r15) */
 goto L8c60004e;
L8c60004e: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c600050;
L8c600050: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c600052;
L8c600052: if((*sr&1u))goto L8c600060; /* bt 0x8c600060 */
 goto L8c600054;
L8c600054: r[0]=56u; /* mov #0x38,r0 */
 goto L8c600056;
L8c600056: r[0]=sx(m,r[4]+r[0],2); /* mov.w @(r0,r4),r0 */
 goto L8c600058;
L8c600058: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c60005a;
L8c60005a: if(!(*sr&1u))goto L8c600060; /* bf 0x8c600060 */
 goto L8c60005c;
L8c60005c: r[0]=0u;goto L8c600148; /* bra 0x8c600148 */
 goto L8c600060;
L8c600060: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c600062;
L8c600062: *sr=(*sr&~1u)|!(r[0]&0x1fu); /* tst #0x1f,r0 */
 goto L8c600064;
L8c600064: if((*sr&1u))goto L8c60006a; /* bt 0x8c60006a */
 goto L8c600066;
L8c600066: r[0]=4294967282u;goto L8c600148; /* bra 0x8c600148 */
 goto L8c60006a;
L8c60006a: r[2]=rd(m,0x8c6000e8u,4); /* mov.l 0x8c6000e8,r2 */
 goto L8c60006c;
L8c60006c: target=r[2];*pr=0x8c600070u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c600070;
L8c600070: r[12]=r[0]; /* mov r0,r12 */
 goto L8c600072;
L8c600072: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c600074;
L8c600074: target=r[3];*pr=0x8c600078u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c600078;
L8c600078: r[0]=*sr; /* stc SR,r0 */
 goto L8c60007a;
L8c60007a: r[3]=sx(m,0x8c6000d6u,2); /* mov.w 0x8c6000d6,r3 */
 goto L8c60007c;
L8c60007c: r[0]>>=2; /* shlr2 r0 */
 goto L8c60007e;
L8c60007e: r[0]>>=2; /* shlr2 r0 */
 goto L8c600080;
L8c600080: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c600082;
L8c600082: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c600084;
L8c600084: r[0]=*sr; /* stc SR,r0 */
 goto L8c600086;
L8c600086: r[0]&=r[3]; /* and r3,r0 */
 goto L8c600088;
L8c600088: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c60008a;
L8c60008a: *sr=r[0]; /* ldc r0,SR */
 goto L8c60008c;
L8c60008c: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c60008e;
L8c60008e: r[0]=56u; /* mov #0x38,r0 */
 goto L8c600090;
L8c600090: r[13]=0u; /* mov #0x0,r13 */
 goto L8c600092;
L8c600092: r[3]=r[2]; /* mov r2,r3 */
 goto L8c600094;
L8c600094: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c600096;
L8c600096: wr(m,r[3]+r[0],r[13],2); /* mov.w r13,@(r0,r3) */
 goto L8c600098;
L8c600098: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c60009a;
L8c60009a: wr(m,r[3]+0x20u,r[13],4); /* mov.l r13,@(0x20,r3) */
 goto L8c60009c;
L8c60009c: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c60009e;
L8c60009e: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c6000a0;
L8c6000a0: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c6000a2;
L8c6000a2: r[8]=rd(m,r[14],4); /* mov.l @r14,r8 */
 goto L8c6000a4;
L8c6000a4: r[5]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r5 */
 goto L8c6000a6;
L8c6000a6: r[3]=rd(m,0x8c6000ecu,4); /* mov.l 0x8c6000ec,r3 */
 goto L8c6000a8;
L8c6000a8: r[8]+=16u; /* add #0x10,r8 */
 goto L8c6000aa;
L8c6000aa: target=r[3];*pr=0x8c6000aeu;r[4]=rd(m,r[15]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6000ae;
L8c6000ae: wr(m,r[8],r[0],4); /* mov.l r0,@r8 */
 goto L8c6000b0;
L8c6000b0: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6000b2;
L8c6000b2: wr(m,r[3]+0xcu,r[12],4); /* mov.l r12,@(0xc,r3) */
 goto L8c6000b4;
L8c6000b4: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c6000b6;
L8c6000b6: wr(m,r[2]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r2) */
 goto L8c6000b8;
L8c6000b8: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6000ba;
L8c6000ba: r[0]=58u; /* mov #0x3a,r0 */
 goto L8c6000bc;
L8c6000bc: r[5]=r[13]; /* mov r13,r5 */
 goto L8c6000be;
L8c6000be: wr(m,r[3]+r[0],r[13],2); /* mov.w r13,@(r0,r3) */
 goto L8c6000c0;
L8c6000c0: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6000c2;
L8c6000c2: wr(m,r[3]+0x24u,r[13],4); /* mov.l r13,@(0x24,r3) */
 goto L8c6000c4;
L8c6000c4: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c6000c6;
L8c6000c6: wr(m,r[2]+0x34u,r[13],4); /* mov.l r13,@(0x34,r2) */
 goto L8c6000c8;
L8c6000c8: r[2]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r2 */
 goto L8c6000ca;
L8c6000ca: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6000cc;
L8c6000cc: wr(m,r[3]+0x3cu,r[2],4); /* mov.l r2,@(0x3c,r3) */
 goto L8c6000ce;
L8c6000ce: r[3]=sx(m,0x8c6000d8u,2); /* mov.w 0x8c6000d8,r3 */
 goto L8c6000d0;
L8c6000d0: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c6000d2;
L8c6000d2: r[4]+=r[3];goto L8c6000fc; /* bra 0x8c6000fc */
 goto L8c6000f0;
L8c6000f0: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c6000f2;
L8c6000f2: wr(m,r[4]+r[0],r[13],2); /* mov.w r13,@(r0,r4) */
 goto L8c6000f4;
L8c6000f4: r[5]+=1u; /* add #0x1,r5 */
 goto L8c6000f6;
L8c6000f6: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6000f8;
L8c6000f8: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c6000fa;
L8c6000fa: r[4]+=84u; /* add #0x54,r4 */
 goto L8c6000fc;
L8c6000fc: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c6000fe;
L8c6000fe: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c600100;
L8c600100: r[3]+=1u; /* add #0x1,r3 */
 goto L8c600102;
L8c600102: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c600104;
L8c600104: if(!(*sr&1u))goto L8c6000f0; /* bf 0x8c6000f0 */
 goto L8c600106;
L8c600106: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c600108;
L8c600108: r[7]=0u; /* mov #0x0,r7 */
 goto L8c60010a;
L8c60010a: r[1]=sx(m,0x8c600234u,2); /* mov.w 0x8c600234,r1 */
 goto L8c60010c;
L8c60010c: r[5]=r[7]; /* mov r7,r5 */
 goto L8c60010e;
L8c60010e: r[2]=r[3]; /* mov r3,r2 */
 goto L8c600110;
L8c600110: r[6]=r[7]; /* mov r7,r6 */
 goto L8c600112;
L8c600112: r[1]+=r[2]; /* add r2,r1 */
 goto L8c600114;
L8c600114: wr(m,r[3]+0x14u,r[1],4); /* mov.l r1,@(0x14,r3) */
 goto L8c600116;
L8c600116: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c600118;
L8c600118: r[1]=sx(m,0x8c600234u,2); /* mov.w 0x8c600234,r1 */
 goto L8c60011a;
L8c60011a: r[2]=r[3]; /* mov r3,r2 */
 goto L8c60011c;
L8c60011c: r[1]+=r[2]; /* add r2,r1 */
 goto L8c60011e;
L8c60011e: r[1]+=84u; /* add #0x54,r1 */
 goto L8c600120;
L8c600120: wr(m,r[3]+0x18u,r[1],4); /* mov.l r1,@(0x18,r3) */
 goto L8c600122;
L8c600122: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c600124;
L8c600124: *pr=0x8c600128u;r[4]=rd(m,r[4]+0x14u,4);status=call(ctx,0x8c600464u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c600464 */
 goto L8c600128;
L8c600128: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c60012a;
L8c60012a: r[2]=*sr; /* stc SR,r2 */
 goto L8c60012c;
L8c60012c: r[3]=sx(m,0x8c600236u,2); /* mov.w 0x8c600236,r3 */
 goto L8c60012e;
L8c60012e: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c600130;
L8c600130: r[0]<<=2; /* shll2 r0 */
 goto L8c600132;
L8c600132: r[2]&=r[3]; /* and r3,r2 */
 goto L8c600134;
L8c600134: r[0]<<=2; /* shll2 r0 */
 goto L8c600136;
L8c600136: r[0]|=r[2]; /* or r2,r0 */
 goto L8c600138;
L8c600138: *sr=r[0]; /* ldc r0,SR */
 goto L8c60013a;
L8c60013a: r[15]+=20u; /* add #0x14,r15 */
 goto L8c60013c;
L8c60013c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c60013e;
L8c60013e: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c600140;
L8c600140: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c600142;
L8c600142: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c600144;
L8c600144: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c5ffd28u,m,f,sr,r,pr); /* bra 0x8c5ffd28 */
 goto L8c600148;
L8c600148: r[15]+=20u; /* add #0x14,r15 */
 goto L8c60014a;
L8c60014a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c60014c;
L8c60014c: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c60014e;
L8c60014e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c600150;
L8c600150: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c600152;
L8c600152: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
