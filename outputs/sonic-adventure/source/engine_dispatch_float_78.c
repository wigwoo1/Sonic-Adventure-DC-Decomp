#include "engine_dispatch_float_78.h"

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

SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_float_78_8c652d3a(
    uint8_t *ram, struct sa_fpu_banks *f, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *ctx,
    sa_engine_dispatch_call call) {
    r[15] -= 4u;
    write32(ram, r[15], *pr);
    r[15] -= 4u;
    r[5] = r[15];
    r[3] = read32(ram, 0x8c652da0u);
    r[2] = read32(ram, 0x8c652d9cu);
    const unsigned bank = (f->fpscr >> 21) & 1u;
    write32(ram, r[15], f->bank[bank][4]);
    r[5] = read32(ram, r[5]) & r[3];
    *pr = 0x8c652d4eu;
    r[4] = 0x78u;
    int status = call(ctx, r[2], ram, f, sr, r, pr);
    if (status)
        return status;
    r[15] += 4u;
    *pr = read32(ram, r[15]);
    r[15] += 4u;
    r[0] = 0u;
    (void)fpul;
    (void)macl;
    return 0;
}
