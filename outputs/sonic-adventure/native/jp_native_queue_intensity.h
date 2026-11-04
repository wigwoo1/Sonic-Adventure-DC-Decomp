/* Extended queue version: same owner, additional real SDK header formats. */
#ifndef JP_NATIVE_QUEUE_INTENSITY_H
#define JP_NATIVE_QUEUE_INTENSITY_H
#include "jp_native_queue.h"
/* Discard only the active, uncommitted batch. Previous packets and SDK
 * context changes survive. Call njSetkmPointer afterwards, as a culled
 * sprite does. Existing views of the discarded reservation expire. */
JP_QUEUE_API int jp_queue_discard_active(void);
#endif
