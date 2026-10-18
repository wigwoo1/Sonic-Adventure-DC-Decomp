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

 SA_EXPORT void sa_empty_stub_8c014b86(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c02e720(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c02e734(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c034db2(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c047126(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c60215e(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c611708(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c6224a0(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c63eeb0(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c64241a(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c645f80(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c645f84(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c653440(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c65b56a(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c65c70c(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c65ca8e(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c65ce02(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c6608a0(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c665a54(struct sa_cpu_state *state)
 { state->pc = state->pr; }
 SA_EXPORT void sa_empty_stub_8c666e02(struct sa_cpu_state *state)
 { state->pc = state->pr; }
