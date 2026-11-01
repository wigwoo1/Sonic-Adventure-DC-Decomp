#ifndef SA_TMU0_CLOCK_H
#define SA_TMU0_CLOCK_H
#include "vertex_bus.h"
/* Persistent authored platform state; no guest RAM or borrowed pointers. */
struct sa_tmu0_clock {
 uint64_t peripheral_cycles,underflows;
 uint32_t tcor,tcnt,tcr,tstr,tocr;
};
struct sa_tmu0_binding {struct sa_tmu0_clock *state;struct sa_vertex_bus parent;};
SA_STAGE_EXPORT int sa_tmu0_init(struct sa_tmu0_clock *);
SA_STAGE_EXPORT int sa_tmu0_advance(struct sa_tmu0_clock *,uint64_t);
SA_STAGE_EXPORT int sa_tmu0_read(const struct sa_tmu0_clock *,uint32_t,unsigned,uint32_t *);
SA_STAGE_EXPORT int sa_tmu0_write(struct sa_tmu0_clock *,uint32_t,uint32_t,unsigned);
SA_STAGE_EXPORT uint32_t sa_tmu0_irq_pending(const struct sa_tmu0_clock *);
SA_STAGE_EXPORT int sa_tmu0_bind(struct sa_tmu0_binding *,struct sa_tmu0_clock *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
#endif
