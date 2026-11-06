#ifndef JP_NATIVE_EXIT_ASSETS_H
#define JP_NATIVE_EXIT_ASSETS_H
#include "jp_native_asset_exit.h"
#include "jp_native_character_exit.h"
#ifndef JP_EXIT_ASSETS_API
#define JP_EXIT_ASSETS_API __declspec(dllexport)
#endif
typedef int (*jp_assets_boot_cleanup)(void *);
JP_EXIT_ASSETS_API int jp_exit_attach_bootstrap_assets(jp_assets_boot_cleanup,void *,uint64_t);
JP_EXIT_ASSETS_API int jp_exit_attach_bootstrap_characters(jp_assets_boot_cleanup,void *,uint64_t);
#endif
