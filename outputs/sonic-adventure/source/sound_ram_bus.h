#ifndef SA_SOUND_RAM_BUS_H
#define SA_SOUND_RAM_BUS_H
#include "vertex_bus.h"
/* Borrowed mutable 2-MiB memory and append-only trace output. No ARM execution,
 * automatic device progress, fabricated completion, timing or audio output. */
struct sa_sound_ram_memory {
 struct sa_vertex_bus parent;
 uint8_t *ram;
 struct sa_vertex_bus_event *trace;
 uint32_t bytes,trace_capacity,trace_count,trace_dropped;
 uint64_t reads,writes,read_bytes,write_bytes;
};
SA_STAGE_EXPORT int sa_sound_ram_bus_init(struct sa_sound_ram_memory *,const struct sa_vertex_bus *,uint8_t *,uint32_t,struct sa_vertex_bus_event *,uint32_t,struct sa_vertex_bus *);
#endif
