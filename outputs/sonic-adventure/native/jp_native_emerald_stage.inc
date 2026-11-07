int jp_stage_resources_open(int stage);
int jp_stage_resources_close(void);
int jp_stage_resources_get(unsigned index, struct resource_view *out);
int jp_emerald_set_select(unsigned act);
int jp_emerald_set_close(void);

enum { STAGE_RESOURCES = 1, STAGE_RING_BINDING = 2, STAGE_SET = 4, STAGE_FLOOR = 8 };
static unsigned stage_session;

/* Borrow the stage's static geometry through close. The sound system owns
 * the bank after loading it, including replacement and global shutdown. */
__declspec(dllexport) int jp_emerald_stage_close(void)
{
    if (stage_session & STAGE_SET) {
        if (jp_emerald_set_close()) return -1;
        stage_session &= ~STAGE_SET;
    }
    if (stage_session & STAGE_FLOOR) {
        jp_emerald_ring_floor(NULL, 0);
        stage_session &= ~STAGE_FLOOR;
    }
    if (stage_session & STAGE_RING_BINDING) {
        if (jp_emerald_ring_bind(0)) return -1;
        stage_session &= ~STAGE_RING_BINDING;
    }
    if (stage_session & STAGE_RESOURCES) {
        if (jp_stage_resources_close()) return -1;
        stage_session &= ~STAGE_RESOURCES;
    }
    return 0;
}

__declspec(dllexport) int jp_emerald_stage_open(unsigned act,
    const struct jp_stage_triangle *triangles, size_t count)
{
    struct resource_view existing;
    if (stage_session || act >= 3 || !triangles || !count ||
        jp_stage_resources_get(0, &existing) == 0) return -1;
    if (jp_stage_resources_open(1)) return -1;
    stage_session = STAGE_RESOURCES;
    if (jp_emerald_ring_bind(1)) goto failed;
    stage_session |= STAGE_RING_BINDING;
    if (jp_emerald_set_select(act) < 0) goto failed;
    stage_session |= STAGE_SET;
    if (jp_effects_load_file(EMERALD_BANK, 0, 0)) goto failed;
    jp_emerald_ring_floor(triangles, count);
    stage_session |= STAGE_FLOOR;
    return 0;
failed:
    (void)jp_emerald_stage_close();
    return -1;
}
