#ifndef JP_NATIVE_WINDOW_INPUT_H
#define JP_NATIVE_WINDOW_INPUT_H
#include "jp_native_window.h"
/* Borrow the actual HWND on the window's creating thread. NULL for NULL,
 * closed or closing windows, and foreign threads. Never cast jp_window* to
 * HWND. The caller must keep the window alive and stop polling before close;
 * calling this with an already freed jp_window is invalid. */
JP_PACKET_API void *jp_window_native_handle(struct jp_window *);
#endif
