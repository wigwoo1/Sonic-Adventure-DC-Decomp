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

#define BRANCH_THUNK(name, target) \
    SA_EXPORT void name(uint8_t *ram, struct sa_cpu_state *state) \
    { (void)ram; state->pc = (target); }

BRANCH_THUNK(sa_branch_thunk_8c09da40, 0x8c09cc80u)
BRANCH_THUNK(sa_branch_thunk_8c6021d2, 0x8c6021d6u)
BRANCH_THUNK(sa_branch_thunk_8c603bec, 0x8c603bfcu)
BRANCH_THUNK(sa_branch_thunk_8c603c24, 0x8c603c2eu)
BRANCH_THUNK(sa_branch_thunk_8c6453c0, 0x8c6453ccu)
