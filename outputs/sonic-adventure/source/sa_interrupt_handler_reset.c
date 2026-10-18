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
/* Static C recovery of retail interrupt handler helper 8c6411b0..8c6412c2.
 * Subordinate routines use the supplied service dispatcher. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_handler_reset(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6411b0;
L8c6411b0: r[0]=r[5]; /* mov r5,r0 */
 goto L8c6411b2;
L8c6411b2: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c6411b4;
L8c6411b4: if((*sr&1u))goto L8c641240; /* bt 0x8c641240 */
 goto L8c6411b6;
L8c6411b6: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6411b8;
L8c6411b8: if((*sr&1u))goto L8c641248; /* bt 0x8c641248 */
 goto L8c6411ba;
L8c6411ba: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c6411bc;
L8c6411bc: if((*sr&1u))goto L8c641250; /* bt 0x8c641250 */
 goto L8c6411be;
L8c6411be: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c6411c0;
L8c6411c0: if((*sr&1u))goto L8c641258; /* bt 0x8c641258 */
 goto L8c6411c2;
L8c6411c2: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c6411c4;
L8c6411c4: if((*sr&1u))goto L8c641260; /* bt 0x8c641260 */
 goto L8c6411c6;
L8c6411c6: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c6411c8;
L8c6411c8: if(!(*sr&1u))goto L8c6411ce; /* bf 0x8c6411ce */
 goto L8c6411ca;
L8c6411ca: goto L8c6412a0; /* bra 0x8c6412a0 */
 goto L8c6411ce;
L8c6411ce: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c6411d0;
L8c6411d0: if(!(*sr&1u))goto L8c6411d6; /* bf 0x8c6411d6 */
 goto L8c6411d2;
L8c6411d2: goto L8c6412a8; /* bra 0x8c6412a8 */
 goto L8c6411d6;
L8c6411d6: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c6411d8;
L8c6411d8: if(!(*sr&1u))goto L8c6411de; /* bf 0x8c6411de */
 goto L8c6411da;
L8c6411da: goto L8c6412b0; /* bra 0x8c6412b0 */
 goto L8c6411de;
L8c6411de: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c6411e0;
L8c6411e0: if(!(*sr&1u))goto L8c6411e6; /* bf 0x8c6411e6 */
 goto L8c6411e2;
L8c6411e2: goto L8c6412b8; /* bra 0x8c6412b8 */
 goto L8c6411e6;
L8c6411e6: goto L8c6412be; /* bra 0x8c6412be */
 goto L8c641240;
L8c641240: r[0]=sx(m,0x8c641280u,2); /* mov.w 0x8c641280,r0 */
 goto L8c641242;
L8c641242: r[3]=rd(m,0x8c64128cu,4); /* mov.l 0x8c64128c,r3 */
 goto L8c641244;
L8c641244: wr(m,r[4]+r[0],r[3],4);goto L8c6412be; /* bra 0x8c6412be */
 goto L8c641248;
L8c641248: r[0]=sx(m,0x8c641282u,2); /* mov.w 0x8c641282,r0 */
 goto L8c64124a;
L8c64124a: r[1]=rd(m,0x8c641290u,4); /* mov.l 0x8c641290,r1 */
 goto L8c64124c;
L8c64124c: goto L8c6412b4; /* bra 0x8c6412b4 */
 goto L8c641250;
L8c641250: r[0]=sx(m,0x8c641284u,2); /* mov.w 0x8c641284,r0 */
 goto L8c641252;
L8c641252: r[2]=rd(m,0x8c641294u,4); /* mov.l 0x8c641294,r2 */
 goto L8c641254;
L8c641254: goto L8c6412bc; /* bra 0x8c6412bc */
 goto L8c641258;
L8c641258: r[0]=sx(m,0x8c641286u,2); /* mov.w 0x8c641286,r0 */
 goto L8c64125a;
L8c64125a: r[1]=rd(m,0x8c641298u,4); /* mov.l 0x8c641298,r1 */
 goto L8c64125c;
L8c64125c: goto L8c6412b4; /* bra 0x8c6412b4 */
 goto L8c641260;
L8c641260: r[0]=sx(m,0x8c641288u,2); /* mov.w 0x8c641288,r0 */
 goto L8c641262;
L8c641262: r[2]=rd(m,0x8c64129cu,4); /* mov.l 0x8c64129c,r2 */
 goto L8c641264;
L8c641264: goto L8c6412bc; /* bra 0x8c6412bc */
 goto L8c6412a0;
L8c6412a0: r[0]=sx(m,0x8c641300u,2); /* mov.w 0x8c641300,r0 */
 goto L8c6412a2;
L8c6412a2: r[1]=rd(m,0x8c64130cu,4); /* mov.l 0x8c64130c,r1 */
 goto L8c6412a4;
L8c6412a4: goto L8c6412b4; /* bra 0x8c6412b4 */
 goto L8c6412a8;
L8c6412a8: r[0]=sx(m,0x8c641302u,2); /* mov.w 0x8c641302,r0 */
 goto L8c6412aa;
L8c6412aa: r[2]=rd(m,0x8c641310u,4); /* mov.l 0x8c641310,r2 */
 goto L8c6412ac;
L8c6412ac: goto L8c6412bc; /* bra 0x8c6412bc */
 goto L8c6412b0;
L8c6412b0: r[0]=sx(m,0x8c641304u,2); /* mov.w 0x8c641304,r0 */
 goto L8c6412b2;
L8c6412b2: r[1]=rd(m,0x8c641314u,4); /* mov.l 0x8c641314,r1 */
 goto L8c6412b4;
L8c6412b4: wr(m,r[4]+r[0],r[1],4);goto L8c6412be; /* bra 0x8c6412be */
 goto L8c6412b8;
L8c6412b8: r[0]=sx(m,0x8c641306u,2); /* mov.w 0x8c641306,r0 */
 goto L8c6412ba;
L8c6412ba: r[2]=rd(m,0x8c641318u,4); /* mov.l 0x8c641318,r2 */
 goto L8c6412bc;
L8c6412bc: wr(m,r[4]+r[0],r[2],4); /* mov.l r2,@(r0,r4) */
 goto L8c6412be;
L8c6412be: return 0; /* rts */
}
