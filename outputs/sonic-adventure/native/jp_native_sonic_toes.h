#ifndef JP_NATIVE_SONIC_TOES_H
#define JP_NATIVE_SONIC_TOES_H
#include <stdint.h>
/* Serialized creating-thread service. Owns twelve real Motion/model views.
 * Caller retains the active player record and prevents other direct mutations.
 * Close restores original toe models before retiring views. Call original
 * non-full-speed step before freeing a record when its blend fields need reset.
 * Task attachment/automatic retirement is a separate caller responsibility. */
struct jp_sonic_toes_info {uint32_t phase,leases,thread,native_error;uint64_t steps;};
__declspec(dllexport) int jp_sonic_toes_bind(void);
__declspec(dllexport) int jp_sonic_toes_step(void *player);
__declspec(dllexport) int jp_sonic_toes_close(void);
__declspec(dllexport) int jp_sonic_toes_snapshot(struct jp_sonic_toes_info *);
/* Read-only compiler layout diagnostic: seven uint32_t values. */
__declspec(dllexport) int jp_sonic_toes_layout(uint32_t *);
#endif
