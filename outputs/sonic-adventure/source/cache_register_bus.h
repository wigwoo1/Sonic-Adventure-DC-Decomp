#ifndef SA_CACHE_REGISTER_BUS_H
#define SA_CACHE_REGISTER_BUS_H
#include "event_register_bus.h"
struct sa_cache_register_memory {
 struct sa_event_memory event;
 struct sa_vertex_bus event_bus;
 uint32_t ccr,ic_requests,oc_requests;
};
SA_STAGE_EXPORT void sa_cache_register_bus_init(struct sa_cache_register_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
