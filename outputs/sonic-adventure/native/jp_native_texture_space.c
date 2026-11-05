/* Native Kamui allocation policy: independent host allocations share one
 * budget, so the largest permitted request equals the remaining allowance.
 * This is neither console VRAM fragmentation nor an OS allocation guarantee.
 * Compile instead of the texture-budget/standalone owner, never beside it. */
#include "jp_native_texture_budget.c"
KMSTATUS kmGetFreeTextureMem(PKMUINT32 total,
                                                PKMUINT32 largest)
{
    if(!total || !largest)return KMSTATUS_INVALID_PARAMETER;
    struct jp_texture_budget budget;
    if(jp_texture_budget_query(&budget)){
        /* Original njCalcTexture ignores the status. Define safe outputs. */
        *total=*largest=0;return KMSTATUS_NOT_ENOUGH_MEMORY;
    }
    /* The original SDK API has 32-bit byte counts even on a 64-bit host. */
    KMUINT32 allowance=budget.available>0xffffffffu?0xffffffffu:
                        (KMUINT32)budget.available;
    *total=*largest=allowance;return KMSTATUS_SUCCESS;
}
