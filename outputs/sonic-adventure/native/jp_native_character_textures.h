/* Same original character_users/lists, existing native texture/asset owners. */
#ifndef JP_NATIVE_CHARACTER_TEXTURES_H
#define JP_NATIVE_CHARACTER_TEXTURES_H
#include "jp_native_assets.h"
#ifndef JP_CHARACTER_TEXTURE_API
#define JP_CHARACTER_TEXTURE_API __declspec(dllexport)
#endif
enum jp_character_texture_error {JP_CHARACTER_TEXTURE_OK,JP_CHARACTER_TEXTURE_ARGUMENT,
    JP_CHARACTER_TEXTURE_STATE,JP_CHARACTER_TEXTURE_THREAD,JP_CHARACTER_TEXTURE_REENTRANT,
    JP_CHARACTER_TEXTURE_LIMIT,JP_CHARACTER_TEXTURE_ASSET,JP_CHARACTER_TEXTURE_NATIVE,
    JP_CHARACTER_TEXTURE_BUSY};
struct jp_character_texture_info {
    uint64_t starts,acquires,releases,recoveries;
    uint32_t phase,status,native_error,creator_thread;
    int32_t partial_character;uint32_t loaded_lists;
    int8_t users[8];
};
/* Borrow initialized actual Frames texture/video/memory and asset providers.
 * Serialized creating-thread calls only. Original users count logical character
 * references; they must outlive all tasks/draw submissions using those lists.
 * Direct SDK mutations of these bindings are forbidden while this owner is live.
 * Failed partial load retains real references for recover(). Clear the reported
 * asset/memory/texture error explicitly before recovery; no success is invented.
 * release_all explicitly retires ALL logical character users. Shutdown instead
 * requires all users/partial work retired. Neither closes borrowed providers.
 * List pointers expire with owner shutdown; unchecked original APIs are private. */
JP_CHARACTER_TEXTURE_API int jp_character_textures_initialize(void);
JP_CHARACTER_TEXTURE_API int jp_character_textures_acquire(uint32_t);
JP_CHARACTER_TEXTURE_API int jp_character_textures_release(uint32_t);
JP_CHARACTER_TEXTURE_API int jp_character_textures_release_all(void);
JP_CHARACTER_TEXTURE_API int jp_character_textures_recover(void);
JP_CHARACTER_TEXTURE_API int jp_character_textures_shutdown(void);
JP_CHARACTER_TEXTURE_API int jp_character_textures_snapshot(struct jp_character_texture_info *);
JP_CHARACTER_TEXTURE_API int jp_character_textures_list(uint32_t,uint32_t,NJS_TEXLIST **);
#endif
