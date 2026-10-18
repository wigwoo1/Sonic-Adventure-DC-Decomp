#include "mask_input.h"

/* Retail DAT_8c6607ec is 0x0fffffff in Rev A. */
SA_STAGE_EXPORT uint32_t sa_mask_input_8c6607be(uint32_t value)
{
    return value & 0x0fffffffu;
}
