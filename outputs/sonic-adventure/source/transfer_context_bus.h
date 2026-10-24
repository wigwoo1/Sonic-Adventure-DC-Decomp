#ifndef SA_TRANSFER_CONTEXT_BUS_H
#define SA_TRANSFER_CONTEXT_BUS_H
#include "hardware_transfer_bus.h"
#define SA_TRANSFER_CONTEXT_WORDS 48u
struct sa_transfer_context_memory {
 struct sa_hardware_transfer_memory hardware;
 struct sa_vertex_bus hardware_bus;
 uint32_t transfer_registers[SA_TRANSFER_CONTEXT_WORDS];
};
/* Explicit word storage, physical/P1/P2 aliases; no DMA/IRQ progression. */
SA_STAGE_EXPORT void sa_transfer_context_bus_init(struct sa_transfer_context_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t,uint32_t);
#endif
