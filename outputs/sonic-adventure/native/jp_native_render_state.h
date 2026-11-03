/* Authored ownership for the selected GCC model renderer's scratch and state. */
#ifndef JP_NATIVE_RENDER_STATE_H
#define JP_NATIVE_RENDER_STATE_H
#include <stddef.h>
#include <stdint.h>
#pragma push_macro("NULL")
#undef NULL
#include <ninja.h>
#pragma pop_macro("NULL")
#define JP_RENDER_API __declspec(dllexport)
enum jp_render_error { JP_RENDER_OK,JP_RENDER_ARGUMENT,JP_RENDER_BUDGET,
    JP_RENDER_ALLOCATION,JP_RENDER_STATE,JP_RENDER_GUARD,JP_RENDER_NOT_INITIALIZED };
/* The pinned GCC renderer has a 16-byte projected point and at most 2048. */
JP_RENDER_API int jp_render_state_initialize(uint32_t vertices,size_t byte_budget);
JP_RENDER_API void jp_render_state_shutdown(void);
JP_RENDER_API int jp_render_state_configure(uint32_t width,uint32_t height,
    float distance,float near_distance,float far_distance,float aspect_x,float aspect_y);
JP_RENDER_API int jp_render_state_status(void);
JP_RENDER_API void jp_render_state_clear_error(void);
JP_RENDER_API int jp_render_state_check_guards(void);
JP_RENDER_API int jp_render_state_reset_scratch(void);
JP_RENDER_API void *jp_render_state_scratch(void);
JP_RENDER_API size_t jp_render_state_allocated_bytes(void);
JP_RENDER_API uint32_t jp_render_state_capacity(void);
JP_RENDER_API int jp_render_state_accept_buffer(NJS_VERTEX_BUF *,Int);
#endif
