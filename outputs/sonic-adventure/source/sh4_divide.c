#include "sh4_divide.h"
static uint32_t quotient(uint32_t a,uint32_t b,uint32_t mode,int *inexact){
 uint32_t sign=(a^b)&0x80000000u;
 if((a&0x7fffffffu)==0u){*inexact=0;return sign;}
 uint32_t ma=(a&0x7fffffu)|0x800000u,mb=(b&0x7fffffu)|0x800000u;
 int power=(int)((a>>23)&255u)-(int)((b>>23)&255u),exponent=power-(ma<mb);
 if(exponent>127){*inexact=1;return sign|(mode==0u?0x7f800000u:0x7f7fffffu);}
 int quantum=exponent-23;if(quantum< -149)quantum=-149;
 int shift=power-quantum;
 if(shift< -1){*inexact=1;return sign;}
 uint64_t numerator=ma,denominator=mb;
 if(shift>=0)numerator<<=(unsigned)shift;else denominator<<=(unsigned)(-shift);
 uint32_t significand=(uint32_t)(numerator/denominator);uint64_t remainder=numerator%denominator;*inexact=remainder!=0u;
 if(mode==0u && (remainder*2u>denominator || (remainder*2u==denominator && (significand&1u))))significand++;
 if(significand>=0x1000000u){significand>>=1;quantum++;}
 if(quantum+150>=255){*inexact=1;return sign|(mode==0u?0x7f800000u:0x7f7fffffu);}
 if(significand<0x800000u)return sign|significand;
 return sign|((uint32_t)(quantum+150)<<23)|(significand&0x7fffffu);
}
int sa_sh4_divide(uint32_t *a,uint32_t *b,uint32_t *fpscr,uint32_t *destination){
 uint32_t status=*fpscr,mode=status&3u,aa=*a&0x7fffffffu,bb=*b&0x7fffffffu;
 if((status&0x80000u)!=0u || mode>1u || aa>=0x7f800000u || bb>=0x7f800000u)return -1;
 status&=~0x3f000u;
 if((status&0x40000u)==0u && ((aa!=0u && aa<0x800000u) || (bb!=0u && bb<0x800000u))){*fpscr=status|0x20000u;return 1;}
 if((status&0x40000u)!=0u){if(aa<0x800000u)*a&=0x80000000u;if(bb<0x800000u)*b&=0x80000000u;}
 if(!(*b&0x7fffffffu)){
  unsigned invalid=!(*a&0x7fffffffu);status|=invalid?0x10040u:0x8020u;*fpscr=status;
  if(status&(invalid?0x800u:0x400u))return 1;
  *destination=invalid?0x7fbfffffu:((*a^*b)&0x80000000u)|0x7f800000u;return 0;
 }
 uint32_t left=*a,right=*b;int inexact,unused;uint32_t result=quotient(left,right,mode,&inexact),nearest=quotient(left,right,0u,&unused);
 if((nearest&0x7fffffffu)==0x7f800000u){status|=0x4010u;inexact=1;}
 if((result&0x7fffffffu)<0x800000u){if((status&0x40000u)!=0u && (result&0x7fffffffu)!=0u){result&=0x80000000u;inexact=1;}if(inexact)status|=0x2008u;}
 if(inexact)status|=0x1004u;*fpscr=status;
 if((status&0x380u)!=0u)return 1;*destination=result;return 0;
}
