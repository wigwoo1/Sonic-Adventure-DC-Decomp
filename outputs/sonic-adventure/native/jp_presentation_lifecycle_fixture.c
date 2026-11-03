/* Authored TEST fixture, included after privately extracted game functions.
 * These globals are controlled test inputs, never production service stubs.
 * All record layouts and lifecycle algorithms come from the pinned dependency.
 */
#define JP_PROBE __declspec(dllexport)
int frame_step;
struct task_work *camera_work;
struct task *player_tasks[PLAYER_COUNT];
struct player *player_records[PLAYER_COUNT];
Float port_wide_anchor;
Int port_wide_cover;

JP_PROBE void jp_lifecycle_reset(int keep, int redraw)
{
    keeping = keep;
    frame_redrawing = redraw;
    present_task = NULL;
    lost = FALSE;
    memset(marks, 0, sizeof marks);
    marks_used = 0;
    memset(first_held, 0, sizeof first_held);
    memset(later_held, 0, sizeof later_held);
    first_draws.count = later_draws.count = 0;
    memset(&first_flats, 0, sizeof first_flats);
    memset(&later_flats, 0, sizeof later_flats);
    memset(shadows, 0, sizeof shadows);
    shadows_count = 0;
    memset(player_tasks, 0, sizeof player_tasks);
    memset(player_records, 0, sizeof player_records);
    camera_work = NULL;
    port_wide_anchor = WIDE_MIDDLE;
    port_wide_cover = FALSE;
}

JP_PROBE int jp_lifecycle_seed(struct task *task, Uint32 flags)
{
    if (!task || first_draws.count == FIRST_MOST || later_draws.count == LATER_MOST ||
        first_flats.count == FLATS_MOST || later_flats.count == FLATS_MOST ||
        shadows_count == SHADOWS_MOST) return 0;
    first_held[first_draws.count++].task = task;
    later_held[later_draws.count++].task = task;
    first_flats.flat[first_flats.count++].task = task;
    later_flats.flat[later_flats.count++].task = task;
    shadows[shadows_count++].task = task;
    marks_add(task, flags);
    return 1;
}

JP_PROBE struct task *jp_lifecycle_owner(int list, int index)
{
    if (index < 0) return NULL;
    if (list == 0 && index < first_draws.count) return first_held[index].task;
    if (list == 1 && index < later_draws.count) return later_held[index].task;
    if (list == 2 && index < first_flats.count) return first_flats.flat[index].task;
    if (list == 3 && index < later_flats.count) return later_flats.flat[index].task;
    if (list == 4 && index < shadows_count) return shadows[index].task;
    return NULL;
}

/* 1=ordinary/live owner, 2=deleted without destructor, 3=resources released. */
JP_PROBE int jp_lifecycle_owner_kind(int list, int index)
{
    struct task *owner = jp_lifecycle_owner(list, index);
    if (!owner) return 0;
    if (owner == &deleted_task) return 2;
    if (owner == &released_task) return 3;
    return 1;
}
JP_PROBE int jp_lifecycle_shadow_count(void) { return shadows_count; }
JP_PROBE Uint32 jp_lifecycle_flags(struct task *task)
{
    Uint32 *flags = marks_find(task);
    return flags ? *flags : 0;
}
JP_PROBE int jp_lifecycle_replays(int index, int second, int sprite)
{
    if (index < 0 || index >= first_draws.count) return -1;
    struct held draw = first_held[index];
    draw.kind = sprite ? PRESENT_NJ_SPRITE : PRESENT_MODEL;
    return replays(&draw, second);
}
JP_PROBE void jp_lifecycle_redraw(int value) { frame_redrawing = value; }
JP_PROBE void jp_lifecycle_camera(struct task *task) { camera_work = task ? task->work : NULL; }
JP_PROBE struct task *jp_lifecycle_current(void) { return present_task; }
JP_PROBE void jp_lifecycle_wide(Float anchor, Int cover) { present_wide(anchor, cover); }
JP_PROBE Float jp_lifecycle_anchor(void) { return port_wide_anchor; }
JP_PROBE Int jp_lifecycle_cover(void) { return port_wide_cover; }
JP_PROBE int jp_lifecycle_lost(void) { return lost; }

/* Create player records using the real heap; the test owns the player record
 * independently and must release it after clearing its binding. */
JP_PROBE struct player *jp_lifecycle_bind_player(struct task *task, int slot, int count)
{
    if (!task || !task->work || !task->motion || slot < 0 || slot >= PLAYER_COUNT ||
        count < 0 || count > SHAPES_KEPT + 1 || task->work->collision) return NULL;
    struct player *player = heap_calloc(1, sizeof(struct player));
    struct collision_work *collision = heap_calloc(1, sizeof(struct collision_work));
    struct collision_shape *shapes = count ? heap_calloc(count, sizeof(struct collision_shape)) : NULL;
    if (!player || !collision || (count && !shapes)) {
        if (player) heap_free(player);
        if (collision) heap_free(collision);
        if (shapes) heap_free(shapes);
        return NULL;
    }
    collision->flags = COLLISION_OWN_SHAPES;
    collision->count = count;
    collision->shapes = shapes;
    task->work->collision = collision;
    player_tasks[slot] = task;
    player_records[slot] = player;
    return player;
}
JP_PROBE void jp_lifecycle_unbind_player(struct task *task)
{
    for (int i = 0; i < PLAYER_COUNT; i++) if (player_tasks[i] == task) {
        player_tasks[i] = NULL;
        player_records[i] = NULL;
    }
}
JP_PROBE void *jp_lifecycle_region(struct task *task, int which, unsigned *bytes)
{
    *bytes = 0;
    if (which == 0 && task->motion) { *bytes = sizeof(*task->motion); return task->motion; }
    if (which == 1) {
        for (int i = 0; i < PLAYER_COUNT; i++) if (player_tasks[i] == task && player_records[i]) {
            *bytes = sizeof(struct player); return player_records[i];
        }
    }
    if (which == 2 && task->work && task->work->collision) {
        *bytes = task->work->collision->count * sizeof(struct collision_shape);
        return task->work->collision->shapes;
    }
    return NULL;
}
