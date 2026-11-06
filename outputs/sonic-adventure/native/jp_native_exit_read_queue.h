#ifndef JP_NATIVE_EXIT_READ_QUEUE_H
#define JP_NATIVE_EXIT_READ_QUEUE_H
#include "jp_native_read_queue_exit.h"
#ifndef JP_EXIT_READ_QUEUE_API
#define JP_EXIT_READ_QUEUE_API __declspec(dllexport)
#endif
typedef int (*jp_read_queue_boot_cleanup)(void *);
JP_EXIT_READ_QUEUE_API int jp_exit_attach_bootstrap_read_queue(jp_read_queue_boot_cleanup,void *,uint64_t);
#endif
