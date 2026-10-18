#include "driver_event_runtime.h"
#include "vblank_dispatch.h"
struct event_context {
 uint8_t *ram;
 void *context;
 uint32_t (*invoke)(void *,uint32_t,uint32_t,uint32_t *);
 uint32_t last_r0;
};
static void event_callback(void *context,uint32_t callback,uint32_t argument,uint32_t *sr)
{
 struct event_context *runtime=context;
 runtime->last_r0=runtime->invoke(runtime->context,callback,argument,sr);
}
static uint32_t driver_callback(void *context,uint32_t callback,uint32_t argument,uint32_t *sr)
{
 struct event_context *runtime=context;
 if(callback==0x8c645b1eu){
  /* Empty retail event dispatch leaves its entry mask computation in r0;
   * populated dispatch leaves the final callback's result there instead. */
  runtime->last_r0=(*sr&0xffffff0fu)|0x60u;
  sa_vblank_dispatch_8c645b1e(runtime->ram,sr,runtime,event_callback);
  return runtime->last_r0;
 }
 return runtime->invoke(runtime->context,callback,argument,sr);
}
uint32_t sa_driver_event_dispatch_connected(uint8_t *ram,uint32_t *sr,uint32_t index,
 void *context,uint32_t (*invoke)(void *,uint32_t,uint32_t,uint32_t *))
{
 struct event_context runtime={ram,context,invoke,0u};
 return sa_driver_dispatch_8c6524d2(ram,sr,index,&runtime,driver_callback);
}
