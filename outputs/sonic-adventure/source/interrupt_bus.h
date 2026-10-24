#ifndef SA_INTERRUPT_BUS_H
#define SA_INTERRUPT_BUS_H
#include "submit_register_bus.h"
#define SA_INTERRUPT_VRAM_WORDS 2097152u
/* Explicit 32-bit VRAM aperture and normal-event acknowledge storage.
 * No automatic IRQ delivery, DMA transfer, GPU execution or physical timing. */
struct sa_interrupt_memory {
 struct sa_submit_register_memory submit;
 struct sa_vertex_bus submit_bus;
 uint32_t normal_status;
 uint32_t vram[SA_INTERRUPT_VRAM_WORDS];
};
SA_STAGE_EXPORT void sa_interrupt_bus_init(struct sa_interrupt_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
