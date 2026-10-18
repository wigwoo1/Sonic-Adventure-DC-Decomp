#include "countdown_registry_runtime.h"
#include "frame_countdown.h"
struct registry_context {
 uint8_t *ram;
 uint32_t *sr,vbr,*mask_a,*mask_b;
};
static uint32_t read_handle(const uint8_t *ram)
{
 const uint8_t *p=ram+0x8900fcu;
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void write_handle(uint8_t *ram,uint32_t handle)
{
 for(unsigned i=0;i<4;i++)ram[0x8900fcu+i]=(uint8_t)(handle>>(i*8));
}
static void remove_event(void *context,uint32_t handle)
{
 struct registry_context *registry=context;
 *registry->sr &= ~1u;
 (void)sa_event_remove_connected(registry->ram,registry->sr,registry->vbr,
  registry->mask_a,registry->mask_b,handle,0);
 write_handle(registry->ram,0u);
}
static uint32_t register_event(void *context,uint32_t event,uint32_t callback,
 uint32_t priority,uint32_t argument)
{
 struct registry_context *registry=context;
 *registry->sr &= ~1u;
 return sa_event_register_connected(registry->ram,registry->sr,registry->vbr,
  registry->mask_a,registry->mask_b,event,callback,priority,argument,0);
}
void sa_countdown_register_in_registry(uint8_t *ram,uint32_t *sr,uint32_t vbr,
 uint32_t *mask_a,uint32_t *mask_b,uint32_t callback)
{
 struct registry_context context={ram,sr,vbr,mask_a,mask_b};
 struct sa_countdown_registration_services services={&context,remove_event,register_event};
 uint32_t handle=read_handle(ram);
 sa_countdown_register_8c605234(&handle,callback,&services);
 if(callback==0u)*sr |= 1u; /* Final TST of the zero callback. */
 write_handle(ram,handle);
}
