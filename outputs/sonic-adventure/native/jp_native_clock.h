/* Platform clock provider for original wall-count and paused timer-1 domains.
 * One game thread owns this service. Default Windows source is real QPC. */
#ifndef JP_NATIVE_CLOCK_H
#define JP_NATIVE_CLOCK_H
#include <stdint.h>
#if defined(_WIN32)
#define JP_CLOCK_API __declspec(dllexport)
#else
#define JP_CLOCK_API __attribute__((visibility("default")))
#endif
typedef int (*jp_clock_source)(void *context,uint64_t *tick);
enum jp_clock_error { JP_CLOCK_OK,JP_CLOCK_ARGUMENT,JP_CLOCK_BACKWARDS,
    JP_CLOCK_OVERFLOW,JP_CLOCK_NOT_INITIALIZED,JP_CLOCK_SOURCE_FAILURE };
/* callback returns nonzero on success. NULL/NULL/0 selects Windows QPC.
 * Provider/context must outlive use. Errors require explicit reinitialization. */
JP_CLOCK_API int jp_clock_initialize(jp_clock_source source,void *context,uint64_t frequency);
JP_CLOCK_API void jp_clock_shutdown(void);
JP_CLOCK_API int jp_clock_status(void);
JP_CLOCK_API uint64_t jp_clock_wall_counts(void); /* 781250 counts/second */
JP_CLOCK_API uint64_t jp_clock_timer1_ticks(void); /* 12500000 active ticks/second */
JP_CLOCK_API uint8_t jp_clock_read_tstr(void);
JP_CLOCK_API void jp_clock_write_tstr(uint8_t value);
JP_CLOCK_API uint32_t jp_clock_read_tcnt1(void);
JP_CLOCK_API int jp_clock_reset_timer1(uint32_t reload);
JP_CLOCK_API uint64_t jp_clock_frequency(void);
#endif
