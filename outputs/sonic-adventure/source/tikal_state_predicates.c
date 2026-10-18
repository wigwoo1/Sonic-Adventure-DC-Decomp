#include "fpu_cleanup_transfer.h"
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Retail0cb00e74..0cb00e90, no subordinate calls. */
SA_STAGE_EXPORT int sa_tikal_flags_transition(uint8_t *m,uint32_t *sr,uint32_t *r){
 r[0]=(uint32_t)(int32_t)(int16_t)rd(m,r[4]+4,2);*sr=(*sr&~1u)|!(r[0]&3u);
 if(*sr&1u){r[0]=292;r[3]=5;r[2]=6;wr(m,r[4],r[2],1);wr(m,r[6]+r[0],r[3],2);r[0]=1;}else r[0]=0;
 return 0;
}
/* Retail0cb00e90..0cb00ed4, external eligibility8c06cd22 remains a service.
 * PR=SZ=0 and normal/zero finite comparison scope. */
SA_STAGE_EXPORT int sa_tikal_idle_transition(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 if(f->fpscr&0x180000u)return -1;
 r[15]-=4;wr(m,r[15],*pr,4);r[15]-=8;r[3]=rd(m,0x0cb00f24,4);wr(m,r[15]+4,r[4],4);wr(m,r[15],r[6],4);r[6]=0;r[5]=r[6];
 uint32_t target=r[3];*pr=0x0cb00ea2;r[4]=rd(m,r[15]+4,4);int status=call(ctx,target,m,f,sr,r,pr);if(status)return status;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];
 *sr=(*sr&~1u)|!r[0];if(*sr&1u){
 r[3]=rd(m,r[15],4);r[0]=56;fr[3]=0;fr[2]=rd(m,r[3]+r[0],4);uint32_t v=fr[2],e=v&0x7f800000u;
 if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|!(v&0x7fffffffu);
 if(*sr&1u){r[2]=rd(m,r[15]+4,4);r[3]=1;r[0]=292;wr(m,r[2],r[3],1);r[2]=rd(m,r[15],4);r[3]=0;wr(m,r[2]+r[0],r[3],2);r[15]+=8;*pr=rd(m,r[15],4);r[15]+=4;r[0]=1;return 0;}
 }
 r[0]=0;r[15]+=8;*pr=rd(m,r[15],4);r[15]+=4;return 0;
}
