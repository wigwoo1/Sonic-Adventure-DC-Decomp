/* Replace standalone_texture_owner.obj, retaining its single actual owner. */
#include "jp_native_standalone_texture.c"
#include "jp_native_texture_budget.h"
int jp_texture_budget_query(struct jp_texture_budget *out)
{
    if(!ready())return status;
    if(!out)return fail(JP_TEXTURE_ARGUMENT);
    if(standalone.bytes>byte_budget || allocated>byte_budget-standalone.bytes)
        return fail(JP_TEXTURE_STATE);
    struct jp_texture_budget value={byte_budget,allocated+standalone.bytes,
                                   byte_budget-allocated-standalone.bytes};
    *out=value;return JP_TEXTURE_OK;
}
