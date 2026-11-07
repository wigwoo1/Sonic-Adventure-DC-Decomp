#include "jp_native_frames.h"

/* Finish the active recording after player geometry has been submitted.
 * On failure the caller owns cleanup of any partial recording. */
__declspec(dllexport) int jp_emerald_stage_submit_frame(
    const NJS_MATRIX *view, Sint32 blocks, Sint32 step,
    struct emerald_scene_draw *draw, uint64_t *frame)
{
    if (!view || !draw || !frame || jp_queue_status() || jp_frames_status())
        return -1;
    struct emerald_scene_draw result;
    uint64_t id;
    if (jp_emerald_stage_draw_scene(view, blocks, step, &result) ||
        jp_frames_submit(&id)) return -1;
    *draw = result;
    *frame = id;
    return 0;
}
