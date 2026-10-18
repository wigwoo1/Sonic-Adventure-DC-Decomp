#include "sh4_float_i32.h"
int sa_sh4_float_i32(uint32_t raw,uint32_t *fpscr,uint32_t *destination)
{
 uint32_t status=*fpscr,mode=status&3u;
 if((status&0x80000u)!=0u || mode>1u)return -1;
 uint32_t sign=raw&0x80000000u,magnitude=sign!=0u?0u-raw:raw;
 uint32_t result=0u,inexact=0u;
 if(magnitude!=0u){
  unsigned exponent=0u;for(uint32_t v=magnitude;v>1u;v>>=1u)exponent++;
  uint32_t significand;
  if(exponent<=23u)significand=magnitude<<(23u-exponent);
  else{
   unsigned shift=exponent-23u;
   uint32_t remainder=magnitude&((1u<<shift)-1u);
   significand=magnitude>>shift;inexact=remainder!=0u;
   if(mode==0u){uint32_t halfway=1u<<(shift-1u);
    if(remainder>halfway || (remainder==halfway && (significand&1u)!=0u))significand++;
   }
   if(significand==0x1000000u){significand>>=1u;exponent++;}
  }
  result=sign|((exponent+127u)<<23)|(significand&0x7fffffu);
 }
 status&=~0x3f000u;
 if(inexact!=0u)status|=0x1004u;
 *fpscr=status;
 /* The SH-4 FLOAT instruction traps whenever enable.I is set. */
 if((status&0x80u)!=0u)return 1;
 *destination=result;return 0;
}
