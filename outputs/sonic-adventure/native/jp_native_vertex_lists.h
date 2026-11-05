#ifndef JP_NATIVE_VERTEX_LISTS_H
#define JP_NATIVE_VERTEX_LISTS_H
#include "jp_native_queue_allocation.h"
struct jp_vertex_lists {
    uint32_t model; /* 0 normal, 1 original opaque bypass, 2 trans bypass */
    uint32_t words[5]; /* effective op, om, tp, tm, pt in Ninja order */
    size_t minimum_bytes[5]; /* actual host minima in jp_queue_list order */
};
/* Configure real host queues from Ninja's five word-count selectors. This
 * replaces console allocation, not the full SDK configuration lifecycle.
 * Requires an empty inactive queue. Output is unchanged on failure. Console
 * bypass lists remain growable host lists; buffers never contain TA aliases.
 * Future SDK integration must still perform the qualified Ninja blank wait,
 * commit its scalar globals and refresh the actual texture-space query. */
JP_QUEUE_API int jp_vertex_lists_configure(int32_t op,int32_t om,int32_t tp,
    int32_t tm,int32_t pt,struct jp_vertex_lists *out);
#endif
