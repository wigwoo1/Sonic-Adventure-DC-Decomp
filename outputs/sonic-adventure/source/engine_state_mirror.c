#include "engine_state_mirror.h"

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

SA_ENGINE_STATE_EXPORT void sa_engine_state_mirror_8c107ac6(
    uint8_t *ram, uint32_t *sr, uint32_t *r) {
    r[0] = read32(ram, 0x8c107b88u);
    r[1] = read32(ram, r[0]);
    *sr = (*sr & ~1u) | (r[1] == 0u);
    if (r[1] == 0u)
        return;
    r[3] = read32(ram, 0x8c107b74u);
    r[1] = read32(ram, 0x8c107b6cu);
    write32(ram, r[1], r[4]);
    write32(ram, r[3], r[4]);
}
