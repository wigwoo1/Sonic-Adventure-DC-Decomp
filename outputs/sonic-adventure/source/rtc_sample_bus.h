#ifndef SA_RTC_SAMPLE_BUS_H
#define SA_RTC_SAMPLE_BUS_H
#include "driver_signal_bus.h"
struct sa_rtc_sample_memory {
 struct sa_driver_signal_memory driver;
 struct sa_vertex_bus driver_bus;
 const uint32_t *rtc_samples;
 uint32_t rtc_count,rtc_cursor,rtc_configured;
};
SA_STAGE_EXPORT void sa_rtc_sample_bus_init(struct sa_rtc_sample_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t,uint32_t);
SA_STAGE_EXPORT int sa_rtc_set_samples(struct sa_rtc_sample_memory *,const uint32_t *,uint32_t);
#endif
