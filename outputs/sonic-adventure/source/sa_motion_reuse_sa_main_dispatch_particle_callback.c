#include "sa_draw_motion_stop.h"
/* Particle callback/update/draw/state wrappers; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);
SA_STAGE_EXPORT int sa_particle_primary_8c04481a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04481a;
L8c04481a: sa_motion_publish_stop(ctx,0x8c04481au); r[15]-=4;wr(m,r[15],r[14],4);goto L8c04481c; /* mov.l r14,@-r15 */
L8c04481c: sa_motion_publish_stop(ctx,0x8c04481cu); r[15]-=4;wr(m,r[15],*pr,4);goto L8c04481e; /* sts.l PR,@-r15 */
L8c04481e: sa_motion_publish_stop(ctx,0x8c04481eu); r[14]=rd(m,r[4]+0x20u,4);goto L8c044820; /* mov.l @(0x20,r4),r14 */
L8c044820: sa_motion_publish_stop(ctx,0x8c044820u); r[0]=sx(m,r[14],1);goto L8c044822; /* mov.b @r14,r0 */
L8c044822: sa_motion_publish_stop(ctx,0x8c044822u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c044824; /* cmp/eq #0x0,r0 */
L8c044824: sa_motion_publish_stop(ctx,0x8c044824u); if((*sr&1u))goto L8c044832;goto L8c044826; /* bt 0x8c044832 */
L8c044826: sa_motion_publish_stop(ctx,0x8c044826u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c044828; /* cmp/eq #0x1,r0 */
L8c044828: sa_motion_publish_stop(ctx,0x8c044828u); if((*sr&1u))goto L8c0448a0;goto L8c04482a; /* bt 0x8c0448a0 */
L8c04482a: sa_motion_publish_stop(ctx,0x8c04482au); *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c04482c; /* cmp/eq #0x2,r0 */
L8c04482c: sa_motion_publish_stop(ctx,0x8c04482cu); if((*sr&1u))goto L8c0448a8;goto L8c04482e; /* bt 0x8c0448a8 */
L8c04482e: sa_motion_publish_stop(ctx,0x8c044830u); goto L8c0448ae; /* bra 0x8c0448ae */
L8c044832: sa_motion_publish_stop(ctx,0x8c044832u); r[2]=rd(m,0x8c04488cu,4);goto L8c044834; /* mov.l 0x8c04488c,r2 */
L8c044834: sa_motion_publish_stop(ctx,0x8c044834u); r[3]=1u;goto L8c044836; /* mov #0x1,r3 */
L8c044836: sa_motion_publish_stop(ctx,0x8c044836u); wr(m,r[14],r[3],1);goto L8c044838; /* mov.b r3,@r14 */
L8c044838: sa_motion_publish_stop(ctx,0x8c04483au); wr(m,r[4]+0x14u,r[2],4);goto L8c0448ae; /* bra 0x8c0448ae */
L8c0448a0: sa_motion_publish_stop(ctx,0x8c0448a0u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c0448a2; /* lds.l @r15+,PR */
L8c0448a2: sa_motion_publish_stop(ctx,0x8c0448a2u); r[4]=r[14];goto L8c0448a4; /* mov r14,r4 */
L8c0448a4: sa_motion_publish_stop(ctx,0x8c0448a6u); r[14]=rd(m,r[15],4);r[15]+=4;return sa_particle_work_update_8c0448ba(m,f,sr,r,pr,fpul,macl,ctx,call); /* bra 0x8c0448ba */
L8c0448a8: sa_motion_publish_stop(ctx,0x8c0448a8u); r[2]=rd(m,0x8c0449e0u,4);goto L8c0448aa; /* mov.l 0x8c0449e0,r2 */
L8c0448aa: sa_motion_publish_stop(ctx,0x8c0448acu); target=r[2];*pr=0x8c0448aeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0448ae; /* jsr @r2 */
L8c0448ae: sa_motion_publish_stop(ctx,0x8c0448aeu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c0448b0; /* lds.l @r15+,PR */
L8c0448b0: sa_motion_publish_stop(ctx,0x8c0448b2u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

static int secondary_engine(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t entry){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(entry==0x8c0448bau)goto L8c0448ba;goto L8c0448b4;
L8c0448b4: sa_motion_publish_stop(ctx,0x8c0448b4u); r[15]-=4;wr(m,r[15],r[4],4);goto L8c0448b6; /* mov.l r4,@-r15 */
L8c0448b6: sa_motion_publish_stop(ctx,0x8c0448b6u); r[4]=rd(m,r[4]+0x20u,4);goto L8c0448b8; /* mov.l @(0x20,r4),r4 */
L8c0448b8: sa_motion_publish_stop(ctx,0x8c0448b8u); r[15]+=4u;goto L8c0448ba; /* add #0x4,r15 */
L8c0448ba: sa_motion_publish_stop(ctx,0x8c0448bau); r[15]-=4;wr(m,r[15],r[14],4);goto L8c0448bc; /* mov.l r14,@-r15 */
L8c0448bc: sa_motion_publish_stop(ctx,0x8c0448bcu); r[15]-=4;wr(m,r[15],*pr,4);goto L8c0448be; /* sts.l PR,@-r15 */
L8c0448be: sa_motion_publish_stop(ctx,0x8c0448beu); r[15]+=4294967268u;goto L8c0448c0; /* add #-0x1c,r15 */
L8c0448c0: sa_motion_publish_stop(ctx,0x8c0448c0u); r[3]=rd(m,0x8c0449e4u,4);goto L8c0448c2; /* mov.l 0x8c0449e4,r3 */
L8c0448c2: sa_motion_publish_stop(ctx,0x8c0448c4u); target=r[3];*pr=0x8c0448c6u;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0448c6; /* jsr @r3 */
L8c0448c6: sa_motion_publish_stop(ctx,0x8c0448c6u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0448c8; /* tst r0,r0 */
L8c0448c8: sa_motion_publish_stop(ctx,0x8c0448c8u); if(!(*sr&1u))goto L8c0448e6;goto L8c0448ca; /* bf 0x8c0448e6 */
L8c0448ca: sa_motion_publish_stop(ctx,0x8c0448cau); r[0]=sx(m,r[14]+0x6u,2);goto L8c0448cc; /* mov.w @(0x6,r14),r0 */
L8c0448cc: sa_motion_publish_stop(ctx,0x8c0448ccu); r[3]=10u;goto L8c0448ce; /* mov #0xa,r3 */
L8c0448ce: sa_motion_publish_stop(ctx,0x8c0448ceu); r[0]+=1u;goto L8c0448d0; /* add #0x1,r0 */
L8c0448d0: sa_motion_publish_stop(ctx,0x8c0448d0u); wr(m,r[14]+0x6u,r[0],2);goto L8c0448d2; /* mov.w r0,@(0x6,r14) */
L8c0448d2: sa_motion_publish_stop(ctx,0x8c0448d2u); r[0]+=4294967295u;goto L8c0448d4; /* add #-0x1,r0 */
L8c0448d4: sa_motion_publish_stop(ctx,0x8c0448d4u); r[0]=r[0]&65535u;goto L8c0448d6; /* extu.w r0,r0 */
L8c0448d6: sa_motion_publish_stop(ctx,0x8c0448d6u); *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c0448d8; /* cmp/ge r3,r0 */
L8c0448d8: sa_motion_publish_stop(ctx,0x8c0448d8u); if(!(*sr&1u))goto L8c0448e6;goto L8c0448da; /* bf 0x8c0448e6 */
L8c0448da: sa_motion_publish_stop(ctx,0x8c0448dau); r[1]=2u;goto L8c0448dc; /* mov #0x2,r1 */
L8c0448dc: sa_motion_publish_stop(ctx,0x8c0448dcu); wr(m,r[14],r[1],1);goto L8c0448de; /* mov.b r1,@r14 */
L8c0448de: sa_motion_publish_stop(ctx,0x8c0448deu); r[15]+=28u;goto L8c0448e0; /* add #0x1c,r15 */
L8c0448e0: sa_motion_publish_stop(ctx,0x8c0448e0u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c0448e2; /* lds.l @r15+,PR */
L8c0448e2: sa_motion_publish_stop(ctx,0x8c0448e4u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c0448e6: sa_motion_publish_stop(ctx,0x8c0448e6u); r[3]=rd(m,0x8c0449e8u,4);goto L8c0448e8; /* mov.l 0x8c0449e8,r3 */
L8c0448e8: sa_motion_publish_stop(ctx,0x8c0448eau); target=r[3];*pr=0x8c0448ecu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0448ec; /* jsr @r3 */
L8c0448ec: sa_motion_publish_stop(ctx,0x8c0448ecu); r[2]=rd(m,0x8c0449ecu,4);goto L8c0448ee; /* mov.l 0x8c0449ec,r2 */
L8c0448ee: sa_motion_publish_stop(ctx,0x8c0448eeu); r[5]=r[14];goto L8c0448f0; /* mov r14,r5 */
L8c0448f0: sa_motion_publish_stop(ctx,0x8c0448f0u); r[5]+=32u;goto L8c0448f2; /* add #0x20,r5 */
L8c0448f2: sa_motion_publish_stop(ctx,0x8c0448f4u); target=r[2];*pr=0x8c0448f6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0448f6; /* jsr @r2 */
L8c0448f6: sa_motion_publish_stop(ctx,0x8c0448f6u); r[2]=rd(m,0x8c0449f0u,4);goto L8c0448f8; /* mov.l 0x8c0449f0,r2 */
L8c0448f8: sa_motion_publish_stop(ctx,0x8c0448f8u); r[3]=rd(m,r[2],4);goto L8c0448fa; /* mov.l @r2,r3 */
L8c0448fa: sa_motion_publish_stop(ctx,0x8c0448fau); r[1]=rd(m,r[3]+0x18u,4);goto L8c0448fc; /* mov.l @(0x18,r3),r1 */
L8c0448fc: sa_motion_publish_stop(ctx,0x8c0448fcu); *sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c0448fe; /* tst r1,r1 */
L8c0448fe: sa_motion_publish_stop(ctx,0x8c044900u); take=(*sr&1u);wr(m,r[15],r[1],4);if(take)goto L8c04490a;goto L8c044902; /* bt/s 0x8c04490a */
L8c044902: sa_motion_publish_stop(ctx,0x8c044902u); r[3]=rd(m,0x8c0449f4u,4);goto L8c044904; /* mov.l 0x8c0449f4,r3 */
L8c044904: sa_motion_publish_stop(ctx,0x8c044904u); r[5]=rd(m,r[15],4);goto L8c044906; /* mov.l @r15,r5 */
L8c044906: sa_motion_publish_stop(ctx,0x8c044908u); target=r[3];*pr=0x8c04490au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04490a; /* jsr @r3 */
L8c04490a: sa_motion_publish_stop(ctx,0x8c04490au); r[3]=rd(m,r[14]+0x1cu,4);goto L8c04490c; /* mov.l @(0x1c,r14),r3 */
L8c04490c: sa_motion_publish_stop(ctx,0x8c04490cu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c04490e; /* tst r3,r3 */
L8c04490e: sa_motion_publish_stop(ctx,0x8c044910u); take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c04491a;goto L8c044912; /* bt/s 0x8c04491a */
L8c044912: sa_motion_publish_stop(ctx,0x8c044912u); r[3]=rd(m,0x8c0449f8u,4);goto L8c044914; /* mov.l 0x8c0449f8,r3 */
L8c044914: sa_motion_publish_stop(ctx,0x8c044914u); r[5]=rd(m,r[15],4);goto L8c044916; /* mov.l @r15,r5 */
L8c044916: sa_motion_publish_stop(ctx,0x8c044918u); target=r[3];*pr=0x8c04491au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04491a; /* jsr @r3 */
L8c04491a: sa_motion_publish_stop(ctx,0x8c04491au); r[0]=sx(m,r[14]+0x3u,1);goto L8c04491c; /* mov.b @(0x3,r14),r0 */
L8c04491c: sa_motion_publish_stop(ctx,0x8c04491cu); r[0]=r[0]&255u;goto L8c04491e; /* extu.b r0,r0 */
L8c04491e: sa_motion_publish_stop(ctx,0x8c04491eu); *sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c044920; /* tst #0x1,r0 */
L8c044920: sa_motion_publish_stop(ctx,0x8c044920u); if((*sr&1u))goto L8c044928;goto L8c044922; /* bt 0x8c044928 */
L8c044922: sa_motion_publish_stop(ctx,0x8c044922u); r[4]=rd(m,0x8c0449fcu,4);goto L8c044924; /* mov.l 0x8c0449fc,r4 */
L8c044924: sa_motion_publish_stop(ctx,0x8c044926u); goto L8c04492a; /* bra 0x8c04492a */
L8c044928: sa_motion_publish_stop(ctx,0x8c044928u); r[4]=rd(m,0x8c044a00u,4);goto L8c04492a; /* mov.l 0x8c044a00,r4 */
L8c04492a: sa_motion_publish_stop(ctx,0x8c04492au); r[2]=rd(m,0x8c044a04u,4);goto L8c04492c; /* mov.l 0x8c044a04,r2 */
L8c04492c: sa_motion_publish_stop(ctx,0x8c04492eu); target=r[2];*pr=0x8c044930u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044930; /* jsr @r2 */
L8c044930: sa_motion_publish_stop(ctx,0x8c044930u); r[3]=rd(m,0x8c044a08u,4);goto L8c044932; /* mov.l 0x8c044a08,r3 */
L8c044932: sa_motion_publish_stop(ctx,0x8c044934u); target=r[3];*pr=0x8c044936u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044936; /* jsr @r3 */
L8c044936: sa_motion_publish_stop(ctx,0x8c044936u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c044938; /* fldi1 fr3 */
L8c044938: sa_motion_publish_stop(ctx,0x8c044938u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c04493a; /* fadd fr3,fr3 */
L8c04493a: sa_motion_publish_stop(ctx,0x8c04493au); r[0]=16u;goto L8c04493c; /* mov #0x10,r0 */
L8c04493c: sa_motion_publish_stop(ctx,0x8c04493cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c04493e; /* fldi0 fr4 */
L8c04493e: sa_motion_publish_stop(ctx,0x8c04493eu); r[4]=rd(m,0x8c044a0cu,4);goto L8c044940; /* mov.l 0x8c044a0c,r4 */
L8c044940: sa_motion_publish_stop(ctx,0x8c044940u); r[3]=rd(m,0x8c0449e8u,4);goto L8c044942; /* mov.l 0x8c0449e8,r3 */
L8c044942: sa_motion_publish_stop(ctx,0x8c044942u); fstore(m,f,3u,r[0]+r[15]);goto L8c044944; /* fmov fr3,@(r0,r15) */
L8c044944: sa_motion_publish_stop(ctx,0x8c044944u); r[0]=24u;goto L8c044946; /* mov #0x18,r0 */
L8c044946: sa_motion_publish_stop(ctx,0x8c044946u); fstore(m,f,4u,r[0]+r[15]);goto L8c044948; /* fmov fr4,@(r0,r15) */
L8c044948: sa_motion_publish_stop(ctx,0x8c044948u); r[0]=20u;goto L8c04494a; /* mov #0x14,r0 */
L8c04494a: sa_motion_publish_stop(ctx,0x8c04494cu); target=r[3];*pr=0x8c04494eu;fstore(m,f,4u,r[0]+r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04494e; /* jsr @r3 */
L8c04494e: sa_motion_publish_stop(ctx,0x8c04494eu); r[2]=rd(m,r[14]+0x14u,4);goto L8c044950; /* mov.l @(0x14,r14),r2 */
L8c044950: sa_motion_publish_stop(ctx,0x8c044950u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c044952; /* tst r2,r2 */
L8c044952: sa_motion_publish_stop(ctx,0x8c044954u); take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c04495e;goto L8c044956; /* bt/s 0x8c04495e */
L8c044956: sa_motion_publish_stop(ctx,0x8c044956u); r[3]=rd(m,0x8c044a10u,4);goto L8c044958; /* mov.l 0x8c044a10,r3 */
L8c044958: sa_motion_publish_stop(ctx,0x8c044958u); r[5]=rd(m,r[15],4);goto L8c04495a; /* mov.l @r15,r5 */
L8c04495a: sa_motion_publish_stop(ctx,0x8c04495cu); target=r[3];*pr=0x8c04495eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04495e; /* jsr @r3 */
L8c04495e: sa_motion_publish_stop(ctx,0x8c04495eu); r[3]=rd(m,r[14]+0x18u,4);goto L8c044960; /* mov.l @(0x18,r14),r3 */
L8c044960: sa_motion_publish_stop(ctx,0x8c044960u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c044962; /* tst r3,r3 */
L8c044962: sa_motion_publish_stop(ctx,0x8c044964u); take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c04496e;goto L8c044966; /* bt/s 0x8c04496e */
L8c044966: sa_motion_publish_stop(ctx,0x8c044966u); r[3]=rd(m,0x8c0449f4u,4);goto L8c044968; /* mov.l 0x8c0449f4,r3 */
L8c044968: sa_motion_publish_stop(ctx,0x8c044968u); r[5]=rd(m,r[15],4);goto L8c04496a; /* mov.l @r15,r5 */
L8c04496a: sa_motion_publish_stop(ctx,0x8c04496cu); target=r[3];*pr=0x8c04496eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04496e; /* jsr @r3 */
L8c04496e: sa_motion_publish_stop(ctx,0x8c04496eu); r[5]=r[15];goto L8c044970; /* mov r15,r5 */
L8c044970: sa_motion_publish_stop(ctx,0x8c044970u); r[3]=rd(m,0x8c044a14u,4);goto L8c044972; /* mov.l 0x8c044a14,r3 */
L8c044972: sa_motion_publish_stop(ctx,0x8c044972u); r[6]=r[15];goto L8c044974; /* mov r15,r6 */
L8c044974: sa_motion_publish_stop(ctx,0x8c044974u); r[5]+=16u;goto L8c044976; /* add #0x10,r5 */
L8c044976: sa_motion_publish_stop(ctx,0x8c044976u); r[6]+=4u;goto L8c044978; /* add #0x4,r6 */
L8c044978: sa_motion_publish_stop(ctx,0x8c04497au); target=r[3];*pr=0x8c04497cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04497c; /* jsr @r3 */
L8c04497c: sa_motion_publish_stop(ctx,0x8c04497cu); r[2]=rd(m,0x8c044a08u,4);goto L8c04497e; /* mov.l 0x8c044a08,r2 */
L8c04497e: sa_motion_publish_stop(ctx,0x8c044980u); target=r[2];*pr=0x8c044982u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044982; /* jsr @r2 */
L8c044982: sa_motion_publish_stop(ctx,0x8c044982u); r[3]=rd(m,0x8c0449e4u,4);goto L8c044984; /* mov.l 0x8c0449e4,r3 */
L8c044984: sa_motion_publish_stop(ctx,0x8c044986u); target=r[3];*pr=0x8c044988u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044988; /* jsr @r3 */
L8c044988: sa_motion_publish_stop(ctx,0x8c044988u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c04498a; /* tst r0,r0 */
L8c04498a: sa_motion_publish_stop(ctx,0x8c04498au); if(!(*sr&1u))goto L8c0449ba;goto L8c04498c; /* bf 0x8c0449ba */
L8c04498c: sa_motion_publish_stop(ctx,0x8c04498cu); r[0]=4u;goto L8c04498e; /* mov #0x4,r0 */
L8c04498e: sa_motion_publish_stop(ctx,0x8c04498eu); fload(m,f,3u,r[0]+r[15]);goto L8c044990; /* fmov.s @(r0,r15),fr3 */
L8c044990: sa_motion_publish_stop(ctx,0x8c044990u); r[0]=32u;goto L8c044992; /* mov #0x20,r0 */
L8c044992: sa_motion_publish_stop(ctx,0x8c044992u); fload(m,f,2u,r[0]+r[14]);goto L8c044994; /* fmov.s @(r0,r14),fr2 */
L8c044994: sa_motion_publish_stop(ctx,0x8c044994u); r[3]=16u;goto L8c044996; /* mov #0x10,r3 */
L8c044996: sa_motion_publish_stop(ctx,0x8c044996u); r[3]<<=8;goto L8c044998; /* shll8 r3 */
L8c044998: sa_motion_publish_stop(ctx,0x8c044998u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c04499a; /* fadd fr3,fr2 */
L8c04499a: sa_motion_publish_stop(ctx,0x8c04499au); fstore(m,f,2u,r[0]+r[14]);goto L8c04499c; /* fmov fr2,@(r0,r14) */
L8c04499c: sa_motion_publish_stop(ctx,0x8c04499cu); r[0]=8u;goto L8c04499e; /* mov #0x8,r0 */
L8c04499e: sa_motion_publish_stop(ctx,0x8c04499eu); fload(m,f,3u,r[0]+r[15]);goto L8c0449a0; /* fmov.s @(r0,r15),fr3 */
L8c0449a0: sa_motion_publish_stop(ctx,0x8c0449a0u); r[0]=36u;goto L8c0449a2; /* mov #0x24,r0 */
L8c0449a2: sa_motion_publish_stop(ctx,0x8c0449a2u); fload(m,f,2u,r[0]+r[14]);goto L8c0449a4; /* fmov.s @(r0,r14),fr2 */
L8c0449a4: sa_motion_publish_stop(ctx,0x8c0449a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0449a6; /* fadd fr3,fr2 */
L8c0449a6: sa_motion_publish_stop(ctx,0x8c0449a6u); fstore(m,f,2u,r[0]+r[14]);goto L8c0449a8; /* fmov fr2,@(r0,r14) */
L8c0449a8: sa_motion_publish_stop(ctx,0x8c0449a8u); r[0]=12u;goto L8c0449aa; /* mov #0xc,r0 */
L8c0449aa: sa_motion_publish_stop(ctx,0x8c0449aau); fload(m,f,3u,r[0]+r[15]);goto L8c0449ac; /* fmov.s @(r0,r15),fr3 */
L8c0449ac: sa_motion_publish_stop(ctx,0x8c0449acu); r[0]=40u;goto L8c0449ae; /* mov #0x28,r0 */
L8c0449ae: sa_motion_publish_stop(ctx,0x8c0449aeu); fload(m,f,2u,r[0]+r[14]);goto L8c0449b0; /* fmov.s @(r0,r14),fr2 */
L8c0449b0: sa_motion_publish_stop(ctx,0x8c0449b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0449b2; /* fadd fr3,fr2 */
L8c0449b2: sa_motion_publish_stop(ctx,0x8c0449b2u); fstore(m,f,2u,r[0]+r[14]);goto L8c0449b4; /* fmov fr2,@(r0,r14) */
L8c0449b4: sa_motion_publish_stop(ctx,0x8c0449b4u); r[2]=rd(m,r[14]+0x1cu,4);goto L8c0449b6; /* mov.l @(0x1c,r14),r2 */
L8c0449b6: sa_motion_publish_stop(ctx,0x8c0449b6u); r[2]+=r[3];goto L8c0449b8; /* add r3,r2 */
L8c0449b8: sa_motion_publish_stop(ctx,0x8c0449b8u); wr(m,r[14]+0x1cu,r[2],4);goto L8c0449ba; /* mov.l r2,@(0x1c,r14) */
L8c0449ba: sa_motion_publish_stop(ctx,0x8c0449bau); r[15]+=28u;goto L8c0449bc; /* add #0x1c,r15 */
L8c0449bc: sa_motion_publish_stop(ctx,0x8c0449bcu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c0449be; /* lds.l @r15+,PR */
L8c0449be: sa_motion_publish_stop(ctx,0x8c0449c0u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_particle_secondary_8c0448b4(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_shared_service call){
 return secondary_engine(m,f,sr,r,pr,fpul,macl,ctx,call,0x8c0448b4u);
}

SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_shared_service call){
 return secondary_engine(m,f,sr,r,pr,fpul,macl,ctx,call,0x8c0448bau);
}

SA_STAGE_EXPORT int sa_particle_draw_wrapper_8c6215d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6215d6;
L8c6215d6: sa_motion_publish_stop(ctx,0x8c6215d6u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c6215d8; /* sts.l PR,@-r15 */
L8c6215d8: sa_motion_publish_stop(ctx,0x8c6215d8u); r[15]+=4294967288u;goto L8c6215da; /* add #-0x8,r15 */
L8c6215da: sa_motion_publish_stop(ctx,0x8c6215dau); r[3]=rd(m,0x8c62161cu,4);goto L8c6215dc; /* mov.l 0x8c62161c,r3 */
L8c6215dc: sa_motion_publish_stop(ctx,0x8c6215deu); target=r[3];*pr=0x8c6215e0u;wr(m,r[15]+0x4u,r[4],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215e0; /* jsr @r3 */
L8c6215e0: sa_motion_publish_stop(ctx,0x8c6215e0u); r[4]=r[15];goto L8c6215e2; /* mov r15,r4 */
L8c6215e2: sa_motion_publish_stop(ctx,0x8c6215e2u); wr(m,r[4],r[8],4);goto L8c6215e4; /* mov.l r8,@r4 */
L8c6215e4: sa_motion_publish_stop(ctx,0x8c6215e6u); target=0x8c620f2cu;*pr=0x8c6215e8u;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215e8; /* bsr 0x8c620f2c */
L8c6215e8: sa_motion_publish_stop(ctx,0x8c6215e8u); r[4]=r[15];goto L8c6215ea; /* mov r15,r4 */
L8c6215ea: sa_motion_publish_stop(ctx,0x8c6215eau); r[8]=rd(m,r[4],4);goto L8c6215ec; /* mov.l @r4,r8 */
L8c6215ec: sa_motion_publish_stop(ctx,0x8c6215ecu); r[3]=rd(m,0x8c621624u,4);goto L8c6215ee; /* mov.l 0x8c621624,r3 */
L8c6215ee: sa_motion_publish_stop(ctx,0x8c6215f0u); target=r[3];*pr=0x8c6215f2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215f2; /* jsr @r3 */
L8c6215f2: sa_motion_publish_stop(ctx,0x8c6215f2u); r[15]+=8u;goto L8c6215f4; /* add #0x8,r15 */
L8c6215f4: sa_motion_publish_stop(ctx,0x8c6215f4u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c6215f6; /* lds.l @r15+,PR */
L8c6215f6: sa_motion_publish_stop(ctx,0x8c6215f8u); return 0; /* rts */
}

SA_STAGE_EXPORT int sa_particle_render_state_save_8c605dac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c605dac;
L8c605dac: sa_motion_publish_stop(ctx,0x8c605dacu); r[5]=rd(m,0x8c605eb8u,4);goto L8c605dae; /* mov.l 0x8c605eb8,r5 */
L8c605dae: sa_motion_publish_stop(ctx,0x8c605daeu); r[0]=sx(m,0x8c605eb4u,2);goto L8c605db0; /* mov.w 0x8c605eb4,r0 */
L8c605db0: sa_motion_publish_stop(ctx,0x8c605db0u); r[3]=rd(m,r[5],4);goto L8c605db2; /* mov.l @r5,r3 */
L8c605db2: sa_motion_publish_stop(ctx,0x8c605db2u); r[4]=rd(m,0x8c605ebcu,4);goto L8c605db4; /* mov.l 0x8c605ebc,r4 */
L8c605db4: sa_motion_publish_stop(ctx,0x8c605db4u); r[2]=rd(m,r[3]+r[0],4);goto L8c605db6; /* mov.l @(r0,r3),r2 */
L8c605db6: sa_motion_publish_stop(ctx,0x8c605db6u); r[0]+=4u;goto L8c605db8; /* add #0x4,r0 */
L8c605db8: sa_motion_publish_stop(ctx,0x8c605db8u); r[1]=rd(m,0x8c605ec4u,4);goto L8c605dba; /* mov.l 0x8c605ec4,r1 */
L8c605dba: sa_motion_publish_stop(ctx,0x8c605dbau); wr(m,r[4],r[2],4);goto L8c605dbc; /* mov.l r2,@r4 */
L8c605dbc: sa_motion_publish_stop(ctx,0x8c605dbcu); r[3]=rd(m,r[5],4);goto L8c605dbe; /* mov.l @r5,r3 */
L8c605dbe: sa_motion_publish_stop(ctx,0x8c605dbeu); r[2]=rd(m,r[3]+r[0],4);goto L8c605dc0; /* mov.l @(r0,r3),r2 */
L8c605dc0: sa_motion_publish_stop(ctx,0x8c605dc0u); r[0]+=4u;goto L8c605dc2; /* add #0x4,r0 */
L8c605dc2: sa_motion_publish_stop(ctx,0x8c605dc2u); wr(m,r[4]+0x4u,r[2],4);goto L8c605dc4; /* mov.l r2,@(0x4,r4) */
L8c605dc4: sa_motion_publish_stop(ctx,0x8c605dc4u); r[3]=rd(m,r[5],4);goto L8c605dc6; /* mov.l @r5,r3 */
L8c605dc6: sa_motion_publish_stop(ctx,0x8c605dc6u); r[2]=rd(m,r[3]+r[0],4);goto L8c605dc8; /* mov.l @(r0,r3),r2 */
L8c605dc8: sa_motion_publish_stop(ctx,0x8c605dc8u); r[0]+=4u;goto L8c605dca; /* add #0x4,r0 */
L8c605dca: sa_motion_publish_stop(ctx,0x8c605dcau); wr(m,r[4]+0x8u,r[2],4);goto L8c605dcc; /* mov.l r2,@(0x8,r4) */
L8c605dcc: sa_motion_publish_stop(ctx,0x8c605dccu); r[3]=rd(m,r[5],4);goto L8c605dce; /* mov.l @r5,r3 */
L8c605dce: sa_motion_publish_stop(ctx,0x8c605dceu); r[5]=rd(m,0x8c605ec0u,4);goto L8c605dd0; /* mov.l 0x8c605ec0,r5 */
L8c605dd0: sa_motion_publish_stop(ctx,0x8c605dd0u); r[2]=rd(m,r[3]+r[0],4);goto L8c605dd2; /* mov.l @(r0,r3),r2 */
L8c605dd2: sa_motion_publish_stop(ctx,0x8c605dd2u); wr(m,r[4]+0xcu,r[2],4);goto L8c605dd4; /* mov.l r2,@(0xc,r4) */
L8c605dd4: sa_motion_publish_stop(ctx,0x8c605dd4u); r[3]=rd(m,r[5],4);goto L8c605dd6; /* mov.l @r5,r3 */
L8c605dd6: sa_motion_publish_stop(ctx,0x8c605dd6u); r[2]=rd(m,r[3],4);goto L8c605dd8; /* mov.l @r3,r2 */
L8c605dd8: sa_motion_publish_stop(ctx,0x8c605dd8u); wr(m,r[4]+0x10u,r[2],4);goto L8c605dda; /* mov.l r2,@(0x10,r4) */
L8c605dda: sa_motion_publish_stop(ctx,0x8c605ddau); r[3]=rd(m,r[5],4);goto L8c605ddc; /* mov.l @r5,r3 */
L8c605ddc: sa_motion_publish_stop(ctx,0x8c605ddcu); r[2]=rd(m,r[3]+0x4u,4);goto L8c605dde; /* mov.l @(0x4,r3),r2 */
L8c605dde: sa_motion_publish_stop(ctx,0x8c605ddeu); wr(m,r[4]+0x1cu,r[2],4);goto L8c605de0; /* mov.l r2,@(0x1c,r4) */
L8c605de0: sa_motion_publish_stop(ctx,0x8c605de0u); r[3]=rd(m,r[5],4);goto L8c605de2; /* mov.l @r5,r3 */
L8c605de2: sa_motion_publish_stop(ctx,0x8c605de2u); r[2]=rd(m,r[3]+0xcu,4);goto L8c605de4; /* mov.l @(0xc,r3),r2 */
L8c605de4: sa_motion_publish_stop(ctx,0x8c605de4u); r[3]=sx(m,0x8c605eb6u,2);goto L8c605de6; /* mov.w 0x8c605eb6,r3 */
L8c605de6: sa_motion_publish_stop(ctx,0x8c605de6u); wr(m,r[4]+0x20u,r[2],4);goto L8c605de8; /* mov.l r2,@(0x20,r4) */
L8c605de8: sa_motion_publish_stop(ctx,0x8c605de8u); r[2]=rd(m,r[1],4);goto L8c605dea; /* mov.l @r1,r2 */
L8c605dea: sa_motion_publish_stop(ctx,0x8c605deau); *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c605dec; /* tst r3,r2 */
L8c605dec: sa_motion_publish_stop(ctx,0x8c605deeu); take=(*sr&1u);r[6]=rd(m,r[5],4);if(take)goto L8c605dfc;goto L8c605df0; /* bt/s 0x8c605dfc */
L8c605df0: sa_motion_publish_stop(ctx,0x8c605df0u); r[2]=rd(m,r[6]+0x10u,4);goto L8c605df2; /* mov.l @(0x10,r6),r2 */
L8c605df2: sa_motion_publish_stop(ctx,0x8c605df2u); wr(m,r[4]+0x14u,r[2],4);goto L8c605df4; /* mov.l r2,@(0x14,r4) */
L8c605df4: sa_motion_publish_stop(ctx,0x8c605df4u); r[1]=rd(m,r[5],4);goto L8c605df6; /* mov.l @r5,r1 */
L8c605df6: sa_motion_publish_stop(ctx,0x8c605df6u); r[2]=rd(m,r[1]+0x8u,4);goto L8c605df8; /* mov.l @(0x8,r1),r2 */
L8c605df8: sa_motion_publish_stop(ctx,0x8c605dfau); wr(m,r[4]+0x18u,r[2],4);goto L8c605e06; /* bra 0x8c605e06 */
L8c605dfc: sa_motion_publish_stop(ctx,0x8c605dfcu); r[0]=rd(m,r[6]+0x8u,4);goto L8c605dfe; /* mov.l @(0x8,r6),r0 */
L8c605dfe: sa_motion_publish_stop(ctx,0x8c605dfeu); wr(m,r[4]+0x14u,r[0],4);goto L8c605e00; /* mov.l r0,@(0x14,r4) */
L8c605e00: sa_motion_publish_stop(ctx,0x8c605e00u); r[2]=rd(m,r[5],4);goto L8c605e02; /* mov.l @r5,r2 */
L8c605e02: sa_motion_publish_stop(ctx,0x8c605e02u); r[1]=rd(m,r[2]+0x10u,4);goto L8c605e04; /* mov.l @(0x10,r2),r1 */
L8c605e04: sa_motion_publish_stop(ctx,0x8c605e04u); wr(m,r[4]+0x18u,r[1],4);goto L8c605e06; /* mov.l r1,@(0x18,r4) */
L8c605e06: sa_motion_publish_stop(ctx,0x8c605e08u); return 0; /* rts */
}

SA_STAGE_EXPORT int sa_particle_render_state_restore_8c605e0a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c605e0a;
L8c605e0a: sa_motion_publish_stop(ctx,0x8c605e0au); r[4]=rd(m,0x8c605ebcu,4);goto L8c605e0c; /* mov.l 0x8c605ebc,r4 */
L8c605e0c: sa_motion_publish_stop(ctx,0x8c605e0cu); r[5]=rd(m,0x8c605eb8u,4);goto L8c605e0e; /* mov.l 0x8c605eb8,r5 */
L8c605e0e: sa_motion_publish_stop(ctx,0x8c605e0eu); r[0]=sx(m,0x8c605eb4u,2);goto L8c605e10; /* mov.w 0x8c605eb4,r0 */
L8c605e10: sa_motion_publish_stop(ctx,0x8c605e10u); r[2]=rd(m,r[4],4);goto L8c605e12; /* mov.l @r4,r2 */
L8c605e12: sa_motion_publish_stop(ctx,0x8c605e12u); r[3]=rd(m,r[5],4);goto L8c605e14; /* mov.l @r5,r3 */
L8c605e14: sa_motion_publish_stop(ctx,0x8c605e14u); r[1]=rd(m,0x8c605ec4u,4);goto L8c605e16; /* mov.l 0x8c605ec4,r1 */
L8c605e16: sa_motion_publish_stop(ctx,0x8c605e16u); wr(m,r[3]+r[0],r[2],4);goto L8c605e18; /* mov.l r2,@(r0,r3) */
L8c605e18: sa_motion_publish_stop(ctx,0x8c605e18u); r[0]+=4u;goto L8c605e1a; /* add #0x4,r0 */
L8c605e1a: sa_motion_publish_stop(ctx,0x8c605e1au); r[2]=rd(m,r[4]+0x4u,4);goto L8c605e1c; /* mov.l @(0x4,r4),r2 */
L8c605e1c: sa_motion_publish_stop(ctx,0x8c605e1cu); r[3]=rd(m,r[5],4);goto L8c605e1e; /* mov.l @r5,r3 */
L8c605e1e: sa_motion_publish_stop(ctx,0x8c605e1eu); wr(m,r[3]+r[0],r[2],4);goto L8c605e20; /* mov.l r2,@(r0,r3) */
L8c605e20: sa_motion_publish_stop(ctx,0x8c605e20u); r[0]+=4u;goto L8c605e22; /* add #0x4,r0 */
L8c605e22: sa_motion_publish_stop(ctx,0x8c605e22u); r[2]=rd(m,r[4]+0x8u,4);goto L8c605e24; /* mov.l @(0x8,r4),r2 */
L8c605e24: sa_motion_publish_stop(ctx,0x8c605e24u); r[3]=rd(m,r[5],4);goto L8c605e26; /* mov.l @r5,r3 */
L8c605e26: sa_motion_publish_stop(ctx,0x8c605e26u); wr(m,r[3]+r[0],r[2],4);goto L8c605e28; /* mov.l r2,@(r0,r3) */
L8c605e28: sa_motion_publish_stop(ctx,0x8c605e28u); r[0]+=4u;goto L8c605e2a; /* add #0x4,r0 */
L8c605e2a: sa_motion_publish_stop(ctx,0x8c605e2au); r[2]=rd(m,r[4]+0xcu,4);goto L8c605e2c; /* mov.l @(0xc,r4),r2 */
L8c605e2c: sa_motion_publish_stop(ctx,0x8c605e2cu); r[3]=rd(m,r[5],4);goto L8c605e2e; /* mov.l @r5,r3 */
L8c605e2e: sa_motion_publish_stop(ctx,0x8c605e2eu); r[5]=rd(m,0x8c605ec0u,4);goto L8c605e30; /* mov.l 0x8c605ec0,r5 */
L8c605e30: sa_motion_publish_stop(ctx,0x8c605e30u); wr(m,r[3]+r[0],r[2],4);goto L8c605e32; /* mov.l r2,@(r0,r3) */
L8c605e32: sa_motion_publish_stop(ctx,0x8c605e32u); r[2]=rd(m,r[4]+0x10u,4);goto L8c605e34; /* mov.l @(0x10,r4),r2 */
L8c605e34: sa_motion_publish_stop(ctx,0x8c605e34u); r[3]=rd(m,r[5],4);goto L8c605e36; /* mov.l @r5,r3 */
L8c605e36: sa_motion_publish_stop(ctx,0x8c605e36u); wr(m,r[3],r[2],4);goto L8c605e38; /* mov.l r2,@r3 */
L8c605e38: sa_motion_publish_stop(ctx,0x8c605e38u); r[2]=rd(m,r[4]+0x1cu,4);goto L8c605e3a; /* mov.l @(0x1c,r4),r2 */
L8c605e3a: sa_motion_publish_stop(ctx,0x8c605e3au); r[3]=rd(m,r[5],4);goto L8c605e3c; /* mov.l @r5,r3 */
L8c605e3c: sa_motion_publish_stop(ctx,0x8c605e3cu); wr(m,r[3]+0x4u,r[2],4);goto L8c605e3e; /* mov.l r2,@(0x4,r3) */
L8c605e3e: sa_motion_publish_stop(ctx,0x8c605e3eu); r[2]=rd(m,r[4]+0x20u,4);goto L8c605e40; /* mov.l @(0x20,r4),r2 */
L8c605e40: sa_motion_publish_stop(ctx,0x8c605e40u); r[3]=rd(m,r[5],4);goto L8c605e42; /* mov.l @r5,r3 */
L8c605e42: sa_motion_publish_stop(ctx,0x8c605e42u); wr(m,r[3]+0xcu,r[2],4);goto L8c605e44; /* mov.l r2,@(0xc,r3) */
L8c605e44: sa_motion_publish_stop(ctx,0x8c605e44u); r[2]=rd(m,r[1],4);goto L8c605e46; /* mov.l @r1,r2 */
L8c605e46: sa_motion_publish_stop(ctx,0x8c605e46u); r[6]=rd(m,r[5],4);goto L8c605e48; /* mov.l @r5,r6 */
L8c605e48: sa_motion_publish_stop(ctx,0x8c605e48u); r[3]=sx(m,0x8c605eb6u,2);goto L8c605e4a; /* mov.w 0x8c605eb6,r3 */
L8c605e4a: sa_motion_publish_stop(ctx,0x8c605e4au); *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c605e4c; /* tst r3,r2 */
L8c605e4c: sa_motion_publish_stop(ctx,0x8c605e4eu); take=(*sr&1u);r[7]=rd(m,r[4]+0x14u,4);if(take)goto L8c605e5c;goto L8c605e50; /* bt/s 0x8c605e5c */
L8c605e50: sa_motion_publish_stop(ctx,0x8c605e50u); wr(m,r[6]+0x10u,r[7],4);goto L8c605e52; /* mov.l r7,@(0x10,r6) */
L8c605e52: sa_motion_publish_stop(ctx,0x8c605e52u); r[1]=rd(m,r[4]+0x18u,4);goto L8c605e54; /* mov.l @(0x18,r4),r1 */
L8c605e54: sa_motion_publish_stop(ctx,0x8c605e54u); r[2]=rd(m,r[5],4);goto L8c605e56; /* mov.l @r5,r2 */
L8c605e56: sa_motion_publish_stop(ctx,0x8c605e56u); wr(m,r[2]+0x8u,r[1],4);goto L8c605e58; /* mov.l r1,@(0x8,r2) */
L8c605e58: sa_motion_publish_stop(ctx,0x8c605e5au); goto L8c605e64; /* bra 0x8c605e64 */
L8c605e5c: sa_motion_publish_stop(ctx,0x8c605e5cu); wr(m,r[6]+0x8u,r[7],4);goto L8c605e5e; /* mov.l r7,@(0x8,r6) */
L8c605e5e: sa_motion_publish_stop(ctx,0x8c605e5eu); r[2]=rd(m,r[4]+0x18u,4);goto L8c605e60; /* mov.l @(0x18,r4),r2 */
L8c605e60: sa_motion_publish_stop(ctx,0x8c605e60u); r[1]=rd(m,r[5],4);goto L8c605e62; /* mov.l @r5,r1 */
L8c605e62: sa_motion_publish_stop(ctx,0x8c605e62u); wr(m,r[1]+0x10u,r[2],4);goto L8c605e64; /* mov.l r2,@(0x10,r1) */
L8c605e64: sa_motion_publish_stop(ctx,0x8c605e66u); return 0; /* rts */
}
