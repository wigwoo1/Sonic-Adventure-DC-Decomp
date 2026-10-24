#ifndef SA_VIDEO_SIGNAL_BUS_H
#define SA_VIDEO_SIGNAL_BUS_H
#include "cache_register_bus.h"
struct sa_video_signal_memory {
 struct sa_cache_register_memory cache;
 struct sa_vertex_bus cache_bus;
 uint32_t port_data;
};
/* Caller-supplied sampled input; no physical cable or GPIO direction model. */
SA_STAGE_EXPORT void sa_video_signal_bus_init(struct sa_video_signal_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t);
#endif
