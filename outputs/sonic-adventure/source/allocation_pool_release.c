#include "allocation_pool_release.h"
uint32_t sa_allocation_pool_release_8c664aec(uint8_t *ram,uint32_t *sr,uint32_t address,uint32_t incoming_r0)
{
 for(uint32_t offset=0u;offset<0x18000u;offset+=0x18u){
  if(address==0x8c8a83e4u+offset)ram[0x8a83e4u+offset]&=0xfeu;
 }
 *sr|=1u; /* Final unsigned bound comparison; r0 is never written. */
 return incoming_r0;
}
