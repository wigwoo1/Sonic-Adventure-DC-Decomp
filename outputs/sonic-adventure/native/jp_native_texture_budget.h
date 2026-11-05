#ifndef JP_NATIVE_TEXTURE_BUDGET_H
#define JP_NATIVE_TEXTURE_BUDGET_H
#include <stddef.h>
struct jp_texture_budget {size_t limit, charged, available;};
/* Owning game thread; read-only snapshot of the actual host owner budget.
 * Charged bytes include record/binding metadata and the standalone font.
 * Separate immutable-frame storage is outside this owner's budget. Available
 * is a budget allowance, not a promise that the OS can allocate a contiguous
 * block. Failure leaves the caller's snapshot unchanged. */
__declspec(dllexport) int jp_texture_budget_query(struct jp_texture_budget *out);
#endif
