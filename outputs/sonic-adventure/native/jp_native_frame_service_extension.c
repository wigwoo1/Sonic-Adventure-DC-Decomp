/* Appended to the frozen authored wait core; shares its single owner. */
#include "jp_native_present.h"
int jp_wait_service(uint32_t milliseconds)
{
    int rc=access_owner();if(rc)return rc;if(milliseconds>50)return JP_WAIT_ARGUMENT;if(busy)return JP_WAIT_REENTRANT;
    busy=1;uint32_t done;rc=pump(&done);
    if(!rc && milliseconds){
        uint64_t now;if(sample(&now))rc=terminal;
        else if(now<state.next_deadline){
            uint64_t delta=state.next_deadline-now;
            uint32_t remaining=delta>=UINT64_C(39062)?50:(uint32_t)((delta*1000+781249)/781250);
            if(remaining>milliseconds)remaining=milliseconds;
            rc=idle(remaining);
        }
        if(!rc)rc=pump(&done);
    }
    busy=0;return rc;
}
