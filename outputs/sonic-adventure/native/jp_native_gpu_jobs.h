/* Authored bounded asynchronous GPU ownership. Not a game/vsync scheduler. */
#ifndef JP_NATIVE_GPU_JOBS_H
#define JP_NATIVE_GPU_JOBS_H
#include "jp_native_direct_window.h"
enum jp_gpu_job_error {JP_GPU_JOB_PENDING=100,JP_GPU_JOB_STALE,JP_GPU_JOB_FULL};
struct jp_gpu_timing {
    uint64_t id,frame;
    uint64_t cpu_frequency,cpu_begin,cpu_prepared,cpu_submitted,cpu_completed_observed;
    uint64_t gpu_frequency,gpu_begin,gpu_end,gpu_elapsed_counts;
    uint32_t gpu_timing_valid,width,height,reserved;
};
/* Exactly four outstanding jobs per renderer. Each owns an RGBA8 target and
 * event/timestamp queries. Frame/video/texture owners may retire after submit.
 * Per-job budget includes target/depth, input payloads and CPU job bookkeeping,
 * but not driver overhead. Caller controls the sum of four retained budgets.
 * Config dimensions select the render size; canvas/state come from the frame.
 * Submission preserves output ID/report on failure, and never changes a window.
 * IDs are never recycled within this module's lifetime, including new owners.
 * All job calls run on the thread which created the renderer.
 */
JP_PACKET_API int jp_gpu_submit_frame(struct jp_gpu *,uint64_t,
    const struct jp_gpu_config *,uint64_t *,struct jp_gpu_report *,struct jp_gpu_failure *);
/* Explicit-state counterpart for consumers already owning a configuration;
 * like jp_window_render_configured_frame, does not bind captured registers. */
JP_PACKET_API int jp_gpu_submit_configured_frame(struct jp_gpu *,uint64_t,
    const struct jp_gpu_config *,uint64_t *,struct jp_gpu_report *,struct jp_gpu_failure *);
/* PENDING preserves timing output. Success means the completion event and
 * queries were observed ready, not scanout. Invalid/disjoint timestamps yield
 * gpu_timing_valid=0; zero counts must not then be used as a render estimate.
 * cpu_completed_observed includes polling delay; it is not a GPU timestamp.
 * A ready result is immutable. Failed jobs can be released after poll failure.
 */
JP_PACKET_API int jp_gpu_job_poll(struct jp_gpu *,uint64_t,struct jp_gpu_timing *,struct jp_gpu_failure *);
JP_PACKET_API int jp_gpu_job_release(struct jp_gpu *,uint64_t,struct jp_gpu_failure *);
JP_PACKET_API unsigned jp_gpu_jobs_live(struct jp_gpu *);
JP_PACKET_API size_t jp_gpu_timing_size(void);
/* Integer conversion to the original 781250 counts/second wall domain.
 * Rejects reversed timestamps, zero/unrepresentable frequency or overflow;
 * preserves output on failure. This does not align CPU/GPU absolute clocks.
 */
JP_PACKET_API int jp_gpu_elapsed_counts(uint64_t begin,uint64_t end,uint64_t frequency,uint64_t *out);
/* Copy a completed job to a same-sized window using GPU CopyResource, without
 * readback/CPU image transfer. Present is explicit, interval 0 or 1. Jobs remain
 * owned until release, permitting repeated pictures or out-of-order retirement.
 * A window resize mismatch is rejected; scaling is not silently substituted.
 */
JP_PACKET_API int jp_window_copy_job(struct jp_window *,uint64_t,struct jp_gpu_failure *);
JP_PACKET_API int jp_window_present_job(struct jp_window *,uint64_t,unsigned,struct jp_gpu_failure *);
/* GPU close rejects new submissions and defers final release while jobs/windows
 * exist. Poll/release remain available to drain jobs after that close request.
 * Release of a pending job returns PENDING: caller must poll rather than lose
 * its outstanding completion evidence. Device failure permits release.
 */
#endif
