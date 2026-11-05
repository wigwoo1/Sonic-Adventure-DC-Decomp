#ifndef JP_NATIVE_NINJA_GLOBAL_H
#define JP_NATIVE_NINJA_GLOBAL_H
#include <stdint.h>
enum jp_ninja_global_error { JP_NINJA_GLOBAL_OK,JP_NINJA_GLOBAL_NOT_READY,
    JP_NINJA_GLOBAL_ACTIVE,JP_NINJA_GLOBAL_PROVIDER,JP_NINJA_GLOBAL_ARGUMENT };
struct jp_ninja_global_info {uint32_t light_slots,material_bytes,control_bytes,
    queue_bytes,source_bytes,destination_bytes,pointer_bytes,status;};
/* One game thread. Requires ready video/queue/frame owners and no queued or
 * submitted work. The caller keeps native vsync callbacks quiescent during
 * startup. Logical dimensions come from the actual native video owner. */
__declspec(dllexport) int jp_ninja_global_initialize(void);
__declspec(dllexport) int jp_ninja_global_status(void);
__declspec(dllexport) int jp_ninja_global_snapshot(struct jp_ninja_global_info *out);
#endif
