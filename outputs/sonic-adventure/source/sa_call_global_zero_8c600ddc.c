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

SA_EXPORT void sa_call_global_zero_8c600ddc(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t callback =
        (uint32_t)ram[0x600df0u] |
        ((uint32_t)ram[0x600df1u] << 8) |
        ((uint32_t)ram[0x600df2u] << 16) |
        ((uint32_t)ram[0x600df3u] << 24);
    /* The original JMP delay slot loads zero into R5 before the tail call. */
    state->r[3] = callback;
    state->r[5] = 0;
    state->pc = callback;
}
