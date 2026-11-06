#ifndef JP_NATIVE_SYSTEM_READ_QUEUE_H
#define JP_NATIVE_SYSTEM_READ_QUEUE_H
#include "jp_native_read_queue_exit.h"
#ifndef JP_SYSTEM_READ_QUEUE_API
#define JP_SYSTEM_READ_QUEUE_API __declspec(dllexport)
#endif
/* Reuse the existing System registration and real constructor guard. Start
 * rejects an existing queue owner. Explicit guarded adopt accepts only an
 * initialized, unclaimed queue and never replaces another claimant. */
JP_SYSTEM_READ_QUEUE_API int jp_system_start_read_queue(void);
JP_SYSTEM_READ_QUEUE_API int jp_system_adopt_read_queue(void);
#endif
