/* Keep the qualified single wait/chain owner and healthy deletion semantics.
 * A terminal wait stops dispatch, not ownership: real GDFS/RTC callbacks must
 * still be identifiable and removable before their context/storage is freed.
 * Never reset the terminal clock or permit new registrations to recover it.
 */
#define syChainDeleteHandler jp_chain_delete_healthy
#define jp_vblank_chain_describe jp_chain_describe_healthy
#include "jp_native_vblank_cancel.c"
#undef syChainDeleteHandler
#undef jp_vblank_chain_describe
static int retirement_access(void)
{
    if(!initialized)return chain_fail(JP_CHAIN_NOT_INITIALIZED);
    if(owner!=GetCurrentThreadId())return JP_CHAIN_THREAD;
    if(busy)return chain_fail(JP_CHAIN_REENTRANT);
    return JP_CHAIN_OK;
}
JP_CLOCK_API void syChainDeleteHandler(unsigned long handle)
{
    if(!terminal){jp_chain_delete_healthy(handle);return;}
    if(retirement_access())return;
    if(!jp_chain_original_live(handle)){chain_fail(JP_CHAIN_ARGUMENT);return;}
    jp_chain_original_delete(handle);
    if(handle==game_handle){game_handle=0;game_callback=NULL;}
    chain_status=JP_CHAIN_OK;
}
JP_CLOCK_API int jp_vblank_chain_describe(uint32_t handle,struct jp_chain_record *out)
{
    if(!terminal)return jp_chain_describe_healthy(handle,out);
    int rc=retirement_access();if(rc)return rc;
    if(!out||!jp_chain_original_live(handle))return chain_fail(JP_CHAIN_ARGUMENT);
    return jp_chain_original_describe(handle,out)?JP_CHAIN_OK:chain_fail(JP_CHAIN_ARGUMENT);
}
