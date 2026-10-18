#include "callback_install.h"
static void write_word(uint8_t *ram,uint32_t address,uint32_t value)
{
 uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(value>>(i*8));
}
void sa_callback_install_8c6412c2(uint8_t *ram,uint32_t *sr,uint32_t driver,
 uint32_t index,uint32_t callback,uint32_t argument,void *context,
 void (*remove)(void *,uint32_t,uint32_t,uint32_t *))
{
 uint32_t saved_mask=*sr&0xf0u;
 *sr=(*sr&0xffffff0fu)|0xf0u;
 if(callback!=0u) {
  *sr=(*sr&~1u)|((index>>29)&1u); /* SHLL after SHLL2 supplies T. */
  uint32_t slot=driver+0x230u+(index<<3);
  write_word(ram,slot,callback);
  write_word(ram,slot+4u,argument);
 } else {
  *sr|=1u;
  remove(context,driver,index,sr);
 }
 *sr=(*sr&0xffffff0fu)|saved_mask;
}
/* Retail 0x8c64133e..0x8c64135a. */
SA_STAGE_EXPORT int sa_callback_register_primary_8c64133e(uint8_t *ram,
 uint32_t *sr,uint32_t callback,uint32_t argument,void *context,
 void (*remove)(void *,uint32_t,uint32_t,uint32_t *))
{
 uint32_t driver=0;
 const uint8_t *slot=ram+0x67c004u;
 for(unsigned i=0;i<4;i++)driver|=(uint32_t)slot[i]<<(i*8);
 sa_callback_install_8c6412c2(ram,sr,driver,0u,callback,argument,context,remove);
 return 0;
}
static int register_callback_index(uint8_t *ram,uint32_t *sr,uint32_t index,
 uint32_t callback,uint32_t argument,void *context,
 void (*remove)(void *,uint32_t,uint32_t,uint32_t *))
{
 uint32_t driver=0;
 const uint8_t *slot=ram+0x67c004u;
 for(unsigned i=0;i<4;i++)driver|=(uint32_t)slot[i]<<(i*8);
 sa_callback_install_8c6412c2(ram,sr,driver,index,callback,argument,context,remove);
 return 0;
}
/* Retail 0x8c6415f2..0x8c64160d. */
SA_STAGE_EXPORT int sa_callback_register_index3_8c6415f2(uint8_t *ram,
 uint32_t *sr,uint32_t callback,uint32_t argument,void *context,
 void (*remove)(void *,uint32_t,uint32_t,uint32_t *))
{ return register_callback_index(ram,sr,3u,callback,argument,context,remove); }
/* Retail 0x8c64162a..0x8c641645. */
SA_STAGE_EXPORT int sa_callback_register_index5_8c64162a(uint8_t *ram,
 uint32_t *sr,uint32_t callback,uint32_t argument,void *context,
 void (*remove)(void *,uint32_t,uint32_t,uint32_t *))
{ return register_callback_index(ram,sr,5u,callback,argument,context,remove); }
/* Retail 0x8c64167e..0x8c64169a. */
SA_STAGE_EXPORT int sa_callback_register_index8_8c64167e(uint8_t *ram,
 uint32_t *sr,uint32_t callback,uint32_t argument,void *context,
 void (*remove)(void *,uint32_t,uint32_t,uint32_t *))
{ (void)argument; return register_callback_index(ram,sr,8u,callback,0u,context,remove); }
void sa_callback_default_8c6411b0(uint8_t *ram,uint32_t *sr,uint32_t driver,uint32_t index)
{
 static const uint32_t defaults[9]={0x8c641698u,0x8c64169cu,0x8c6416a0u,
  0x8c6416a4u,0x8c6416e0u,0x8c6416e4u,0x8c6416e8u,0x8c6416ecu,0x8c6416f0u};
 *sr=(*sr&~1u)|(index<9u?1u:0u);
 if(index<9u)write_word(ram,driver+0x230u+index*8u,defaults[index]);
}
static void restore_default(void *context,uint32_t driver,uint32_t index,uint32_t *sr)
{
 sa_callback_default_8c6411b0(context,sr,driver,index);
}
void sa_callback_install_connected(uint8_t *ram,uint32_t *sr,uint32_t driver,
 uint32_t index,uint32_t callback,uint32_t argument)
{
 sa_callback_install_8c6412c2(ram,sr,driver,index,callback,argument,ram,restore_default);
}
uint32_t sa_driver_enable_8c658c20(uint32_t index)
{
 (void)index;
 return 1u;
}
uint32_t sa_vblank_callback_register_8c64135a(uint8_t *ram,uint32_t *sr,
 uint32_t callback,uint32_t argument)
{
 (void)sa_driver_enable_8c658c20(1u);
 const uint8_t *p=ram+0x67c004u;
 uint32_t driver=(uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
 sa_callback_install_connected(ram,sr,driver,1u,callback,argument);
 return 0u;
}
uint32_t sa_driver_enable_8c658c24(uint32_t enabled)
{
 (void)enabled;
 return 1u;
}
uint32_t sa_secondary_callback_register_8c64137c(uint8_t *ram,uint32_t *sr,
 uint32_t callback,uint32_t argument)
{
 *sr=(*sr&~1u)|(callback==0u?1u:0u);
 (void)sa_driver_enable_8c658c24(callback!=0u?1u:0u);
 const uint8_t *p=ram+0x67c004u;
 uint32_t driver=(uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
 sa_callback_install_connected(ram,sr,driver,2u,callback,argument);
 return 0u;
}
