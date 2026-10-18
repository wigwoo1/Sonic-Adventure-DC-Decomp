#include "entity_allocation_release.h"
uint32_t sa_entity_allocation_release_8c07d6c0(uint8_t *ram,uint32_t *sr,uint32_t entity,
 void *context,uint32_t (*release)(void *,uint32_t,uint32_t *))
{
 uint8_t *slot=ram+((entity&0x1fffffffu)-0x0c000000u)+0x78u;
 uint32_t allocation=(uint32_t)slot[0]|((uint32_t)slot[1]<<8)|
  ((uint32_t)slot[2]<<16)|((uint32_t)slot[3]<<24);
 *sr=(*sr&~1u)|(allocation==0u);
 if(allocation!=0u){
  (void)release(context,allocation,sr);
  /* Clear the original entity slot even if the nested call changes it. */
  for(unsigned byte=0;byte<4;byte++)slot[byte]=0;
 }
 return 0x78u;
}
