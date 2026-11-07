__declspec(dllexport) int jp_light_dash_target_check(void)
{
    struct task_work player = {0}, rings[3] = {0};
    struct collision_shape shape = {0};
    struct collision_work collision = {.shapes = &shape};
    for (int i = 0; i < 3; ++i) rings[i].collision = &collision;
    rings[0].pos.x = -3;
    rings[1].pos.x = 8;
    rings[2].pos.x = 4;
    struct near_task saved[4], saved_other = near_rings_player1[0];
    memcpy(saved, near_rings, sizeof saved);
    near_rings[0] = (struct near_task){&rings[0], 9};
    near_rings[1] = (struct near_task){&rings[1], 64};
    near_rings[2] = (struct near_task){&rings[2], 16};
    near_rings[3] = (struct near_task){0};
    int failed = jp_emerald_light_dash_target(&player) != &rings[2];
    struct task_motion motion = {0};
    struct player record = {0};
    failed |= jp_emerald_light_dash_steer(&player, &motion, &record) != 1 ||
              motion.speed.x < 3.99F || motion.speed.x > 4.01F ||
              record.own.sonic.dash_steps != 5 || motion.aim.y;
    near_rings[2].distance = 1;
    failed |= jp_emerald_light_dash_steer(&player, &motion, &record) != 1 ||
              motion.speed.x < 1.99F || motion.speed.x > 2.01F;
    near_rings[1].distance = 625;
    near_rings[2].distance = 400;
    failed |= jp_emerald_light_dash_steer(&player, &motion, &record) != 1 ||
              motion.speed.x < 15.99F || motion.speed.x > 16.01F;
    near_rings[2].distance = 16;
    near_rings[1].distance = 16;
    failed |= jp_emerald_light_dash_target(&player) != &rings[1];
    near_rings[1].work = NULL;
    failed |= jp_emerald_light_dash_target(&player) != NULL;
    failed |= jp_emerald_light_dash_steer(&player, &motion, &record) != 0 ||
              motion.speed.x < 15.99F || motion.speed.x > 16.01F;
    PLAYER_VARS(&player)->player = 1;
    near_rings_player1[0] = (struct near_task){0};
    failed |= jp_emerald_light_dash_target(&player) != NULL;
    record = (struct player){0};
    motion = (struct task_motion){0};
    record.consts.speed_limit = 50;
    motion.accel.x = 1;
    failed |= jp_emerald_light_dash_step(&player, &motion, &record) != 2 ||
              record.speed.x != 1 || motion.speed.x != 1;
    player.flags = PLAYER_ON_GROUND;
    failed |= jp_emerald_light_dash_step(&player, &motion, &record) != -1;
    Uint32 saved_buttons = pad_action_buttons;
    Uint32 saved_on = pad_records[1].on;
    pad_action_buttons = 4;
    pad_records[1].on = 4;
    player.flags = SONIC_CHARGED | PLAYER_ATTACKING;
    if (jp_emerald_try_light_dash(&player, &motion, &record) ||
        !(player.flags & SONIC_CHARGED)) failed |= 2;
    pad_records[1].on = 0;
    record.upgrades = PLAYER_SONIC_ANCIENT_LIGHT;
    if (jp_emerald_try_light_dash(&player, &motion, &record) ||
        !(player.flags & SONIC_CHARGED)) failed |= 4;
    record.upgrades = 0;
    if (!jp_emerald_try_light_dash(&player, &motion, &record)) failed |= 8;
    if (player.mode != SONIC_LIGHT_DASH || record.anim.action != ACTION_LIGHT_DASH ||
        record.own.sonic.dash_steps != LIGHT_DASH_STEPS || record.speed.x != LIGHT_DASH_SPEED)
        failed |= 16;
    if ((player.flags & SONIC_CHARGED) || (player.flags & PLAYER_ATTACKING) != PLAYER_GUARD)
        failed |= 32;
    if (jp_emerald_try_light_dash(&player, &motion, &record) != FALSE) failed |= 64;
    NJS_POINT3 old_history, prior = {0};
    struct task_work *saved_player = player_work[1];
    player_work[1] = &player;
    player_get_history(1, 1, &old_history, NULL);
    player_set_history(1, 1, &prior, NULL);
    player.pos.x = 2;
    if (jp_emerald_light_dash_duration(&player, &record) != 1 ||
        record.own.sonic.dash_steps != LIGHT_DASH_STEPS - 1) failed |= 128;
    player.pos.x = LIGHT_DASH_STOP;
    if (jp_emerald_light_dash_duration(&player, &record) != 0 ||
        player.mode != SONIC_STOP || record.anim.action != ACTION_SKID ||
        record.own.sonic.dash_steps || record.speed.x != LIGHT_DASH_END_SPEED ||
        (player.flags & PLAYER_GUARD)) failed |= 256;
    player.mode = SONIC_LIGHT_DASH;
    player.pos.x = 10;
    record.speed.x = 1;
    if (jp_emerald_light_dash_duration(&player, &record) != 0 ||
        player.mode != SONIC_STOP || record.speed.x != 1 ||
        jp_emerald_light_dash_duration(&player, &record) != -1) failed |= 512;
    player_set_history(1, 1, &old_history, NULL);
    player_work[1] = saved_player;
    pad_records[1].on = saved_on;
    pad_action_buttons = saved_buttons;
    memcpy(near_rings, saved, sizeof saved);
    near_rings_player1[0] = saved_other;
    return failed;
}
