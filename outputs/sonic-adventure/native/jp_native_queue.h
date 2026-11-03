/* Authored host-owned transport for Ninja's 32-byte header/vertex blocks.
 * One game thread owns the context. Reserve the complete producer's bound
 * before calling SDK draw routines: raw C writers must obey that bound. */
#ifndef JP_NATIVE_QUEUE_H
#define JP_NATIVE_QUEUE_H
#include <stddef.h>
#include <stdint.h>
#define JP_QUEUE_API __declspec(dllexport)
enum jp_queue_list { JP_QUEUE_OPAQUE, JP_QUEUE_TRANSLUCENT, JP_QUEUE_PUNCH,
                     JP_QUEUE_OPAQUE_MODIFIER, JP_QUEUE_TRANSLUCENT_MODIFIER,
                     JP_QUEUE_LISTS };
enum jp_queue_error { JP_QUEUE_OK, JP_QUEUE_ARGUMENT, JP_QUEUE_STATE,
                      JP_QUEUE_BOUNDS, JP_QUEUE_BUDGET, JP_QUEUE_ALLOCATION,
                      JP_QUEUE_GUARD, JP_QUEUE_NOT_INITIALIZED };
JP_QUEUE_API int jp_queue_initialize(size_t byte_budget);
JP_QUEUE_API void jp_queue_shutdown(void);
JP_QUEUE_API int jp_queue_reset_frame(void);
JP_QUEUE_API int jp_queue_reserve(unsigned list, uint32_t additional_blocks);
JP_QUEUE_API int jp_queue_status(void);
JP_QUEUE_API void jp_queue_clear_error(void);
JP_QUEUE_API size_t jp_queue_allocated_bytes(void);
JP_QUEUE_API size_t jp_queue_committed_bytes(unsigned list);
/* Views expire on reserve/growth, reset, shutdown or subsequent writes. */
JP_QUEUE_API const void *jp_queue_data(unsigned list);
JP_QUEUE_API size_t jp_queue_active_capacity_blocks(void);
JP_QUEUE_API int jp_queue_set_context(unsigned index, const uint32_t words[4]);
JP_QUEUE_API int jp_queue_select_context(unsigned index);
JP_QUEUE_API int jp_queue_get_context(unsigned index, uint32_t words[4]);
JP_QUEUE_API int jp_queue_get_parameter(uint32_t words[4]);
JP_QUEUE_API unsigned jp_queue_context_count(void);
JP_QUEUE_API void *jp_queue_parameter_cursor(unsigned list);
JP_QUEUE_API void *jp_queue_descriptor_cursor(unsigned list);
JP_QUEUE_API uint64_t jp_queue_cache_hint_count(void);
JP_QUEUE_API const void *jp_queue_last_cache_hint(void);
#endif
