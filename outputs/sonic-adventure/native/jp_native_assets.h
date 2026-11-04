/* Native file/PRS -> actual PVM texture ownership. One game thread. */
#ifndef JP_NATIVE_ASSETS_H
#define JP_NATIVE_ASSETS_H
#include "jp_native_pvm.h"
enum jp_asset_error { JP_ASSET_OK,JP_ASSET_ARGUMENT,JP_ASSET_STATE,JP_ASSET_THREAD,
    JP_ASSET_OPEN,JP_ASSET_READ,JP_ASSET_LIMIT,JP_ASSET_PRS,JP_ASSET_PVM,
    JP_ASSET_TEXTURE,JP_ASSET_ALLOCATION };
struct jp_asset_limits { size_t file_bytes,decoded_bytes,buffer_bytes;uint32_t resident_names; };
struct jp_asset_info {
    uint64_t files_read,decodes,pvm_loads,transfers,bytes_read,bytes_decoded;
    size_t owned_bytes,decoded_bytes;
    uint32_t status,os_error,prs_error,resident_refs,pending_refs;
};
/* Moves ownership of field bindings and attrs, preserving destination filename
 * pointers and actual record counts. All validation precedes any field writes. */
JP_TEXTURE_API int jp_texture_validate_move(NJS_TEXNAME *from,NJS_TEXNAME *to,uint32_t count);
JP_TEXTURE_API int jp_texture_move_names(NJS_TEXNAME *from,NJS_TEXNAME *to,uint32_t count);
JP_TEXTURE_API int jp_assets_initialize(const wchar_t *root,const struct jp_asset_limits *limits);
JP_TEXTURE_API int jp_assets_shutdown(void);
JP_TEXTURE_API int jp_assets_status(void);
JP_TEXTURE_API void jp_assets_clear_error(void);
JP_TEXTURE_API size_t jp_assets_info_size(void);
JP_TEXTURE_API int jp_assets_snapshot(struct jp_asset_info *out);
/* One owned decoded view may be live. Release it before reading another file. */
JP_TEXTURE_API int jp_assets_read_prs(const char *relative,const void **out,size_t *bytes);
JP_TEXTURE_API int jp_assets_release_data(void);
/* SDK 1/-1; partial PVM references are tracked for explicit recovery. */
JP_TEXTURE_API int jp_assets_load_prs(const char *relative,NJS_TEXLIST *out,uint32_t capacity,size_t filename_bytes);
JP_TEXTURE_API int jp_assets_prepare_game(NJS_TEXLIST *out,NJS_TEXNAME *scratch,uint32_t capacity);
JP_TEXTURE_API int jp_assets_commit_game(NJS_TEXLIST *scratch,NJS_TEXLIST *out);
JP_TEXTURE_API int jp_assets_load_game_file(const char *relative,NJS_TEXLIST *out);
JP_TEXTURE_API int jp_assets_make_prs_path(const char *name,char *out,size_t bytes);
JP_TEXTURE_API int jp_assets_recover_pending(void);
JP_TEXTURE_API int jp_assets_release_residents(void);
#endif
