/* One game-thread logical vertical clock, independent of window refresh. */
#ifndef JP_NATIVE_FRAME_WAIT_H
#define JP_NATIVE_FRAME_WAIT_H
#include <stdint.h>
#include "jp_native_clock.h"
enum jp_wait_error {JP_WAIT_OK,JP_WAIT_ARGUMENT,JP_WAIT_NOT_INITIALIZED,
    JP_WAIT_CLOCK,JP_WAIT_REENTRANT,JP_WAIT_CLOSED,JP_WAIT_OVERFLOW,
    JP_WAIT_HOST,JP_WAIT_NO_CALLBACK};
typedef int (*jp_wait_idle)(void *,uint32_t milliseconds);
struct jp_wait_snapshot {
    uint64_t ticks,callbacks,idle_calls,last_wall,next_deadline;
    uint32_t rate_numerator,rate_denominator,max_catchup,status;
};
/* Initialize the shared native clock first. 60000/1001 is the NTSC logical
 * tick rate. It is not the display FPS setting. idle is required, returns
 * positive=healthy, 0=closed, negative=host failure, and may wait at most ms.
 * Caller owns context/callback lifetimes. max_catchup (1..4096) bounds work per
 * pump, retaining overdue ticks rather than silently dropping them.
 * One creating thread owns all calls. No worker timer/IRQ runs game callbacks.
 */
JP_CLOCK_API int jp_wait_initialize(uint32_t numerator,uint32_t denominator,
    uint32_t max_catchup,jp_wait_idle idle,void *context);
JP_CLOCK_API int jp_wait_shutdown(void);
JP_CLOCK_API int jp_wait_status(void);
JP_CLOCK_API int jp_wait_snapshot(struct jp_wait_snapshot *);
/* Installing/removing a handler anchors a fresh phase at current wall time,
 * avoiding retroactive callbacks after a load/pacing reset. Each next deadline
 * uses integer rational accumulation, rounded up to a wall count.
 * Existing terminal clock/host errors require reinitialization.
 */
JP_CLOCK_API int jp_wait_set_vsync(void (*callback)(void));
void njSetVSyncFunction(void (*callback)(void));
/* Pump outputs the number of processed ticks, including a partial batch on
 * terminal callback/host error. Bad argument/thread/reentrant calls preserve it.
 * A callback may inspect status/snapshot, but cannot reenter or mutate service.
 */
JP_CLOCK_API int jp_wait_pump(uint32_t *processed);
/* Drain overdue ticks before sampling the original frame's late counter.
 * No timed sleep; each bounded batch pumps host messages. Very long stalls
 * retain their callback work. Pacing reset explicitly reanchors the phase.
 */
JP_CLOCK_API int jp_wait_sync(void);
JP_CLOCK_API int jp_wait_until_le(const volatile int *counter,int threshold);
JP_CLOCK_API int jp_wait_while_equal(const volatile int *counter,int value);
/* Real Windows message wait/pump with the verified native window as context.
 * Original frame/present adapters return on terminal failure. The application
 * must check jp_wait_status and stop the game iteration after such an error.
 */
JP_CLOCK_API int jp_wait_window_idle(void *window,uint32_t milliseconds);
#endif
