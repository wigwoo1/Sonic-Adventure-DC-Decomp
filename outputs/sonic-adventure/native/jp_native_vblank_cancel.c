/* One wait/chain owner. Permit cleanup after terminal wait failure, while
 * keeping thread/reentry guards and preserving recycled unrelated slots. */
#include "jp_native_vblank_rearm.c"
JP_CLOCK_API int jp_vblank_cancel_ninja_blank(void)
{
    if(!initialized)return chain_fail(JP_CHAIN_NOT_INITIALIZED);
    if(owner!=GetCurrentThreadId())return JP_CHAIN_THREAD;
    if(busy)return chain_fail(JP_CHAIN_REENTRANT);
    uint32_t handle=jp_ninja_blank_handle();struct jp_chain_record record;
    if(jp_chain_original_describe(handle,&record) && record.callback==(uintptr_t)njBlank)
        jp_chain_original_delete(handle);
    jp_ninja_blank_bind(0);return JP_CHAIN_OK;
}
