/* Ordinary unladen walking returns to stand after ground drag reaches zero. */
__declspec(dllexport) int jp_sonic_step_level_stop(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->work || !task->motion || !task->motion->record ||
        !triangles || !count || !floor) return -1;
    struct task_work *work = task->work;
    struct player *player = task->motion->record;
    if (work->mode == JP_SONIC_WALK &&
        (work->flags & (PLAYER_ON_GROUND | PLAYER_EVENT_DRIVEN | PLAYER_HOLDING)) ==
            PLAYER_ON_GROUND &&
        (player->ground_flags & (WORLD_NO_SLOPE | WORLD_CLING)) == WORLD_NO_SLOPE &&
        work->collision && work->collision->shapes &&
        !player->speed.x && !player->speed.y && !player->speed.z &&
        !player_read_stick(work, 0, 0)) {
        work->mode = JP_SONIC_STAND;
        work->flags &= ~PLAYER_ON_PATH;
        player->idle_steps = 0;
        player->anim.action = ACTION_STAND;
    }
    return jp_sonic_step_level_modes(task, triangles, count, floor);
}
