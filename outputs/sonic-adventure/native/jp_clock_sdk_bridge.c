/* Real wall count service, independent of timer 1's rendering exclusions. */
#include <ninja.h>
#include <shinobi.h>
#include "jp_native_clock.h"
_Static_assert(sizeof(Uint32)==4,"SDK count is a wrapping 32-bit value");
Uint32 syTmrGetCount(void)
{
    return (Uint32)jp_clock_wall_counts();
}
