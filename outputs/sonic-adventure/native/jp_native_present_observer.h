/* Optional diagnostic observation. Genuine presentation functions always run. */
#ifndef JP_NATIVE_PRESENT_OBSERVER_H
#define JP_NATIVE_PRESENT_OBSERVER_H
#include <stdint.h>
typedef void (*jp_present_record_observer)(int32_t,const void *,int32_t,int32_t,float,uint32_t);
typedef void (*jp_present_untimed_observer)(int32_t);
__declspec(dllexport) void jp_present_set_observers(jp_present_record_observer record,jp_present_untimed_observer untimed);
void jp_present_observe_record(int32_t kind,const void *what,int32_t count,int32_t entry,float priority,uint32_t attributes);
void jp_present_observe_untimed(int32_t begin);
#endif
