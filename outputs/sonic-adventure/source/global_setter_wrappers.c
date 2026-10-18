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

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void store_r4(uint8_t *ram, struct sa_cpu_state *state,
                     uint32_t literal, unsigned pointer_register,
                     int copy_return_value)
{
    uint32_t target = read32(ram, literal);
    state->r[pointer_register] = target;
    write32(ram, target, state->r[4]);
    if (copy_return_value) {
        state->r[0] = state->r[4];
    }
    state->pc = state->pr;
}

SA_EXPORT void sa_global_setter_8c041ea4(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c041ec4u, 2u, 0); }
SA_EXPORT void sa_global_setter_8c0585dc(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c05861cu, 3u, 0); }
SA_EXPORT void sa_global_setter_8c05f01c(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c05f10cu, 3u, 0); }
SA_EXPORT void sa_global_setter_8c09b4aa(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c09b510u, 3u, 0); }
SA_EXPORT void sa_global_setter_8c109ce0(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c109de0u, 3u, 0); }
SA_EXPORT void sa_global_setter_8c10d39c(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c10d3a4u, 3u, 0); }
SA_EXPORT void sa_global_setter_8c60d1f2(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c60d26cu, 3u, 0); }
SA_EXPORT void sa_global_setter_8c60d1f8(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c60d270u, 2u, 0); }
SA_EXPORT void sa_global_setter_8c60ede0(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c60ee34u, 3u, 0); }
SA_EXPORT void sa_global_setter_8c640766(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c640774u, 2u, 0); }
SA_EXPORT void sa_global_setter_8c647534(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c6475f4u, 3u, 1); }
SA_EXPORT void sa_global_setter_8c657960(uint8_t *ram, struct sa_cpu_state *s)
{ store_r4(ram, s, 0x8c657a00u, 5u, 1); }
