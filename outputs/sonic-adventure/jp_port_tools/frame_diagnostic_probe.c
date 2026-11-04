/* Preserve the older diagnostic observer; prepare a REAL owned TCW too.
 * The old fixture copied/entered pixels but left texparambuffer zero. That
 * was sufficient for paired raw-header tests, not for a renderer consumer.
 * This diagnostic version runs the actual SDK parameter builder. No game
 * dependency or original instruction body is replaced with a fixture hook.
 */
#define jp_sprite_probe_prepare_texture jp_unprepared_sprite_texture
#include "primitive_diagnostic_probe.c"
#undef jp_sprite_probe_prepare_texture
__declspec(dllexport) NJS_TEXLIST *jp_sprite_probe_prepare_texture(void)
{
    NJS_TEXLIST *list=jp_unprepared_sprite_texture();if(!list)return NULL;
    njSetTextureParam(jp_textures_record(0));
    return jp_textures_status()?NULL:list;
}
