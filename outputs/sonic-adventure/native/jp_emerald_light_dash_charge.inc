/* Scoped Sonic player-zero charge path. The owning scheduler must run level
 * zero, including retirement of the charged-speed task, before freeing Sonic. */
__declspec(dllexport) int jp_emerald_light_dash_charge(struct task *task)
{
    if (!task || !task->work || !task->motion || !task->motion->record) return -1;
    struct task_work *work = task->work;
    struct player *record = task->motion->record;
    if (PLAYER_VARS(work)->player != 0 || player_work[0] != work) return -1;
    if (work->mode == SONIC_SPIN_CHARGE)
        sonic_spin_charge(work, task->motion, record);
    else if ((work->mode != SONIC_STAND && work->mode != SONIC_WALK) ||
             (work->flags & SONIC_CHARGED) ||
             !sonic_try_spin(work, task->motion, record)) return 0;
    if (work->flags & PLAYER_BALL) record->own.sonic.curled_steps++;
    return (work->flags & SONIC_CHARGED) ? 2 : 1;
}

typedef int (*charge_input)(int held);
int jp_input_poll(void);

static void charge_fixture_step(struct task *task)
{
    int *result = task->extra;
    if (task->work->flags & SONIC_CHARGED) {
        *result = jp_emerald_try_light_dash(task->work, task->motion,
                                          task->motion->record) ? 3 : 0;
    } else *result = jp_emerald_light_dash_charge(task);
}

static int charge_fixture_input(charge_input input, int held, int edge)
{
    if (input) {
        if (input(held) || jp_input_poll()) return -1;
        pad_scan();
    } else {
        pad_records[0].on = held ? PDD_DGT_TB : 0;
        pad_records[0].press = held && edge ? PDD_DGT_TB : 0;
        pad_records[0].release = !held && edge ? PDD_DGT_TB : 0;
    }
    if (!!(pad_records[0].on & PDD_DGT_TB) != held) return -1;
    return 0;
}

static int light_dash_charge_check(charge_input input)
{
    struct task *task = task_create(TASK_WORK | TASK_MOTION, 1, charge_fixture_step);
    if (!task) return 1;
    struct task_work *work = task->work;
    struct player record = {0};
    int result = -1;
    task->motion->record = &record;
    task->extra = &result;
    work->mode = SONIC_STAND;
    struct task_work *saved_player = player_work[0];
    PDS_PERIPHERAL saved_pad = pad_records[0];
    Uint32 saved_buttons = pad_action_buttons;
    player_work[0] = work;
    pad_action_buttons = PDD_DGT_TB;
    int failed = 0;
    for (int crystal = 0; crystal <= 1; ++crystal) {
        int threshold = crystal ? RING_CHARGE_STEPS : CHARGE_STEPS;
        record.upgrades = PLAYER_SONIC_LIGHT_SHOES | (crystal ? PLAYER_SONIC_CRYSTAL_RING : 0);
        record.consts.top_speed = 6;
        for (int i = 0; i <= threshold; ++i) {
            failed |= charge_fixture_input(input, 1, i == 0) != 0;
            task_step_level(0);
            task_step_level(1);
            failed |= result != (i == threshold ? 2 : 1);
            if (i < threshold) failed |= work->mode != SONIC_SPIN_CHARGE;
        }
        failed |= work->mode != SONIC_STAND ||
                  !(work->flags & SONIC_CHARGED) || record.consts.top_speed != CHARGED_TOP_SPEED;
        failed |= charge_fixture_input(input, 0, 1) != 0;
        task_step_level(0);
        task_step_level(1);
        failed |= result != 3 || work->mode != SONIC_LIGHT_DASH;
        task_step_level(0);
        task_step_level(0);
        failed |= record.consts.top_speed != 6;
        work->mode = SONIC_STAND;
        work->flags = 0;
        record.speed.x = 0;
    }
    pad_records[0] = saved_pad;
    pad_action_buttons = saved_buttons;
    player_work[0] = saved_player;
    task->motion->record = NULL;
    task->extra = NULL;
    task_destroy(task);
    return failed;
}

__declspec(dllexport) int jp_emerald_light_dash_charge_check(void)
{
    return light_dash_charge_check(NULL);
}

__declspec(dllexport) int jp_emerald_light_dash_charge_input_check(charge_input input)
{
    return input ? light_dash_charge_check(input) : -1;
}
