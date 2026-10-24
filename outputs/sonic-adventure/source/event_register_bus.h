#ifndef SA_EVENT_REGISTER_BUS_H
#define SA_EVENT_REGISTER_BUS_H
#include "interrupt_bus.h"
struct sa_event_memory {
 struct sa_interrupt_memory interrupt;
 struct sa_vertex_bus interrupt_bus;
 uint32_t masks[2];
};
SA_STAGE_EXPORT void sa_event_bus_init(struct sa_event_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
