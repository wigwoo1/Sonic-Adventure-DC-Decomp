#ifndef SA_RENDER_REGISTER_BUS_H
#define SA_RENDER_REGISTER_BUS_H
#include "vertex_bus.h"
#define SA_RENDER_REGISTER_WORDS 2048u
/* Bounded host register storage, not GPU execution/status/timing emulation. */
struct sa_render_register_memory {
 struct sa_vertex_memory vertex;
 struct sa_vertex_bus vertex_bus;
 uint32_t registers[SA_RENDER_REGISTER_WORDS];
};
SA_STAGE_EXPORT void sa_render_register_bus_init(struct sa_render_register_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
