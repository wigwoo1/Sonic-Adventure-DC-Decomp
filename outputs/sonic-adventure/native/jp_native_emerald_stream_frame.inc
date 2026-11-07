/* Synchronous research host handoff. The sink borrows the retained frame. */
static int stream_player_frame(struct task *task, NJS_TEXLIST *textures,
    NJS_OBJECT *aura, const struct jp_model_bound *bound,
    const uint32_t *words, uint32_t count, int airborne,
    int (*present)(uint64_t, Float, int))
{
    struct emerald_scene_draw scene;
    uint64_t frame = 0;
    if (jp_model_reserve(bound) || jp_frames_prepare_textures(words, count) ||
        jp_emerald_submit_player_frame(task, textures, textures, aura, &scene, &frame))
        return -1;
    int delivered = present(frame, task->work->pos.y, airborne);
    int released = jp_frame_release(frame);
    return delivered || released ? -1 : 0;
}
