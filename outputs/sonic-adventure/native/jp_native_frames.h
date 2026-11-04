/* Owned command snapshots and texture lifetimes, one game thread.
 * This is submission storage, not kmRender/kmFlip or a graphics backend.
 */
#ifndef JP_NATIVE_FRAMES_H
#define JP_NATIVE_FRAMES_H
#include <stddef.h>
#include <stdint.h>
#define JP_FRAME_API __declspec(dllexport)
enum jp_frame_error { JP_FRAME_OK,JP_FRAME_ARGUMENT,JP_FRAME_STATE,
    JP_FRAME_CAPACITY,JP_FRAME_BUDGET,JP_FRAME_ALLOCATION,JP_FRAME_RESOURCE,
    JP_FRAME_STALE,JP_FRAME_GUARD,JP_FRAME_NOT_INITIALIZED };
JP_FRAME_API int jp_frames_initialize(uint32_t frames,uint32_t resources,size_t budget);
/* Require an empty/inactive queue; discard its recording via reset/shutdown
 * first. Previously submitted IDs expire on successful owner reinitialization. */
JP_FRAME_API int jp_frames_shutdown(void);
JP_FRAME_API int jp_frames_status(void);
JP_FRAME_API void jp_frames_clear_error(void);
JP_FRAME_API size_t jp_frames_accounted_bytes(void);
JP_FRAME_API size_t jp_frames_pinned_bytes(void);
JP_FRAME_API size_t jp_frames_orphaned_bytes(void);
JP_FRAME_API uint32_t jp_frames_live_count(void);
/* Reserve texture ownership BEFORE entering an unchecked producer. A pinned
 * model must also pass jp_model_preflight and jp_model_reserve. Duplicate
 * preparation within a recording has no additional resource cost. */
JP_FRAME_API int jp_frames_prepare_texture(uint32_t surface_word);
/* All-or-nothing preparation; failure releases only newly added recording
 * references. Earlier draws and existing frame snapshots remain intact. */
JP_FRAME_API int jp_frames_prepare_textures(const uint32_t *words,uint32_t count);
/* Require completed batches and njSetkmPointer. On success snapshots all five
 * actual list streams and resets the live queue for the next recording.
 * Failure preserves the recording, existing snapshots and output ID. */
JP_FRAME_API int jp_frames_submit(uint64_t *id);
JP_FRAME_API int jp_frame_release(uint64_t id);
/* Read-only views survive queue/texture reset, growth, release and shutdown.
 * They expire on release(id) or successful frames shutdown/reinitialization.
 * Lists are raw committed packets, without fabricated end-of-list commands.
 * Resource data remains encoded: no decoding or rasterization is implied. */
JP_FRAME_API const void *jp_frame_list(uint64_t id,unsigned list,size_t *bytes);
JP_FRAME_API const void *jp_frame_surface(uint64_t id,uint32_t word,size_t *bytes);
JP_FRAME_API uint32_t jp_frame_surface_count(uint64_t id);
#endif
