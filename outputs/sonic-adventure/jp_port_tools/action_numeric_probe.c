/* Float-ABI observer for real action math. Raw words avoid host/Python NaN
 * conversions; every result comes from the actual Frame provider function. */
#define JP_ACTION_NUMERIC_API __declspec(dllimport)
#include "jp_native_action_numeric.h"
#include <stdint.h>
#include <string.h>
__declspec(dllexport) int jp_action_numeric_probe(unsigned operation,
    const uint32_t *input,uint32_t *out)
{
    if(operation>1||!input||!out)return 1;
    float values[4],result;memcpy(values,input,operation?4:16);
    if(operation)result=jp_action_fsrra(values[0]);
    else result=jp_action_fipr_self(values[0],values[1],values[2],values[3]);
    memcpy(out,&result,4);return 0;
}
