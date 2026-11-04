/* Native transport for real SDK-generated Ninja context headers. */
#ifndef JP_NATIVE_CONTEXT_BRIDGE_H
#define JP_NATIVE_CONTEXT_BRIDGE_H
#include "jp_native_queue.h"
#undef NULL
#include <kamui.h>
#include <ninja.h>
#define JP_CONTEXT_API __declspec(dllexport)
JP_CONTEXT_API int jp_ninja_contexts_initialize(void);
/* Pure context generation may use caller-owned readable/writable contexts.
 * Updating an actual queue context requires an idle native queue. */
JP_CONTEXT_API int jp_context_update_guard(KMVERTEXCONTEXT *context);
#endif
