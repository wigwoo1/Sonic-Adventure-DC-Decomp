#include "stage00_task.h"
/* SH-4 FSQRT binary32 normal/zero/+infinity path; integer-only exact rounding.
 * Reference: Renesas SH-4 Software Manual rev6, section9.43.
 * Unsupported negative/nonfinite/denormal and PR/RM modes return -1 untouched. */
SA_STAGE_EXPORT int sa_sh4_sqrt(uint32_t raw,uint32_t *fpscr,uint32_t *destination){
 uint32_t status=*fpscr,absolute=raw&0x7fffffffu,mode=status&3u;
 if((status&0x80000u)||mode>1u)return -1;
 if(absolute==0u||raw==0x7f800000u){*fpscr=status&~0x3f000u;*destination=raw;return 0;}
 if((raw&0x80000000u)||absolute<0x800000u||absolute>=0x7f800000u)return -1;
 int exponent=(int)(raw>>23)-127;
 uint64_t significand=(raw&0x7fffffu)|0x800000u;
 if(exponent%2){significand<<=1;exponent--;}
 exponent/=2;
 uint64_t radicand=significand<<23,root=0;
 for(uint64_t bit=1ull<<23;bit;bit>>=1){uint64_t trial=root|bit;if(trial*trial<=radicand)root=trial;}
 int inexact=root*root!=radicand;
 if(mode==0u){uint64_t midpoint=2*root+1,scaled=radicand*4;
  if(scaled>midpoint*midpoint||(scaled==midpoint*midpoint&&(root&1)))root++;
 }
 if(root==0x1000000u){root>>=1;exponent++;}
 uint32_t result=((uint32_t)(exponent+127)<<23)|((uint32_t)root&0x7fffffu);
 status&=~0x3f000u;if(inexact)status|=0x1004u;*fpscr=status;
 if(status&0x80u)return 1;
 *destination=result;return 0;
}
