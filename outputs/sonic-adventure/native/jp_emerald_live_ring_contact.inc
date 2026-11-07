/* The player record is borrowed only while the stage drains the ring list. */
static struct collision_work *ring_live_player;
static unsigned ring_live_pairs;

__declspec(dllexport) unsigned jp_emerald_live_ring_pairs(void)
{
    return ring_live_pairs;
}

static void ring_collide_live(struct collision_work *ring)
{
    struct collision_work *player = ring_live_player;
    struct task_work *work = player->owner->work;
    struct collision_work saved = *player;
    Uint32 saved_flags = work->flags;
    for (Uint16 index = 0; index < player->count; ++index) {
        struct collision_shape *shape = &player->shapes[index];
        if ((shape->flags & (SHAPE_KIND | SHAPE_OFF)) != SHAPE_SPHERE ||
            !(shape->radius > 0 && shape->radius <= 1000.0F)) continue;
        struct collision_shape saved_shape = *shape;
        player->current = index;
        jp_collision_sphere_pair(player, ring);
        *shape = saved_shape;
        *player = saved;
        work->flags = saved_flags;
        ++ring_live_pairs;
        ++ring_sphere_pairs;
    }
    /* Pickup consumes the ring's hit flag on its next tick. It needs no
     * pointer back to the player, and the player keeps its prior hit state. */
    if (ring->hit == player) ring->hit = NULL;
    if (ring->shapes[ring->current].hit == player)
        ring->shapes[ring->current].hit = NULL;
}

__declspec(dllexport) int jp_emerald_multi_sphere_check(void)
{
    struct task_work work[2] = {0};
    struct task tasks[2] = {{.work = &work[0]}, {.work = &work[1]}};
    struct collision_shape shapes[] = {
        {.flags = SHAPE_CYLINDER, .radius = 4},
        {.flags = SHAPE_SPHERE | SHAPE_IN_WORLD, .radius = 4},
        {.flags = SHAPE_SPHERE | SHAPE_IN_WORLD, .radius = 5},
    };
    struct collision_work player = {.owner = &tasks[0], .kind = COLLISION_PLAYER,
                                    .count = 2, .shapes = shapes};
    struct collision_work ring = {.owner = &tasks[1], .kind = COLLISION_RING,
                                  .count = 1, .shapes = shapes + 2};
    ring_live_player = &player;
    ring_collide_live(&ring);
    ring_live_player = NULL;
    return !(ring.flags & COLLISION_HIT) || ring.hit_shape != 1 || ring.hit ||
           shapes[2].hit || player.current || player.flags || work[0].flags;
}
