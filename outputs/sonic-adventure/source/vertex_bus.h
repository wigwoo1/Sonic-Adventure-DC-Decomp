#ifndef SA_VERTEX_BUS_H
#define SA_VERTEX_BUS_H
#include "stage00_task.h"
typedef int (*sa_vertex_read)(void *,uint32_t,unsigned,uint32_t *);
typedef int (*sa_vertex_write)(void *,uint32_t,uint32_t,unsigned);
typedef int (*sa_vertex_cache)(void *,uint32_t,unsigned);
struct sa_vertex_bus {void *context;sa_vertex_read read;sa_vertex_write write;sa_vertex_cache cache;};
struct sa_vertex_bus_event {uint32_t kind,address,value,width;};
/* Bounded, coherent-RAM, MMU-off software queue model. Not a cache/GPU emulator. */
struct sa_vertex_memory {
 uint8_t *ram;uint32_t qacr[2],queue[16],invalidates,prefetches,flushes,trace_count,trace_dropped;
 struct sa_vertex_bus_event trace[4096];
};
SA_STAGE_EXPORT void sa_vertex_bus_init(struct sa_vertex_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
