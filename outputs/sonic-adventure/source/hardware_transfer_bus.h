#ifndef SA_HARDWARE_TRANSFER_BUS_H
#define SA_HARDWARE_TRANSFER_BUS_H
#include "display_timing_bus.h"
struct sa_hardware_transfer_memory {
 struct sa_display_timing_memory timing;
 struct sa_vertex_bus timing_bus;
 const uint32_t *fifo_samples;
 uint32_t fifo_count,fifo_cursor,control_word;
};
/* Borrowed raw FIFO status input; little-endian control-word host storage.
 * No physical FIFO, AICA reset, bus arbitration or automatic completion. */
SA_STAGE_EXPORT void sa_hardware_transfer_bus_init(struct sa_hardware_transfer_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t,uint32_t);
SA_STAGE_EXPORT int sa_hardware_transfer_set_samples(struct sa_hardware_transfer_memory *,const uint32_t *,uint32_t);
#endif
