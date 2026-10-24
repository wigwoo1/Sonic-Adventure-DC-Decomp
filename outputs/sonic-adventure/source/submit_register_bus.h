#ifndef SA_SUBMIT_REGISTER_BUS_H
#define SA_SUBMIT_REGISTER_BUS_H
#include "frame_command_bus.h"
/* Bounded command-register storage only. No DMA, interrupts or completion. */
struct sa_submit_register_memory {
 struct sa_frame_command_memory frame;
 struct sa_vertex_bus frame_bus;
 uint32_t dma_registers[7];
};
SA_STAGE_EXPORT void sa_submit_register_bus_init(struct sa_submit_register_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
