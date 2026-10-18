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
/* Retail sound low-index/MIDI path. Packer varargs follow the recovered SH-4 ABI. */
SA_STAGE_EXPORT int sa_sound_midi_enqueue(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643b9c;
L8c643b9c: r[2]=rd(m,0x8c643bf4u,4); /* mov.l 0x8c643bf4,r2 */
 goto L8c643b9e;
L8c643b9e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643ba0;
L8c643ba0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643ba2;
L8c643ba2: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643ba4;
L8c643ba4: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643ba6;
L8c643ba6: if(!(*sr&1u))goto L8c643bae; /* bf 0x8c643bae */
 goto L8c643ba8;
L8c643ba8: r[14]=rd(m,0x8c643bf8u,4); /* mov.l 0x8c643bf8,r14 */
 goto L8c643baa;
L8c643baa: goto L8c643bde; /* bra 0x8c643bde */
 goto L8c643bae;
L8c643bae: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643bb0;
L8c643bb0: if(!(*sr&1u))goto L8c643bb8; /* bf 0x8c643bb8 */
 goto L8c643bb2;
L8c643bb2: r[14]=rd(m,0x8c643bfcu,4); /* mov.l 0x8c643bfc,r14 */
 goto L8c643bb4;
L8c643bb4: goto L8c643bde; /* bra 0x8c643bde */
 goto L8c643bb8;
L8c643bb8: r[2]=rd(m,0x8c643c00u,4); /* mov.l 0x8c643c00,r2 */
 goto L8c643bba;
L8c643bba: r[3]=sx(m,0x8c643bf2u,2); /* mov.w 0x8c643bf2,r3 */
 goto L8c643bbc;
L8c643bbc: r[1]=rd(m,0x8c643c04u,4); /* mov.l 0x8c643c04,r1 */
 goto L8c643bbe;
L8c643bbe: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c643bc0;
L8c643bc0: r[2]+=9u; /* add #0x9,r2 */
 goto L8c643bc2;
L8c643bc2: r[3]=sx(m,r[5],1); /* mov.b @r5,r3 */
 goto L8c643bc4;
L8c643bc4: r[4]=rd(m,0x8c643c00u,4); /* mov.l 0x8c643c00,r4 */
 goto L8c643bc6;
L8c643bc6: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c643bc8;
L8c643bc8: r[0]=sx(m,r[5]+0x1u,1); /* mov.b @(0x1,r5),r0 */
 goto L8c643bca;
L8c643bca: r[3]=rd(m,0x8c643c08u,4); /* mov.l 0x8c643c08,r3 */
 goto L8c643bcc;
L8c643bcc: r[1]=rd(m,0x8c643c0cu,4); /* mov.l 0x8c643c0c,r1 */
 goto L8c643bce;
L8c643bce: wr(m,r[3],r[0],1); /* mov.b r0,@r3 */
 goto L8c643bd0;
L8c643bd0: r[3]+=1u; /* add #0x1,r3 */
 goto L8c643bd2;
L8c643bd2: r[0]=sx(m,r[5]+0x2u,1); /* mov.b @(0x2,r5),r0 */
 goto L8c643bd4;
L8c643bd4: wr(m,r[3],r[0],1); /* mov.b r0,@r3 */
 goto L8c643bd6;
L8c643bd6: r[0]=sx(m,r[5]+0x3u,1); /* mov.b @(0x3,r5),r0 */
 goto L8c643bd8;
L8c643bd8: target=r[1];*pr=0x8c643bdcu;wr(m,r[2],r[0],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c643bdc;
L8c643bdc: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643bde;
L8c643bde: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643be0;
L8c643be0: if((*sr&1u))goto L8c643bea; /* bt 0x8c643bea */
 goto L8c643be2;
L8c643be2: r[2]=rd(m,0x8c643c14u,4); /* mov.l 0x8c643c14,r2 */
 goto L8c643be4;
L8c643be4: r[4]=rd(m,0x8c643c10u,4); /* mov.l 0x8c643c10,r4 */
 goto L8c643be6;
L8c643be6: target=r[2];*pr=0x8c643beau;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643bea;
L8c643bea: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643bec;
L8c643bec: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643bee;
L8c643bee: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
