#ifndef SA_FRAME_EVENTS_BUS_H
#define SA_FRAME_EVENTS_BUS_H
#include "frame_command_bus.h"
/* Authored untimed TA-list service. It consumes accepted command packets,
 * produces list-completion status only, and does not render pixels. */
struct sa_frame_events_bus {
 struct sa_vertex_bus parent;
 struct sa_frame_command_memory *frame;
 uint32_t *interrupt_pending;
 uint32_t cursor,pending,list,active,headers,vertices,ends,rejected,enabled;
};
SA_STAGE_EXPORT int sa_frame_events_bus_init(struct sa_frame_events_bus *,struct sa_frame_command_memory *,const struct sa_vertex_bus *,struct sa_vertex_bus *,uint32_t *,const uint32_t *,uint32_t);
#endif
