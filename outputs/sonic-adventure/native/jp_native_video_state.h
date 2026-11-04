/* Authored versioned native render-register storage. One game thread.
 * This is captured rendering state, not a replacement for hardware scheduling.
 */
#ifndef JP_NATIVE_VIDEO_STATE_H
#define JP_NATIVE_VIDEO_STATE_H
#include "jp_native_packets.h"
enum jp_video_error {JP_VIDEO_OK,JP_VIDEO_ARGUMENT,JP_VIDEO_UNSUPPORTED,JP_VIDEO_NOT_INITIALIZED,JP_VIDEO_OVERFLOW};
struct jp_video_snapshot {
    uint64_t generation;
    uint32_t palette[1024],fog_table[128];
    uint32_t palette_mode,fog_color,vertex_fog_color,fog_density,clamp_min,clamp_max;
    uint32_t cull_bits,texture_control,fb_write_control,border_color;
    uint32_t logical_width,logical_height,clip_x,clip_y,presort,clear_argb;
    uint32_t background_isp,background_tsp,background_texture,background_bytes;
    uint32_t background_packet[62];
    uint32_t global_clip,global_clip_width,global_clip_height;
};
typedef int (*jp_video_device_source)(void *,struct jp_video_snapshot *);
JP_PACKET_API size_t jp_video_snapshot_size(void);
JP_PACKET_API int jp_video_set_device_source(jp_video_device_source source,void *context);
JP_PACKET_API int jp_video_initialize(unsigned width,unsigned height,uint32_t clear_argb,int presort);
JP_PACKET_API void jp_video_shutdown(void);
JP_PACKET_API int jp_video_status(void);
JP_PACKET_API void jp_video_clear_error(void);
JP_PACKET_API int jp_video_write(uint32_t address,uint32_t value);
JP_PACKET_API int jp_video_read(uint32_t address,uint32_t *value);
JP_PACKET_API int jp_video_set_canvas(unsigned width,unsigned height,uint32_t clear_argb,int presort);
JP_PACKET_API int jp_video_set_clip(unsigned left,unsigned top,unsigned right,unsigned bottom);
JP_PACKET_API int jp_video_capture(struct jp_video_snapshot *out);
/* Implemented by the replacement stateful frame owner. Copies immutable state;
 * on error *out is unchanged. The original owner source remains preserved. */
JP_PACKET_API int jp_frame_video(uint64_t frame,struct jp_video_snapshot *out);
JP_PACKET_API int jp_video_palette_rgba(const struct jp_video_snapshot *state,uint32_t argb[1024]);
#endif
