#ifndef SA_DRIVER_SIGNAL_BUS_H
#define SA_DRIVER_SIGNAL_BUS_H
#include "startup_platform_bus.h"
struct sa_driver_signal_memory {
 struct sa_startup_platform_memory startup;
 struct sa_vertex_bus startup_bus;
 const uint32_t *samples;
 uint32_t count,cursor,configured;
 uint32_t dma_operation;
};
SA_STAGE_EXPORT void sa_driver_signal_bus_init(struct sa_driver_signal_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t,uint32_t);
SA_STAGE_EXPORT int sa_driver_signal_set_samples(struct sa_driver_signal_memory *,const uint32_t *,uint32_t);
#endif
