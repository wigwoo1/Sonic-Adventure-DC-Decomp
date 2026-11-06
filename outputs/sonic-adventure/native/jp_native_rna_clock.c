/* SDK count samples the existing native781250Hz domain. No separate timer. */
#include <stdint.h>
#include <shinobi.h>
#define JP_G2_API __declspec(dllimport)
#include "jp_native_g2_audio.h"
extern __declspec(dllimport) uint64_t jp_clock_wall_counts(void);
extern __declspec(dllimport) int jp_clock_status(void);
static uint32_t last_count;
static int clock_error;
/* Errors are sticky during a renderer lifetime. A frozen count is accompanied
 * by an explicit error and propagates through the native owner's service.
 * The raw SDK only uses this count for its diagnostic transfer timestamps. */
int jp_rna_clock_begin(void)
{
    int gate=jp_g2_gate_get();
    if(gate<0){clock_error=0x100-gate;return clock_error;}
    uint64_t count=jp_clock_wall_counts();int error=jp_clock_status();
    if(error){clock_error=error;return error;}
    last_count=(uint32_t)count;clock_error=0;return 0;
}
int jp_rna_clock_error(void){return clock_error;}
Uint32 syTmrGetCount(void)
{
    if(clock_error)return last_count;
    int gate=jp_g2_gate_get();
    if(gate<0){clock_error=0x100-gate;return last_count;}
    uint64_t count=jp_clock_wall_counts();int error=jp_clock_status();
    if(error)clock_error=error;
    else last_count=(uint32_t)count;
    return last_count;
}
