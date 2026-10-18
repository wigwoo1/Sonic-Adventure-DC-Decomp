#include "frame_runtime.h"
static void register_tick(void *context,uintptr_t token)
{
 struct sa_frame_runtime *runtime=context;
 sa_countdown_register_8c605234(runtime->registration_handle,(uint32_t)token,
  &runtime->registration);
}
static void configured(void *context)
{
 struct sa_frame_runtime *runtime=context;
 runtime->configured_8c055c44(runtime->context);
}
void sa_frame_runtime_configure(struct sa_frame_runtime *runtime,uint32_t ticks,uint32_t limit)
{
 struct sa_frame_countdown_services services={runtime,register_tick,configured};
 sa_frame_countdown_configure_8c051760(runtime->state,ticks,limit,&services);
}
void sa_frame_runtime_deliver_tick(struct sa_frame_runtime *runtime)
{
 sa_frame_countdown_tick_8c051810(runtime->state,runtime->mode,
  runtime->dispatch_table,&runtime->tick);
}
