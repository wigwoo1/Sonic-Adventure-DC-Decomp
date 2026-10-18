#include "sh4_fmac.h"
/* Adding positive zero leaves a nonzero exact finite product intact;
 * the existing FMAC core rounds that exact product once and handles FPSCR. */
SA_STAGE_EXPORT int sa_sh4_multiply(uint32_t *a,uint32_t *b,uint32_t *fpscr,uint32_t *destination){
 uint32_t status=*fpscr;
 if((status&0x80000u)!=0u || (status&3u)>1u)return -1;
 unsigned na=(*a&0x7fffffffu)>0x7f800000u,nb=(*b&0x7fffffffu)>0x7f800000u;
 if(na||nb){
  unsigned invalid=(na&&(*a&0x400000u))||(nb&&(*b&0x400000u));status&=~0x3f000u;
  if(invalid)status|=0x10040u;*fpscr=status;if((status&0x380u)||(invalid&&(status&0x800u)))return 1;
  *destination=0x7fbfffffu;return 0;
 }
 uint32_t zero=0u;return sa_sh4_fmac(a,b,&zero,fpscr,destination);
}

