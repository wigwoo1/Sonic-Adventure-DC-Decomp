/* Authored decoder for captured Kamui three-point background parameters. */
#ifndef JP_NATIVE_BACKGROUND_H
#define JP_NATIVE_BACKGROUND_H
#include "jp_native_video_state.h"
/* Allocates a combined plan, leaving inputs and *out unchanged on failure.
 * Native interpolation extends the source plane over the logical viewport.
 * Caller frees the resulting plan through jp_packets_destroy. */
JP_PACKET_API int jp_background_prepend(const struct jp_video_snapshot *video,
    const struct jp_packet_plan *foreground,size_t budget,struct jp_packet_plan **out);
#endif
