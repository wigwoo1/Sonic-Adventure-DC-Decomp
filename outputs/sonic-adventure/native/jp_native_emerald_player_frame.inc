int jp_sonic_draw_with_assets(struct task *, NJS_TEXLIST *, NJS_TEXLIST *, NJS_OBJECT *);

/* Draw an already stepped player and the stage into one retained frame. */
__declspec(dllexport) int jp_emerald_submit_player_frame(
    struct task *player, NJS_TEXLIST *body, NJS_TEXLIST *effect, NJS_OBJECT *aura,
    struct emerald_scene_draw *draw, uint64_t *frame)
{
    if (!player || !player->work || !body || !effect || !draw || !frame ||
        emerald_stage_stepping || !stage_terrain_ready ||
        !(stage_session & STAGE_SET) || jp_queue_status() || jp_frames_status())
        return -1;
    NJS_MATRIX view;
    if (jp_emerald_point_view(&player->work->pos, &view) ||
        (!stage_ring_textures_ready && jp_emerald_stage_ring_textures_open_disc()) ||
        !njPushMatrix(0)) return -1;
    njSetMatrix(0, &view);
    emerald_stage_stepping = 1;
    int failed = jp_sonic_draw_with_assets(player, body, effect, aura);
    emerald_stage_stepping = 0;
    njPopMatrix(1);
    if (failed) return -1;
    return jp_emerald_stage_submit_frame(&view, -1, 0, draw, frame);
}
