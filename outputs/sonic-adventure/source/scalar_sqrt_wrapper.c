#include "fpu_cleanup_transfer.h"
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *);
/* Complete 8c63a9c4..8c63a9d8. Signed sqrt wrapper; FSQRT numerical service explicit. */
SA_STAGE_EXPORT int sa_scalar_signed_sqrt(struct sa_fpu_banks *f,uint32_t *sr,void *ctx,instruction sqrt){
 if(f->fpscr&0x180000u||!sqrt)return -1;
 unsigned b=(f->fpscr>>21)&1u;uint32_t *fr=f->bank[b];fr[0]=0;
 uint32_t v=fr[4],a=v&0x7fffffffu;
 if(a>=0x7f800000u||(a&&a<0x800000u))return -1;
 f->fpscr&=~0x3f000u;unsigned negative=(v>>31)&&a;*sr=(*sr&~1u)|negative;
 fr[0]=v;if(negative)fr[0]&=0x7fffffffu;
 int status=sqrt(ctx,0xf06du,f);if(status)return status;
 if(negative)f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;
 return 0;
}
