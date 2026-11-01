#ifndef SA_RENDER_COMPLETION_H
#define SA_RENDER_COMPLETION_H
#include "vertex_bus.h"
#define SA_RENDER_PARAMETER_COUNT 16u
/* Authored host-renderer lifecycle, not a rasterizer or measured hardware time.
 * A backend must explicitly acknowledge the submitted job before IRQ generation.
 * Register facts: KallistiOS dc/pvr/pvr_regs.h and dc/asic.h. No code copied. */
struct sa_render_completion {
 uint64_t submitted,completed,cancelled,active_id;
 uint32_t pending,enabled,parameters[SA_RENDER_PARAMETER_COUNT],job[SA_RENDER_PARAMETER_COUNT];
};
struct sa_render_completion_binding {struct sa_render_completion *state;struct sa_vertex_bus parent;};
SA_STAGE_EXPORT void sa_render_completion_init(struct sa_render_completion *,const uint32_t *);
SA_STAGE_EXPORT int sa_render_completion_finish(struct sa_render_completion *,uint64_t);
SA_STAGE_EXPORT int sa_render_completion_bind(struct sa_render_completion_binding *,struct sa_render_completion *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
#endif
