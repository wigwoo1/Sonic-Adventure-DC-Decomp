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

static uint32_t ram_offset(uint32_t address) {
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address) {
    uint32_t offset = ram_offset(address);
    return (uint32_t)ram[offset] |
           ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) |
           ((uint32_t)ram[offset + 3] << 24);
}

/* 8c64b2d4..8c64b2dc: load the indirect target into R3, set R6=0x48,
 * jump through R3, and clear R5 in the jump delay slot. */
SA_EXPORT void sa_indirect_tail_wrapper_8c64b2d4(uint8_t *ram,
                                                struct sa_cpu_state *state) {
    state->r[3] = read32(ram, 0x8c64b2fcu);
    state->r[6] = 0x48;
    state->r[5] = 0;
    state->pc = state->r[3];
}
