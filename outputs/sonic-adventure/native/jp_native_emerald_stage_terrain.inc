#include "jp_native_pvm.h"

enum { STAGE_TEXTURE_LIMIT = 512, STAGE_TEXTURE_NAME_BYTES = 28 };
static NJS_TEXLIST stage_textures;
static char (*stage_texture_names)[STAGE_TEXTURE_NAME_BYTES];
static int stage_terrain_error;
static int stage_terrain_ready;

int jp_emerald_draw_land(Sint32 blocks, Bool shown, Sint32 step);

static int close_stage_textures(void)
{
    stage_terrain_ready = 0;
    if (!stage_textures.textures) return 0;
    if (jp_pvm_loaded_count(&stage_textures) &&
        jp_pvm_release_loaded(&stage_textures) != 1) return -1;
    if (_nj_current_texlist == &stage_textures) _nj_current_texlist = NULL;
    free(stage_texture_names);
    free(stage_textures.textures);
    stage_texture_names = NULL;
    stage_textures = (NJS_TEXLIST){0};
    return 0;
}

static void draw_stage_terrain_model(NJS_MODEL *model)
{
    if (!stage_terrain_error)
        stage_terrain_error = jp_emerald_stage_mesh_submit(model);
}

/* Load the stage's decoded PVM and register the native mesh consumer.
 * The stage session retires the task before dropping its texture list. */
__declspec(dllexport) int jp_emerald_stage_terrain_open(void)
{
    struct resource_view archive;
    if (!(stage_session & STAGE_FLOOR) || emerald_stage_stepping ||
        stage_textures.textures || stage_land_entries ||
        jp_stage_resources_get(7, &archive) || archive.bytes < 12) return -1;
    const unsigned char *bytes = archive.data;
    int16_t count;
    memcpy(&count, bytes+10, sizeof count);
    if (memcmp(bytes, "PVMH", 4) || count <= 0 || count > STAGE_TEXTURE_LIMIT) return -1;
    NJS_TEXNAME *textures = calloc(count, sizeof *textures);
    char (*names)[STAGE_TEXTURE_NAME_BYTES] = calloc(count, sizeof *names);
    if (!textures || !names) {
        free(textures);
        free(names);
        return -1;
    }
    stage_texture_names = names;
    stage_textures = (NJS_TEXLIST){textures, (Uint32)count};
    for (int i = 0; i < count; ++i) textures[i].filename = names[i];
    if (jp_pvm_load_memory(&stage_textures, count, sizeof *names, archive.data, archive.bytes) != 1 ||
        jp_emerald_stage_land_open_native(&stage_textures, draw_stage_terrain_model)) {
        (void)close_stage_textures();
        return -1;
    }
    stage_terrain_ready = 1;
    return 0;
}

__declspec(dllexport) const NJS_TEXLIST *jp_emerald_stage_terrain_textures(void)
{
    return stage_terrain_ready ? &stage_textures : NULL;
}

__declspec(dllexport) int jp_emerald_stage_terrain_draw(Sint32 blocks, Bool shown, Sint32 step)
{
    if (!stage_terrain_ready || emerald_stage_stepping) return -1;
    NJS_TEXLIST *saved = _nj_current_texlist;
    stage_terrain_error = 0;
    emerald_stage_stepping = 1;
    int count = jp_emerald_draw_land(blocks, shown, step);
    emerald_stage_stepping = 0;
    _nj_current_texlist = saved;
    return stage_terrain_error ? -1 : count;
}
