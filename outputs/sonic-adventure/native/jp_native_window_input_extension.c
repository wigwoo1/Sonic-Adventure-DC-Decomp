/* Appended inside the actual GPU/window owner's translation unit. No copied
 * window layout or second device/window owner. */
#include "jp_native_window_input.h"
void *jp_window_native_handle(struct jp_window *window)
{return valid_window(window)?(void *)window->handle:NULL;}
