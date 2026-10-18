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

/* 8c644de4..8c644dec: load the two literal arguments, set R6=1 in the
 * unconditional branch delay slot, then tail-transfer to 8c644d98. */
SA_EXPORT void sa_tail_wrapper_8c644de4(uint8_t *ram,
                                        struct sa_cpu_state *state) {
    state->r[4] = read32(ram, 0x8c644e0cu);
    state->r[5] = read32(ram, 0x8c644e08u);
    state->r[6] = 1;
    state->pc = 0x8c644d98u;
}
