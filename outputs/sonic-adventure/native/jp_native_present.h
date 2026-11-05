/* Native Kamui-facing submission/presentation, one game-thread owner. */
#ifndef JP_NATIVE_PRESENT_H
#define JP_NATIVE_PRESENT_H
#include "jp_native_gpu_jobs.h"
#include "jp_native_frame_wait.h"
struct kmi_device_extension;
struct kmi_interrupt_statistics;
enum jp_present_error {JP_PRESENT_OK,JP_PRESENT_ARGUMENT,JP_PRESENT_NOT_INITIALIZED,
    JP_PRESENT_STATE,JP_PRESENT_CLOCK,JP_PRESENT_FRAME,JP_PRESENT_GPU,
    JP_PRESENT_WAIT,JP_PRESENT_TIMEOUT,JP_PRESENT_THREAD,JP_PRESENT_REENTRANT,
    JP_PRESENT_TIMING,JP_PRESENT_PENDING};
struct jp_present_info {
    uint64_t frame,job,submissions,completions,presentations,eor_calls;
    uint64_t submitted_tick,required_tick,presented_tick,timeouts;
    uint32_t status,ready,history,gpu_timing_valid,timeout_ms,interval;
};
/* EOV means native submission has copied the captured vertex lists into an
 * owned GPU job, not that the GPU has finished drawing. EOR follows observed
 * GPU completion. Fatal codes retain 4 for a flip timeout; other native
 * terminal errors use 0x10000 + jp_present_error, separate from console codes.
 * Callback arguments are borrowed. Dispatch is on the creating game thread.
 * Read-only snapshots are allowed in callbacks; mutation/reentry is rejected.
 * Removing the fatal handler leaves the terminal error observable through
 * status; the native default does not enter the console's infinite halt loop.
 */
struct jp_present_callback_info {
    uint64_t eov_calls,fatal_calls;
    uint32_t last_fatal,reserved;
};
JP_CLOCK_API int jp_present_callback_snapshot(struct jp_present_callback_info *);
struct jp_present_cadence_info {uint64_t tick;int32_t remaining,wait;};
/* Native progressive logical ticks, not physical scanout/interlaced fields.
 * A changed count applies when the current countdown expires. At least one
 * future boundary is always required after observed readiness. */
JP_CLOCK_API int jp_present_cadence_snapshot(struct jp_present_cadence_info *);
/* GPU/window/device/context must remain valid through shutdown. The device is
 * the SAME typed record used by SDK video setters, not a copied record.
 * Render size must match the window; scaling is not substituted. Clock and
 * wait services must be initialized first. Do not reinitialize video/clock/wait
 * owners until shutdown. One pending submission at a time.
 * Timeout (1..60000 ms) bounds each flip and shutdown, including logical waits.
 * Stats use measured native submission-to-observed-completion wall duration;
 * ta_time is synchronous native preparation duration. Neither is claimed to
 * reproduce a console DMA/raster duration. GPU-only timing stays separately
 * qualified by its validity flag. No unavailable sample is a zero-cost estimate.
 */
JP_CLOCK_API int jp_present_initialize(struct jp_gpu *,struct jp_window *,
    volatile struct kmi_device_extension *,const struct jp_gpu_config *,uint32_t timeout_ms,unsigned interval);
/* Production convenience wrapper passes the real kmiDeviceExtension owner. */
JP_CLOCK_API int jp_present_initialize_sdk(struct jp_gpu *,struct jp_window *,
    const struct jp_gpu_config *,uint32_t timeout_ms,unsigned interval);
JP_CLOCK_API int jp_present_status(void);
JP_CLOCK_API int jp_present_poll(void);
JP_CLOCK_API int jp_present_snapshot(struct jp_present_info *);
JP_CLOCK_API size_t jp_present_stats_size(void);
JP_CLOCK_API int jp_present_read_stats(struct kmi_interrupt_statistics *,size_t);
JP_CLOCK_API int jp_present_timing_ready(void);
/* Drain owns only captured frame/job, not borrowed GPU/window/video owners.
 * Shutdown does not dispatch game callbacks, allowing drain after window close.
 * A failed drain retains ownership and reports failure, rather than forgetting
 * an outstanding GPU job. Source recording remains caller-owned if capture
 * failed before an owned frame was produced. */
JP_CLOCK_API int jp_present_shutdown(void);
/* New scheduling primitive appended to the preserved wait core. */
JP_CLOCK_API int jp_wait_service(uint32_t max_milliseconds);
#endif
