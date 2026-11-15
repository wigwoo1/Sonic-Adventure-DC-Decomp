#include "object/placement.h"

int jp_emerald_set_get(unsigned, struct set_entry *);
const struct task *jp_emerald_placement_task(unsigned);
int jp_emerald_stage_tick(struct task *, struct task_work *);
int jp_emerald_update_sampled(struct task *, struct task_work *);
int jp_sonic_step_level_dispatch(struct task *, const struct jp_stage_triangle *,
                                 size_t, struct jp_stage_floor *);

static void stage_ring_player_step(struct task *task)
{
    struct light_dash_mode_fixture *route = task->extra;
    route->result = jp_sonic_step_level_dispatch(task, route->triangles, route->count,
                                                 &route->floor);
}

static int stage_ring_update(struct task *task)
{
    struct light_dash_mode_fixture *route = task->extra;
    route->result = -1;
    return jp_emerald_update_sampled(task, NULL) || route->result;
}

/* The caller owns the open stage and its geometry. Start outside pickup
 * range so the stage has to supply a target before the dash can collect it. */
static int light_dash_stage_ring(const struct jp_stage_triangle *triangles,
    size_t count, unsigned index, struct player_action *actions, light_dash_observer observe)
{
    struct set_entry entry;
    if (jp_emerald_set_get(index, &entry) || (entry.id & 0x7fff)) return -1;
    NJS_POINT3 origin = entry.pos;
    origin.x += 12;
    struct jp_stage_floor floor;
    if (!jp_stage_floor_below(triangles, count, origin.x, origin.y + 4, origin.z, &floor))
        return -2;
    origin.y = floor.height;
    if (dash_capture_input_phase) return -1;
    struct task_work work = {.pos = origin, .mode = SONIC_STAND, .flags = PLAYER_ON_GROUND};
    struct event_work event = {0};
    work.event = &event;
    work.ang.y = 0x8000;
    struct player record = {0};
    record.upgrades = PLAYER_SONIC_LIGHT_SHOES | PLAYER_SONIC_CRYSTAL_RING;
    record.ground_flags = WORLD_NO_SLOPE;
    record.ground_normal.y = 1;
    record.consts.center_y = 2;
    record.consts.top_speed = 6;
    record.consts.gravity = 0.08F;
    record.consts.resist.x = -0.02F;
    record.consts.brake_drag = -0.25F;
    record.level = 1;
    struct task_motion motion = {.record = &record, .gravity = 1};
    if (player_tasks[0]) return -1;
    struct task *task = task_create(0, 1, stage_ring_player_step);
    if (!task) return -1;
    struct light_dash_mode_fixture route = {.triangles = triangles, .count = count, .floor = floor};
    task->work = &work;
    task->motion = &motion;
    task->extra = &route;
    player_tasks[0] = task;
    struct collision_shape shape = {.flags = SHAPE_SPHERE | SHAPE_IN_WORLD,
                                    .center = {origin.x, origin.y + 2, origin.z}, .radius = 4};
    struct collision_work collision = {.kind = COLLISION_PLAYER, .count = 1,
                                       .owner = task, .shapes = &shape, .radius = 25};
    work.collision = &collision;
    struct task_work *previous_player = player_work[0];
    PDS_PERIPHERAL previous_pad = pad_records[0];
    Uint32 previous_buttons = pad_action_buttons;
    Bool pads = pad_is_enabled(), player = pad_player_is_enabled(0);
    Uint8 history_index = player_history_index[0];
    enum { RECOVERY_LIMIT = 24, BRAKE_LIMIT = 80, HISTORY_SLOTS = RECOVERY_LIMIT + BRAKE_LIMIT + 6 };
    NJS_POINT3 previous_positions[HISTORY_SLOTS];
    Angle previous_angles[HISTORY_SLOTS][3];
    for (unsigned i = 0; i < HISTORY_SLOTS; ++i) {
        Uint8 index = history_index + i;
        previous_positions[i] = player_history_pos[index][0];
        memcpy(previous_angles[i], player_history_ang[index][0], sizeof previous_angles[i]);
    }
    player_history_pos[history_index][0] = origin;
    player_work[0] = &work;
    pad_action_buttons = PDD_DGT_TB;
    pad_enable();
    pad_enable_player(0);
    if (actions) {
        record.anim.actions = actions;
        record.anim.pace = &record.action_speed;
        record.anim.speed = &record.speed.x;
        record.anim.state = PLAYER_ANIM_NEW;
        player_anim_step(&record.anim);
        player_anim_step(&record.anim);
    }

    int result = -3;
    /* The first tick initializes the ring; the next registers its shape. */
    if (jp_emerald_stage_tick(task, NULL) < 0 ||
        jp_emerald_stage_tick(task, NULL) < 0) goto restore;
    const struct task *target = jp_emerald_placement_task(index);
    result = -4;
    if (!target || !target->work->collision ||
        (target->work->collision->flags & COLLISION_HIT) ||
        jp_emerald_light_dash_target(&work) != target->work) goto restore;
    result = -7;
    dash_capture_input(1);
    for (int tick = 0; tick <= RING_CHARGE_STEPS; ++tick) {
        if (stage_ring_update(task) || !(pad_records[0].on & PDD_DGT_TB) ||
            (!!(pad_records[0].press & PDD_DGT_TB) != (tick == 0)) ||
            work.mode != (tick == RING_CHARGE_STEPS ? SONIC_STAND : SONIC_SPIN_CHARGE))
            goto restore;
    }
    if (actions && record.anim.current != ACTION_UNCURL) goto restore;
    if (!(work.flags & SONIC_CHARGED) || record.consts.top_speed != CHARGED_TOP_SPEED)
        goto restore;
    dash_capture_input(0);
    Sint16 rings_before_dash = rings;
    result = -5;
    if (stage_ring_update(task) ||
        !(pad_records[0].release & PDD_DGT_TB) || (pad_records[0].on & PDD_DGT_TB) ||
        work.mode != SONIC_LIGHT_DASH || work.pos.x >= origin.x) goto restore;
    if (actions && record.anim.current != ACTION_LIGHT_DASH) goto restore;
    result = -6;
    if (stage_ring_update(task)) goto restore;
    if (rings != rings_before_dash + 1) goto restore;
    if (observe && observe(task, 0)) goto restore;
    if (record.consts.top_speed != 6) goto restore;
    result = -8;
    for (int tick = 0; tick < RECOVERY_LIMIT && work.mode == SONIC_LIGHT_DASH; ++tick) {
        if (stage_ring_update(task)) goto restore;
    }
    if (work.mode != SONIC_STOP || record.anim.action != ACTION_SKID ||
        (work.flags & PLAYER_GUARD) || record.own.sonic.dash_steps ||
        record.speed.x > LIGHT_DASH_END_SPEED) goto restore;
    if (actions && record.anim.current != ACTION_SKID) goto restore;
    if (observe && observe(task, 1)) goto restore;
    result = -9;
    for (int tick = 0; tick < BRAKE_LIMIT && work.mode == SONIC_STOP; ++tick) {
        if (stage_ring_update(task)) goto restore;
    }
    if (work.mode != SONIC_STAND || record.anim.action != ACTION_STAND ||
        record.speed.x || record.speed.y || record.speed.z ||
        work.pos.y != route.floor.height) goto restore;
    if (actions) {
        result = -10;
        if (stage_ring_update(task) ||
            work.mode != SONIC_STAND || record.anim.current != ACTION_STAND) goto restore;
        if (observe && observe(task, 2)) goto restore;
    }
    result = 0;

restore:
    if (actions) {
        record.anim.state = PLAYER_ANIM_FREE;
        player_anim_step(&record.anim);
        if (record.anim.blend) result = -11;
    }
    work.flags &= ~SONIC_CHARGED;
    task_step_level(0);
    task_step_level(0);
    dash_capture_input_phase = 0;
    if (!player) pad_disable_player(0);
    if (!pads) pad_disable();
    pad_action_buttons = previous_buttons;
    for (unsigned i = 0; i < HISTORY_SLOTS; ++i) {
        Uint8 index = history_index + i;
        player_history_pos[index][0] = previous_positions[i];
        memcpy(player_history_ang[index][0], previous_angles[i], sizeof previous_angles[i]);
    }
    player_history_index[0] = history_index;
    player_work[0] = previous_player;
    player_tasks[0] = NULL;
    task->work = NULL;
    task->motion = NULL;
    task->extra = NULL;
    task_destroy(task);
    pad_records[0] = previous_pad;
    return result;
}

__declspec(dllexport) int jp_emerald_light_dash_stage_ring(
    const struct jp_stage_triangle *triangles, size_t count, unsigned index)
{
    return light_dash_stage_ring(triangles, count, index, NULL, NULL);
}

static int light_dash_stage_ring_animated(
    const struct jp_stage_triangle *triangles, size_t count, unsigned index,
    NJS_ACTION *stand, NJS_ACTION *spin, NJS_ACTION *uncurl, NJS_ACTION *dash, NJS_ACTION *skid,
    light_dash_observer observe)
{
    struct player_action actions[ACTION_LIGHT_DASH + 1] = {0};
    if (!stand || !spin || !uncurl ||
        jp_emerald_bind_light_dash_actions(actions, ACTION_LIGHT_DASH + 1, dash, skid))
        return -1;
    actions[ACTION_STAND] = (struct player_action){
        stand, SONIC_NODES, PLAYER_ANIM_LOOP, ACTION_STAND, 0.0625F, 0.5F};
    actions[ACTION_SPIN] = (struct player_action){
        spin, 74, PLAYER_ANIM_LOOP_PACE, ACTION_SPIN, 0.25F, 0.2F};
    actions[ACTION_UNCURL] = (struct player_action){
        uncurl, SONIC_NODES, PLAYER_ANIM_ONCE, ACTION_LAND, 0.0625F, 1.0F};
    return light_dash_stage_ring(triangles, count, index, actions, observe);
}

__declspec(dllexport) int jp_emerald_light_dash_stage_ring_animated(
    const struct jp_stage_triangle *triangles, size_t count, unsigned index,
    NJS_ACTION *stand, NJS_ACTION *spin, NJS_ACTION *uncurl, NJS_ACTION *dash, NJS_ACTION *skid)
{
    return light_dash_stage_ring_animated(triangles, count, index, stand, spin, uncurl, dash, skid, NULL);
}

__declspec(dllexport) int jp_emerald_light_dash_stage_ring_observe(
    const struct jp_stage_triangle *triangles, size_t count, unsigned index,
    NJS_ACTION *stand, NJS_ACTION *spin, NJS_ACTION *uncurl, NJS_ACTION *dash, NJS_ACTION *skid,
    light_dash_observer observe)
{
    if (!observe) return -1;
    return light_dash_stage_ring_animated(triangles, count, index, stand, spin, uncurl, dash, skid, observe);
}
