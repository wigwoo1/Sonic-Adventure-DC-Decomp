#ifndef JP_NATIVE_SYSTEM_ASSETS_H
#define JP_NATIVE_SYSTEM_ASSETS_H
#include "jp_native_system.h"
#include "jp_native_asset_exit.h"
#include "jp_native_character_exit.h"
struct jp_asset_limits;
/* Same System/main guard and bootstrap registration. Starting never steals
 * independent providers. Explicit adoption requires the caller's existing
 * initialization guard and may transfer live decoded views/character users.
 * Character ownership requires this System's actual asset capability first. */
JP_SYSTEM_API int jp_system_start_assets(const wchar_t *,const struct jp_asset_limits *);
JP_SYSTEM_API int jp_system_adopt_assets(void);
JP_SYSTEM_API int jp_system_start_character_textures(void);
JP_SYSTEM_API int jp_system_adopt_character_textures(void);
#endif
