#include "jp_native_textures.h"

enum { STAGE_SPARKLE_TEXTURES = 8 };
static NJS_TEXNAME ring_texture_entries[1];
static NJS_TEXNAME sparkle_texture_entries[STAGE_SPARKLE_TEXTURES];
static char ring_texture_names[1][STAGE_TEXTURE_NAME_BYTES];
static char sparkle_texture_names[STAGE_SPARKLE_TEXTURES][STAGE_TEXTURE_NAME_BYTES];
static NJS_TEXLIST stage_ring_textures = {ring_texture_entries, 1};
static NJS_TEXLIST stage_sparkle_textures = {sparkle_texture_entries, STAGE_SPARKLE_TEXTURES};
static int stage_ring_textures_ready;

static int close_stage_ring_textures(void)
{
    stage_ring_textures_ready = 0;
    if (jp_pvm_loaded_count(&stage_sparkle_textures) &&
        jp_pvm_release_loaded(&stage_sparkle_textures) != 1) return -1;
    if (jp_pvm_loaded_count(&stage_ring_textures) &&
        jp_pvm_release_loaded(&stage_ring_textures) != 1) return -1;
    if (_nj_current_texlist == &stage_ring_textures ||
        _nj_current_texlist == &stage_sparkle_textures) _nj_current_texlist = 0;
    return 0;
}

__declspec(dllexport) int jp_emerald_stage_ring_textures_open(
    const void *ring, size_t ring_bytes, const void *sparkles, size_t sparkle_bytes)
{
    if (!(stage_session & STAGE_SET) || emerald_stage_stepping ||
        stage_ring_textures_ready || !ring || !sparkles ||
        jp_memory_status() || jp_textures_status() ||
        jp_pvm_loaded_count(&stage_ring_textures) ||
        jp_pvm_loaded_count(&stage_sparkle_textures)) return -1;

    ring_texture_entries[0].filename = ring_texture_names[0];
    for (int i = 0; i < STAGE_SPARKLE_TEXTURES; ++i)
        sparkle_texture_entries[i].filename = sparkle_texture_names[i];
    if (jp_pvm_load_memory(&stage_ring_textures, 1, STAGE_TEXTURE_NAME_BYTES,
                           ring, ring_bytes) != 1 ||
        jp_pvm_load_memory(&stage_sparkle_textures, STAGE_SPARKLE_TEXTURES,
                           STAGE_TEXTURE_NAME_BYTES, sparkles, sparkle_bytes) != 1 ||
        jp_pvm_loaded_count(&stage_ring_textures) != 1 ||
        jp_pvm_loaded_count(&stage_sparkle_textures) != STAGE_SPARKLE_TEXTURES) {
        /* The loader latches errors and blocks release until they are cleared.
         * Both subsystems were clean on entry, so these errors belong to us. */
        jp_memory_clear_error();
        jp_textures_clear_error();
        (void)close_stage_ring_textures();
        return -1;
    }
    stage_ring_textures_ready = 1;
    return 0;
}

__declspec(dllexport) const NJS_TEXLIST *jp_emerald_stage_ring_texture_list(int sparkle)
{
    if (!stage_ring_textures_ready) return 0;
    return sparkle ? &stage_sparkle_textures : &stage_ring_textures;
}

/* Sprite animation and model data remain borrowed for this draw. */
__declspec(dllexport) int jp_emerald_stage_draw_owned_rings(
    NJS_MODEL *model, const struct simple_mesh_span *span, const NJS_SPRITE *sprite,
    int *models, int *sprites)
{
    if (!stage_ring_textures_ready || !sprite) return -1;
    NJS_SPRITE bound = *sprite;
    bound.tlist = &stage_sparkle_textures;
    return jp_emerald_stage_draw_rings(model, span, &stage_ring_textures,
                                      &bound, models, sprites);
}
