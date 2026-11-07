/* Neutral vertical jump against the static stage floor. The caller handles
 * airborne action playback, input steering and non-floor collision. */
__declspec(dllexport) int jp_sonic_step_vertical_jump(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->work || !task->motion || !task->motion->record ||
        !triangles || !count || !floor) return -1;
    struct task_work *work = task->work;
    struct task_motion *motion = task->motion;
    struct player *player = motion->record;
    if (work->mode != JP_SONIC_JUMP || !work->collision ||
        !work->collision->shapes || player->speed.x || player->speed.z ||
        !(player->consts.gravity > 0) || (work->flags & PLAYER_EVENT_DRIVEN))
        return -1;

    Float speed = player->speed.y;
    speed += -player->consts.gravity + player->consts.resist.y*speed;
    Float next = work->pos.y + speed;
    struct jp_stage_floor contact;
    int landed = speed <= 0 &&
        jp_stage_floor_below(triangles, count, work->pos.x, work->pos.y,
                             work->pos.z, &contact) && next <= contact.height;
    work->flags &= ~PLAYER_ON_GROUND;
    work->pos.y = landed ? contact.height : next;
    motion->speed.y = player->speed.y = landed ? 0 : speed;
    if (landed) {
        work->flags = (work->flags & ~PLAYER_ATTACKING) | PLAYER_ON_LAND;
        work->mode = JP_SONIC_STAND;
        player->idle_steps = 0;
        player->anim.action = speed > -5.0F ? ACTION_LANDED : ACTION_HARD_LANDING;
        *floor = contact;
    }
    sonic_move_shape(work, motion, player);
    player_note_history(PLAYER_VARS(work)->player);
    return landed;
}
