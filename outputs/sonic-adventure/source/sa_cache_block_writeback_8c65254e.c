#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

typedef void (*sa_cache_writeback_service)(uint32_t address, void *context);

SA_EXPORT void sa_cache_block_writeback_8c65254e(
    struct sa_cpu_state *state,
    sa_cache_writeback_service writeback,
    void *context)
{
    writeback(state->r[4], context);
    state->pc = state->pr;
}
