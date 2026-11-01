#ifndef SA_PIXEL_JOBS_H
#define SA_PIXEL_JOBS_H
#include "pvr_pixels.h"
#include "render_completion.h"
#include "frame_command_bus.h"
struct sa_pixel_job_state {
 uint64_t job_id,processed,last_job;
 uint32_t cursor,count,ready,words[512];
 struct sa_pixel_stats stats;
};
struct sa_pixel_job_binding {struct sa_pixel_job_state *state;struct sa_render_completion *render;const struct sa_frame_command_memory *frame;struct sa_vertex_bus parent;};
SA_STAGE_EXPORT void sa_pixel_job_init(struct sa_pixel_job_state *);
SA_STAGE_EXPORT int sa_pixel_job_bind(struct sa_pixel_job_binding *,struct sa_pixel_job_state *,struct sa_render_completion *,const struct sa_frame_command_memory *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
SA_STAGE_EXPORT int sa_pixel_job_execute(struct sa_pixel_job_state *,struct sa_render_completion *,const uint8_t *,uint32_t,uint32_t *,uint32_t *,uint64_t,const struct sa_pixel_options *);
#endif
