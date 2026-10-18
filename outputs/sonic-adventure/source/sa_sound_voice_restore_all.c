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
/* Retail voice-volume restore entry 8c094720..8c094762.
 * Restores 38 saved levels through subordinate volume services. */
SA_STAGE_EXPORT int sa_sound_voice_restore_all(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c094720;
L8c094720: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c094722;
L8c094722: r[4]=0u; /* mov #0x0,r4 */
 goto L8c094724;
L8c094724: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c094726;
L8c094726: r[14]=r[4]; /* mov r4,r14 */
 goto L8c094728;
L8c094728: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c09472a;
L8c09472a: r[12]=38u; /* mov #0x26,r12 */
 goto L8c09472c;
L8c09472c: r[3]=rd(m,0x8c094860u,4); /* mov.l 0x8c094860,r3 */
 goto L8c09472e;
L8c09472e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c094730;
L8c094730: r[13]=rd(m,0x8c094864u,4); /* mov.l 0x8c094864,r13 */
 goto L8c094732;
L8c094732: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c094734;
L8c094734: r[0]=r[14]; /* mov r14,r0 */
 goto L8c094736;
L8c094736: r[0]<<=2; /* shll2 r0 */
 goto L8c094738;
L8c094738: r[5]=r[14]; /* mov r14,r5 */
 goto L8c09473a;
L8c09473a: *pr=0x8c09473eu;r[4]=rd(m,r[13]+r[0],4);status=call(ctx,0x8c093be0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093be0 */
 goto L8c09473e;
L8c09473e: r[14]+=1u; /* add #0x1,r14 */
 goto L8c094740;
L8c094740: r[0]=r[14]; /* mov r14,r0 */
 goto L8c094742;
L8c094742: r[0]<<=2; /* shll2 r0 */
 goto L8c094744;
L8c094744: r[5]=r[14]; /* mov r14,r5 */
 goto L8c094746;
L8c094746: *pr=0x8c09474au;r[4]=rd(m,r[13]+r[0],4);status=call(ctx,0x8c093be0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093be0 */
 goto L8c09474a;
L8c09474a: r[14]+=1u; /* add #0x1,r14 */
 goto L8c09474c;
L8c09474c: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]); /* cmp/ge r12,r14 */
 goto L8c09474e;
L8c09474e: if(!(*sr&1u))goto L8c094734; /* bf 0x8c094734 */
 goto L8c094750;
L8c094750: r[3]=rd(m,0x8c094868u,4); /* mov.l 0x8c094868,r3 */
 goto L8c094752;
L8c094752: target=r[3];*pr=0x8c094756u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c094756;
L8c094756: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c094758;
L8c094758: r[0]=0u; /* mov #0x0,r0 */
 goto L8c09475a;
L8c09475a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c09475c;
L8c09475c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09475e;
L8c09475e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
