#include "jp_native_clock.h"
#include <stddef.h>
#include <limits.h>
#define WALL_RATE UINT64_C(781250)
#define TIMER1_RATE UINT64_C(12500000)
static jp_clock_source provider;
static void *provider_context;
static uint64_t frequency,last_tick,wall,wall_fraction,timer1,timer1_fraction;
static uint32_t reload=12500000;
static uint8_t tstr;
static int status=JP_CLOCK_NOT_INITIALIZED;
#if defined(_WIN32)
__declspec(dllimport) int __stdcall QueryPerformanceCounter(int64_t *);
__declspec(dllimport) int __stdcall QueryPerformanceFrequency(int64_t *);
static int qpc(void *context,uint64_t *tick)
{
    (void)context;int64_t value;
    if(!QueryPerformanceCounter(&value) || value<0)return 0;
    *tick=(uint64_t)value;return 1;
}
#endif
static int fail(int code) {status=code;return code;}
/* Preserve fractional ticks across observations. Restricting provider frequency
 * bounds the product; quotient/remainder scaling needs no floating point. */
static int advance(uint64_t delta,uint64_t rate,uint64_t *value,uint64_t *fraction)
{
    uint64_t whole=delta/frequency,part=(delta%frequency)*rate+*fraction;
    if(whole>UINT64_MAX/rate)return 0;
    whole*=rate;
    uint64_t extra=part/frequency;
    if(whole>UINT64_MAX-extra || *value>UINT64_MAX-(whole+extra))return 0;
    *value+=whole+extra;*fraction=part%frequency;return 1;
}
static int sample(void)
{
    if(status!=JP_CLOCK_OK)return 0;
    uint64_t now;
    if(!provider(provider_context,&now)) {fail(JP_CLOCK_SOURCE_FAILURE);return 0;}
    if(now<last_tick) {fail(JP_CLOCK_BACKWARDS);return 0;}
    uint64_t w=wall,wf=wall_fraction,t=timer1,tf=timer1_fraction,delta=now-last_tick;
    if(!advance(delta,WALL_RATE,&w,&wf) || ((tstr&2) && !advance(delta,TIMER1_RATE,&t,&tf))) {
        fail(JP_CLOCK_OVERFLOW);return 0;
    }
    last_tick=now;wall=w;wall_fraction=wf;timer1=t;timer1_fraction=tf;return 1;
}
int jp_clock_initialize(jp_clock_source source,void *context,uint64_t hz)
{
    if(!source) {
        if(context || hz)return fail(JP_CLOCK_ARGUMENT);
#if defined(_WIN32)
        int64_t value;
        if(!QueryPerformanceFrequency(&value) || value<=0)return fail(JP_CLOCK_SOURCE_FAILURE);
        source=qpc;hz=(uint64_t)value;
#else
        return fail(JP_CLOCK_ARGUMENT);
#endif
    }
    if(!hz || hz>UINT64_MAX/(TIMER1_RATE+1))return fail(JP_CLOCK_ARGUMENT);
    uint64_t now;
    if(!source(context,&now))return fail(JP_CLOCK_SOURCE_FAILURE);
    provider=source;provider_context=context;frequency=hz;last_tick=now;
    wall=wall_fraction=timer1=timer1_fraction=0;reload=12500000;tstr=0;status=JP_CLOCK_OK;
    return status;
}
void jp_clock_shutdown(void)
{
    provider=NULL;provider_context=NULL;frequency=last_tick=0;
    wall=wall_fraction=timer1=timer1_fraction=0;tstr=0;reload=12500000;status=JP_CLOCK_NOT_INITIALIZED;
}
int jp_clock_status(void) {return status;}
uint64_t jp_clock_frequency(void) {return frequency;}
uint64_t jp_clock_wall_counts(void) {sample();return wall;}
uint64_t jp_clock_timer1_ticks(void) {sample();return timer1;}
uint8_t jp_clock_read_tstr(void) {return tstr;}
void jp_clock_write_tstr(uint8_t value) {if(sample())tstr=value;}
uint32_t jp_clock_read_tcnt1(void)
{
    sample();return reload-(uint32_t)(timer1%((uint64_t)reload+1));
}
int jp_clock_reset_timer1(uint32_t value)
{
    if(!sample())return status;
    reload=value;timer1=timer1_fraction=0;return status;
}
