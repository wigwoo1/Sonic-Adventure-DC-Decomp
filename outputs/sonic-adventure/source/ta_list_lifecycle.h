#ifndef SA_TA_LIST_LIFECYCLE_H
#define SA_TA_LIST_LIFECYCLE_H
#include "pixel_jobs.h"
#include "frame_events_bus.h"
struct sa_ta_list_lifecycle {
 uint64_t inits,retired,discarded,soft_resets;
 uint32_t opb_base,isp_base,last_count,enabled;
 struct sa_frame_command_packet last[SA_FRAME_COMMAND_CAPACITY];
};
struct sa_ta_list_binding {
 struct sa_ta_list_lifecycle *state;
 struct sa_frame_command_memory *frame;
 struct sa_frame_events_bus *events;
 struct sa_pixel_job_state *job;
 struct sa_vertex_bus parent;
};
SA_STAGE_EXPORT void sa_ta_list_lifecycle_init(struct sa_ta_list_lifecycle *,uint32_t);
SA_STAGE_EXPORT int sa_ta_list_lifecycle_bind(struct sa_ta_list_binding *,struct sa_ta_list_lifecycle *,struct sa_frame_command_memory *,struct sa_frame_events_bus *,struct sa_pixel_job_state *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
#endif
