#include "engine_dispatch_wrappers.h"

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

static int dispatch_wrapper(uint8_t *ram, struct sa_fpu_banks *f,
    uint32_t *sr, uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *ctx, sa_engine_dispatch_call call, uint32_t selector,
    uint32_t target_literal, uint32_t return_address) {
    r[15] -= 4u;
    write32(ram, r[15], *pr);
    r[3] = read32(ram, target_literal);
    r[4] = selector;
    r[5] = read32(ram, r[15] + 4u);
    *pr = return_address;
    int status = call(ctx, r[3], ram, f, sr, r, pr);
    if (status)
        return status;
    *pr = read32(ram, r[15]);
    r[15] += 4u;
    r[0] = 0u;
    (void)fpul;
    (void)macl;
    return 0;
}

SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_40_8c652f56(
    uint8_t *m, struct sa_fpu_banks *f, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *ctx,
    sa_engine_dispatch_call call) {
    return dispatch_wrapper(m, f, sr, r, pr, fpul, macl, ctx, call,
        0x40u, 0x8c653004u, 0x8c652f60u);
}

SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_b0_8c652d6e(
    uint8_t *m, struct sa_fpu_banks *f, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *ctx,
    sa_engine_dispatch_call call) {
    return dispatch_wrapper(m, f, sr, r, pr, fpul, macl, ctx, call,
        0xb0u, 0x8c652d9cu, 0x8c652d78u);
}

SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_b8_8c652e38(
    uint8_t *m, struct sa_fpu_banks *f, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *ctx,
    sa_engine_dispatch_call call) {
    r[15] -= 4u;
    write32(m, r[15], *pr);
    r[15] -= 4u;
    r[5] = r[4];
    write32(m, r[15], r[4]);
    r[3] = read32(m, 0x8c652e6cu);
    const uint32_t selector = (uint32_t)(int32_t)(int16_t)
        ((uint16_t)m[0x8c652e68u & 0x00ffffffu]
         | ((uint16_t)m[(0x8c652e68u & 0x00ffffffu) + 1u] << 8));
    r[4] = selector;
    *pr = 0x8c652e48u;
    int status = call(ctx, r[3], m, f, sr, r, pr);
    if (status)
        return status;
    r[15] += 4u;
    *pr = read32(m, r[15]);
    r[15] += 4u;
    r[0] = 0u;
    (void)fpul;
    (void)macl;
    return 0;
}

SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_pair_8c666968(
    uint8_t *m, struct sa_fpu_banks *f, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *ctx,
    sa_engine_dispatch_call call) {
    r[15] -= 4u;
    write32(m, r[15], *pr);
    r[3] = read32(m, 0x8c6669b8u);
    r[5] = 1u;
    *pr = 0x8c666972u;
    r[4] = 8u;
    int status = call(ctx, r[3], m, f, sr, r, pr);
    if (status)
        return status;

    r[2] = read32(m, 0x8c6669b8u);
    r[4] = 8u;
    r[5] = 0u;
    *pr = read32(m, r[15]);
    r[15] += 4u;
    status = call(ctx, r[2], m, f, sr, r, pr);
    if (status)
        return status;

    (void)fpul;
    (void)macl;
    return 0;
}
