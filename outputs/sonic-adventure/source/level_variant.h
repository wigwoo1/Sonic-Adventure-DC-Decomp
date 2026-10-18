#ifndef SA_LEVEL_VARIANT_H
#define SA_LEVEL_VARIANT_H
#include <stdint.h>
#ifdef _WIN32
#define SA_LEVEL_EXPORT __declspec(dllexport)
#else
#define SA_LEVEL_EXPORT
#endif
/* Native view of the two signed 16-bit retail fields at 8c7493ba/8c7493bc.
 * Semantic field names await recovery of their writers.
 */
struct sa_level_variant_fields { int16_t first,second; };
SA_LEVEL_EXPORT uint32_t sa_level_variant_8c04f7e0(const struct sa_level_variant_fields *fields);
#endif
