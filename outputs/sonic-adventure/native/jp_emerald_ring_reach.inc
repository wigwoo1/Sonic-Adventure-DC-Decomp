static void emerald_clear_near_lists(void)
{
    near_task_count = near_ring_count = 0;
    near_task_count_player1 = near_ring_count_player1 = 0;
    near_tasks[0].work = near_rings[0].work = NULL;
    near_tasks_player1[0].work = near_rings_player1[0].work = NULL;
}

static void remove_near_work(struct near_task *list, Uint16 *count,
                             const struct task_work *work)
{
    Uint16 kept = 0;
    for (Uint16 i = 0; i < *count; ++i)
        if (list[i].work != work) list[kept++] = list[i];
    list[kept] = (struct near_task){0};
    *count = kept;
}

static void emerald_forget_near_ring(const struct task_work *work)
{
    remove_near_work(near_rings, &near_ring_count, work);
    remove_near_work(near_rings_player1, &near_ring_count_player1, work);
}

__declspec(dllexport) unsigned jp_emerald_near_ring_count(void)
{
    return near_ring_count;
}

__declspec(dllexport) int jp_emerald_ring_reach_check(void)
{
    struct task_work work[2] = {{0}, {.id = RING_ID}};
    struct task tasks[2] = {{.work = &work[0]}, {.work = &work[1]}};
    struct collision_work player = {.kind = COLLISION_PLAYER, .radius = 25, .owner = &tasks[0]};
    struct collision_work ring = {.kind = COLLISION_RING, .radius = 5, .owner = &tasks[1],
                                  .flags = COLLISION_TARGET};
    struct task_work *saved = player_work[0];
    player_work[0] = &work[0];
    emerald_clear_near_lists();
    work[1].pos.x = 30;
    Bool tangent = list_near_and_test_reach(&work[0], &player, &work[1], &ring);
    work[1].pos.x = 29;
    Bool inside = list_near_and_test_reach(&work[0], &player, &work[1], &ring);
    work[1].pos.x = 100;
    Bool far = list_near_and_test_reach(&work[0], &player, &work[1], &ring);
    int failed = tangent || !inside || far || near_ring_count != 2 ||
                 near_rings[0].work != &work[1] || near_rings[0].distance != 900 ||
                 near_rings[1].distance != 841 || near_rings[2].work;
    near_rings[2] = (struct near_task){&work[0], 4};
    near_ring_count = 3;
    near_rings_player1[0] = (struct near_task){&work[1], 841};
    near_ring_count_player1 = 1;
    emerald_forget_near_ring(&work[1]);
    failed |= near_ring_count != 1 || near_rings[0].work != &work[0] ||
              near_rings[0].distance != 4 || near_rings[1].work ||
              near_ring_count_player1 || near_rings_player1[0].work;
    emerald_clear_near_lists();
    player_work[0] = saved;
    return failed || near_rings[0].work != NULL;
}
