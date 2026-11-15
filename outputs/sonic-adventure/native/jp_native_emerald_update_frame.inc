#ifdef JP_EMERALD_SHARED_UPDATE
__declspec(dllexport) int jp_emerald_update(struct task *player, struct task_work *camera);
int jp_input_poll(void);
void pad_scan(void);
#endif

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
#ifdef JP_EMERALD_SHARED_UPDATE
    if (jp_emerald_update(player, camera)) return -1;
    return jp_emerald_submit_player_frame(player, body, effect, aura, draw, frame);
#else
    struct task_work *work = player->work;
    struct task_motion *motion = player->motion;
    task_step_level(1);
    if (player_tasks[0] != player || player->work != work || player->motion != motion)
        return -1;
    return jp_emerald_step_and_submit(player, camera, body, effect, aura, draw, frame);
#endif
}
#ifdef JP_EMERALD_SHARED_UPDATE
/* Advance gameplay independently of whether the host presents this tick. */
__declspec(dllexport) int jp_emerald_update(struct task *player, struct task_work *camera)
{
    if (!player || player_tasks[0] != player || !player->work || !player->motion ||
        task_get_current_root_step() || emerald_stage_stepping ||
        !(stage_session & STAGE_FLOOR)) return -1;
    struct task_work *work = player->work;
    struct task_motion *motion = player->motion;
    task_step_level(0);
    if (player_tasks[0] != player || player->work != work || player->motion != motion)
        return -1;
    task_step_level(1);
    if (player_tasks[0] != player || player->work != work || player->motion != motion)
        return -1;
    return jp_emerald_stage_tick(player, camera) < 0 ? -1 : 0;
}

/* Sample before level zero so both the charged-speed task and Sonic
 * see the release edge on the same gameplay tick. */
__declspec(dllexport) int jp_emerald_update_sampled(struct task *player, struct task_work *camera)
{
    if (!player || player_tasks[0] != player || !player->work || !player->motion ||
        task_get_current_root_step() || emerald_stage_stepping ||
        !(stage_session & STAGE_FLOOR) || jp_input_poll()) return -1;
    pad_scan();
    return jp_emerald_update(player, camera);
}

/* One input sample and gameplay tick, followed by a retained frame. The
 * scheduled player consumes the sampled pad state without polling again. */
__declspec(dllexport) int jp_emerald_update_sampled_and_submit(
    struct task *player, struct task_work *camera, NJS_TEXLIST *body,
    NJS_TEXLIST *effect, NJS_OBJECT *aura, struct emerald_scene_draw *draw,
    uint64_t *frame)
{
    if (!body || !effect || !draw || !frame || !stage_terrain_ready ||
        !(stage_session & STAGE_SET) || jp_queue_status() || jp_frames_status()) return -1;
    if (jp_emerald_update_sampled(player, camera)) return -1;
    return jp_emerald_submit_player_frame(player, body, effect, aura, draw, frame);
}
#endif

