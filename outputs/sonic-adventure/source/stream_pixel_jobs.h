#ifndef SA_STREAM_PIXEL_JOBS_H
#define SA_STREAM_PIXEL_JOBS_H
#include "ta_stream.h"
#include "pixel_jobs.h"
SA_STAGE_EXPORT int sa_ta_stream_restore_packet(struct sa_ta_stream *,unsigned,const struct sa_frame_command_packet *);
SA_STAGE_EXPORT int sa_ta_stream_set_cursor(struct sa_ta_stream *,uint32_t);
SA_STAGE_EXPORT int sa_ta_stream_clone_range(struct sa_ta_stream *,uint32_t,struct sa_ta_stream **);
SA_STAGE_EXPORT int sa_ta_stream_flatten(struct sa_ta_stream *,uint32_t **,uint32_t *);
SA_STAGE_EXPORT void sa_ta_stream_release_flat(struct sa_ta_stream *,uint32_t *);
struct sa_stream_pixel_job {
 uint64_t job_id,processed,last_job;
 uint32_t cursor,count,ready;
 struct sa_pixel_stats stats;
 struct sa_ta_stream *snapshot;
};
struct sa_stream_pixel_binding {
 struct sa_stream_pixel_job *state;struct sa_render_completion *render;
 struct sa_ta_stream *stream;struct sa_vertex_bus parent;
};
SA_STAGE_EXPORT void sa_stream_pixel_job_dispose(struct sa_stream_pixel_job *);
SA_STAGE_EXPORT int sa_stream_pixel_job_bind(struct sa_stream_pixel_binding *,struct sa_stream_pixel_job *,struct sa_render_completion *,struct sa_ta_stream *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
SA_STAGE_EXPORT int sa_stream_pixel_job_execute(struct sa_stream_pixel_job *,struct sa_render_completion *,const uint8_t *,uint32_t,uint32_t *,uint32_t *,uint64_t,const struct sa_pixel_options *);
SA_STAGE_EXPORT int sa_stream_banked_pixel_job_execute(struct sa_stream_pixel_job *,struct sa_render_completion *,const uint8_t *,uint32_t,uint8_t *,uint32_t,uint32_t *,uint32_t *,uint64_t,const struct sa_pixel_options *);
#endif
