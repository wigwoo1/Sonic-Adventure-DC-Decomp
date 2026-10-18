#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 8c66005c..8c66018e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_sub_response(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66005c;
L8c66005c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66005e;
L8c66005e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c660060;
L8c660060: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c660062;
L8c660062: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c660064;
L8c660064: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c660066;
L8c660066: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c660068;
L8c660068: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c66006a;
L8c66006a: r[10]=r[5]; /* mov r5,r10 */
 goto L8c66006c;
L8c66006c: r[14]=r[7]; /* mov r7,r14 */
 goto L8c66006e;
L8c66006e: r[13]=0u; /* mov #0x0,r13 */
 goto L8c660070;
L8c660070: wr(m,r[15],r[4],1); /* mov.b r4,@r15 */
 goto L8c660072;
L8c660072: r[12]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r12 */
 goto L8c660074;
L8c660074: r[11]=r[6]; /* mov r6,r11 */
 goto L8c660076;
L8c660076: r[5]=r[12]; /* mov r12,r5 */
 goto L8c660078;
L8c660078: *pr=0x8c66007cu;r[4]=r[7];status=call(ctx,0x8c65ff5au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65ff5a */
 goto L8c66007c;
L8c66007c: r[0]=r[13]; /* mov r13,r0 */
 goto L8c66007e;
L8c66007e: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c660080;
L8c660080: wr(m,r[14]+0x3u,r[0],1); /* mov.b r0,@(0x3,r14) */
 goto L8c660082;
L8c660082: r[0]=sx(m,r[10]+0x2u,1); /* mov.b @(0x2,r10),r0 */
 goto L8c660084;
L8c660084: r[5]=sx(m,r[15],1); /* mov.b @r15,r5 */
 goto L8c660086;
L8c660086: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c660088;
L8c660088: r[5]=r[5]&255u; /* extu.b r5,r5 */
 goto L8c66008a;
L8c66008a: *sr=(*sr&~1u)|!(r[5]&r[0]); /* tst r5,r0 */
 goto L8c66008c;
L8c66008c: take=(*sr&1u);r[4]=1u;if(take)goto L8c66009c;goto L8c660090; /* bt/s 0x8c66009c */
 goto L8c660090;
L8c660090: r[0]=r[4]; /* mov r4,r0 */
 goto L8c660092;
L8c660092: wr(m,r[14]+0x4u,r[0],1); /* mov.b r0,@(0x4,r14) */
 goto L8c660094;
L8c660094: wr(m,r[14],r[13],1); /* mov.b r13,@r14 */
 goto L8c660096;
L8c660096: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c660098;
L8c660098: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66009a;
L8c66009a: wr(m,r[14]+0x24u,r[3],4); /* mov.l r3,@(0x24,r14) */
 goto L8c66009c;
L8c66009c: r[0]=sx(m,r[10]+0x3u,1); /* mov.b @(0x3,r10),r0 */
 goto L8c66009e;
L8c66009e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6600a0;
L8c6600a0: *sr=(*sr&~1u)|!(r[5]&r[0]); /* tst r5,r0 */
 goto L8c6600a2;
L8c6600a2: if((*sr&1u))goto L8c6600b0; /* bt 0x8c6600b0 */
 goto L8c6600a4;
L8c6600a4: r[0]=4u; /* mov #0x4,r0 */
 goto L8c6600a6;
L8c6600a6: wr(m,r[14]+0x4u,r[0],1); /* mov.b r0,@(0x4,r14) */
 goto L8c6600a8;
L8c6600a8: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6600aa;
L8c6600aa: wr(m,r[14],r[13],1); /* mov.b r13,@r14 */
 goto L8c6600ac;
L8c6600ac: wr(m,r[14]+0x3u,r[0],1); /* mov.b r0,@(0x3,r14) */
 goto L8c6600ae;
L8c6600ae: wr(m,r[14]+0x24u,r[13],4); /* mov.l r13,@(0x24,r14) */
 goto L8c6600b0;
L8c6600b0: r[2]=sx(m,r[10],1); /* mov.b @r10,r2 */
 goto L8c6600b2;
L8c6600b2: r[6]=sx(m,0x8c66011cu,2); /* mov.w 0x8c66011c,r6 */
 goto L8c6600b4;
L8c6600b4: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c6600b6;
L8c6600b6: *sr=(*sr&~1u)|!(r[5]&r[2]); /* tst r5,r2 */
 goto L8c6600b8;
L8c6600b8: if((*sr&1u))goto L8c660124; /* bt 0x8c660124 */
 goto L8c6600ba;
L8c6600ba: r[0]=sx(m,r[14]+0x7u,1); /* mov.b @(0x7,r14),r0 */
 goto L8c6600bc;
L8c6600bc: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6600be;
L8c6600be: if(!(*sr&1u))goto L8c6600ca; /* bf 0x8c6600ca */
 goto L8c6600c0;
L8c6600c0: r[2]=sx(m,r[14],1); /* mov.b @r14,r2 */
 goto L8c6600c2;
L8c6600c2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6600c4;
L8c6600c4: if(!(*sr&1u))goto L8c660116; /* bf 0x8c660116 */
 goto L8c6600c6;
L8c6600c6: goto L8c66012a; /* bra 0x8c66012a */
 goto L8c6600ca;
L8c6600ca: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6600cc;
L8c6600cc: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6600ce;
L8c6600ce: if(!(*sr&1u))goto L8c6600fa; /* bf 0x8c6600fa */
 goto L8c6600d0;
L8c6600d0: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto L8c6600d2;
L8c6600d2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6600d4;
L8c6600d4: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c6600d6;
L8c6600d6: if(!(*sr&1u))goto L8c6600e0; /* bf 0x8c6600e0 */
 goto L8c6600d8;
L8c6600d8: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6600da;
L8c6600da: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c6600dc;
L8c6600dc: wr(m,r[14],r[0],1);goto L8c660116; /* bra 0x8c660116 */
 goto L8c6600e0;
L8c6600e0: r[1]=rd(m,0x8c660120u,4); /* mov.l 0x8c660120,r1 */
 goto L8c6600e2;
L8c6600e2: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c6600e4;
L8c6600e4: r[0]=sx(m,0x8c66011eu,2); /* mov.w 0x8c66011e,r0 */
 goto L8c6600e6;
L8c6600e6: r[3]+=1u; /* add #0x1,r3 */
 goto L8c6600e8;
L8c6600e8: wr(m,r[14]+0x24u,r[3],4); /* mov.l r3,@(0x24,r14) */
 goto L8c6600ea;
L8c6600ea: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c6600ec;
L8c6600ec: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c6600ee;
L8c6600ee: r[0]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r0 */
 goto L8c6600f0;
L8c6600f0: *sr=(*sr&~1u)|(r[0]>r[3]); /* cmp/hi r3,r0 */
 goto L8c6600f2;
L8c6600f2: if((*sr&1u))goto L8c660106; /* bt 0x8c660106 */
 goto L8c6600f4;
L8c6600f4: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6600f6;
L8c6600f6: wr(m,r[14]+0x4u,r[0],1);goto L8c66012a; /* bra 0x8c66012a */
 goto L8c6600fa;
L8c6600fa: r[0]=sx(m,r[14]+0x7u,1); /* mov.b @(0x7,r14),r0 */
 goto L8c6600fc;
L8c6600fc: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6600fe;
L8c6600fe: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c660100;
L8c660100: if(!(*sr&1u))goto L8c660110; /* bf 0x8c660110 */
 goto L8c660102;
L8c660102: r[0]=r[4]; /* mov r4,r0 */
 goto L8c660104;
L8c660104: wr(m,r[14]+0x3u,r[0],1); /* mov.b r0,@(0x3,r14) */
 goto L8c660106;
L8c660106: r[0]=r[6]; /* mov r6,r0 */
 goto L8c660108;
L8c660108: wr(m,r[14],r[13],1); /* mov.b r13,@r14 */
 goto L8c66010a;
L8c66010a: wr(m,r[14]+0x4u,r[0],1); /* mov.b r0,@(0x4,r14) */
 goto L8c66010c;
L8c66010c: wr(m,r[14]+0x24u,r[13],4);goto L8c66012a; /* bra 0x8c66012a */
 goto L8c660110;
L8c660110: r[0]=sx(m,r[14]+0x7u,1); /* mov.b @(0x7,r14),r0 */
 goto L8c660112;
L8c660112: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660114;
L8c660114: if((*sr&1u))goto L8c66012a; /* bt 0x8c66012a */
 goto L8c660116;
L8c660116: r[0]=r[13]; /* mov r13,r0 */
 goto L8c660118;
L8c660118: wr(m,r[14]+0x4u,r[0],1);goto L8c66012a; /* bra 0x8c66012a */
 goto L8c660124;
L8c660124: r[0]=r[6]; /* mov r6,r0 */
 goto L8c660126;
L8c660126: wr(m,r[14]+0x4u,r[0],1); /* mov.b r0,@(0x4,r14) */
 goto L8c660128;
L8c660128: wr(m,r[14],r[13],1); /* mov.b r13,@r14 */
 goto L8c66012a;
L8c66012a: r[3]=sx(m,r[14],1); /* mov.b @r14,r3 */
 goto L8c66012c;
L8c66012c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66012e;
L8c66012e: if((*sr&1u))goto L8c66015c; /* bt 0x8c66015c */
 goto L8c660130;
L8c660130: r[0]=sx(m,r[14]+0x7u,1); /* mov.b @(0x7,r14),r0 */
 goto L8c660132;
L8c660132: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660134;
L8c660134: if((*sr&1u))goto L8c66015c; /* bt 0x8c66015c */
 goto L8c660136;
L8c660136: r[3]=rd(m,0x8c66022cu,4); /* mov.l 0x8c66022c,r3 */
 goto L8c660138;
L8c660138: r[1]=sx(m,0x8c660228u,2); /* mov.w 0x8c660228,r1 */
 goto L8c66013a;
L8c66013a: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c66013c;
L8c66013c: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c66013e;
L8c66013e: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c660140;
L8c660140: if(!(*sr&1u))goto L8c66015c; /* bf 0x8c66015c */
 goto L8c660142;
L8c660142: r[0]=rd(m,r[12],4); /* mov.l @r12,r0 */
 goto L8c660144;
L8c660144: r[4]=r[0]; /* mov r0,r4 */
 goto L8c660146;
L8c660146: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c660148;
L8c660148: if((*sr&1u))goto L8c66015c; /* bt 0x8c66015c */
 goto L8c66014a;
L8c66014a: r[2]=sx(m,0x8c66022au,2); /* mov.w 0x8c66022a,r2 */
 goto L8c66014c;
L8c66014c: *sr=(*sr&~1u)|(r[2]==r[4]); /* cmp/eq r2,r4 */
 goto L8c66014e;
L8c66014e: if((*sr&1u))goto L8c66015c; /* bt 0x8c66015c */
 goto L8c660150;
L8c660150: r[5]=rd(m,r[12],4); /* mov.l @r12,r5 */
 goto L8c660152;
L8c660152: r[3]=rd(m,0x8c660230u,4); /* mov.l 0x8c660230,r3 */
 goto L8c660154;
L8c660154: r[5]=r[5]&255u; /* extu.b r5,r5 */
 goto L8c660156;
L8c660156: r[5]+=1u; /* add #0x1,r5 */
 goto L8c660158;
L8c660158: target=r[3];*pr=0x8c66015cu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66015c;
L8c66015c: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c66015e;
L8c66015e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660160;
L8c660160: if((*sr&1u))goto L8c66016e; /* bt 0x8c66016e */
 goto L8c660162;
L8c660162: r[0]=16u; /* mov #0x10,r0 */
 goto L8c660164;
L8c660164: r[5]=r[14]; /* mov r14,r5 */
 goto L8c660166;
L8c660166: r[6]=r[12]; /* mov r12,r6 */
 goto L8c660168;
L8c660168: wr(m,r[11]+r[0],r[13],1); /* mov.b r13,@(r0,r11) */
 goto L8c66016a;
L8c66016a: *pr=0x8c66016eu;r[4]=r[11];status=call(ctx,0x8c66018eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c66018e */
 goto L8c66016e;
L8c66016e: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c660170;
L8c660170: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660172;
L8c660172: if((*sr&1u))goto L8c66017c; /* bt 0x8c66017c */
 goto L8c660174;
L8c660174: r[0]=16u; /* mov #0x10,r0 */
 goto L8c660176;
L8c660176: wr(m,r[11]+r[0],r[13],1); /* mov.b r13,@(r0,r11) */
 goto L8c660178;
L8c660178: *pr=0x8c66017cu;r[4]=r[11];status=call(ctx,0x8c6602d2u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6602d2 */
 goto L8c66017c;
L8c66017c: r[0]=0u; /* mov #0x0,r0 */
 goto L8c66017e;
L8c66017e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c660180;
L8c660180: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660182;
L8c660182: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c660184;
L8c660184: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c660186;
L8c660186: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c660188;
L8c660188: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66018a;
L8c66018a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
