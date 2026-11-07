/* Diagnostic entry, compiled alongside the original private list owners. */
__declspec(dllexport) int jp_collision_lists_check(void)
{
    struct list_view {
        Uint16 *count;
        struct collision_work **records;
        Uint16 capacity;
    };
#define LIST(name, array) { &collision_##name##_count, array, ROOM(array) }
    const struct list_view lists[] = {
        LIST(player, collision_players), LIST(attack, collision_attacks),
        LIST(item, collision_items), LIST(enemy, collision_enemies),
        LIST(object, collision_objects), LIST(mutual_object, collision_mutual_objects),
        LIST(pickup, collision_pickups), LIST(ring, collision_rings),
        LIST(wall, collision_walls),
    };
#undef LIST
    struct task_work work = {0};
    struct collision_shape shape = {0};
    struct collision_work record = {.shapes = &shape, .count = 1};
    work.collision = &record;
    collision_clear_lists();
    for (Uint16 kind = 0; kind < ROOM(lists); ++kind) {
        const struct list_view *list = &lists[kind];
        record.kind = kind;
        record.flags = COLLISION_HIT | COLLISION_OWN_SHAPES;
        record.hit = shape.hit = &record;
        work.flags = COLLISION_TOUCHED | COLLISION_DAMAGED;
        shape.flags = SHAPE_DRAWN;
        shape.levels = SHAPE_HURT;
        collision_register(&work);
        if (*list->count != 1 || list->records[0] != &record) return 10 + kind;
        if (record.flags != COLLISION_OWN_SHAPES || record.hit || shape.hit ||
            record.hit_shape != -1 || record.own_shape != -1 || shape.hit_shape != -1 ||
            shape.flags || shape.levels || work.flags) return 20 + kind;
        for (int slot = 1; slot <= list->capacity; ++slot) collision_register(&work);
        if (*list->count != list->capacity ||
            list->records[list->capacity - 1] != &record) return 30 + kind;
    }
    dat_8c1b010c = dat_8c1b010d = 1;
    collision_clear_lists();
    for (int i = 0; i < ROOM(lists); ++i) {
        if (*lists[i].count) return 40 + i;
        for (int slot = 0; slot < lists[i].capacity; ++slot) lists[i].records[slot] = NULL;
    }
    if (dat_8c1b010c || dat_8c1b010d) return 50;
    record.kind = COLLISION_PLAYER;
    record.hit = &record;
    int redraw = frame_redrawing;
    Sint16 mode = edit_mode;
    frame_redrawing = 1;
    collision_register(&work);
    frame_redrawing = 0;
    edit_mode = 1;
    collision_register(&work);
    edit_mode = mode;
    frame_redrawing = redraw;
    if (collision_player_count || record.hit != &record) return 51;
    work.collision = NULL;
    collision_register(&work);
    return collision_player_count ? 52 : 0;
}
