#ifndef JP_NATIVE_EXIT_HISTORY_H
#define JP_NATIVE_EXIT_HISTORY_H
#include "jp_native_history_exit.h"
#ifndef JP_EXIT_HISTORY_API
#define JP_EXIT_HISTORY_API __declspec(dllexport)
#endif
typedef int (*jp_history_boot_cleanup)(void *);
JP_EXIT_HISTORY_API int jp_exit_attach_bootstrap_history(jp_history_boot_cleanup,void *,uint64_t);
#endif
