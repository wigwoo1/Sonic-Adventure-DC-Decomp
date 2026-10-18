#include "owner_table_release.h"
static uint32_t read_word(const uint8_t *p)
{
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void write_word(uint8_t *p,uint32_t value)
{
 for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(value>>(i*8));
}
uint32_t sa_owner_table_release_8c0945d0(uint8_t *ram,uint32_t *sr,uint32_t owner)
{
 /* Retail table has 38 records of 52 bytes; it processes every matching row. */
 for(unsigned index=0;index<38;index++){
  uint8_t *row=ram+0x79e40cu+index*52u;
  if(read_word(row+8u)==owner){
   write_word(row+4u,0u);
   write_word(row+12u,read_word(row+12u)|0x10u);
  }
 }
 *sr|=1u; /* Terminal unsigned bound comparison. */
 return 0u;
}
