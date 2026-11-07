/* The caller has advanced Sonic once. Advance the stage once and submit its
 * frame. A render failure does not roll back simulation. */
__declspec(dllexport) int jp_emerald_step_and_submit(
    struct task *player, struct task_work *camera, NJS_TEXLIST *body,
    NJS_TEXLIST *effect, NJS_OBJECT *aura, struct emerald_scene_draw *draw,
    uint64_t *frame)
{
    if (!player || !body || !effect || !draw || !frame || emerald_stage_stepping ||
        !stage_terrain_ready || !(stage_session & STAGE_SET) ||
        jp_queue_status() || jp_frames_status()) return -1;
    if (jp_emerald_stage_tick(player, camera) < 0) return -1;
    return jp_emerald_submit_player_frame(player, body, effect, aura, draw, frame);
}
