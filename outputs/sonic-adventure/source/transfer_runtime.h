#ifndef SA_TRANSFER_RUNTIME_H
#define SA_TRANSFER_RUNTIME_H
#include "g2_transfer_host.h"
#include "vertex_bus.h"
/* Authored, untimed native platform. Persistent state borrows no stack pointers.
 * A binding borrows RAM and its parent bus only for the duration of one entry. */
struct sa_transfer_runtime {
 uint32_t registers[32],progress[4],active[4];
 struct sa_g2_transfer jobs[4];
 uint32_t pending,polls,transfers,bytes,deliveries,blocked,in_irq,enabled;
};
struct sa_transfer_runtime_binding {
 struct sa_transfer_runtime *state;
 struct sa_vertex_bus parent;
 uint8_t *ram;
};
SA_STAGE_EXPORT int sa_transfer_runtime_init(struct sa_transfer_runtime *,uint32_t);
SA_STAGE_EXPORT int sa_transfer_runtime_bind(struct sa_transfer_runtime_binding *,struct sa_transfer_runtime *,uint8_t *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
SA_STAGE_EXPORT int sa_transfer_runtime_tick(struct sa_transfer_runtime_binding *);
SA_STAGE_EXPORT int sa_transfer_runtime_irq_ready(struct sa_transfer_runtime_binding *,uint32_t *);
#endif
