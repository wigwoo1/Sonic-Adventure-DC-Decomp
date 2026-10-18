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
/* Retail filesystem device-status formatter, 8c6450ea..8c6451a4.
 * BIOS status service is dispatched through the external call boundary. */
SA_STAGE_EXPORT int sa_file_device_status(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6450ea;
L8c6450ea: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6450ec;
L8c6450ec: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6450ee;
L8c6450ee: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c6450f0;
L8c6450f0: r[3]=rd(m,0x8c6451f4u,4); /* mov.l 0x8c6451f4,r3 */
 goto L8c6450f2;
L8c6450f2: r[14]=r[5]; /* mov r5,r14 */
 goto L8c6450f4;
L8c6450f4: target=r[3];*pr=0x8c6450f8u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6450f8;
L8c6450f8: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6450fa;
L8c6450fa: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6450fc;
L8c6450fc: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c6450fe;
L8c6450fe: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c645100;
L8c645100: if((*sr&1u))goto L8c64511a; /* bt 0x8c64511a */
 goto L8c645102;
L8c645102: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c645104;
L8c645104: if((*sr&1u))goto L8c64511e; /* bt 0x8c64511e */
 goto L8c645106;
L8c645106: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c645108;
L8c645108: if((*sr&1u))goto L8c645128; /* bt 0x8c645128 */
 goto L8c64510a;
L8c64510a: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c64510c;
L8c64510c: if((*sr&1u))goto L8c64512e; /* bt 0x8c64512e */
 goto L8c64510e;
L8c64510e: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c645110;
L8c645110: if((*sr&1u))goto L8c645132; /* bt 0x8c645132 */
 goto L8c645112;
L8c645112: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c645114;
L8c645114: if((*sr&1u))goto L8c645136; /* bt 0x8c645136 */
 goto L8c645116;
L8c645116: goto L8c64519a; /* bra 0x8c64519a */
 goto L8c64511a;
L8c64511a: r[4]=0u;goto L8c64519a; /* bra 0x8c64519a */
 goto L8c64511e;
L8c64511e: wr(m,r[14],r[5],4); /* mov.l r5,@r14 */
 goto L8c645120;
L8c645120: r[4]=4u; /* mov #0x4,r4 */
 goto L8c645122;
L8c645122: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c645124;
L8c645124: wr(m,r[14]+0x8u,r[2],4);goto L8c64519a; /* bra 0x8c64519a */
 goto L8c645128;
L8c645128: r[4]=1u; /* mov #0x1,r4 */
 goto L8c64512a;
L8c64512a: wr(m,r[14],r[5],4);goto L8c64519a; /* bra 0x8c64519a */
 goto L8c64512e;
L8c64512e: r[4]=65u;goto L8c64519a; /* bra 0x8c64519a */
 goto L8c645132;
L8c645132: r[4]=67u;goto L8c64519a; /* bra 0x8c64519a */
 goto L8c645136;
L8c645136: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c645138;
L8c645138: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c64513a;
L8c64513a: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c64513c;
L8c64513c: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c64513e;
L8c64513e: take=(*sr&1u);r[4]=5u;if(take)goto L8c64516a;goto L8c645142; /* bt/s 0x8c64516a */
 goto L8c645142;
L8c645142: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c645144;
L8c645144: if((*sr&1u))goto L8c645170; /* bt 0x8c645170 */
 goto L8c645146;
L8c645146: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c645148;
L8c645148: if((*sr&1u))goto L8c645174; /* bt 0x8c645174 */
 goto L8c64514a;
L8c64514a: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c64514c;
L8c64514c: if((*sr&1u))goto L8c645178; /* bt 0x8c645178 */
 goto L8c64514e;
L8c64514e: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c645150;
L8c645150: if((*sr&1u))goto L8c64517c; /* bt 0x8c64517c */
 goto L8c645152;
L8c645152: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c645154;
L8c645154: if((*sr&1u))goto L8c645180; /* bt 0x8c645180 */
 goto L8c645156;
L8c645156: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c645158;
L8c645158: if((*sr&1u))goto L8c645184; /* bt 0x8c645184 */
 goto L8c64515a;
L8c64515a: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c64515c;
L8c64515c: if((*sr&1u))goto L8c645188; /* bt 0x8c645188 */
 goto L8c64515e;
L8c64515e: *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto L8c645160;
L8c645160: if((*sr&1u))goto L8c64518c; /* bt 0x8c64518c */
 goto L8c645162;
L8c645162: *sr=(*sr&~1u)|(r[0]==0x10u); /* cmp/eq #0x10,r0 */
 goto L8c645164;
L8c645164: if((*sr&1u))goto L8c645190; /* bt 0x8c645190 */
 goto L8c645166;
L8c645166: goto L8c645196; /* bra 0x8c645196 */
 goto L8c64516a;
L8c64516a: r[3]=4294967269u; /* mov #-0x1b,r3 */
 goto L8c64516c;
L8c64516c: wr(m,r[14],r[3],4);goto L8c64519a; /* bra 0x8c64519a */
 goto L8c645170;
L8c645170: r[1]=4294967268u;goto L8c645192; /* bra 0x8c645192 */
 goto L8c645174;
L8c645174: r[2]=4294967267u;goto L8c645198; /* bra 0x8c645198 */
 goto L8c645178;
L8c645178: r[1]=4294967266u;goto L8c645192; /* bra 0x8c645192 */
 goto L8c64517c;
L8c64517c: r[2]=4294967265u;goto L8c645198; /* bra 0x8c645198 */
 goto L8c645180;
L8c645180: r[1]=4294967264u;goto L8c645192; /* bra 0x8c645192 */
 goto L8c645184;
L8c645184: r[2]=4294967263u;goto L8c645198; /* bra 0x8c645198 */
 goto L8c645188;
L8c645188: r[1]=4294967262u;goto L8c645192; /* bra 0x8c645192 */
 goto L8c64518c;
L8c64518c: r[2]=4294967261u;goto L8c645198; /* bra 0x8c645198 */
 goto L8c645190;
L8c645190: r[1]=4294967260u; /* mov #-0x24,r1 */
 goto L8c645192;
L8c645192: wr(m,r[14],r[1],4);goto L8c64519a; /* bra 0x8c64519a */
 goto L8c645196;
L8c645196: r[2]=4294967270u; /* mov #-0x1a,r2 */
 goto L8c645198;
L8c645198: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c64519a;
L8c64519a: r[0]=r[4]; /* mov r4,r0 */
 goto L8c64519c;
L8c64519c: r[15]+=16u; /* add #0x10,r15 */
 goto L8c64519e;
L8c64519e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6451a0;
L8c6451a0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
