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
/* Static C recovery of USA Rev A transfer lifecycle. Hardware reads require IO specialization before runtime integration. */
SA_STAGE_EXPORT int sa_engine_transfer_channels_arm(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ee9c;
L8c10ee9c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10ee9e;
L8c10ee9e: r[14]=0u; /* mov #0x0,r14 */
 goto L8c10eea0;
L8c10eea0: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10eea2;
L8c10eea2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10eea4;
L8c10eea4: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10eea6;
L8c10eea6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10eea8;
L8c10eea8: r[13]=sx(m,0x8c10eed0u,2); /* mov.w 0x8c10eed0,r13 */
 goto L8c10eeaa;
L8c10eeaa: r[12]=rd(m,0x8c10eed8u,4); /* mov.l 0x8c10eed8,r12 */
 goto L8c10eeac;
L8c10eeac: r[11]=rd(m,0x8c10eed4u,4); /* mov.l 0x8c10eed4,r11 */
 goto L8c10eeae;
L8c10eeae: r[4]=rd(m,r[12],4); /* mov.l @r12,r4 */
 goto L8c10eeb0;
L8c10eeb0: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10eeb2;
L8c10eeb2: target=r[11];*pr=0x8c10eeb6u;r[4]+=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c10eeb6;
L8c10eeb6: r[14]+=52u; /* add #0x34,r14 */
 goto L8c10eeb8;
L8c10eeb8: *sr=(*sr&~1u)|(r[14]>=r[13]); /* cmp/hs r13,r14 */
 goto L8c10eeba;
L8c10eeba: if(!(*sr&1u))goto L8c10eeae; /* bf 0x8c10eeae */
 goto L8c10eebc;
L8c10eebc: r[3]=rd(m,0x8c10eedcu,4); /* mov.l 0x8c10eedc,r3 */
 goto L8c10eebe;
L8c10eebe: r[2]=1u; /* mov #0x1,r2 */
 goto L8c10eec0;
L8c10eec0: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10eec2;
L8c10eec2: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c10eec4;
L8c10eec4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10eec6;
L8c10eec6: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10eec8;
L8c10eec8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10eeca;
L8c10eeca: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10eecc;
L8c10eecc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
