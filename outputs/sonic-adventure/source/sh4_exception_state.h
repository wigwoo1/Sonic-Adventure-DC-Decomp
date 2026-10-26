#ifndef SA_SH4_EXCEPTION_STATE_H
#define SA_SH4_EXCEPTION_STATE_H
#include "sh4_integer_state.h"
#include "vertex_bus.h"
/* Authored CPU/platform support, not recovered game-code credit. Ordinary
 * interrupt acceptance is explicit and untimed. Existing ABI structs stay intact.
 * RTE begin restores SR/banks and captures SPC; caller executes the original
 * delay slot before committing that captured branch target. */
struct sa_sh4_exception_state {
 uint32_t ssr,spc,sgr,expevt,intevt,tra,returned;
};
enum {SA_EXCEPTION_SSR=0,SA_EXCEPTION_SPC=1,SA_EXCEPTION_SGR=2};
SA_STAGE_EXPORT int sa_sh4_exception_stc(const struct sa_sh4_exception_state *,unsigned,uint32_t,uint32_t *,unsigned);
SA_STAGE_EXPORT int sa_sh4_exception_ldc(struct sa_sh4_exception_state *,unsigned,uint32_t,uint32_t,unsigned);
SA_STAGE_EXPORT int sa_sh4_rte_begin(const struct sa_sh4_exception_state *,uint32_t *,uint32_t[16],struct sa_sh4_integer_extra *,uint32_t *,unsigned);
SA_STAGE_EXPORT int sa_sh4_interrupt_enter(struct sa_sh4_exception_state *,uint32_t *,uint32_t[16],struct sa_sh4_integer_extra *,uint32_t *,uint32_t,uint32_t,unsigned,unsigned);
struct sa_exception_bus {
 struct sa_vertex_bus parent;
 struct sa_sh4_exception_state *state;
 uint32_t expevt_reads,intevt_reads,tra_reads;
};
SA_STAGE_EXPORT int sa_exception_bus_init(struct sa_exception_bus *,const struct sa_vertex_bus *,struct sa_sh4_exception_state *,struct sa_vertex_bus *);
#endif
