#ifndef SA_PVR_DMA_BUS_H
#define SA_PVR_DMA_BUS_H
#include "vertex_bus.h"
/* Authored, untimed channel-2 texture transfer service. No game descriptors or
 * helper return values are changed by this backend. */
struct sa_pvr_dma_bus {
 struct sa_vertex_bus parent;
 uint8_t *ram;
 uint32_t *interrupt_pending;
 uint32_t registers[10];
 uint32_t source,destination,length,progress,started,completed,pending,signature;
 int32_t status;
 uint32_t enabled,active;
 uint32_t error_status,error_reads,error_writes,error_signature,error_count,error_dropped;
 uint32_t error_trace[256][3];
};
SA_STAGE_EXPORT int sa_pvr_dma_bus_init(struct sa_pvr_dma_bus *,uint8_t *,
 const struct sa_vertex_bus *,struct sa_vertex_bus *,uint32_t *,const uint32_t *,uint32_t);
#endif
