#ifndef SA_FRAME_COMMAND_BUS_H
#define SA_FRAME_COMMAND_BUS_H
#include "render_register_bus.h"
#define SA_FRAME_COMMAND_CAPACITY 64u
struct sa_frame_command_packet {uint32_t destination,queue,words[8];};
/* Captures accepted SQ packets; no GPU consumption, status, timing or pixels. */
struct sa_frame_command_memory {
 struct sa_render_register_memory graphics;
 struct sa_vertex_bus graphics_bus;
 uint32_t packet_count,packet_capacity;
 struct sa_frame_command_packet packets[SA_FRAME_COMMAND_CAPACITY];
};
SA_STAGE_EXPORT void sa_frame_command_bus_init(struct sa_frame_command_memory *,uint8_t *,struct sa_vertex_bus *);
#endif
