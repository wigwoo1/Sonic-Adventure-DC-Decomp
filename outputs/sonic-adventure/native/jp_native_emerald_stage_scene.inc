#include "jp_native_queue.h"

struct emerald_scene_draw {
    size_t terrain_bytes;
    size_t terrain_opaque_bytes;
    int terrain, rings, sparkles;
};

/* The caller owns the active recording. On failure, discard that recording
 * because a draw consumer may already have appended packets. */
__declspec(dllexport) int jp_emerald_stage_draw_scene(
    const NJS_MATRIX *view, Sint32 blocks, Sint32 step, struct emerald_scene_draw *out)
{
    if (!view || !out || emerald_stage_stepping || !stage_terrain_ready ||
        !(stage_session & STAGE_SET)) return -1;
    if (!stage_ring_textures_ready && jp_emerald_stage_ring_textures_open_disc())
        return -1;

    NJS_MATRIX saved, active;
    memcpy(&active, view, sizeof active);
    njGetMatrix(&saved);
    njSetMatrix(0, &active);
    size_t opaque = jp_queue_committed_bytes(0);
    size_t other = jp_queue_committed_bytes(1);
    struct emerald_scene_draw result = {0};
    result.terrain = jp_emerald_stage_terrain_draw(blocks, TRUE, step);
    result.terrain_opaque_bytes = jp_queue_committed_bytes(0)-opaque;
    result.terrain_bytes = result.terrain_opaque_bytes + jp_queue_committed_bytes(1)-other;
    int failed = result.terrain < 0 ||
        jp_emerald_stage_draw_disc_rings(&result.rings, &result.sparkles);
    njSetMatrix(0, &saved);
    if (failed) return -1;
    *out = result;
    return 0;
}
