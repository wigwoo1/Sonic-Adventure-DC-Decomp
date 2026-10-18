#include "fpu_cleanup_transfer.h"
#include "input_io.h"
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
static void wr(uint8_t *m,const struct sa_input_io *io,uint32_t a,uint32_t v,unsigned n){if((a&0x1fffffffu)<0x0c000000u||(a&0x1fffffffu)>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail callback registrar 8c653640..8c65368c.
 * Explicit VBR and MMIO interface preserve interrupt-register writes;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_callback_register(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t vbr,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c653640;
L8c653640: r[0]=*sr; /* stc SR,r0 */
 goto L8c653642;
L8c653642: r[3]=sx(m,0x8c6536a0u,2); /* mov.w 0x8c6536a0,r3 */
 goto L8c653644;
L8c653644: r[0]>>=2; /* shlr2 r0 */
 goto L8c653646;
L8c653646: r[0]>>=2; /* shlr2 r0 */
 goto L8c653648;
L8c653648: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65364a;
L8c65364a: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65364c;
L8c65364c: r[0]=*sr; /* stc SR,r0 */
 goto L8c65364e;
L8c65364e: r[0]&=r[3]; /* and r3,r0 */
 goto L8c653650;
L8c653650: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c653652;
L8c653652: *sr=r[0]; /* ldc r0,SR */
 goto L8c653654;
L8c653654: r[2]=vbr; /* stc VBR,r2 */
 goto L8c653656;
L8c653656: r[0]=sx(m,0x8c6536a2u,2); /* mov.w 0x8c6536a2,r0 */
 goto L8c653658;
L8c653658: r[6]=rd(m,0x8c6536a4u,4); /* mov.l 0x8c6536a4,r6 */
 goto L8c65365a;
L8c65365a: wr(m,io,r[2]+r[0],r[4],4); /* mov.l r4,@(r0,r2) */
 goto L8c65365c;
L8c65365c: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65365e;
L8c65365e: wr(m,io,r[6],r[4],4); /* mov.l r4,@r6 */
 goto L8c653660;
L8c653660: r[7]=rd(m,0x8c6536a8u,4); /* mov.l 0x8c6536a8,r7 */
 goto L8c653662;
L8c653662: wr(m,io,r[7],r[4],4); /* mov.l r4,@r7 */
 goto L8c653664;
L8c653664: r[3]=rd(m,0x8c6536acu,4); /* mov.l 0x8c6536ac,r3 */
 goto L8c653666;
L8c653666: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c653668;
L8c653668: wr(m,io,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65366a;
L8c65366a: r[2]=rd(m,0x8c6536b0u,4); /* mov.l 0x8c6536b0,r2 */
 goto L8c65366c;
L8c65366c: wr(m,io,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c65366e;
L8c65366e: r[1]=rd(m,0x8c6536b4u,4); /* mov.l 0x8c6536b4,r1 */
 goto L8c653670;
L8c653670: wr(m,io,r[6],r[1],4); /* mov.l r1,@r6 */
 goto L8c653672;
L8c653672: r[0]=14u; /* mov #0xe,r0 */
 goto L8c653674;
L8c653674: wr(m,io,r[7],r[0],4); /* mov.l r0,@r7 */
 goto L8c653676;
L8c653676: r[1]=*sr; /* stc SR,r1 */
 goto L8c653678;
L8c653678: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65367a;
L8c65367a: r[3]=sx(m,0x8c6536a0u,2); /* mov.w 0x8c6536a0,r3 */
 goto L8c65367c;
L8c65367c: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65367e;
L8c65367e: r[0]<<=2; /* shll2 r0 */
 goto L8c653680;
L8c653680: r[1]&=r[3]; /* and r3,r1 */
 goto L8c653682;
L8c653682: r[0]<<=2; /* shll2 r0 */
 goto L8c653684;
L8c653684: r[0]|=r[1]; /* or r1,r0 */
 goto L8c653686;
L8c653686: *sr=r[0]; /* ldc r0,SR */
 goto L8c653688;
L8c653688: return 0; /* rts */
}
