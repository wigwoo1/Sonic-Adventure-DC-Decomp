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
/* USA Rev A sound-work record creation0940da; provisional name. Uses recovered slot selector093f02 and initializes the selected record. */
SA_STAGE_EXPORT int sa_sound_voice_record_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0940da;
L8c0940da: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0940dc;
L8c0940dc: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c0940de;
L8c0940de: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c0940e0;
L8c0940e0: wr(m,r[15]+0x8u,r[5],4); /* mov.l r5,@(0x8,r15) */
 goto L8c0940e2;
L8c0940e2: wr(m,r[15]+0xcu,r[6],4); /* mov.l r6,@(0xc,r15) */
 goto L8c0940e4;
L8c0940e4: wr(m,r[15]+0x4u,r[7],4); /* mov.l r7,@(0x4,r15) */
 goto L8c0940e6;
L8c0940e6: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c0940e8;
L8c0940e8: *pr=0x8c0940ecu;r[4]=rd(m,r[15]+0xcu,4);status=call(ctx,0x8c093f02u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093f02 */
 goto L8c0940ec;
L8c0940ec: r[5]=r[0]; /* mov r0,r5 */
 goto L8c0940ee;
L8c0940ee: *sr=(*sr&~1u)|((int32_t)r[5]>=0); /* cmp/pz r5 */
 goto L8c0940f0;
L8c0940f0: if((*sr&1u))goto L8c0940fa; /* bt 0x8c0940fa */
 goto L8c0940f2;
L8c0940f2: r[15]+=16u; /* add #0x10,r15 */
 goto L8c0940f4;
L8c0940f4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0940f6;
L8c0940f6: r[0]=4294967295u;return 0; /* rts */
L8c0940fa: r[4]=52u; /* mov #0x34,r4 */
 goto L8c0940fc;
L8c0940fc: r[3]=rd(m,0x8c094224u,4); /* mov.l 0x8c094224,r3 */
 goto L8c0940fe;
L8c0940fe: *macl=r[4]*r[5]; /* mul.l r4,r5 */
 goto L8c094100;
L8c094100: r[2]=120u; /* mov #0x78,r2 */
 goto L8c094102;
L8c094102: fr[4]=0; /* fldi0 fr4 */
 goto L8c094104;
L8c094104: r[0]=36u; /* mov #0x24,r0 */
 goto L8c094106;
L8c094106: r[1]=4294967295u; /* mov #-0x1,r1 */
 goto L8c094108;
L8c094108: r[4]=*macl; /* sts MACL,r4 */
 goto L8c09410a;
L8c09410a: r[5]=0u; /* mov #0x0,r5 */
 goto L8c09410c;
L8c09410c: r[4]+=r[3]; /* add r3,r4 */
 goto L8c09410e;
L8c09410e: wr(m,r[4]+0x4u,r[2],4); /* mov.l r2,@(0x4,r4) */
 goto L8c094110;
L8c094110: wr(m,r[4],r[1],4); /* mov.l r1,@r4 */
 goto L8c094112;
L8c094112: r[1]=17u; /* mov #0x11,r1 */
 goto L8c094114;
L8c094114: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c094116;
L8c094116: wr(m,r[4]+0x8u,r[2],4); /* mov.l r2,@(0x8,r4) */
 goto L8c094118;
L8c094118: wr(m,r[4]+0xcu,r[1],4); /* mov.l r1,@(0xc,r4) */
 goto L8c09411a;
L8c09411a: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c09411c;
L8c09411c: wr(m,r[4]+0x10u,r[2],4); /* mov.l r2,@(0x10,r4) */
 goto L8c09411e;
L8c09411e: wr(m,r[4]+0x14u,r[5],4); /* mov.l r5,@(0x14,r4) */
 goto L8c094120;
L8c094120: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c094122;
L8c094122: wr(m,r[4]+0x18u,r[2],4); /* mov.l r2,@(0x18,r4) */
 goto L8c094124;
L8c094124: wr(m,r[4]+0x1cu,r[5],4); /* mov.l r5,@(0x1c,r4) */
 goto L8c094126;
L8c094126: wr(m,r[4]+0x20u,r[5],4); /* mov.l r5,@(0x20,r4) */
 goto L8c094128;
L8c094128: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,r[0]+r[4],f->bank[bank][pair],4);wr(m,r[0]+r[4]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[4],fr[4],4); /* fmov fr4,@(r0,r4) */
 goto L8c09412a;
L8c09412a: r[0]=40u; /* mov #0x28,r0 */
 goto L8c09412c;
L8c09412c: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,r[0]+r[4],f->bank[bank][pair],4);wr(m,r[0]+r[4]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[4],fr[4],4); /* fmov fr4,@(r0,r4) */
 goto L8c09412e;
L8c09412e: r[0]=44u; /* mov #0x2c,r0 */
 goto L8c094130;
L8c094130: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,r[0]+r[4],f->bank[bank][pair],4);wr(m,r[0]+r[4]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[4],fr[4],4); /* fmov fr4,@(r0,r4) */
 goto L8c094132;
L8c094132: r[15]+=16u; /* add #0x10,r15 */
 goto L8c094134;
L8c094134: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c094136;
L8c094136: return 0; /* rts */
}
