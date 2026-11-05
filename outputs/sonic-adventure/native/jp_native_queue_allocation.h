/* Host allocation for the existing five queues, on their owning game thread. */
#ifndef JP_NATIVE_QUEUE_ALLOCATION_H
#define JP_NATIVE_QUEUE_ALLOCATION_H
#include "jp_native_queue.h"
/* Minimum byte capacities, in jp_queue_list order, each divisible by 32.
 * Requires an empty, inactive recording. Keeps larger pools and permits later
 * normal reserve/growth, including lists with a zero minimum. Counts guards
 * and all old/new allocations against the queue's transient memory budget.
 * Failure preserves all buffers, cursors, contexts and submitted frames;
 * clear the usual sticky queue error before retrying. No console aliases.
 */
JP_QUEUE_API int jp_queue_preallocate(const size_t minimum_bytes[JP_QUEUE_LISTS]);
/* Read-only query; invalid lists have zero capacity. */
JP_QUEUE_API size_t jp_queue_capacity_bytes(unsigned list);
#endif
