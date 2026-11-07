/* The active action table supplies the original spin and landing assets. */
static int step_level_with_jump_animation(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    int was_airborne = task->work->mode == JP_SONIC_JUMP;
    int result = jp_sonic_step_level_dispatch(task, triangles, count, floor);
    if (!result && (was_airborne || task->work->mode == JP_SONIC_JUMP))
        player_anim_step(&task->motion->record->anim);
    return result;
}
