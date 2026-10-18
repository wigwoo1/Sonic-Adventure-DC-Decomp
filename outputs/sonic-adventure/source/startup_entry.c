#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_startup_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

/* Retail 0x8c010000 masks the startup control register, sets bit 11, and
   tail-transfers to 0x8c0100a0. MMIO is supplied by the platform port. */
SA_EXPORT void sa_startup_entry_8c010000(struct sa_startup_state *state,
                                          uint32_t *startup_control_register)
{
    state->r[0] = 0xff00001cu;
    state->r[1] = *startup_control_register & 0x000089afu;
    state->r[2] = 0x00000800u;
    state->r[1] |= state->r[2];
    *startup_control_register = state->r[1];
    state->r[0] = 0x8c0100a0u;
    state->pc = state->r[0];
}
