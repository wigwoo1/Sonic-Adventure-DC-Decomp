#ifndef JP_NATIVE_DEVICE_H
#define JP_NATIVE_DEVICE_H
#include <stdint.h>
#ifndef JP_DEVICE_API
#define JP_DEVICE_API __declspec(dllexport)
#endif
struct jp_gpu;struct jp_window;
enum jp_device_error {JP_DEVICE_OK,JP_DEVICE_ARGUMENT,JP_DEVICE_STATE,
    JP_DEVICE_THREAD,JP_DEVICE_QUEUE,JP_DEVICE_CLOCK,JP_DEVICE_WAIT,
    JP_DEVICE_GPU,JP_DEVICE_WINDOW,JP_DEVICE_VIDEO,JP_DEVICE_PRESENT,
    JP_DEVICE_GLOBAL,JP_DEVICE_CALLBACK,JP_DEVICE_ACTIVE};
enum jp_device_phase {JP_DEVICE_COLD,JP_DEVICE_PREPARED,JP_DEVICE_GPU_READY,
    JP_DEVICE_DISPLAY_READY,JP_DEVICE_READY,JP_DEVICE_DRAINING};
struct jp_device_options {
    uint32_t render_width,render_height,visible,present_interval,timeout_ms,reserved;
    uint64_t gpu_budget;
};
struct jp_device_info {
    uint32_t phase,status,resources,video,display,bpp,cable;
    uint32_t logical_width,logical_height,render_width,render_height;
    int32_t wait_count;
    uint64_t logical_ticks;
};
/* One creating thread. Borrow ready empty queue/frame/texture owners. Device
 * owns the real GPU/window/video/clock/wait/presenter it creates. No other live
 * presenter/clock/wait may exist. Options default to 640x480, visible, interval
 * 0, 5s timeout, 64MiB job budget. Sizes select rendering, not gameplay rate.
 * The first implementation rejects console anti-alias modes explicitly.
 */
JP_DEVICE_API int jp_device_configure(const struct jp_device_options *);
JP_DEVICE_API int jp_device_initialize(int32_t mode,int32_t frame,int32_t count);
JP_DEVICE_API int jp_device_status(void);
JP_DEVICE_API int jp_device_snapshot(struct jp_device_info *);
/* Borrowed handles: valid until completed shutdown, never close directly.
 * External retained frames/jobs must be released before shutdown completes.
 * A failed drain retains all owners so it can be retried safely. */
JP_DEVICE_API struct jp_gpu *jp_device_gpu(void);
JP_DEVICE_API struct jp_window *jp_device_window(void);
JP_DEVICE_API int jp_device_shutdown(void);
/* Terminal shutdown authority transfer, on the creating thread. Claim only
 * during quiescent startup or a partial drain. The token is never recycled.
 * After claim, ordinary configure/initialize/shutdown cannot replace/close
 * these owners. The exit service quiesces render/wait before task cleanup,
 * then finishes after dependent frames/assets have retired. Failed calls
 * retain ownership; repeated successful quiesce/finish are harmless. */
JP_DEVICE_API int jp_device_exit_claim(uint64_t *token);
JP_DEVICE_API int jp_device_exit_quiesce(uint64_t token);
JP_DEVICE_API int jp_device_exit_finish(uint64_t token);
#endif
