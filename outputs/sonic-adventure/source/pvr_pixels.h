#ifndef SA_PVR_PIXELS_H
#define SA_PVR_PIXELS_H
#include "stage00_task.h"
/* Authored bounded host renderer. It is not a bit-exact PowerVR implementation.
 * Header/layout facts: KallistiOS pvr_header.h/pvr_texture.c; mip placement
 * corroborated by Flycast TexCache.cpp. No third-party implementation copied. */
struct sa_pixel_texture {uint32_t width,height,address,format,linear;};
struct sa_pixel_options {
 uint32_t width,height,input_width,input_height,textures,mip_level,cull_area,alpha_ref;
};
struct sa_pixel_stats {uint64_t triangles,fragments,depth_passed,texture_samples,pixels;};
SA_STAGE_EXPORT int sa_pixel_texture_info(uint32_t,uint32_t,uint32_t,struct sa_pixel_texture *);
SA_STAGE_EXPORT int sa_pixel_texture_decode(const uint8_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t,struct sa_pixel_texture *);
SA_STAGE_EXPORT int sa_pixel_draw(const uint32_t *,uint32_t,const uint8_t *,uint32_t,uint32_t *,uint32_t *,uint64_t,const struct sa_pixel_options *,struct sa_pixel_stats *);
#endif
