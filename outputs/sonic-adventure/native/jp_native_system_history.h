#ifndef JP_NATIVE_SYSTEM_HISTORY_H
#define JP_NATIVE_SYSTEM_HISTORY_H
#include "jp_native_history_exit.h"
#ifndef JP_SYSTEM_HISTORY_API
#define JP_SYSTEM_HISTORY_API __declspec(dllexport)
#endif
/* The existing System registration acquires actual History cleanup authority,
 * including resources retained after a failed constructor. Start rejects a
 * foreign preexisting owner. Explicit adopt requires the existing main guard.
 * It neither creates another registration nor closes borrowed VMU/LCD owners. */
JP_SYSTEM_HISTORY_API int jp_system_start_history(const struct jp_history_options *);
JP_SYSTEM_HISTORY_API int jp_system_adopt_history(void);
#endif
