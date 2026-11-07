/* Dispatch ordinary level movement and neutral jumps from sampled input. */
__declspec(dllexport) int jp_sonic_step_level_dispatch(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->work || !task->motion || !task->motion->record ||
        !triangles || !count || !floor) return -1;
    struct task_work *work = task->work;
    struct player *player = task->motion->record;
    if (work->mode == JP_SONIC_JUMP)
        return jp_sonic_step_vertical_jump(task, triangles, count, floor) < 0 ? -1 : 0;

    unsigned slot = PLAYER_VARS(work)->player;
    if (slot >= PLAYER_COUNT) return -1;
    if (pad_records[slot].press & pad_jump_buttons) {
        if (player->speed.x || player->speed.z || player_read_stick(work, 0, 0) ||
            !work->collision || !work->collision->shapes) return -1;
        return jp_sonic_try_ground_jump(task) == 1 ? 0 : -1;
    }
    return jp_sonic_step_level_stop(task, triangles, count, floor);
}
