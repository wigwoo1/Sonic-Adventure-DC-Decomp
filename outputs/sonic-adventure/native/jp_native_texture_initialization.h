/* Host policy around genuine SDK initialization of a caller-owned table. */
#ifndef JP_NATIVE_TEXTURE_INITIALIZATION_H
#define JP_NATIVE_TEXTURE_INITIALIZATION_H
#include "jp_native_textures.h"
JP_TEXTURE_API int jp_texture_sdk_limits(uint32_t bindings,size_t byte_budget);
JP_TEXTURE_API int jp_texture_begin_sdk_init(NJS_TEXMEMLIST *records,Uint32 count);
JP_TEXTURE_API void jp_texture_finish_sdk_init(void);
#endif
