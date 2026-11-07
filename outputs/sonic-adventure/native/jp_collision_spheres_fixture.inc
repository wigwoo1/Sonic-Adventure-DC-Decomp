/* Direct entry for focused verification of the original pair worker. */
__declspec(dllexport) float jp_collision_sphere_pair(struct collision_work *a,
                                                     struct collision_work *b)
{
    return collide_sphere_sphere(a, b);
}

__declspec(dllexport) int jp_collision_spheres_check(void)
{
    struct task_work work[2] = {0};
    struct task tasks[2] = {{.work = &work[0]}, {.work = &work[1]}};
    struct collision_shape shapes[2] = {
        {.flags = SHAPE_SPHERE | SHAPE_IN_WORLD, .radius = 2},
        {.flags = SHAPE_SPHERE | SHAPE_IN_WORLD | SHAPE_KEEPS_HIT, .radius = 2}
    };
    struct collision_work records[2] = {
        {.kind = COLLISION_PLAYER, .owner = &tasks[0], .shapes = &shapes[0], .count = 1},
        {.kind = COLLISION_RING, .owner = &tasks[1], .shapes = &shapes[1], .count = 1}
    };
    shapes[1].center.x = 4;
    if (jp_collision_sphere_pair(records, records + 1) != 16 ||
        records[0].flags || records[1].flags) return 1;
    shapes[1].center.x = 3;
    if (jp_collision_sphere_pair(records, records + 1) != 9) return 2;
    if (records[0].flags != (COLLISION_HIT | COLLISION_HIT_KEPT) ||
        records[1].flags != COLLISION_HIT) return 3;
    for (int i = 0; i < 2; ++i) {
        if (records[i].hit != &records[1 - i] || shapes[i].hit != &records[1 - i] ||
            work[i].flags != COLLISION_TOUCHING || work[i].pos.x) return 4 + i;
    }
    shapes[1].center.x = 0;
    if (jp_collision_sphere_pair(records, records + 1) != 0) return 6;
    return 0;
}
