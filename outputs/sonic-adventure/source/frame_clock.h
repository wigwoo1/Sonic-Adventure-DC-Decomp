#ifndef SA_FRAME_CLOCK_H
#define SA_FRAME_CLOCK_H
#include "tmu0_clock.h"
#define SA_FRAME_CLOCK_TRACE_CAPACITY 1024u
struct sa_frame_clock_core {
 struct sa_tmu0_clock timer;
 uint64_t fraction,pixels,in_events,out_events,wraps,host_frames,idle_waits;
 uint32_t pclk_hz,video_hz,fb_control,spg_control,load,vblank_int,vblank;
 uint32_t hpos,line,field,pending,enabled;
};
struct sa_frame_clock_state {
 struct sa_frame_clock_core core;
 uint64_t timer_reads,scan_reads;
 uint32_t trace_count;
 struct sa_vertex_bus_event trace[SA_FRAME_CLOCK_TRACE_CAPACITY];
};
struct sa_frame_clock_binding {struct sa_frame_clock_state *state;struct sa_vertex_bus parent;};
SA_STAGE_EXPORT int sa_frame_clock_init(struct sa_frame_clock_state *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
SA_STAGE_EXPORT int sa_frame_clock_begin_entry(struct sa_frame_clock_state *);
SA_STAGE_EXPORT int sa_frame_clock_advance(struct sa_frame_clock_state *,uint64_t);
SA_STAGE_EXPORT int sa_frame_clock_next_in(struct sa_frame_clock_state *,uint32_t);
SA_STAGE_EXPORT uint32_t sa_frame_clock_status(const struct sa_frame_clock_state *);
SA_STAGE_EXPORT int sa_frame_clock_read(struct sa_frame_clock_state *,uint32_t,unsigned,uint32_t *);
SA_STAGE_EXPORT int sa_frame_clock_write(struct sa_frame_clock_state *,uint32_t,uint32_t,unsigned);
SA_STAGE_EXPORT int sa_frame_clock_bind(struct sa_frame_clock_binding *,struct sa_frame_clock_state *,const struct sa_vertex_bus *,struct sa_vertex_bus *);
#endif
