/* Preserve the qualified single wait/chain owner and its previous source.
 * The new arming boundary identifies the actual handler, rather than treating
 * any subsequent occupant of its recycled numeric slot as a live njBlank.
 */
#include "jp_native_vblank_chain.h"
#define jp_vblank_arm_ninja_blank jp_vblank_arm_ninja_blank_previous
#include "jp_native_vblank_chain.c"
#undef jp_vblank_arm_ninja_blank
int jp_vblank_arm_ninja_blank(void)
{
    int code=chain_access();if(code)return code;
    struct jp_chain_record previous;
    if(jp_chain_original_describe(jp_ninja_blank_handle(),&previous) &&
       previous.callback==(uintptr_t)njBlank)return chain_fail(JP_CHAIN_ARGUMENT);
    uint32_t handle=syChainAddHandler(0x900,njBlank,88,NULL);
    if(!handle)return chain_status;
    jp_ninja_blank_bind(handle);return JP_CHAIN_OK;
}
