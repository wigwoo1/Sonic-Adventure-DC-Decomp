#include "engine_callback_flag_reset.h"

static uint32_t read32(const uint8_t *ram, uint32_t address) {
    const uint32_t a = address & 0x00ffffffu;
    return (uint32_t)ram[a] | ((uint32_t)ram[a + 1] << 8)
        | ((uint32_t)ram[a + 2] << 16) | ((uint32_t)ram[a + 3] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    const uint32_t a = address & 0x00ffffffu;
    ram[a] = (uint8_t)value;
    ram[a + 1] = (uint8_t)(value >> 8);
    ram[a + 2] = (uint8_t)(value >> 16);
    ram[a + 3] = (uint8_t)(value >> 24);
}

/* The conditional target 0x8c665a54 is a retail no-op leaf. Its call has no
 * net RAM/register effect, but the caller still updates PR and executes the
 * delay slot before returning. */
SA_ENGINE_FLAG_EXPORT void sa_engine_callback_flag_reset_8c651368(
    uint8_t *ram, uint32_t *sr, uint32_t *r, uint32_t *pr) {
    r[15] -= 4u;
    write32(ram, r[15], *pr);
    r[2] = 0x8c67c004u;
    r[3] = read32(ram, r[2]);
    *sr = (*sr & ~1u) | (r[3] == 0u);
    if (r[3] == 0u) {
        *pr = read32(ram, r[15]);
        r[15] += 4u;
        return;
    }
    r[3] = 0x8c665a54u;
    *pr = 0x8c651378u;
    /* Retail callee 0x8c665a54 is exactly RTS/NOP. */
    r[3] = 0x8c67c004u;
    r[2] = 0u;
    write32(ram, r[3], r[2]);
    *pr = read32(ram, r[15]);
    r[15] += 4u;
}
