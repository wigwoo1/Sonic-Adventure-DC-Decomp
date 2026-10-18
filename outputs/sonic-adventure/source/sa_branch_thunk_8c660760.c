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

SA_EXPORT void sa_branch_thunk_8c660760(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    (void)ram;
    state->pc = 0x8c65fc92u;
}
