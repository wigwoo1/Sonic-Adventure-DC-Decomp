#include "fpu_cleanup_transfer.h"

typedef int (*sa_transition_service)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);
typedef void (*sa_transition_observer)(void *, uint32_t, const uint8_t *,
    const struct sa_fpu_banks *, const uint32_t *, const uint32_t *, const uint32_t *);

int sa_main_dispatch_transition_8c0aff1a(uint8_t *, struct sa_fpu_banks *,
    uint32_t *, uint32_t *, uint32_t *, uint32_t *, uint32_t *, void *, sa_transition_service);
int sa_main_dispatch_early_gate_8c0afe72(uint8_t *, struct sa_fpu_banks *,
    uint32_t *, uint32_t *, uint32_t *, uint32_t *, uint32_t *, void *, sa_transition_service);
void sa_main_dispatch_value_apply_8c07c298(uint8_t *, struct sa_fpu_banks *,
    uint32_t *, uint32_t *, uint32_t *, void *, sa_transition_observer);

struct sa_transition_context {
    void *user;
    sa_transition_service external;
    sa_transition_observer observe;
    uint32_t *fpul;
    uint32_t *macl;
    int native_values;
};

static int dispatch(void *context, uint32_t target, uint8_t *ram,
    struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r, uint32_t *pr)
{
    struct sa_transition_context *services = (struct sa_transition_context *)context;
    if (services->observe)
        services->observe(services->user, target, ram, fpu, sr, r, pr);
    if (target == 0x8c0afe72u)
        return sa_main_dispatch_early_gate_8c0afe72(ram, fpu, sr, r, pr,
            services->fpul, services->macl, context, dispatch);
    if (services->native_values && target == 0x8c07c298u) {
        sa_main_dispatch_value_apply_8c07c298(ram, fpu, sr, r, pr,
            services->user, services->observe);
        return 0;
    }
    if (!services->external)
        return -2;
    return services->external(services->user, target, ram, fpu, sr, r, pr);
}

/* Host integration only, not an additional original-game routine.
 * The outer helper and early dependency execute natively. Remaining targets
 * are explicit services. Optional read-only observation covers both levels.
 */
SA_STAGE_EXPORT int sa_main_dispatch_transition_early_native(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *user,
    sa_transition_service external, sa_transition_observer observe)
{
    if (!external)
        return -2;
    struct sa_transition_context context = {user, external, observe, fpul, macl, 0};
    return sa_main_dispatch_transition_8c0aff1a(ram, fpu, sr, r, pr,
        fpul, macl, &context, dispatch);
}

/* Further native connection: early gate -> value wrapper -> value writer.
 * A null external service is allowed for these closed paths. Reaching another
 * target without an external service returns -2 with partial guest state.
 * The older entry above retains its original external-service boundary.
 */
SA_STAGE_EXPORT int sa_main_dispatch_transition_values_native(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *user,
    sa_transition_service external, sa_transition_observer observe)
{
    struct sa_transition_context context = {user, external, observe, fpul, macl, 1};
    return sa_main_dispatch_transition_8c0aff1a(ram, fpu, sr, r, pr,
        fpul, macl, &context, dispatch);
}
