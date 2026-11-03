/* Authored native ownership for SDK texture records and opaque 32-bit handles.
 * One game thread. External SDK type declarations remain an external input. */
#ifndef JP_NATIVE_TEXTURES_H
#define JP_NATIVE_TEXTURES_H
#include <stddef.h>
#include <stdint.h>
#undef NULL
#include <ninja.h>
#define JP_TEXTURE_API __declspec(dllexport)
enum jp_texture_error { JP_TEXTURE_OK, JP_TEXTURE_ARGUMENT, JP_TEXTURE_CAPACITY,
    JP_TEXTURE_BUDGET, JP_TEXTURE_ALLOCATION, JP_TEXTURE_STALE, JP_TEXTURE_STATE,
    JP_TEXTURE_EXHAUSTED, JP_TEXTURE_NOT_INITIALIZED };
JP_TEXTURE_API int jp_textures_initialize(uint32_t records,uint32_t bindings,size_t byte_budget);
JP_TEXTURE_API void jp_textures_shutdown(void);
JP_TEXTURE_API int jp_textures_status(void);
JP_TEXTURE_API void jp_textures_clear_error(void);
JP_TEXTURE_API size_t jp_textures_allocated_bytes(void);
JP_TEXTURE_API uint32_t jp_textures_live_surfaces(void);
/* Record views expire on shutdown/reinitialization; handles never recycle. */
JP_TEXTURE_API NJS_TEXMEMLIST *jp_textures_record(uint32_t index);
JP_TEXTURE_API NJS_TEXMEMLIST *jp_texture_resolve(uint32_t handle);
/* Loader RECORD consumers must validate the field's handle phase as well. */
JP_TEXTURE_API NJS_TEXMEMLIST *jp_texture_resolve_field(Uint32 *field);
JP_TEXTURE_API int jp_texture_store_handle(Uint32 *field,NJS_TEXMEMLIST *record);
JP_TEXTURE_API void jp_texture_forget_field(Uint32 *field);
JP_TEXTURE_API int jp_texture_can_enter(Uint32 *field,const NJS_TEXINFO *info);
JP_TEXTURE_API int jp_texture_free_slot_available(void);
JP_TEXTURE_API int jp_texture_retire_record(NJS_TEXMEMLIST *record);
JP_TEXTURE_API intptr_t jp_texture_record_index(const NJS_TEXMEMLIST *record);
JP_TEXTURE_API int jp_textures_accept_sdk_init(NJS_TEXMEMLIST *records,Uint32 count);
JP_TEXTURE_API int jp_textures_validate_list(const NJS_TEXLIST *list);
JP_TEXTURE_API const NJS_TEXMEMLIST *jp_renderer_texture_lookup(const NJS_TEXLIST *list,Uint32 index);
/* Copies the exact caller-supplied payload; no decoding or GPU upload claimed.
 * Fill dimensions, format, flags and TextureSize first. No retail data needed. */
JP_TEXTURE_API int jp_texture_copy_surface(NJS_TEXINFO *info,const void *bytes,size_t count);
JP_TEXTURE_API uint32_t jp_texture_surface_word(const NJS_TEXSURFACE *surface);
JP_TEXTURE_API const void *jp_texture_surface_data(uint32_t word);
JP_TEXTURE_API size_t jp_texture_surface_bytes(uint32_t word);
JP_TEXTURE_API int jp_texture_surface_owned(const NJS_TEXSURFACE *surface);
#endif
