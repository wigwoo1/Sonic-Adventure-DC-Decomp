/* Level-ground Sonic update for the native beach collision provider.
 * The caller chooses the walking mode and owns input polling. */
__declspec(dllexport) int jp_sonic_step_level_walk(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->work || !task->motion || !task->motion->record ||
        !triangles || !count || !floor) return -1;
    struct task_work *work = task->work;
    struct task_motion *motion = task->motion;
    struct player *record = motion->record;
    if (!(work->flags & PLAYER_ON_GROUND) || (work->flags & PLAYER_EVENT_DRIVEN) ||
        (record->ground_flags & (WORLD_NO_SLOPE | WORLD_CLING)) != WORLD_NO_SLOPE ||
        !work->collision || !work->collision->shapes) return -1;

    if (record->anim.previous == ACTION_TURN && record->anim.current != ACTION_TURN)
        record->speed.x += record->consts.trot_speed;
    player_accel_level(work, motion, record);
    player_apply_accel(work, motion, record);
    NJS_VECTOR next = work->pos;
    motion->speed.y = 0;
    njAddVector(&next, &motion->speed);
    struct jp_stage_floor contact;
    if (!jp_stage_floor_below(triangles, count, next.x, next.y + 4.0F, next.z,
                              &contact) || contact.height < next.y - 4.0F) return -1;
    next.y = contact.height;
    work->pos = next;
    player_take_speed(work, motion, record);
    sonic_set_walk_action(work, motion, record);
    player_anim_step(&record->anim);
    sonic_move_shape(work, motion, record);
    player_note_history(PLAYER_VARS(work)->player);
    *floor = contact;
    return 0;
}
