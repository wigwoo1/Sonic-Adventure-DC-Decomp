/* Synchronous bounded source views for native Ninja memory-texture uploads.
 * One game thread, one borrowed source view. Uploaded surfaces are owned by
 * the existing texture/frame owner; the borrowed view may then be released. */
#ifndef JP_NATIVE_TEXTURE_UPLOAD_H
#define JP_NATIVE_TEXTURE_UPLOAD_H
#include <stddef.h>
#include <stdint.h>
#ifndef SG_XPT_H
#undef NULL
#endif
#include <ninja.h>
#ifndef JP_TEXTURE_API
#define JP_TEXTURE_API __declspec(dllexport)
#endif
enum jp_memory_error { JP_MEMORY_OK, JP_MEMORY_ARGUMENT, JP_MEMORY_STATE,
    JP_MEMORY_SOURCE, JP_MEMORY_UNSUPPORTED, JP_MEMORY_TEXTURE, JP_MEMORY_THREAD };
JP_TEXTURE_API int jp_memory_bind_source(const void *pixels,size_t bytes);
JP_TEXTURE_API int jp_memory_unbind_source(void);
/* Reset the serialized loader owner only after ending its borrowed view. */
JP_TEXTURE_API int jp_memory_shutdown(void);
JP_TEXTURE_API int jp_memory_status(void);
JP_TEXTURE_API void jp_memory_clear_error(void);
JP_TEXTURE_API int jp_memory_texture_error(void);
JP_TEXTURE_API int jp_memory_load_list(NJS_TEXLIST *list,const void *pixels,size_t bytes);
JP_TEXTURE_API int jp_memory_raw_size(unsigned type,unsigned width,unsigned height,size_t *bytes);
/* Used by the privately compiled original bitmap queue. Dimensions describe
 * the caller's actual complete pixel allocation, not an arbitrary pointer. */
struct bitmap;
JP_TEXTURE_API int jp_bitmap_source_begin(struct bitmap *bitmap);
/* Internal bridge implemented IN PLACE OF the existing retained texture owner.
 * These inspect the same binding table; no second registry/record owner. */
JP_TEXTURE_API int jp_memory_owner_can_bind(Uint32 *field);
JP_TEXTURE_API NJS_TEXMEMLIST *jp_memory_owner_bound(Uint32 *field);
#endif
