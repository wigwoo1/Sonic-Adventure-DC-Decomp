#include "sh4_multiply_extended.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);

SA_STAGE_EXPORT int sa_sh4_multiply_extended(uint32_t *a,uint32_t *b,uint32_t *fpscr,uint32_t *destination){
 const uint32_t left=*a,right=*b;
 uint32_t status=*fpscr;
 if((status&0x80000u)||(status&3u)>1u)return -1;
 const uint32_t ax=left&0x7fffffffu,bx=right&0x7fffffffu;
 /* NaN priority and all ordinary finite behavior stay in the established core. */
 if(ax>0x7f800000u||bx>0x7f800000u||(ax!=0x7f800000u&&bx!=0x7f800000u))
  return sa_sh4_multiply(a,b,fpscr,destination);
 status&=~0x3f000u;
 const unsigned ad=ax!=0u&&ax<0x800000u,bd=bx!=0u&&bx<0x800000u;
 if((ad||bd)&&!(status&0x40000u)){
  *fpscr=status|0x20000u; /* FPU error precedes infinity arithmetic. */
  return 1;
 }
 const unsigned zero_a=ax==0u||ad,zero_b=bx==0u||bd;
 const unsigned invalid=(ax==0x7f800000u&&zero_b)||(bx==0x7f800000u&&zero_a);
 if(invalid)status|=0x10040u;
 *fpscr=status;
 if((status&0x380u)||(invalid&&(status&0x800u)))return 1;
 *destination=invalid?0x7fbfffffu:((left^right)&0x80000000u)|0x7f800000u;
 return 0;
}
