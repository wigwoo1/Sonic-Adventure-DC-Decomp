#ifndef SA_VRAM_APERTURE_BUS_H
#define SA_VRAM_APERTURE_BUS_H
#include "vertex_bus.h"
/* Authored platform adapter; not an original-game routine. The parent's VRAM
 * storage retains the published 0x05000000 (bank-linear) layout. */
struct sa_vram_aperture_event { uint32_t address, mapped, value, operation; };
struct sa_vram_aperture_bus {
 struct sa_vertex_bus parent;
 struct sa_vram_aperture_event *trace;
 uint32_t capacity,count,dropped,attempts,signature;
};
SA_STAGE_EXPORT int sa_vram_aperture_bus_init(struct sa_vram_aperture_bus *,
 const struct sa_vertex_bus *,struct sa_vertex_bus *,
 struct sa_vram_aperture_event *,uint32_t);
#endif
