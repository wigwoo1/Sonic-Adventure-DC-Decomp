struct emerald_camera_mode {
    int32_t row, mode, level, type, adjust, cut;
    NJS_POINT3 position, target;
    int32_t pitch, yaw;
    Float distance;
};

static int camera_row = -1;
static int camera_level;
static int camera_adjust_id;
static int camera_cut_requested;
static int saved_row[CAMERA_LEVELS], saved_level[CAMERA_LEVELS], saved_adjust[CAMERA_LEVELS];

static int camera_rows(struct resource_view *view)
{
    if (emerald_camera_act < 0 ||
        jp_stage_resources_get(4 + emerald_camera_act, view) || view->bytes < 64) return -1;
    uint32_t count;
    memcpy(&count, view->data, sizeof count);
    if (!count || count > CAMERA_COLLISIONS_MAX || count > view->bytes/64 - 1) return -1;
    return (int)count;
}

static struct camera_collision camera_read_row(const struct resource_view *view, int index)
{
    struct camera_collision row;
    memcpy(&row, (const unsigned char *)view->data + (index+1)*64, sizeof row);
    return row;
}

static void emerald_camera_reset(void)
{
    camera_row = -1;
    camera_level = CAMERA_LEVEL_NORMAL;
    camera_adjust_id = CAMERA_ADJUST_NONE;
    camera_cut_requested = 0;
}

static void camera_leave_region(const struct resource_view *view)
{
    int previous = camera_read_row(view, camera_row).priority;
    int restored = camera_read_row(view, saved_row[camera_level]).priority;
    int level = camera_level;
    camera_row = saved_row[level];
    camera_level = saved_level[level];
    camera_adjust_id = previous >= PRIORITY_HIGH && restored >= PRIORITY_HIGH
        ? CAMERA_ADJUST_NONE : saved_adjust[level];
    camera_cut_requested |= previous < restored;
}

static void camera_enter_region(const struct resource_view *view, int index,
                                const struct camera_collision *row, int level)
{
    int previous = camera_read_row(view, camera_row).priority;
    saved_row[level] = camera_row;
    saved_level[level] = camera_level;
    saved_adjust[level] = camera_adjust_id;
    camera_row = index;
    camera_level = level;
    camera_adjust_id = previous >= PRIORITY_HIGH && row->priority >= PRIORITY_HIGH
        ? CAMERA_ADJUST_NONE : row->adjust;
    camera_cut_requested |= previous < row->priority;
}

static void emerald_camera_choose(void)
{
    struct resource_view view;
    int count = camera_rows(&view);
    if (count < 0) return;
    if (camera_row < 0) camera_row = 0;
    camera_cut_requested = 0;
    unsigned char contacted[CAMERA_COLLISIONS_MAX] = {0};
    for (unsigned i = 0; i < emerald_camera_count; ++i)
        contacted[emerald_camera_hits[i]] = 1;

    /* A drop in level restarts the reverse scan. Bound restarts in case a
     * malformed layout makes two region transitions cycle. */
    for (int pass = 0; pass < CAMERA_LEVELS; ++pass) {
        int initial_level = camera_level;
        int index;
        for (index = count-1; index > 0; --index) {
            struct camera_collision row = camera_read_row(&view, index);
            if (row.mode < 0 || row.mode >= CAMERA_MODES) continue;
            int level = camera_mode_metadata[(unsigned)row.mode][0];
            if (level < camera_level || level == CAMERA_LEVEL_COLI ||
                level == CAMERA_LEVEL_EVENT || level == CAMERA_LEVEL_DEBUG) continue;
            if (camera_level == CAMERA_LEVEL_NORMAL) {
                if (camera_row != index && contacted[index])
                    camera_enter_region(&view, index, &row, level);
            } else {
                int inside = contacted[index] && row.type != CAMCOL_PLANE;
                if (camera_row == index) {
                    if (!inside) camera_leave_region(&view);
                } else if (inside && level > camera_level) {
                    camera_enter_region(&view, index, &row, level);
                } else if (inside && level == camera_level &&
                           camera_read_row(&view, camera_row).priority < row.priority) {
                    camera_leave_region(&view);
                    camera_enter_region(&view, index, &row, level);
                }
            }
            if (camera_level < initial_level) break;
        }
        if (!index) break;
    }
}

__declspec(dllexport) int jp_emerald_camera_mode(struct emerald_camera_mode *out)
{
    struct resource_view view;
    int count = camera_rows(&view);
    int index = camera_row < 0 ? 0 : camera_row;
    if (!out || index >= count) return -1;
    struct camera_collision row = camera_read_row(&view, index);
    if (row.mode < 0 || row.mode >= CAMERA_MODES) return -1;
    *out = (struct emerald_camera_mode){index, row.mode, camera_level,
        camera_mode_metadata[(unsigned)row.mode][1], camera_adjust_id, camera_cut_requested,
        row.cam_pos, row.dir_pos, row.cam_ang_x, row.cam_ang_y, row.distance};
    return 0;
}
