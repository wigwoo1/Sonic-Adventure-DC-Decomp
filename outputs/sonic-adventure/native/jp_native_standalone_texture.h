/* Direct SDK surfaces (such as the debug font) live outside record tables. */
#ifndef JP_NATIVE_STANDALONE_TEXTURE_H
#define JP_NATIVE_STANDALONE_TEXTURE_H
#include "jp_native_texture_initialization.h"
JP_TEXTURE_API int jp_texture_replace_standalone_surface(NJS_TEXSURFACE *surface,
    const void *pixels,size_t bytes);
#endif
