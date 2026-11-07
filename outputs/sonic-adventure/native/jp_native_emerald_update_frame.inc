/* Run the player scheduler, then update and render Emerald Coast. The
 * game thread owns the recording and must not retry a completed step. */
__declspec(dllexport) int jp_emerald_update_and_submit(
    struct task *player, struct task_work *camera, NJS_TEXLIST *body,
    NJS_TEXLIST *effect, NJS_OBJECT *aura, struct emerald_scene_draw *draw,
    uint64_t *frame)
{
    if (!player || player_tasks[0] != player || !player->work || !player->motion ||
        !body || !effect || !draw || !frame || task_get_current_root_step() ||
        emerald_stage_stepping || !stage_terrain_ready || !(stage_session & STAGE_SET) ||
        jp_queue_status() || jp_frames_status()) return -1;
    struct task_work *work = player->work;
    struct task_motion *motion = player->motion;
    task_step_level(1);
    if (player_tasks[0] != player || player->work != work || player->motion != motion)
        return -1;
    return jp_emerald_step_and_submit(player, camera, body, effect, aura, draw, frame);
}
