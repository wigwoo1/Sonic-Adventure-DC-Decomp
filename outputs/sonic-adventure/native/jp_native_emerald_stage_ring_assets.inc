/* This private include is generated from the verified JP source baseline. */
#include "stage_ring_data.inc"

__declspec(dllexport) int jp_emerald_stage_draw_disc_rings(int *models, int *sprites)
{
    if (!stage_ring_textures_ready && jp_emerald_stage_ring_textures_open_disc())
        return -1;
    return jp_emerald_stage_draw_owned_rings(&stage_ring_model, &stage_ring_span,
                                             &stage_ring_sprite, models, sprites);
}
