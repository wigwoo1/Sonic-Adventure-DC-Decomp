/* Authored compatibility boundary for the pinned GCC model renderer. */
#ifndef JP_RENDERER_HOST_COMPAT_H
#define JP_RENDERER_HOST_COMPAT_H
/* The SDK enum bool must precede SH4ZAM's inclusion of stdbool.h. */
#include <ninja.h>
#include "jp_sh4zam_host_config.h"
#include <sh4zam/shz_vector.h>
#include "system/port.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#define ALWAYS_INLINE static inline __attribute__((always_inline))
static inline shz_vec3_t zam_load_vec3(const NJS_POINT3 *p)
{
    return shz_vec3_init(p->x, p->y, p->z);
}
static inline Float zam_fipr(shz_vec4_t a, shz_vec4_t b)
{
    /* Preserve the helper's SH-4 FIPR value contract instead of replacing it
     * with a differently rounded host dot product. */
    uint32_t x[4], y[4], result;
    const float av[4] = {a.x,a.y,a.z,a.w}, bv[4] = {b.x,b.y,b.z,b.w};
    __builtin_memcpy(x, av, sizeof x); __builtin_memcpy(y, bv, sizeof y);
    result = sh4_fipr(x, y, 0);
    float value; __builtin_memcpy(&value, &result, sizeof value);
    return value;
}
#endif
