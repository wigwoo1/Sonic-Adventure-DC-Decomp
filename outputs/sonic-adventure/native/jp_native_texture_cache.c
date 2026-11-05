/* Replace texture_space.obj: preserve the actual owner and original SDK
 * cache variable. Higher modules call an operation rather than redeclaring
 * the SDK's plain extern variable with Windows dllimport attributes. */
#include "jp_native_texture_space.c"
__declspec(dllexport) void jp_texture_cache_maximum(uint32_t bytes)
{_nj_tex_max_free_size=(Uint32)bytes;}
