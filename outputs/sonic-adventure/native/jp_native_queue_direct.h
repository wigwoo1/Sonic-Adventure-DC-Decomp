/* Direct SDK header transport. Compile the direct queue IN PLACE OF the
 * previous frame queue. The input packet is valid through this atomic call
 * and must not alias queue storage (which may grow). One game thread. */
#ifndef JP_NATIVE_QUEUE_DIRECT_H
#define JP_NATIVE_QUEUE_DIRECT_H
#include "jp_native_queue.h"
JP_QUEUE_API int jp_queue_append_direct_translucent(const uint32_t header[4],
                                                   const void *vertices,
                                                   uint32_t blocks);
#endif
