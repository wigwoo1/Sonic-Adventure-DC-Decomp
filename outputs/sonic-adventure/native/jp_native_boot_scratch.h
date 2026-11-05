/* Authored caller-owned scratch support for the original boot buffer. */
#ifndef JP_NATIVE_BOOT_SCRATCH_H
#define JP_NATIVE_BOOT_SCRATCH_H
#include "jp_native_render_state.h"
/* Register before the genuine njInit3D call. The caller keeps this extent
 * alive until an owned reinitialization or shutdown. Registration does not
 * select, zero, resize, free or change the contents of that extent.
 * Original NJS_VERTEX_BUF records occupy 64 bytes; the selected GCC model
 * producer writes 16-byte projected points into the same allocation. Its
 * capacity query retains its separate 2048-point lighting-array limit.
 * A live borrowed binding can only be registered again with identical bounds.
 */
JP_RENDER_API int jp_render_state_register_buffer(void *buffer,size_t bytes,
    uint32_t original_vertices);
JP_RENDER_API int jp_render_state_buffer_is_borrowed(void);
JP_RENDER_API size_t jp_render_state_buffer_bytes(void);
#endif
