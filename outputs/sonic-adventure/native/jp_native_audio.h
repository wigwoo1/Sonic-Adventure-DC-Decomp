#ifndef JP_NATIVE_AUDIO_H
#define JP_NATIVE_AUDIO_H
#include <stdint.h>
#ifndef JP_AUDIO_API
#define JP_AUDIO_API __declspec(dllexport)
#endif
enum jp_audio_error {JP_AUDIO_OK,JP_AUDIO_ARGUMENT,JP_AUDIO_STATE,JP_AUDIO_THREAD,
    JP_AUDIO_MEMORY,JP_AUDIO_WORKER,JP_AUDIO_COM,JP_AUDIO_DEVICE,JP_AUDIO_FORMAT,
    JP_AUDIO_IO,JP_AUDIO_TIMEOUT,JP_AUDIO_QUEUE};
enum jp_audio_state {JP_AUDIO_COLD,JP_AUDIO_OPENING,JP_AUDIO_READY,
    JP_AUDIO_CLOSING,JP_AUDIO_CLOSED,JP_AUDIO_FAILED};
struct jp_audio_config {
    uint32_t bytes,rate,capacity_frames,latency_ms,timeout_ms,muted;
    float gain;uint32_t reserved;
};
struct jp_audio_info {
    uint64_t submitted,consumed,silence,discarded,events,clock_position,
        clock_frequency,source_hash,clipped_samples,client_frames,last_user_frame;
    uint32_t state,status,os_error,rate,capacity_frames,queued_frames,
        device_buffer_frames,channels,worker_live,creator_thread,worker_thread,
        session_muted;
    uint64_t stream_latency_hns;
};
/* One creating thread owns API calls. A real WASAPI worker owns its COM/audio
 * interfaces and consumes copied interleaved stereo float32 PCM independently
 * of game FPS. Rates44100/48000; finite input clips to[-1,1] after gain.
 * Capacity4096..1048576 frames, latency20..500ms, timeout1..60000ms.
 * Muting applies only to this unique, nonpersistent audio session. No endpoint
 * volume changes or audio capture. No sound-driver/MIDI success stubs.
 * Open failure/timeout retains handles until close succeeds; close never kills
 * the worker or frees its live buffers. Retry the same close after a timeout.
 */
JP_AUDIO_API int jp_audio_open(const struct jp_audio_config *);
JP_AUDIO_API int jp_audio_submit(const float *stereo,uint32_t frames);
/* Wait for copied PCM to pass the actual stream clock, not just enter a buffer.
 * last_user_frame is a conservative clock target at packet submission plus
 * one device-buffer capacity and the reported maximum stream latency. It is
 * conservative after a worker underrun; it is
 * not an index in the count of successfully submitted client_frames.
 * Timeout preserves queued data. Discard clears software-queued PCM only;
 * already scheduled device packets remain scheduled until played or close.
 */
JP_AUDIO_API int jp_audio_drain(uint32_t timeout_ms);
JP_AUDIO_API int jp_audio_discard_queued(void);
JP_AUDIO_API int jp_audio_close(uint32_t timeout_ms);
JP_AUDIO_API int jp_audio_status(void);
JP_AUDIO_API int jp_audio_snapshot(struct jp_audio_info *);
#endif
