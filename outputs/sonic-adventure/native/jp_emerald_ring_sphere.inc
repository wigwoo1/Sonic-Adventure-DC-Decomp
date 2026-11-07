/* The collector API supplies a world-space snapshot. Keep its collision
 * record alive across the ring's deferred pickup tick. */
float jp_collision_sphere_pair(struct collision_work *, struct collision_work *);
static struct task_work ring_collector_work;
static struct task ring_collector_task = {.work = &ring_collector_work};
static struct collision_shape ring_collector_shape;
static struct collision_work ring_collector_record = {
    .kind = COLLISION_PLAYER, .count = 1, .owner = &ring_collector_task,
    .shapes = &ring_collector_shape
};
static unsigned ring_sphere_pairs;

__declspec(dllexport) unsigned jp_emerald_ring_sphere_pairs(void)
{
    return ring_sphere_pairs;
}

static void ring_test_collector(struct collision_work *ring,
                                const NJS_POINT3 *center, Float radius)
{
    ring_collector_shape = (struct collision_shape){
        .flags = SHAPE_SPHERE | SHAPE_IN_WORLD, .center = *center, .radius = radius
    };
    collision_reset(&ring_collector_work, &ring_collector_record);
    jp_collision_sphere_pair(&ring_collector_record, ring);
    /* Ring tasks may retire independently. The collector keeps no back-link. */
    collision_reset(&ring_collector_work, &ring_collector_record);
    ++ring_sphere_pairs;
}
