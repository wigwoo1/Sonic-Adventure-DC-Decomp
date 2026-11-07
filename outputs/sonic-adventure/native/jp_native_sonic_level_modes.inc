/* The pinned JP Sonic mode values for this supported ground path. */
enum { JP_SONIC_STAND = 1, JP_SONIC_WALK = 2 };

__declspec(dllexport) int jp_sonic_step_level_modes(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->work || !task->motion || !task->motion->record ||
        !triangles || !count || !floor) return -1;
    struct task_work *work = task->work;
    struct player *player = task->motion->record;
    if (work->mode == JP_SONIC_WALK)
        return jp_sonic_step_level_walk(task, triangles, count, floor);
    if (work->mode != JP_SONIC_STAND ||
        (work->flags & (PLAYER_ON_GROUND | PLAYER_EVENT_DRIVEN)) != PLAYER_ON_GROUND ||
        (player->ground_flags & (WORLD_NO_SLOPE | WORLD_CLING)) != WORLD_NO_SLOPE ||
        !work->collision || !work->collision->shapes) return -1;

    if (player_read_stick(work, 0, 0)) {
        work->mode = JP_SONIC_WALK;
        player->anim.action = ACTION_WALK;
        return jp_sonic_step_level_walk(task, triangles, count, floor);
    }
    if (player->speed.x || player->speed.y || player->speed.z) return -1;
    player_anim_step(&player->anim);
    sonic_move_shape(work, task->motion, player);
    player_note_history(PLAYER_VARS(work)->player);
    return 0;
}
