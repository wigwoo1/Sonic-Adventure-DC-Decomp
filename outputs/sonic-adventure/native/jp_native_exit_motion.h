#ifndef JP_NATIVE_EXIT_MOTION_H
#define JP_NATIVE_EXIT_MOTION_H
#include "jp_native_motion_exit.h"
#ifndef JP_EXIT_MOTION_API
#define JP_EXIT_MOTION_API __declspec(dllexport)
#endif
typedef int (*jp_motion_boot_cleanup)(void *);
JP_EXIT_MOTION_API int jp_exit_attach_bootstrap_motions(jp_motion_boot_cleanup,void *,uint64_t);
#endif
