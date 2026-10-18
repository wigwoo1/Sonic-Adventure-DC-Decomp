/* Original signed loads are significant even though valid stage values are small.
 * Shift as unsigned to reproduce SH-4 wrapping without signed-shift UB in C.
 */
#include "level_variant.h"
SA_LEVEL_EXPORT uint32_t sa_level_variant_8c04f7e0(const struct sa_level_variant_fields *fields) {
 return ((uint32_t)(int32_t)fields->first<<8)|(uint32_t)(int32_t)fields->second;
}
