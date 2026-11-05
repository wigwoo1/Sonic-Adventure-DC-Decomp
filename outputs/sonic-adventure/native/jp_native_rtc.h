/* Authored host boundary for the pinned external Shinobi RTC algorithms. */
#ifndef JP_NATIVE_RTC_H
#define JP_NATIVE_RTC_H
#include <stdint.h>
#ifndef JP_RTC_API
#define JP_RTC_API __declspec(dllexport)
#endif
struct jp_rtc_date {
    uint16_t year;
    uint8_t month,day,hour,minute,second,dayofweek,ageofmoon;
};
/* Counts are unsigned 32-bit seconds since local 1950-01-01. Return zero on
 * success. Provider/context must survive until shutdown; NULL/NULL selects
 * Windows local civil time. This clock is separate from paused game timers. */
typedef int (*jp_rtc_reader)(void *,uint32_t *);
enum jp_rtc_error {JP_RTC_OK,JP_RTC_ARGUMENT,JP_RTC_NOT_INITIALIZED,
    JP_RTC_THREAD,JP_RTC_REENTRANT,JP_RTC_CHAIN,JP_RTC_HOST,
    JP_RTC_UNSTABLE,JP_RTC_RANGE,JP_RTC_UNSUPPORTED,JP_RTC_STATE};
struct jp_rtc_info {
    uint64_t reads,ticks,updates,failures;
    int64_t offset;
    uint32_t count,mode,chain,initialized,status,read_error;
};
/* One game-thread owner; initialize the real wait/vblank service first.
 * RTC registers a typed callback at original priority 92. Remove it BEFORE
 * closing/resetting the wait owner. Reinitialization cannot orphan a handler.
 * Calendar helpers also accept calls before initialization. Invalid dates or
 * overlapping input/output buffers leave outputs unchanged. The count range
 * ends during 2086; later/earlier dates fail rather than wrap.
 * syRtcSetDate sets a session-local virtual clock offset, never the OS clock.
 * Shutdown discards that offset; persistence of a user clock setting is not
 * implemented. Unsupported register-address logging fails explicitly.
 * An exit claim delegates terminal shutdown; ordinary shutdown/configure/init
 * then reject. Cancel is allowed until the first final-close attempt. Tokens
 * never recycle; final-close retries use the same creator-thread token.
 * Current integration is a component, not the complete sbInitSystem.
 */
JP_RTC_API int jp_rtc_configure(jp_rtc_reader,void *);
JP_RTC_API int jp_rtc_shutdown(void);
JP_RTC_API int jp_rtc_status(void);
JP_RTC_API int jp_rtc_snapshot(struct jp_rtc_info *);
JP_RTC_API int jp_rtc_exit_claim(uint64_t *);
JP_RTC_API int jp_rtc_exit_cancel(uint64_t);
JP_RTC_API int jp_rtc_exit_finish(uint64_t);
JP_RTC_API long syRtcInit(void);
JP_RTC_API void syRtcFinish(void);
JP_RTC_API void syRtcExecServer(void);
JP_RTC_API long syRtcGetDate(struct jp_rtc_date *);
JP_RTC_API long syRtcSetDate(const struct jp_rtc_date *);
JP_RTC_API void syRtcSetServerMode(long);
JP_RTC_API long syRtcGetStat(void);
JP_RTC_API long syRtcCompareDate(const struct jp_rtc_date *,const struct jp_rtc_date *);
JP_RTC_API void syRtcDateToCount(const struct jp_rtc_date *,unsigned long *);
JP_RTC_API void syRtcCountToDate(unsigned long,struct jp_rtc_date *);
#endif
