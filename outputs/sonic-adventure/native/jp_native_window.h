#ifndef JP_NATIVE_WINDOW_H
#define JP_NATIVE_WINDOW_H
#include "jp_native_gpu.h"
#include "jp_native_video_state.h"
struct jp_window;
JP_PACKET_API int jp_gpu_draw_bound_frame(struct jp_gpu *,uint64_t,const struct jp_gpu_config *,void *,size_t,struct jp_gpu_report *,struct jp_gpu_failure *);
/* A window shares the renderer's hardware device. Close windows before GPU
 * shutdown; premature GPU close is deferred safely until its windows close.
 * Window calls and message pumping run on their creating game thread.
 */
JP_PACKET_API int jp_window_open(struct jp_gpu *,unsigned width,unsigned height,int visible,struct jp_window **out,struct jp_gpu_failure *);
JP_PACKET_API void jp_window_close(struct jp_window *);
JP_PACKET_API int jp_window_pump(struct jp_window *); /* nonzero if still open */
JP_PACKET_API int jp_window_resize(struct jp_window *,unsigned width,unsigned height,struct jp_gpu_failure *);
JP_PACKET_API int jp_window_upload(struct jp_window *,const void *rgba,size_t bytes,struct jp_gpu_failure *);
JP_PACKET_API int jp_window_readback(struct jp_window *,void *rgba,size_t bytes,struct jp_gpu_failure *);
JP_PACKET_API int jp_window_present(struct jp_window *,unsigned interval,struct jp_gpu_failure *);
JP_PACKET_API uint64_t jp_window_presented(struct jp_window *);
JP_PACKET_API int jp_window_size(struct jp_window *,unsigned *width,unsigned *height);
/* Initial synchronous presentation path: real bound frame -> renderer readback
 * -> swapchain upload -> Present. The extra round trip is explicit temporary
 * integration work, not a 60/120/240 FPS performance qualification. */
JP_PACKET_API int jp_window_draw_frame(struct jp_window *,uint64_t,const struct jp_gpu_config *,unsigned interval,struct jp_gpu_report *,struct jp_gpu_failure *);
#endif
