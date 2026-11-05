#include "jp_native_present_observer.h"
static jp_present_record_observer record_observer;
static jp_present_untimed_observer untimed_observer;
void jp_present_set_observers(jp_present_record_observer record,jp_present_untimed_observer untimed)
{record_observer=record;untimed_observer=untimed;}
void jp_present_observe_record(int32_t kind,const void *what,int32_t count,int32_t entry,float priority,uint32_t attributes)
{if(record_observer)record_observer(kind,what,count,entry,priority,attributes);}
void jp_present_observe_untimed(int32_t begin)
{if(untimed_observer)untimed_observer(begin);}
