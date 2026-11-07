/* The full dispatcher applies these gates before selecting a shape worker. */
static Bool sphere_pair_enabled(struct collision_work *a, struct collision_work *b)
{
    Uint32 flags_a = a->shapes[a->current].flags;
    Uint32 flags_b = b->shapes[b->current].flags;
    return (flags_a & (SHAPE_KIND | SHAPE_OFF)) == SHAPE_SPHERE &&
           (flags_b & (SHAPE_KIND | SHAPE_OFF)) == SHAPE_SPHERE &&
           !collision_ignores(a, b);
}

__declspec(dllexport) int jp_collision_sphere_filter_check(void)
{
    struct collision_shape shapes[2] = {{0}, {0}};
    struct collision_work a = {.kind = COLLISION_PLAYER, .shapes = &shapes[0]};
    struct collision_work b = {.kind = COLLISION_RING, .shapes = &shapes[1]};
    if (!sphere_pair_enabled(&a, &b)) return 1;
    shapes[1].flags = SHAPE_IGNORE_PLAYERS;
    if (sphere_pair_enabled(&a, &b)) return 2;
    shapes[1].flags = SHAPE_OFF;
    if (sphere_pair_enabled(&a, &b)) return 3;
    shapes[1].flags = SHAPE_CYLINDER;
    if (sphere_pair_enabled(&a, &b)) return 4;
    shapes[1].flags = 0;
    shapes[0].flags = SHAPE_IGNORE_OTHERS;
    if (sphere_pair_enabled(&a, &b)) return 5;
    return 0;
}
