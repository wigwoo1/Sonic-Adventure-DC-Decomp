/* Authored native submission path. No game simulation or vsync scheduler. */
#ifndef JP_NATIVE_DIRECT_WINDOW_H
#define JP_NATIVE_DIRECT_WINDOW_H
#include "jp_native_window.h"
/* Render the retained frame directly into this window's current backbuffer.
 * No image readback, staging allocation, CPU image buffer or image upload.
 * Success means commands submitted, not GPU completion or physical scanout.
 * The caller may release the retained frame after return: GPU inputs are owned
 * D3D resources. Explicit readback is a separate diagnostic operation.
 * Validation failures preserve backbuffer/report. Device failure after issuing
 * commands can alter the backbuffer; no Present is issued by this function.
 * All window operations must run on its creating thread. */
JP_PACKET_API int jp_window_render_frame(struct jp_window *,uint64_t,
    const struct jp_gpu_config *,struct jp_gpu_report *,struct jp_gpu_failure *);
/* Explicit-state counterpart of jp_gpu_draw_frame for consumers which already
 * own a configuration. Does not bind the frame's captured video registers.
 * Dimensions always follow the window. Production game integration uses the
 * captured-state function above. */
JP_PACKET_API int jp_window_render_configured_frame(struct jp_window *,uint64_t,
    const struct jp_gpu_config *,struct jp_gpu_report *,struct jp_gpu_failure *);
/* Render then Present, preserving the old research/readback API separately.
 * Present failure preserves report/counter, but rendering may have succeeded.
 * Interval is 0 or 1; neither option establishes 60/120/240 gameplay pacing. */
JP_PACKET_API int jp_window_draw_direct_frame(struct jp_window *,uint64_t,
    const struct jp_gpu_config *,unsigned interval,
    struct jp_gpu_report *,struct jp_gpu_failure *);
#endif
