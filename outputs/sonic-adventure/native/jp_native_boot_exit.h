#ifndef JP_NATIVE_BOOT_EXIT_H
#define JP_NATIVE_BOOT_EXIT_H
#include "jp_native_boot_lifetimes.h"
#ifndef JP_SYSTEM_BOOT_API
#define JP_SYSTEM_BOOT_API __declspec(dllexport)
#endif
#ifndef JP_EXIT_BOOT_API
#define JP_EXIT_BOOT_API __declspec(dllexport)
#endif
typedef int (*jp_boot_cleanup_identity)(void *);
JP_SYSTEM_BOOT_API int jp_system_main_initialization_guard(uint32_t);
JP_SYSTEM_BOOT_API int jp_system_adopt_boot_lifetimes(uint32_t,uint32_t);
JP_EXIT_BOOT_API int jp_exit_attach_boot_lifetimes(jp_boot_cleanup_identity,void *,uint64_t,uint64_t,uint32_t);
#endif
