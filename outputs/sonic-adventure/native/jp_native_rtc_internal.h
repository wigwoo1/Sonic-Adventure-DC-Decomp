#ifndef JP_NATIVE_RTC_INTERNAL_H
#define JP_NATIVE_RTC_INTERNAL_H
#include "jp_native_rtc.h"
extern volatile unsigned long sygRtcCount;
extern volatile long sygRtcStat;
void jp_rtc_source_server(void);
long jp_rtc_source_date(struct jp_rtc_date *);
void jp_rtc_source_mode(long);
long jp_rtc_source_stat(void);
long jp_rtc_source_compare(const struct jp_rtc_date *,const struct jp_rtc_date *);
void jp_rtc_source_to_count(const struct jp_rtc_date *,unsigned long *);
void jp_rtc_source_to_date(unsigned long,struct jp_rtc_date *);
unsigned long jp_rtc_source_check(unsigned long);
unsigned char jp_rtc_source_week(unsigned short,unsigned char,unsigned char);
unsigned char jp_rtc_source_moon(struct jp_rtc_date *);
long jp_rtc_host_read(unsigned long *);
unsigned long jp_rtc_bounded_force(void);
#endif
