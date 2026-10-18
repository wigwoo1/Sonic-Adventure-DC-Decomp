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

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static uint32_t read16_signed(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (uint32_t)(int32_t)(int16_t)value;
}

static uint32_t read8_signed(const uint8_t *ram, uint32_t address)
{
    return (uint32_t)(int32_t)(int8_t)ram[ram_offset(address)];
}

static void indirect_get(uint8_t *ram, struct sa_cpu_state *state,
                         uint32_t literal, uint8_t pointer_register,
                         uint8_t destination_register, uint8_t width)
{
    uint32_t pointer = read32(ram, literal);
    uint32_t value;
    state->r[pointer_register] = pointer;
    if (width == 4u)
        value = read32(ram, pointer);
    else if (width == 2u)
        value = read16_signed(ram, pointer);
    else
        value = read8_signed(ram, pointer);
    state->r[destination_register] = value;
    state->pc = state->pr;
}

SA_EXPORT void sa_global_indirect_getter_8c01b37a(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c01b3c0u, 3, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c04f8f4(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c04f954u, 2, 0, 2); }
SA_EXPORT void sa_global_indirect_getter_8c050a06(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c050a2cu, 2, 0, 1); }
SA_EXPORT void sa_global_indirect_getter_8c0585e2(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c05861cu, 2, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c062704(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c062740u, 3, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c0818de(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c081968u, 2, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c08a2bc(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c08a354u, 1, 0, 2); }
SA_EXPORT void sa_global_indirect_getter_8c08a572(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c08a68cu, 1, 0, 1); }
SA_EXPORT void sa_global_indirect_getter_8c08a71e(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c08a7a8u, 2, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c0a7770(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c0a778cu, 3, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c0df0ca(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c0df0ecu, 3, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c107bc2(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c107c70u, 3, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c646eda(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c646f58u, 3, 0, 4); }
SA_EXPORT void sa_global_indirect_getter_8c657968(uint8_t *ram, struct sa_cpu_state *s)
{ indirect_get(ram, s, 0x8c657a00u, 3, 0, 4); }
