/* The caller supplies the shared stage view and borrows the draw assets
 * through this call. Recorded packets retain their texture surfaces. */
__declspec(dllexport) int jp_emerald_stage_draw_rings(
    NJS_MODEL *model, const struct simple_mesh_span *span, NJS_TEXLIST *textures,
    NJS_SPRITE *sprite, int *models, int *sprites)
{
    if (emerald_stage_stepping || !(stage_session & STAGE_SET) ||
        !model || !span || !textures || !sprite || !models || !sprites ||
        models == sprites) return -1;

    const NJS_POINT3 origin = {0};
    emerald_stage_stepping = 1;
    int ring_count = jp_emerald_ring_draw_level(model, span, textures, &origin);
    int sparkle_count = ring_count < 0 ? -1 :
        jp_emerald_sparkle_draw_level(sprite, &origin, 0, 0);
    emerald_stage_stepping = 0;
    if (sparkle_count < 0) return -1;
    *models = ring_count;
    *sprites = sparkle_count;
    return 0;
}
