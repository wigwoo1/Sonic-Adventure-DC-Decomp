#ifndef JP_NATIVE_SOUND_DISPATCH_H
#define JP_NATIVE_SOUND_DISPATCH_H
#include <stdint.h>
#ifndef JP_SOUND_DISPATCH_API
#define JP_SOUND_DISPATCH_API __declspec(dllexport)
#endif
enum jp_sound_dispatch_error { JP_SOUND_DISPATCH_OK,JP_SOUND_DISPATCH_ARGUMENT,
    JP_SOUND_DISPATCH_UNBOUND,JP_SOUND_DISPATCH_THREAD,JP_SOUND_DISPATCH_BOUND,
    JP_SOUND_DISPATCH_REENTRANT,JP_SOUND_DISPATCH_CALLBACK,JP_SOUND_DISPATCH_UNAVAILABLE };
typedef int32_t (*jp_sound_dispatch_callback)(void *);
struct jp_sound_dispatch_info {
    uint64_t token,service_calls,queue_calls,failures;
    uint32_t bound,status,creator_thread,in_callback,active_kind,has_queue;
    int32_t native_result;uint32_t reserved;
};
/* One binding on the creator thread of the real game/SDK sound owner. The
 * dispatcher owns no emulator or SDK state. Borrow callbacks/context until a
 * successful unbind, before retiring those owners or unloading their module.
 * Tokens never recycle. An optional missing queue callback reports UNAVAILABLE;
 * an absent service binding reports UNBOUND rather than successful SDK work.
 */
JP_SOUND_DISPATCH_API int jp_sound_dispatch_bind(jp_sound_dispatch_callback service,
    jp_sound_dispatch_callback queue,void *context,uint64_t *token);
JP_SOUND_DISPATCH_API int jp_sound_dispatch_unbind(uint64_t token);
/* Callback result is separate from dispatch status and is copied only after
 * actual callback execution. Nested dispatch/unbind cannot retire a live call.
 * Snapshots remain available to that same callback and creator thread.
 */
JP_SOUND_DISPATCH_API int jp_sound_dispatch_service(int32_t *result);
JP_SOUND_DISPATCH_API int jp_sound_dispatch_queue(int32_t *result);
JP_SOUND_DISPATCH_API int jp_sound_dispatch_snapshot(struct jp_sound_dispatch_info *);
#endif
