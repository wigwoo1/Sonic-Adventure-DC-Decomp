#ifndef SA_MOVIE_CLEAR_PROBE_H
#define SA_MOVIE_CLEAR_PROBE_H
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
struct sa_clear_cpu {uint32_t r[16],sr,pr,pc;};
struct sa_clear_stats {
 uint64_t calls,writes,attempts,cpu_hash,write_hash,steps,coverage,fpu_errors,code_errors;
 uint32_t first[16],last[16],first_sr,last_sr,first_pr,last_pr;
};
/* Verification support only. The port continues executing static translated C. */
SA_STAGE_EXPORT int sa_clear_reference(uint8_t *,uint8_t *,struct sa_clear_cpu *,struct sa_clear_stats *,uint32_t);
#endif
