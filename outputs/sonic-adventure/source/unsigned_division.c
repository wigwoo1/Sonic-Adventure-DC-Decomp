#include "unsigned_division.h"
uint32_t sa_unsigned_division_8c10cb10(uint8_t *ram,uint32_t *sr,uint32_t numerator,uint32_t denominator,uint32_t *r1)
{
 *sr=(*sr&~1u)|(denominator==0u);
 if(denominator==0u){
  ram[0x7ac108]=0x4e;ram[0x7ac109]=4;ram[0x7ac10a]=0;ram[0x7ac10b]=0;
  *r1=0x44eu;return 0u;
 }
 /* DIV0U followed by 32 ROTCL/DIV1 pairs and a final ROTCL. M remains zero. */
 *sr&=~0x301u;
 uint32_t remainder=0u,q=0u,t=0u;
 for(unsigned bit=0;bit<32;bit++){
  uint32_t next=numerator>>31;numerator=(numerator<<1)|t;t=next;
  uint32_t old_q=q;q=remainder>>31;
  remainder=(remainder<<1)|t;
  uint32_t before=remainder;
  if(old_q==0u){
   remainder-=denominator;
   uint32_t borrow=remainder>before;
   q=q?(!borrow):borrow;
  }else{
   remainder+=denominator;
   uint32_t carry=remainder<before;
   q=q?(!carry):carry;
  }
  t=!q;
 }
 uint32_t final_t=numerator>>31;
 numerator=(numerator<<1)|t;
 *sr=(*sr&~0x101u)|(q<<8)|final_t;
 *r1=numerator;return numerator;
}
