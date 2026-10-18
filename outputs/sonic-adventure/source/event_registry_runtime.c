#include "event_registry.h"
#include "callback_install.h"
struct install_context {
 uint8_t *ram;
 const struct sa_event_registry_services *external;
};
static void install(void *context,uint32_t service,uint32_t callback,uint32_t argument,uint32_t *sr)
{
 struct install_context *runtime=context;
 if(service==0x8c64135au)
  (void)sa_vblank_callback_register_8c64135a(runtime->ram,sr,callback,argument);
 else if(service==0x8c64137cu)
  (void)sa_secondary_callback_register_8c64137c(runtime->ram,sr,callback,argument);
 else runtime->external->install(runtime->external->context,service,callback,argument,sr);
}
uint32_t sa_event_register_connected(uint8_t *ram,uint32_t *sr,uint32_t vbr,
 uint32_t *mask_a,uint32_t *mask_b,uint32_t event,uint32_t callback,
 uint32_t priority,uint32_t argument,const struct sa_event_registry_services *external)
{
 struct install_context context={ram,external};
 struct sa_event_registry_services services={&context,install};
 return sa_event_register_8c64594c(ram,sr,vbr,mask_a,mask_b,event,callback,priority,argument,&services);
}
uint32_t sa_event_remove_connected(uint8_t *ram,uint32_t *sr,uint32_t vbr,
 uint32_t *mask_a,uint32_t *mask_b,uint32_t handle,
 const struct sa_event_registry_services *external)
{
 struct install_context context={ram,external};
 struct sa_event_registry_services services={&context,install};
 return sa_event_remove_8c645c54(ram,sr,vbr,mask_a,mask_b,handle,&services);
}
