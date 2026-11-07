enum { JP_SONIC_JUMP = 8 };

/* Handle an ordinary jump edge on static ground. Return 1 on launch,
 * 0 without an edge, or -1 outside the supported ground state. */
__declspec(dllexport) int jp_sonic_try_ground_jump(struct task *task)
{
    if (!task || !task->work || !task->motion || !task->motion->record) return -1;
    struct task_work *work = task->work;
    struct player *player = task->motion->record;
    unsigned slot = PLAYER_VARS(work)->player;
    if (slot >= PLAYER_COUNT ||
        (work->mode != JP_SONIC_STAND && work->mode != JP_SONIC_WALK) ||
        !(work->flags & PLAYER_ON_LAND) ||
        (work->flags & (PLAYER_EVENT_DRIVEN | PLAYER_HOLDING | PLAYER_ON_PATH)) ||
        player->ground_task || !(player->consts.jump_speed > 0)) return -1;
    if (!(pad_records[slot].press & pad_jump_buttons)) return 0;

    work->mode = JP_SONIC_JUMP;
    work->flags = (work->flags & ~PLAYER_ON_TASK) | PLAYER_ATTACKING;
    player->speed.y = player->consts.jump_speed;
    player->anim.action = ACTION_SPIN;
    player->action_speed = 5.0F;
    player->jump_steps = 0;
    return 1;
}
