/* Run the supported Light Dash state and velocity phase of a Sonic update.
 * The caller owns collision response and committing the resulting position. */
__declspec(dllexport) int jp_emerald_light_dash_mode(struct task *task)
{
    if (!task || !task->work || !task->motion || !task->motion->record) return -1;
    struct task_work *work = task->work;
    struct task_motion *motion = task->motion;
    struct player *record = motion->record;
    if (PLAYER_VARS(work)->player >= PLAYER_COUNT) return -1;
    if (work->mode == SONIC_LIGHT_DASH) {
        int duration = jp_emerald_light_dash_duration(work, record);
        if (duration <= 0) return duration ? -1 : 2;
    } else {
        if ((work->mode != SONIC_STAND && work->mode != SONIC_WALK) ||
            !jp_emerald_try_light_dash(work, motion, record)) return 0;
    }
    return jp_emerald_light_dash_step(work, motion, record) < 0 ? -1 : 1;
}

struct jp_stage_triangle {
    Uint32 entry, flags;
    Float vertex[3][3];
};
struct jp_stage_floor { Float height; Uint32 entry, flags; };
int jp_stage_floor_below(const struct jp_stage_triangle *, size_t, Float, Float, Float,
                         struct jp_stage_floor *);
void player_note_history(Uint8 player);
Float jp_collision_sphere_pair(struct collision_work *, struct collision_work *);

/* Static level-ground adapter. A rejected floor leaves position and history
 * intact, but the caller must not retry the already evaluated mode tick. */
__declspec(dllexport) int jp_emerald_light_dash_move(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->work || !task->motion || !task->motion->record ||
        !triangles || !count || !floor) return -1;
    struct task_work *work = task->work;
    struct task_motion *motion = task->motion;
    struct player *record = motion->record;
    if (!(work->flags & PLAYER_ON_GROUND) ||
        (record->ground_flags & (WORLD_NO_SLOPE | WORLD_CLING)) != WORLD_NO_SLOPE ||
        !work->collision || !work->collision->count || !work->collision->shapes ||
        (work->collision->shapes[0].flags & (SHAPE_KIND | SHAPE_IN_WORLD)) !=
            (SHAPE_SPHERE | SHAPE_IN_WORLD))
        return -1;
    int result = jp_emerald_light_dash_mode(task);
    if (result != 1) return result;
    NJS_POINT3 next = work->pos;
    motion->speed.y = 0;
    njAddVector(&next, &motion->speed);
    struct jp_stage_floor contact;
    if (!jp_stage_floor_below(triangles, count, next.x, next.y + 4, next.z, &contact) ||
        contact.height < next.y - 4) return -1;
    next.y = contact.height;
    work->pos = next;
    player_take_speed(work, motion, record);
    sonic_move_shape(work, motion, record);
    player_note_history(PLAYER_VARS(work)->player);
    *floor = contact;
    return 1;
}

/* The caller holds leases for the initialized action table through this tick. */
__declspec(dllexport) int jp_emerald_light_dash_move_animated(
    struct task *task, const struct jp_stage_triangle *triangles, size_t count,
    struct jp_stage_floor *floor)
{
    if (!task || !task->motion || !task->motion->record ||
        !task->motion->record->anim.actions) return -1;
    int result = jp_emerald_light_dash_move(task, triangles, count, floor);
    if (result == 1 || result == 2) player_anim_step(&task->motion->record->anim);
    return result;
}

struct light_dash_mode_fixture {
    int result, animated, charging;
    const struct jp_stage_triangle *triangles;
    size_t count;
    struct jp_stage_floor floor;
};

static void light_dash_mode_fixture_step(struct task *task)
{
    struct light_dash_mode_fixture *fixture = task->extra;
    if (fixture->charging) {
        fixture->result = jp_emerald_light_dash_charge(task);
        return;
    }
    fixture->result = fixture->animated
        ? jp_emerald_light_dash_move_animated(task, fixture->triangles, fixture->count, &fixture->floor)
        : jp_emerald_light_dash_move(task, fixture->triangles, fixture->count, &fixture->floor);
}

typedef int (*light_dash_observer)(struct task *, int);
typedef int (*light_dash_input)(int held);
int jp_input_poll(void);

static int light_dash_mode_check(struct player_action *actions, light_dash_observer observe,
                                 const struct jp_stage_triangle *triangles, size_t count,
                                 light_dash_input input)
{
    static const struct jp_stage_triangle flat = {17, 0, {{-32,0,-32}, {-32,0,32}, {64,0,-32}}};
    NJS_POINT3 origin = {0};
    int charge_on_stage = triangles != NULL;
    if (charge_on_stage) origin = (NJS_POINT3){-79, 4, 36.7F};
    else {
        triangles = &flat;
        count = 1;
    }
    struct jp_stage_floor start;
    if (!jp_stage_floor_below(triangles, count, origin.x, origin.y, origin.z, &start))
        return -1;
    origin.y = start.height;
    struct task *task = task_create(TASK_WORK | TASK_MOTION, 1, light_dash_mode_fixture_step);
    if (!task) return 1;
    struct task_work *work = task->work;
    struct event_work event = {0};
    work->event = &event;
    struct player record = {0};
    struct collision_shape shape = {0};
    struct collision_work ring_collision = {.shapes = &shape};
    struct task_work ring = {.pos = {origin.x + 8, origin.y, origin.z}, .collision = &ring_collision};
    struct collision_shape player_shape = {.flags = SHAPE_SPHERE | SHAPE_IN_WORLD, .radius = 4};
    struct collision_work player_collision = {
        .kind = COLLISION_PLAYER, .count = 1, .owner = task, .shapes = &player_shape
    };
    struct near_task saved[2] = {near_rings[0], near_rings[1]};
    PDS_PERIPHERAL saved_pad = pad_records[0];
    Uint32 saved_buttons = pad_action_buttons;
    NJS_POINT3 history;
    struct task_work *saved_player = player_work[0];
    player_work[0] = work;
    player_get_history(0, 1, &history, NULL);
    player_set_history(0, 1, &origin, NULL);
    Uint8 history_index = player_history_index[0];
    NJS_POINT3 overwritten_pos[2];
    Angle overwritten_ang[2][3];
    for (unsigned i = 0; i < 2; ++i) {
        Uint8 index = history_index + i + 1;
        overwritten_pos[i] = player_history_pos[index][0];
        memcpy(overwritten_ang[i], player_history_ang[index][0], sizeof overwritten_ang[i]);
    }
    struct light_dash_mode_fixture fixture = {
        .result = -1, .animated = actions != NULL, .triangles = triangles, .count = count
    };
    task->extra = &fixture;
    task->motion->record = &record;
    work->mode = SONIC_STAND;
    work->pos = origin;
    work->flags = PLAYER_ON_GROUND | (charge_on_stage ? 0 : SONIC_CHARGED);
    work->collision = &player_collision;
    record.ground_flags = WORLD_NO_SLOPE;
    record.ground_normal.y = 1;
    record.consts.center_y = 2;
    if (actions) {
        record.anim.actions = actions;
        record.anim.state = PLAYER_ANIM_NEW;
        player_anim_step(&record.anim);
        player_anim_step(&record.anim);
    }
    near_rings[0] = (struct near_task){&ring, 64};
    near_rings[1] = (struct near_task){0};
    int failed = 0;
    if (charge_on_stage) {
        fixture.charging = 1;
        record.upgrades = PLAYER_SONIC_LIGHT_SHOES | PLAYER_SONIC_CRYSTAL_RING;
        record.consts.top_speed = 6;
        pad_action_buttons = PDD_DGT_TB;
        pad_records[0].on = PDD_DGT_TB;
        pad_records[0].release = 0;
        for (int tick = 0; tick <= RING_CHARGE_STEPS; ++tick) {
            if (input) failed |= charge_fixture_input(input, 1, tick == 0) != 0;
            else pad_records[0].press = tick ? 0 : PDD_DGT_TB;
            task_step_level(0);
            task_step_level(1);
            failed |= fixture.result != (tick == RING_CHARGE_STEPS ? 2 : 1);
        }
        failed |= !(work->flags & SONIC_CHARGED) || record.consts.top_speed != CHARGED_TOP_SPEED;
        fixture.charging = 0;
    }
    if (input) {
        failed |= input(1) || jp_input_poll();
        pad_scan();
        failed |= !(pad_records[0].on & pad_action_buttons);
    } else if (!charge_on_stage) {
        pad_action_buttons = 4;
        pad_records[0].on = 4;
    }
    task_step_level(1);
    failed |= fixture.result != 0 || work->mode != SONIC_STAND || work->pos.x != origin.x;
    if (input) {
        failed |= input(0) || jp_input_poll();
        pad_scan();
        failed |= (pad_records[0].on & pad_action_buttons) != 0;
    } else {
        pad_records[0].on = 0;
        pad_records[0].release = charge_on_stage ? PDD_DGT_TB : 0;
    }
    if (charge_on_stage) task_step_level(0);
    task_step_level(1);
    if (charge_on_stage) {
        task_step_level(0);
        failed |= record.consts.top_speed != 6;
    }
    failed |= fixture.result != 1 || work->mode != SONIC_LIGHT_DASH ||
              record.own.sonic.dash_steps != LIGHT_DASH_STEPS ||
              work->pos.x < origin.x + 7.99F || work->pos.x > origin.x + 8.01F ||
              fixture.floor.entry != start.entry || fixture.floor.height != work->pos.y ||
              player_shape.center.x != work->pos.x || player_shape.center.y != work->pos.y + 2;
    if (actions) failed |= record.anim.current != ACTION_LIGHT_DASH ||
                           record.anim.state != PLAYER_ANIM_LOOP || record.anim.frame != 0;
    ring.pos = work->pos;
    ring.pos.x += 8;
    task_step_level(1);
    failed |= fixture.result != 1 || record.own.sonic.dash_steps != LIGHT_DASH_STEPS - 1 ||
              work->pos.x < origin.x + 15.99F || work->pos.x > origin.x + 16.01F ||
              fixture.floor.height != work->pos.y ||
              player_shape.center.x != work->pos.x || player_shape.center.y != work->pos.y + 2;
    if (actions) failed |= record.anim.frame != 1;
    if (observe && !failed) failed |= observe(task, 0) != 0;
    struct task ring_task = {.work = &ring};
    ring_collision.owner = &ring_task;
    ring_collision.count = 1;
    ring_collision.kind = COLLISION_RING;
    shape = (struct collision_shape){.flags = SHAPE_SPHERE | SHAPE_IN_WORLD,
                                      .center = player_shape.center, .radius = 1};
    jp_collision_sphere_pair(&player_collision, &ring_collision);
    failed |= !(ring_collision.flags & COLLISION_HIT);
    record.own.sonic.dash_steps = 0;
    task_step_level(1);
    failed |= fixture.result != 2 || work->mode != SONIC_STOP || record.anim.action != ACTION_SKID;
    if (observe && !failed) failed |= observe(task, 1) != 0;
    if (actions) {
        failed |= record.anim.current != ACTION_SKID ||
                  (record.anim.state != PLAYER_ANIM_BLENDING && record.anim.state != PLAYER_ANIM_ONCE);
        record.anim.state = PLAYER_ANIM_FREE;
        player_anim_step(&record.anim);
        failed |= record.anim.blend != NULL;
    }
    task->motion->record = NULL;
    work->collision = NULL;
    work->event = NULL;
    task->extra = NULL;
    for (unsigned i = 0; i < 2; ++i) {
        Uint8 index = history_index + i + 1;
        player_history_pos[index][0] = overwritten_pos[i];
        memcpy(player_history_ang[index][0], overwritten_ang[i], sizeof overwritten_ang[i]);
    }
    player_history_index[0] = history_index;
    player_set_history(0, 1, &history, NULL);
    player_work[0] = saved_player;
    task_destroy(task);
    pad_records[0] = saved_pad;
    pad_action_buttons = saved_buttons;
    near_rings[0] = saved[0];
    near_rings[1] = saved[1];
    return failed;
}

__declspec(dllexport) int jp_emerald_light_dash_mode_check(void)
{
    return light_dash_mode_check(NULL, NULL, NULL, 0, NULL);
}

static int light_dash_observe(
    NJS_ACTION *stand, NJS_ACTION *dash, NJS_ACTION *skid, light_dash_observer observe,
    const struct jp_stage_triangle *triangles, size_t count, light_dash_input input)
{
    struct player_action actions[ACTION_LIGHT_DASH + 1] = {0};
    if (!stand || !stand->object || !stand->motion ||
        jp_emerald_bind_light_dash_actions(actions, ACTION_LIGHT_DASH + 1, dash, skid))
        return -1;
    actions[ACTION_STAND] = (struct player_action){
        stand, SONIC_NODES, PLAYER_ANIM_LOOP, ACTION_STAND, 1, 0.5F
    };
    return light_dash_mode_check(actions, observe, triangles, count, input);
}

__declspec(dllexport) int jp_emerald_light_dash_observe_geometry(
    NJS_ACTION *stand, NJS_ACTION *dash, NJS_ACTION *skid, light_dash_observer observe,
    const struct jp_stage_triangle *triangles, size_t count)
{
    return light_dash_observe(stand, dash, skid, observe, triangles, count, NULL);
}

static int dash_capture_input_phase;

__declspec(dllexport) int jp_emerald_dash_capture_input_phase(void)
{
    return dash_capture_input_phase;
}

static int dash_capture_input(int held)
{
    dash_capture_input_phase = held ? 1 : 2;
    return 0;
}

__declspec(dllexport) int jp_emerald_light_dash_observe_geometry_input(
    NJS_ACTION *stand, NJS_ACTION *dash, NJS_ACTION *skid, light_dash_observer observe,
    const struct jp_stage_triangle *triangles, size_t count)
{
    if (!triangles || !count || dash_capture_input_phase) return -1;
    Bool pads = pad_is_enabled(), player = pad_player_is_enabled(0);
    pad_enable();
    pad_enable_player(0);
    int result = light_dash_observe(stand, dash, skid, observe, triangles, count, dash_capture_input);
    dash_capture_input_phase = 0;
    if (!player) pad_disable_player(0);
    if (!pads) pad_disable();
    return result;
}

__declspec(dllexport) int jp_emerald_light_dash_input_check(
    NJS_ACTION *stand, NJS_ACTION *dash, NJS_ACTION *skid, light_dash_input input)
{
    if (!input) return -1;
    return light_dash_observe(stand, dash, skid, NULL, NULL, 0, input);
}

__declspec(dllexport) int jp_emerald_light_dash_observe(
    NJS_ACTION *stand, NJS_ACTION *dash, NJS_ACTION *skid, light_dash_observer observe)
{
    return jp_emerald_light_dash_observe_geometry(stand, dash, skid, observe, NULL, 0);
}

__declspec(dllexport) int jp_emerald_light_dash_playback_check(
    NJS_ACTION *stand, NJS_ACTION *dash, NJS_ACTION *skid)
{
    return jp_emerald_light_dash_observe(stand, dash, skid, NULL);
}

int jp_sonic_draw_with_assets(struct task *, NJS_TEXLIST *, NJS_TEXLIST *, NJS_OBJECT *);

__declspec(dllexport) int jp_emerald_light_dash_draw(
    struct task *task, NJS_TEXLIST *textures, NJS_OBJECT *aura)
{
    struct task_work *work = task->work;
    NJS_VECTOR rebase = {-work->pos.x, -work->pos.y, -work->pos.z};
    if (!njPushMatrix(NULL)) return -1;
    njTranslateV(NULL, &rebase);
    int result = jp_sonic_draw_with_assets(task, textures, textures, aura);
    njPopMatrix(1);
    return result;
}
