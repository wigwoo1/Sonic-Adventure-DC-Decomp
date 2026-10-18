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

static void store(uint8_t *ram, uint32_t address, uint32_t value,
                  unsigned width)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    if (width >= 2u) {
        ram[at + 1u] = (uint8_t)(value >> 8);
    }
    if (width == 4u) {
        ram[at + 2u] = (uint8_t)(value >> 16);
        ram[at + 3u] = (uint8_t)(value >> 24);
    }
}

static void store_constant(uint8_t *ram, struct sa_cpu_state *state,
                           uint32_t literal, unsigned pointer_register,
                           unsigned value_register, uint32_t value,
                           unsigned width)
{
    uint32_t target = read32(ram, literal);
    state->r[pointer_register] = target;
    state->r[value_register] = value;
    store(ram, target, value, width);
    state->pc = state->pr;
}

SA_EXPORT void sa_retail_constant_setter_8c04c688(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c04c780u,0u,1u,0u,2u); }
SA_EXPORT void sa_retail_constant_setter_8c053a00(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c053a78u,2u,3u,1u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c055c44(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c055d08u,1u,2u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0585ba(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c058614u,2u,3u,1u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c05b1da(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c05b23cu,2u,3u,0u,2u); }
SA_EXPORT void sa_retail_constant_setter_8c0818ce(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c081968u,1u,2u,1u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0818d6(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c081968u,0u,3u,2u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0869e0(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c086b00u,2u,3u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c08a074(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c08a0f0u,1u,2u,0u,2u); }
SA_EXPORT void sa_retail_constant_setter_8c08a5ae(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c08a6a8u,1u,2u,1u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c08a5b6(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c08a6a8u,0u,3u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c08fe08(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c08feacu,1u,3u,1u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c08fe10(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c08feacu,0u,2u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0a2ba0(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c0a2c20u,2u,3u,2u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0a3fa0(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c0a4060u,2u,3u,2u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0cb9e8(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c0cba24u,1u,2u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c0df0d0(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c0df0ecu,1u,2u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c63adb0(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c63adbcu,2u,3u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c65ad60(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c65adb8u,2u,3u,0u,4u); }
SA_EXPORT void sa_retail_constant_setter_8c660898(uint8_t *ram, struct sa_cpu_state *s)
{ store_constant(ram,s,0x8c660954u,2u,3u,0u,4u); }
