#ifndef JP_NATIVE_GDFS_READ_QUEUE_H
#define JP_NATIVE_GDFS_READ_QUEUE_H
#include "jp_native_gdfs.h"
/* One nonterminal client lease, held by the actual original loader queue.
 * A lease protects GDFS lifetime and only its stamped file handles. Independent
 * sound/file clients remain usable. Handle authority is suspended while a
 * user callback runs, including callbacks dispatched during a leased call. */
JP_GDFS_API int jp_gdfs_read_queue_claim(uint64_t *);
JP_GDFS_API int jp_gdfs_read_queue_release(uint64_t);
JP_GDFS_API int jp_gdfs_read_queue_enter(uint64_t);
JP_GDFS_API int jp_gdfs_read_queue_leave(uint64_t);
JP_GDFS_API int jp_gdfs_read_queue_close(uint64_t,void *);
#endif
