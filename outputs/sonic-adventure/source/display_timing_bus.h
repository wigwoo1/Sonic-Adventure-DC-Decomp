#ifndef SA_DISPLAY_TIMING_BUS_H
#define SA_DISPLAY_TIMING_BUS_H
#include "video_signal_bus.h"
struct sa_display_timing_memory {
 struct sa_video_signal_memory video;
 struct sa_vertex_bus video_bus;
 const uint32_t *samples;
 uint32_t sample_count,sample_cursor;
};
/* Borrowed raw SPG_STATUS observations; producer retains storage until reset.
 * Each supported read consumes one sample. Exhaustion returns -4 without
 * inventing a transition. This does not generate timing or host display output. */
SA_STAGE_EXPORT void sa_display_timing_bus_init(struct sa_display_timing_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t);
SA_STAGE_EXPORT int sa_display_timing_set_samples(struct sa_display_timing_memory *,const uint32_t *,uint32_t);
#endif
